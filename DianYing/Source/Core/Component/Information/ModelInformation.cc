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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Dy/Core/Component/Resource/ModelResource.h>
#include <Dy/Helper/Geometry/GeometryType.h>
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/LoggingManager.h>

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

  Assimp::Importer assimpImporter;
  const aiScene* assimpModelScene = assimpImporter.ReadFile(
      modelPath.c_str(),
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
  );

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

  // Retrieve vertex buffer informations of this mesh.
  meshInformationDescriptor.mVertices.reserve(mesh->mNumVertices);
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
    meshInformationDescriptor.mVertices.emplace_back(vertexInformation);
  }

  // Retrieve indice for element buffer object (openGL)
  meshInformationDescriptor.mIndices.reserve(mesh->mNumFaces * 3);
  for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
  {
    const aiFace& assimpFace = mesh->mFaces[i];
    for (uint32_t j = 0; j < assimpFace.mNumIndices; ++j)
    {
      meshInformationDescriptor.mIndices.emplace_back(assimpFace.mIndices[j]);
    }
  }
  meshInformationDescriptor.mIndices.shrink_to_fit();

  // Get Mateiral information. IF not exists, just pass only mesh information.
  // Diffuse map, specular map, height map, ambient map, emissive map etc.
  const auto  materialIndex = mesh->mMaterialIndex;
  if (materialIndex == 0)
  {
    this->mMeshInformations.emplace_back(meshInformationDescriptor);
    return;
  }

  // If retrieving material name has been failed, just replace it with model name + "0", "1"...
  aiMaterial* material      = scene->mMaterials[materialIndex];
  aiString    materialName  = {};
  if (const auto ret = material->Get(AI_MATKEY_NAME, materialName); ret == AI_FAILURE)
  {
    assert(false);
  }

  // Get texture informations from material with name?
  PDyMaterialConstructionDescriptor materialDescriptor;
  materialDescriptor.mIsShaderLazyInitialized = true;
  materialDescriptor.mMaterialName            = materialName.C_Str();
  std::vector<std::string> textureNames;

  if (const auto opDiffuse = __pLoadMaterialTextures(material, EDyTextureMapType::Diffuse);
      opDiffuse.has_value())
  {
    const auto& diffuseMaps = opDiffuse.value();
    textureNames.insert(textureNames.end(), diffuseMaps.begin(), diffuseMaps.end());
  }
  if (const auto opSpecular = __pLoadMaterialTextures(material, EDyTextureMapType::Specular);
      opSpecular.has_value())
  {
    const auto& specularMaps  = opSpecular.value();
    textureNames.insert(textureNames.end(), specularMaps.begin(), specularMaps.end());
  }
  if (const auto opNormal = __pLoadMaterialTextures(material, EDyTextureMapType::Normal);
      opNormal.has_value())
  {
    const auto& normalMaps = opNormal.value();
    textureNames.insert(textureNames.end(), normalMaps.begin(), normalMaps.end());
  }
  if (const auto opHeight = __pLoadMaterialTextures(material, EDyTextureMapType::Height);
      opHeight.has_value())
  {
    const auto& heightMaps = opHeight.value();
    textureNames.insert(textureNames.end(), heightMaps.begin(), heightMaps.end());
  }
  materialDescriptor.mTextureName = textureNames;

  // Let InformationManager Initialzie material information instance.
  auto& manInfo = MDyDataInformation::GetInstance();
  if (const auto ptr = manInfo.GetMaterialInformation(materialDescriptor.mMaterialName);
      !ptr)
  {
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateMaterialInformation(materialDescriptor));
  }
  meshInformationDescriptor.mMaterialNames.emplace_back(materialDescriptor.mMaterialName);

  // Create DDyMeshInformation with descriptor.
  this->mMeshInformations  .emplace_back(meshInformationDescriptor);
  this->mBindedMaterialName.emplace_back(materialDescriptor.mMaterialName);
}

