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
/// @TODO 骨、Tangent, Bitangent
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
MDY_SET_IMMUTABLE_STRING(kWarnNotHaveMaterial,          "{}::{} | This mesh does not have meterial information.");
MDY_SET_IMMUTABLE_STRING(kErrorModelFailedToRead,       "{} | Failed to create read model scene.");
MDY_SET_IMMUTABLE_STRING(kModelInformation,             "DDyModelInformation");
MDY_SET_IMMUTABLE_STRING(kFunc__pProcessAssimpMesh,     "__pProcessAssimpMesh");
MDY_SET_IMMUTABLE_STRING(kFunc__pReadMaterialData,      "__pReadMaterialData");
MDY_SET_IMMUTABLE_STRING(kWarnDuplicatedMaterialName,   "{}::{} | Duplicated material name detected. Material name : {}");

///
/// @brief Convert Assimp texture map type to EDyTextureMapType.
///
[[nodiscard]] dy::EDyTextureMapType DyGetTextureMapTypeFromAssimp(aiTextureType type) noexcept
{
  switch (type)
  {
  case aiTextureType_DIFFUSE:       return dy::EDyTextureMapType::Diffuse;
  case aiTextureType_SPECULAR:      return dy::EDyTextureMapType::Specular;
  case aiTextureType_AMBIENT:       return dy::EDyTextureMapType::Ambient;
  case aiTextureType_EMISSIVE:      return dy::EDyTextureMapType::Emissive;
  case aiTextureType_HEIGHT:        return dy::EDyTextureMapType::Height;
  case aiTextureType_NORMALS:       return dy::EDyTextureMapType::Normal;
  case aiTextureType_SHININESS:     return dy::EDyTextureMapType::Shininess;
  case aiTextureType_OPACITY:       return dy::EDyTextureMapType::Opacity;
  case aiTextureType_DISPLACEMENT:  return dy::EDyTextureMapType::Displacement;
  case aiTextureType_LIGHTMAP:      return dy::EDyTextureMapType::LightMap;
  case aiTextureType_REFLECTION:    return dy::EDyTextureMapType::Reflection;
  default:                          return dy::EDyTextureMapType::Custom;
  }
}

///
/// @brief EDyTextureMapType => aiTextureType
///
[[nodiscard]] aiTextureType DyGetAiTextureTypeFromEDyMapType(dy::EDyTextureMapType type) noexcept
{
  using dy::EDyTextureMapType;
  switch (type)
  {
  case EDyTextureMapType::Diffuse:    return aiTextureType_DIFFUSE;
  case EDyTextureMapType::Specular:   return aiTextureType_SPECULAR;
  case EDyTextureMapType::Ambient:    return aiTextureType_AMBIENT;
  case EDyTextureMapType::Emissive:   return aiTextureType_EMISSIVE;
  case EDyTextureMapType::Height:     return aiTextureType_HEIGHT;
  case EDyTextureMapType::Normal:     return aiTextureType_NORMALS;
  case EDyTextureMapType::Shininess:  return aiTextureType_SHININESS;
  case EDyTextureMapType::Opacity:    return aiTextureType_OPACITY;
  case EDyTextureMapType::Displacement: return aiTextureType_DISPLACEMENT;
  case EDyTextureMapType::LightMap:   return aiTextureType_LIGHTMAP;
  case EDyTextureMapType::Reflection: return aiTextureType_REFLECTION;
  case EDyTextureMapType::Custom:     return aiTextureType_NONE;
  default: return aiTextureType_UNKNOWN;
  }
}

///
/// @brief
///
[[nodiscard]] std::string_view DyGetDebugStringFromEDyMapType(dy::EDyTextureMapType type) noexcept
{
  using dy::EDyTextureMapType;
  switch (type)
  {
  case EDyTextureMapType::Diffuse:    return "Diffuse Map";
  case EDyTextureMapType::Specular:   return "Specular Map";
  case EDyTextureMapType::Ambient:    return "Ambient Map";
  case EDyTextureMapType::Emissive:   return "Emissive Map";
  case EDyTextureMapType::Height:     return "Height Map";
  case EDyTextureMapType::Normal:     return "Normal(Bump) Map";
  case EDyTextureMapType::Shininess:  return "Shininess Map";
  case EDyTextureMapType::Opacity:    return "Opacity Map";
  case EDyTextureMapType::Displacement: return "Displacement Map";
  case EDyTextureMapType::LightMap:   return "Light Map";
  case EDyTextureMapType::Reflection: return "Reflection Map";
  case EDyTextureMapType::Custom:     return "Custom";
  default: return "Unknown";
  }
}

} /// ::unnamed namespace

