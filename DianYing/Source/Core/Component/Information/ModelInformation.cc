#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Core/Component/Information/ModelInformation.h>

#include <future>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Dy/Core/Component/Resource/ModelResource.h>
#include <Dy/Helper/Geometry/GeometryType.h>
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Helper/ThreadPool.h>
#include <Dy/Helper/IoHelper.h>
#include <Phitos/Dbg/assert.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kModelInformationTemplate,     "{} | Model information {} : {}");
MDY_SET_IMMUTABLE_STRING(kModelInformationNumbTemplate, "{} | Model information {} No.{} : {}");
MDY_SET_IMMUTABLE_STRING(kModelInformation,             "DDyModelInformation");

} /// ::unnamed namespace

namespace dy
{

DDyModelInformation::DDyModelInformation(const PDyModelConstructionDescriptor& modelConstructionDescriptor)
{
  // Insert name and check name is empty or not.
  this->mModelName = modelConstructionDescriptor.mModelName;
  if (this->mModelName.empty())
  {
    MDY_LOG_CRITICAL_D("{} | Failed to create model information. Model name is not speicified.", kModelInformation);
    throw std::runtime_error("Model name is not specified.");
  }
  MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "name", this->mModelName);

  // Load model information, if failed throw exception outside afterward free scene.
  const auto& modelPath = modelConstructionDescriptor.mModelPath;
  MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "model path", modelPath);

  const auto start = std::chrono::system_clock::now();

  Assimp::Importer assimpImporter;
  const aiScene* assimpModelScene = assimpImporter.ReadFile(
      modelPath.c_str(),
      aiProcess_Triangulate | aiProcess_GenNormals
  );

  const auto end = std::chrono::system_clock::now();
  const auto cnt = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  MDY_LOG_DEBUG_D("{} | Model importing elapsed time : {} us", this->mModelName, cnt.count());

  if (!assimpModelScene || assimpModelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpModelScene->mRootNode)
  {
    assimpImporter.FreeScene();
    MDY_LOG_CRITICAL_D("{} | Failed to create load model scene.", kModelInformation);
    throw std::runtime_error("Could not load model " + modelConstructionDescriptor.mModelName + ".");
  }

  // Process Node
  this->mModelRootPath = modelPath.substr(0, modelPath.find_last_of('/'));
  this->__pProcessAssimpNode(assimpModelScene->mRootNode, assimpModelScene);

  MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "root path", this->mModelRootPath);

  // Output model, submesh, and material information to console.
  this->__pOutputDebugInformationLog();

  assimpImporter.FreeScene();
}

void DDyModelInformation::__pProcessAssimpNode(aiNode* node, const aiScene* scene)
{
  for (uint32_t i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh* assimpMesh = scene->mMeshes[node->mMeshes[i]];
    this->__pProcessAssimpMesh(assimpMesh, scene);
  }

  for (uint32_t i = 0; i < node->mNumChildren; ++i)
  {
    this->__pProcessAssimpNode(node->mChildren[i], scene);
  }
}

void DDyModelInformation::__pProcessAssimpMesh(aiMesh* mesh, const aiScene* scene)
{
  PMeshInformationDescriptor meshInformationDescriptor;
  if (this->__pLoadVertexData(mesh, meshInformationDescriptor) == DY_FAILURE)
  {

  };

  // Retrieve indice for element buffer object (openGL)
  if (this->__pLoadIndiceData(mesh, meshInformationDescriptor) == DY_FAILURE)
  {

  };

  // Get Mateiral information. IF not exists, just pass only mesh information.
  // Diffuse map, specular map, height map, ambient map, emissive map etc.
  const auto materialIndex = mesh->mMaterialIndex;
  if (materialIndex == 0)
  {
    this->mMeshInformations.emplace_back(meshInformationDescriptor);
    return;
  }

  // If retrieving material name has been failed, just replace it with model name + "0", "1"...
  // Get texture informations from material with name?
  aiMaterial* material    = scene->mMaterials[materialIndex];
  auto optMaterialDesc    = this->__pReadMaterialData(material);
  PHITOS_ASSERT(optMaterialDesc.has_value(), "Material must have value.");
  if (optMaterialDesc.has_value())
  {
    meshInformationDescriptor.mMaterialName = optMaterialDesc.value().mMaterialName;
  }

  // Create DDyMeshInformation with material descriptor.
  const auto& materialDescriptor = optMaterialDesc.value();
  this->mMeshInformations.emplace_back(meshInformationDescriptor);

  if (std::find(MDY_BIND_BEGIN_END(this->mBindedMaterialName), materialDescriptor.mMaterialName) == this->mBindedMaterialName.end())
  {
    this->mBindedMaterialName.emplace_back(materialDescriptor.mMaterialName);
  }
}