std::optional<std::vector<std::string>>
DDyModelInformation::__pLoadMaterialTextures(aiMaterial* material, EDyTextureMapType type)
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
  auto& manInfo = dy::MDyDataInformation::GetInstance();

  std::vector<std::string> textureInformationString;

  const uint32_t textureMapCount = material->GetTextureCount(aiTextureType);
  for (uint32_t i = 0; i < textureMapCount; ++i)
  {
    aiString textureLocalPath;
    if (material->GetTexture(aiTextureType, i, &textureLocalPath) != AI_SUCCESS)
    {
      // Error handling
      return std::nullopt;
    }

    // Check there is duplicated texture by comparing texture local path.
    const auto dupTexturePath = std::find(
        this->mTextureLocalPaths.begin(),
        this->mTextureLocalPaths.end(),
        textureLocalPath.C_Str());
    if (dupTexturePath == this->mTextureLocalPaths.end())
    {
      PDyTextureConstructionDescriptor textureDesc;
      {
        {
          // Set each texture name to texture file name except for file type like a .png .jpg.
          const std::string textureLocalPathStr = textureLocalPath.C_Str();
          auto start = textureLocalPathStr.find_last_of('/');
          if (start == std::string::npos)
          {
            start = textureLocalPathStr.find_last_of('\\');
            if (start == std::string::npos)
            {
              start = 0;
            }
          }

          auto count = textureLocalPathStr.find_last_of('.');
          if (count != std::string::npos)
          {
            count -= start;
          }

          textureDesc.mTextureName                      = textureLocalPathStr.substr(start, count);
        }

        textureDesc.mTextureFileLocalPath               = textureLocalPath.C_Str();
        textureDesc.mTextureFileAbsolutePath            = this->mModelRootPath + '/' + textureDesc.mTextureFileLocalPath;
        textureDesc.mTextureType                        = EDyTextureStyleType::D2;
        textureDesc.mTextureMapType                     = type;
        textureDesc.mIsEnabledCustomedTextureParameter  = false;
        textureDesc.mIsEnabledAbsolutePath              = true;
        textureDesc.mIsEnabledCreateMipmap              = true;
      }
      MDY_CALL_ASSERT_SUCCESS(manInfo.CreateTextureInformation(textureDesc));
      this->mTextureLocalPaths.emplace_back(textureDesc.mTextureFileLocalPath);
      textureInformationString.emplace_back(textureDesc.mTextureName);
    }
    else
    {
      // Set each texture name to texture file name except for file type like a .png .jpg.
      const std::string textureLocalPathStr = textureLocalPath.C_Str();
      auto start = textureLocalPathStr.find_last_of('/');
      if (start == std::string::npos)
      {
        start = textureLocalPathStr.find_last_of('\\');
        if (start == std::string::npos) { start = 0; }
      }
      auto count = textureLocalPathStr.find_last_of('.');
      if (count != std::string::npos) { count -= start; }

      const auto textureName = textureLocalPathStr.substr(start, count);

      textureInformationString.emplace_back(textureName);
    }
  }
  return textureInformationString;
}

void DDyModelInformation::__pOutputDebugInformationLog()
{
#if defined(_DEBUG) || !defined(NDEBUG)
  int32_t i = 0;
  for (const auto& baseMaterialName : this->mBindedMaterialName)
  {
    MDY_LOG_DEBUG_D(kModelInformationNumbTemplate, kModelInformation, "base material name", i, baseMaterialName);
    ++i;
  }

  i = 0;
  for (const auto& baseTexturePath : this->mTextureLocalPaths)
  {
    MDY_LOG_DEBUG_D(kModelInformationNumbTemplate, kModelInformation, "innate texture name", i, baseTexturePath);
    ++i;
  }

  i = 0;
  for (const auto& submeshInformation : this->mMeshInformations)
  {
    const auto& submeshInfo = submeshInformation.GetInformation();
    MDY_LOG_DEBUG_D("{} | Model information submesh No.{} | Vertices count : {} | Indices count : {}",
        kModelInformation,
        i,
        submeshInfo.mVertices.size(),
        submeshInfo.mIndices.size()
    );
    for (const auto& submeshInnateMaterial : submeshInfo.mMaterialNames)
    {
      MDY_LOG_DEBUG_D("{} | Model information submesh No.{} | Innate material name : {}", kModelInformation, i, submeshInnateMaterial);
    }
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