namespace dy
{

DDyModelInformation::DDyModelInformation(const PDyModelConstructionDescriptor& modelConstructionDescriptor)
{
  // Insert name and check name is empty or not.
  this->mModelName = modelConstructionDescriptor.mModelName;
  MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "Model name", this->mModelName);

  // Load model information, if failed throw exception outside afterward free scene.
  const auto& modelPath = modelConstructionDescriptor.mModelPath;
  MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "Model full path", modelPath);

  this->mAssimpImporter = std::make_unique<Assimp::Importer>();
  const aiScene* assimpModelScene = this->mAssimpImporter->ReadFile(modelPath.c_str(), aiProcess_Triangulate | aiProcess_GenNormals);

  if (!assimpModelScene || assimpModelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpModelScene->mRootNode)
  {
    this->mAssimpImporter = nullptr;
    MDY_LOG_CRITICAL_D(kErrorModelFailedToRead, kModelInformation);
    throw std::runtime_error("Could not load model " + modelConstructionDescriptor.mModelName + ".");
  }

  this->mGlobalInverseTransform = assimpModelScene->mRootNode->mTransformation.Inverse();

  // Process all meshes and retrieve material, bone, etc information.
  this->mModelRootPath = modelPath.substr(0, modelPath.find_last_of('/'));
  for (TU32 i = 0; i < assimpModelScene->mNumMeshes; ++i)
  {
    this->__pProcessAssimpMesh(assimpModelScene->mMeshes[i], assimpModelScene);
  }

  MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "Model root path", this->mModelRootPath);
  this->mInternalModelGeometryResource = assimpModelScene;

  // Output model, submesh, and material information to console.
  this->__pOutputDebugInformationLog();
}

DDyModelInformation::~DDyModelInformation()
{
  MDY_LOG_INFO_D(kModelInformationTemplate, "~DDyModelInformation", "name", this->mModelName);
  if (this->mLinkedModelResourcePtr)
  {
    this->mLinkedModelResourcePtr->__pfLinkModelInformationPtr(nullptr);
  }
  if (this->mInternalModelGeometryResource) { this->mInternalModelGeometryResource = nullptr; }
  this->mAssimpImporter = nullptr;
}

void DDyModelInformation::__pProcessAssimpMesh(aiMesh* mesh, const aiScene* scene)
{
  // Retrieve vertex and indices for element buffer object.
  PDySubmeshInformationDescriptor meshInformationDescriptor;

  this->__pReadVertexData (mesh, meshInformationDescriptor);
  if (mesh->HasBones())
  {
    meshInformationDescriptor.mIsEnabledSkeletalAnimation = true;
    this->__pReadBoneData (mesh, meshInformationDescriptor);
  }
  if (mesh->HasFaces())
  {
    this->__pReadIndiceData (mesh, meshInformationDescriptor);
  }

  // Get Mateiral information. IF not exists, just pass only mesh information.
  // Diffuse map, specular map, height map, ambient map, emissive map etc.

  // If material is not exist, just emplace mesh information.
  if (const auto materialIndex = mesh->mMaterialIndex; materialIndex == 0)
  {
    MDY_LOG_DEBUG_D(kWarnNotHaveMaterial, kModelInformation, kFunc__pProcessAssimpMesh);
    this->mSubmeshInformations.emplace_back(meshInformationDescriptor);
    return;
  }
  else
  {
    // If retrieving material name has been failed, just replace it with model name + "0", "1"...
    // Get texture informations from material with name?
    aiMaterial* material    = scene->mMaterials[materialIndex];
    auto materialDescriptor = this->__pReadMaterialData(material);

    // Create DDySubmeshInformation with material descriptor.
    meshInformationDescriptor.mMaterialName = materialDescriptor.mMaterialName;
    this->mSubmeshInformations.emplace_back(meshInformationDescriptor);

    if (const auto it = std::find(MDY_BIND_BEGIN_END(this->mBindedMaterialName), materialDescriptor.mMaterialName);
        it == this->mBindedMaterialName.end())
    {
      this->mBindedMaterialName.emplace_back(materialDescriptor.mMaterialName);
    }
  }
}