EDySuccess DDyModelInformation::__pLoadVertexData(const aiMesh* mesh, PMeshInformationDescriptor& desc)
{
  // Retrieve vertex buffer informations of this mesh.
  desc.mVertices.resize(mesh->mNumVertices);

  constexpr int32_t mtThresholdSize = 1'000;
  if (mesh->mNumVertices >= mtThresholdSize)
  {
    constexpr int32_t mtTaskCount = 4;
    dy::FDyThreadPool pool(mtTaskCount);
    std::vector<std::future<bool>> results;

    const int32_t vertexCountSize = static_cast<int32_t>(std::ceil(static_cast<float>(mesh->mNumVertices) / mtTaskCount));
    int32_t start = 0;
    int32_t to    = vertexCountSize;
    for (int32_t i = 0; i < mtTaskCount; ++i)
    {
      // Do
      results.emplace_back(
          pool.Enqueue([](const aiMesh* mesh, PMeshInformationDescriptor& desc, int32_t start, int32_t to) {
            for (int32_t i = start; i < to; ++i)
            {
              DVertexInformation vertexInformation;
              if (mesh->HasPositions()) vertexInformation.mPosition = mesh->mVertices[i];
              if (mesh->HasNormals())   vertexInformation.mNormal   = mesh->mNormals[i];

              // Only supports texture coords 0 set (UV0) for now.
              if (mesh->HasTextureCoords(0))
              {
                const auto& assimpTextureCoord = mesh->mTextureCoords[0][i];
                vertexInformation.mTexCoords = DVector2{assimpTextureCoord.x, assimpTextureCoord.y};
              }
              else vertexInformation.mTexCoords = DVector2{0};

              // また骨、Tangent, Bitangentはしない。

              // Insert vertex information to vertice container of descriptor container.
              desc.mVertices[i] = vertexInformation;
            }
            return true;
          },
          mesh, std::ref(desc), start, to)
      );

      start += vertexCountSize;
      to    += vertexCountSize;
      if (to >= static_cast<int32_t>(mesh->mNumVertices)) to = mesh->mNumVertices;
    };

    // Check result. (needs to check this routine not culled by optimization in release mode).
    for (auto& result : results)
    {
      if (auto flag = result.get())
      {
        MDY_LOG_DEBUG_D("Vertex retriviation job task result {}", flag);
      }
    }
  }
  else
  {
    for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
    {
      DVertexInformation vertexInformation;
      if (mesh->HasPositions()) vertexInformation.mPosition = mesh->mVertices[i];
      if (mesh->HasNormals())   vertexInformation.mNormal   = mesh->mNormals[i];

      // Only supports texture coords 0 set (UV0) for now.
      if (mesh->HasTextureCoords(0))
      {
        const auto& assimpTextureCoord = mesh->mTextureCoords[0][i];
        vertexInformation.mTexCoords = DVector2{assimpTextureCoord.x, assimpTextureCoord.y};
      }
      else vertexInformation.mTexCoords = DVector2{0};

      // また骨、Tangent, Bitangentはしない。

      // Insert vertex information to vertice container of descriptor container.
      desc.mVertices[i] = vertexInformation;
    }
  }

  return DY_SUCCESS;
}

EDySuccess DDyModelInformation::__pLoadIndiceData(const aiMesh* mesh, PMeshInformationDescriptor& desc)
{
  desc.mIndices.reserve(mesh->mNumFaces * 3);
  for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
  {
    const aiFace& assimpFace = mesh->mFaces[i];
    for (uint32_t j = 0; j < assimpFace.mNumIndices; ++j)
    {
      desc.mIndices.emplace_back(assimpFace.mIndices[j]);
    }
  }
  desc.mIndices.shrink_to_fit();
  return DY_SUCCESS;
}

std::optional<PDyMaterialConstructionDescriptor> DDyModelInformation::__pReadMaterialData(const aiMaterial* material)
{
  aiString materialName  = {};
  if (const auto ret = material->Get(AI_MATKEY_NAME, materialName); ret == AI_FAILURE)
  {
    assert(false);
    return std::nullopt;
  }

  PDyMaterialConstructionDescriptor materialDescriptor;
  materialDescriptor.mIsShaderLazyInitialized = true;
  materialDescriptor.mMaterialName            = materialName.C_Str();
  std::vector<std::string> textureNames;

  if (const auto opDiffuse = __pLoadMaterialTextures(material, EDyTextureMapType::Diffuse); opDiffuse.has_value())
  {
    const auto& diffuseMaps = opDiffuse.value();
    textureNames.insert(textureNames.end(), diffuseMaps.begin(), diffuseMaps.end());
  }
  if (const auto opSpecular = __pLoadMaterialTextures(material, EDyTextureMapType::Specular); opSpecular.has_value())
  {
    const auto& specularMaps  = opSpecular.value();
    textureNames.insert(textureNames.end(), specularMaps.begin(), specularMaps.end());
  }
  if (const auto opNormal = __pLoadMaterialTextures(material, EDyTextureMapType::Normal); opNormal.has_value())
  {
    const auto& normalMaps = opNormal.value();
    textureNames.insert(textureNames.end(), normalMaps.begin(), normalMaps.end());
  }
  if (const auto opHeight = __pLoadMaterialTextures(material, EDyTextureMapType::Height); opHeight.has_value())
  {
    const auto& heightMaps = opHeight.value();
    textureNames.insert(textureNames.end(), heightMaps.begin(), heightMaps.end());
  }
  materialDescriptor.mTextureNames = textureNames;

  // Let InformationManager Initialzie material information instance.
  auto& manInfo = MDyDataInformation::GetInstance();
  if (const auto ptr = manInfo.GetMaterialInformation(materialDescriptor.mMaterialName); !ptr)
  {
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateMaterialInformation(materialDescriptor));
    return materialDescriptor;
  }
  else
  {
    MDY_LOG_ERROR_D("{} | Duplicated material name detected. Material name : {}",
                    "DDyModelInformation::__pReadMaterialData", materialDescriptor.mMaterialName);
    return std::nullopt;
  }
}