void DDyModelInformation::__pReadVertexData(const aiMesh* mesh, PDySubmeshInformationDescriptor& desc)
{
  constexpr int32_t mtThresholdSize = 1'000;
  constexpr int32_t mtTaskCount     = 4;
  ///
  /// @brief Read geomtery information from mesh.
  /// Position, normal, texture coordination(UV0), --tangent, bitangent--.
  ///
  static auto pReadGeometryInformation = [](const aiMesh* mesh, PDySubmeshInformationDescriptor& desc, int32_t start, int32_t to) {
    for (int32_t i = start; i < to; ++i)
    {
      DDyVertexInformation vertexInformation;
      if (mesh->HasPositions()) vertexInformation.mPosition = mesh->mVertices[i];
      if (mesh->HasNormals())   vertexInformation.mNormal = mesh->mNormals[i];

      // Only supports texture coords 0 set (UV0) for now.
      if (mesh->HasTextureCoords(0))
      {
        const auto& assimpTextureCoord = mesh->mTextureCoords[0][i];
        vertexInformation.mTexCoords = DDyVector2{ assimpTextureCoord.x, assimpTextureCoord.y };
      }

      // また骨、Tangent, Bitangentはしない。

      // Insert vertex information to vertice container of descriptor container.
      desc.mVertices[i] = vertexInformation;
    }
    return true;
  };

  // Retrieve vertex buffer informations of this mesh.
  desc.mVertices.resize(mesh->mNumVertices);

  if (mesh->mNumVertices >= mtThresholdSize)
  {
    // Do parallel
    FDyThreadPool                   pool(mtTaskCount);
    std::vector<std::future<bool>>  results;

    const auto vertexCountSize = static_cast<int32_t>(std::ceil(static_cast<float>(mesh->mNumVertices) / mtTaskCount));
    {
      TI32 start = 0, to = vertexCountSize;
      for (int32_t i = 0; i < mtTaskCount; ++i)
      {
        results.emplace_back(pool.Enqueue(pReadGeometryInformation, mesh, std::ref(desc), start, to));

        start += vertexCountSize;
        to    += vertexCountSize;
        if (to >= static_cast<int32_t>(mesh->mNumVertices)) { to = mesh->mNumVertices; }
      }
    }

    // Check result. (needs to check this routine not culled by optimization in release mode).
    for (auto& result : results)
    {
      if (auto flag = result.get()) { MDY_LOG_DEBUG_D("Vertex retriviation job task result {}", flag); }
    }
  }
  else
  {
    // Do sequential
    for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
    {
      DDyVertexInformation vertexInformation;
      if (mesh->HasPositions()) vertexInformation.mPosition = mesh->mVertices[i];
      if (mesh->HasNormals())   vertexInformation.mNormal   = mesh->mNormals[i];

      // Only supports texture coords 0 set (UV0) for now.
      if (mesh->HasTextureCoords(0))
      {
        const auto& assimpTextureCoord = mesh->mTextureCoords[0][i];
        vertexInformation.mTexCoords = DDyVector2{assimpTextureCoord.x, assimpTextureCoord.y};
      }

      // @todo また骨、Tangent, Bitangentはしない。

      // Insert vertex information to vertice container of descriptor container.
      desc.mVertices[i] = vertexInformation;
    }
  }
}

void DDyModelInformation::__pReadBoneData(const aiMesh* mesh, PDySubmeshInformationDescriptor& desc)
{
  ///
  /// @brief Add {boneId, weight} to vacant bone data slot of vertexBoneData.
  ///
  static auto pAddBoneData = [](DDyVertexBoneData& vertexBoneData, int32_t boneId, float weight)
  {
    for (TU32 i = 0; i < 4; ++i)
    {
      if (vertexBoneData.mWeights[i] == 0.0f)
      {
        vertexBoneData.mBoneId[i] = boneId;
        vertexBoneData.mWeights[i] = weight;
        return;
      }
    }
  };

  for (TU32 i = 0; i < mesh->mNumBones; ++i)
  {
    TI32              boneId    = MDY_NOT_INITIALIZED_0;
    const std::string boneName  = mesh->mBones[i]->mName.C_Str();

    if (const auto it = this->mBoneStringBoneIdMap.find(boneName); it != this->mBoneStringBoneIdMap.end()) { boneId = it->second; }
    else
    {
      boneId = this->mModelBoneTotalCount;
      DDyGeometryBoneInformation boneInformation;
      boneInformation.mBoneOffsetMatrix = mesh->mBones[i]->mOffsetMatrix;

      this->mOverallModelBoneInformations.emplace_back(boneInformation);
      this->mBoneStringBoneIdMap.try_emplace(boneName, boneId);
      ++this->mModelBoneTotalCount;
    }

    for (TU32 j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
    {
      const int32_t applyVertexId = mesh->mBones[i]->mWeights[j].mVertexId;
      const float   applyWeight   = mesh->mBones[i]->mWeights[j].mWeight;
      pAddBoneData(desc.mVertices[applyVertexId].mVertexBoneData, boneId, applyWeight);
    }
  }
}

void DDyModelInformation::__pReadIndiceData(const aiMesh* mesh, PDySubmeshInformationDescriptor& desc)
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
}

PDyMaterialConstructionDescriptor DDyModelInformation::__pReadMaterialData(const aiMaterial* material)
{
  aiString materialName  = {};
  if (const auto ret = material->Get(AI_MATKEY_NAME, materialName); ret == AI_FAILURE)
  {
    PHITOS_UNEXPECTED_BRANCH();
  }

  // Get material textures information.
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
  // @todo Normal map (tangent space) will be recognized as Height map, not Normal map. so need to rearrangement.
  if (const auto opHeight = __pLoadMaterialTextures(material, EDyTextureMapType::Height); opHeight.has_value())
  {
    const auto& heightMaps = opHeight.value();
    textureNames.insert(textureNames.end(), heightMaps.begin(), heightMaps.end());
  }
  materialDescriptor.mTextureNames = textureNames;

  // Let InformationManager initialize material information instance.
  auto& manInfo = MDyDataInformation::GetInstance();
  if (const auto ptr = manInfo.GetMaterialInformation(materialDescriptor.mMaterialName); !ptr)
  {
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateMaterialInformation(materialDescriptor));
  }
  else
  {
    MDY_LOG_WARNING_D(kWarnDuplicatedMaterialName, kModelInformation, kFunc__pReadMaterialData,
                      kFunc__pReadMaterialData, materialDescriptor.mMaterialName);
  }

  return materialDescriptor;
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
      textureDesc.mIsEnabledCustomedTextureParameter  = false;
      textureDesc.mIsEnabledAbsolutePath              = true;
      textureDesc.mIsEnabledCreateMipmap              = true;
      textureDesc.mTextureMapType                     = type;

      MDY_CALL_ASSERT_SUCCESS(manInfo.CreateTextureInformation(textureDesc));
      this->mTextureLocalPaths.emplace_back(textureDesc.mTextureFileLocalPath);
    }

    MDY_LOG_DEBUG_D("{}::{} | Texture Name : {}", "DDyModelInformation", "__pLoadMaterialTextures", textureName);
    MDY_LOG_DEBUG_D("{}::{} | Map Type : {}",     "DDyModelInformation", "__pLoadMaterialTextures", DyGetDebugStringFromEDyMapType(type).data());
    textureInformationString.emplace_back(textureName);
  }

  return textureInformationString;
}

void DDyModelInformation::__pOutputDebugInformationLog()
{
#if defined(_DEBUG) || !defined(NDEBUG)
  for (auto i = 0; i < this->mBindedMaterialName.size(); ++i)
  {
    MDY_LOG_DEBUG_D(kModelInformationNumbTemplate, kModelInformation.data(), "base material name", i, this->mBindedMaterialName[i]);
  }

  for (auto i = 0; i < this->mTextureLocalPaths.size(); ++i)
  {
    MDY_LOG_DEBUG_D(kModelInformationNumbTemplate, kModelInformation.data(), "innate texture name", i, this->mTextureLocalPaths[i]);
  }

  for (auto i = 0; i < this->mSubmeshInformations.size(); ++i)
  {
    const auto& submeshInfo = this->mSubmeshInformations[i].GetInformation();
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

} /// ::dy namespace