std::optional<std::vector<std::string>>
DDyModelInformation::__pLoadMaterialTextures(const aiMaterial* material, EDyTextureMapType type)
{
  // EDyTextureMapType => aiTextureType
  aiTextureType aiTextureType = aiTextureType_UNKNOWN;
  switch (type)
  {
  case EDyTextureMapType::Diffuse:    aiTextureType = aiTextureType_DIFFUSE;  break;
  case EDyTextureMapType::Specular:   aiTextureType = aiTextureType_SPECULAR; break;
  case EDyTextureMapType::Normal:     aiTextureType = aiTextureType_NORMALS;  break;
  case EDyTextureMapType::Height:     aiTextureType = aiTextureType_HEIGHT;   break;
  default: break;
  }

  // Get Texture Map which is being binded to material (aiMaterial*);
  auto& manInfo = MDyDataInformation::GetInstance();

  std::vector<std::string> textureInformationString;
  const uint32_t textureMapCount = material->GetTextureCount(aiTextureType);
  for (uint32_t i = 0; i < textureMapCount; ++i)
  {
    aiString textureLocalPath;
    if (material->GetTexture(aiTextureType, i, &textureLocalPath) != AI_SUCCESS)
    {
      MDY_LOG_ERROR_D("{} | Failed to read texture information from assimp.", "DDyModelInformation::__pLoadMaterialTextures");
      return std::nullopt;
    }

    // Check there is duplicated texture by comparing texture local path.
    const auto dupTexturePath = std::find(MDY_BIND_BEGIN_END(this->mTextureLocalPaths), textureLocalPath.C_Str());
    const auto textureName    = DyGetFileNameFromPath(textureLocalPath.C_Str());
    if (dupTexturePath == this->mTextureLocalPaths.end())
    {
      PDyTextureConstructionDescriptor textureDesc;
      // Set each texture name to texture file name except for file type like a .png .jpg.
      textureDesc.mTextureName                        = textureName;
      textureDesc.mTextureFileLocalPath               = textureLocalPath.C_Str();
      textureDesc.mTextureFileAbsolutePath            = this->mModelRootPath + '/' + textureDesc.mTextureFileLocalPath;
      textureDesc.mTextureType                        = EDyTextureStyleType::D2;
      textureDesc.mTextureMapType                     = type;
      textureDesc.mIsEnabledCustomedTextureParameter  = false;
      textureDesc.mIsEnabledAbsolutePath              = true;
      textureDesc.mIsEnabledCreateMipmap              = true;

      MDY_CALL_ASSERT_SUCCESS(manInfo.CreateTextureInformation(textureDesc));
      this->mTextureLocalPaths.emplace_back(textureDesc.mTextureFileLocalPath);
    }

    MDY_LOG_CRITICAL_D("Texture Name : {}", textureName);
    textureInformationString.emplace_back(textureName);
  }
  return textureInformationString;
}

void DDyModelInformation::__pOutputDebugInformationLog()
{
#if defined(_DEBUG) || !defined(NDEBUG)
  int32_t i = 0;
  for (const auto& baseMaterialName : this->mBindedMaterialName)
  {
    MDY_LOG_DEBUG_D(kModelInformationNumbTemplate, kModelInformation.data(), "base material name", i, baseMaterialName);
    ++i;

    // @todo TEMPORAL

  }

  i = 0;
  for (const auto& baseTexturePath : this->mTextureLocalPaths)
  {
    MDY_LOG_DEBUG_D(kModelInformationNumbTemplate, kModelInformation.data(), "innate texture name", i, baseTexturePath);
    ++i;
  }

  i = 0;
  for (const auto& submeshInformation : this->mMeshInformations)
  {
    const auto& submeshInfo = submeshInformation.GetInformation();
    MDY_LOG_DEBUG_D("{} | Model information submesh No.{} | Vertices count : {} | Indices count : {}",
        kModelInformation.data(),
        i,
        submeshInfo.mVertices.size(),
        submeshInfo.mIndices.size()
    );
    MDY_LOG_DEBUG_D("{} | Model information submesh No.{} | Innate material name : {}", kModelInformation.data(), i, submeshInfo.mMaterialName);
  }
#endif
}

DDyModelInformation::~DDyModelInformation()
{
  MDY_LOG_INFO_D(kModelInformationTemplate, "~DDyModelInformation", "name", this->mModelName);
  if (this->mNextLevelPtr)
  {
    this->mNextLevelPtr->__pfSetPrevLevel(nullptr);
  }
}

} /// ::dy namespace