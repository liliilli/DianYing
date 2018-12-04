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
#include <Dy/Core/Resource/Information/ModelInformation.h>

#include <future>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Dy/Core/Resource/Resource/ModelResource.h>
#include <Dy/Core/Resource/Internal/GeometryType.h>
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Helper/ThreadPool.h>
#include <Dy/Helper/IoHelper.h>
#include <Dy/Meta/Information/ModelMetaInformation.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kModelInformationTemplate,     "{} | Model information {} : {}");
MDY_SET_IMMUTABLE_STRING(kModelInformationNumbTemplate, "{} | Model information {} No.{} : {}");
MDY_SET_IMMUTABLE_STRING(kWarnNotHaveMaterial,          "{}::{} | This mesh does not have meterial information.");
MDY_SET_IMMUTABLE_STRING(kErrorModelFailedToRead,       "{} | Failed to create read model scene.");
MDY_SET_IMMUTABLE_STRING(kModelInformation,             "DDyModelInformation");
MDY_SET_IMMUTABLE_STRING(kFunc__pProcessAssimpMesh,     "__pProcessMeshInformation");
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

DDyModelInformation::DDyModelInformation(const PDyModelInstanceMetaInfo& modelConstructionDescriptor)
{
  if (modelConstructionDescriptor.mSourceType == EDyResourceSource::Builtin)
  {
    this->mModelName = modelConstructionDescriptor.mSpecifierName;
    auto& descriptor = *modelConstructionDescriptor.mPtrBuiltinModelBuffer;
    for (const auto& submeshInformation : descriptor.mSubmeshConstructionInformations)
    {
      this->mSubmeshInformations.emplace_back(submeshInformation);
    }
  }
  else
  {
#ifdef false
    // Insert name and check name is empty or not.
    this->mModelName = modelConstructionDescriptor.mSpecifierName;
    MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "Model name", this->mModelName);

    // Load model information, if failed throw exception outside afterward free scene.
    const auto& modelPath = modelConstructionDescriptor.mExternalModelPath;
    MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "Model full path", modelPath);

    auto mAssimpImporter = std::make_unique<Assimp::Importer>();
    const aiScene* assimpModelScene = mAssimpImporter->ReadFile(modelPath.c_str(),
      aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_GenNormals);

    if (!assimpModelScene ||
      !assimpModelScene->mRootNode ||
      MDY_BITMASK_FLAG_TRUE(assimpModelScene->mFlags, AI_SCENE_FLAGS_INCOMPLETE))
    {
      mAssimpImporter = nullptr;
      MDY_LOG_CRITICAL_D(kErrorModelFailedToRead, kModelInformation);
      throw std::runtime_error("Could not load model " + modelConstructionDescriptor.mSpecifierName + ".");
    }

    this->mModelRootPath = modelPath.substr(0, modelPath.find_last_of('/'));
    this->mGlobalTransform = assimpModelScene->mRootNode->mTransformation;

    if (assimpModelScene->HasAnimations())
    { // Make animation informations from aiScene.
      MDY_LOG_DEBUG_D("DDyModelInformation | Model : {} Has animations", this->mModelName);
      this->pCreateAnimationInformation(*assimpModelScene);
    }

    // Process all meshes and retrieve material, bone, etc information.
    pProcessNode(*assimpModelScene, *assimpModelScene->mRootNode);

    // Create nodes instead of aiScene->aiNode* dependency.
    this->mRootBoneNode.mName = "RootBone";
    this->pCreateNodeInformation(*assimpModelScene->mRootNode, this->mRootBoneNode);

    // Output model, submesh, and material information to console.
    MDY_LOG_INFO_D(kModelInformationTemplate, kModelInformation, "Model root path", this->mModelRootPath);
    this->__pOutputDebugInformationLog();
#endif
  }

  bool atmFalse = false;
  while (!this->mModelInformationLoaded.compare_exchange_weak(atmFalse, true));
}

DDyModelInformation::~DDyModelInformation()
{
  MDY_LOG_INFO_D(kModelInformationTemplate, "~DDyModelInformation", "name", this->mModelName);

  if (this->__mLinkedModelResourcePtr)      { this->__mLinkedModelResourcePtr->__pfResetModelInformationLink(); }
}

#ifdef false
void DDyModelInformation::pCreateAnimationInformation(const aiScene& aiScene)
{
  for (TU32 i = 0; i < aiScene.mNumAnimations; ++i)
  {
    DMoeAnimationInformation tempAnim;
    tempAnim.mName          = aiScene.mAnimations[i]->mName.data;
    tempAnim.mDuration      = static_cast<float>(aiScene.mAnimations[i]->mDuration);
    tempAnim.mTickPerSecond = static_cast<float>(aiScene.mAnimations[i]->mTicksPerSecond);

    // Create animation channel information from aiNodeAnim;
    for (TU32 y = 0; y < aiScene.mAnimations[i]->mNumChannels; ++y)
    {
      const auto& aiAnimationChannel = *aiScene.mAnimations[i]->mChannels[y];

      DMoeAnimationInformation::DAnimChannel tempChannel;
      // Same to arbitary aiNode that points one aiMesh information and bone.
      // aiMesh and aiBone's name may be not same each other.
      tempChannel.mName = aiAnimationChannel.mNodeName.data;

      for (TU32 z = 0; z < aiAnimationChannel.mNumPositionKeys; ++z)
      {
        tempChannel.mPositionKeys.emplace_back(aiAnimationChannel.mPositionKeys[z].mValue);
        tempChannel.mPositionTime.emplace_back(static_cast<float>(aiAnimationChannel.mPositionKeys[z].mTime));
      }

      for (TU32 z = 0; z < aiAnimationChannel.mNumRotationKeys; ++z)
      {
        tempChannel.mRotationKeys.emplace_back(aiAnimationChannel.mRotationKeys[z].mValue);
        tempChannel.mRotationTime.emplace_back(static_cast<float>(aiAnimationChannel.mRotationKeys[z].mTime));
      }

      for (TU32 z = 0; z < aiAnimationChannel.mNumScalingKeys; ++z)
      {
        tempChannel.mScalingKeys.emplace_back(aiAnimationChannel.mScalingKeys[z].mValue);
        tempChannel.mScalingTime.emplace_back(static_cast<float>(aiAnimationChannel.mScalingKeys[z].mTime));
      }

      tempAnim.mAnimationChannels.emplace_back(tempChannel);
    }

    // After creating animation channel information and insert it to animInfo,
    // insert it to also.
    this->mAnimationInformations.emplace_back(tempAnim);
  }
}

void DDyModelInformation::pProcessNode(const aiScene& aiScene, const aiNode& aiNode)
{
  MDY_LOG_INFO_D("pProcessNode | Processing a node | Model name : {}", this->mModelName);

  for (TU32 i = 0 ; i < aiNode.mNumMeshes; ++i)
  {
    __pProcessMeshInformation(aiScene, aiNode, *aiScene.mMeshes[aiNode.mMeshes[i]]);
  }

  for (TU32 i = 0; i < aiNode.mNumChildren; ++i)
  {
    pProcessNode(aiScene, *aiNode.mChildren[i]);
  }
}

void DDyModelInformation::__pProcessMeshInformation(const aiScene& aiScene, const aiNode& aiNode, const aiMesh& mesh)
{
  // Retrieve vertex and indices for element buffer object.
  PDySubmeshInformationDescriptor meshInformationDescriptor = {};

  if (mesh.HasPositions() || mesh.HasNormals() || mesh.HasTextureCoords(0))
  { // Create position, normals, texture coordinates [UV0] from aiMesh.
    this->__pReadVertexData (mesh, meshInformationDescriptor);
  }

  if (mesh.HasBones())
  { // Create bone information    from aiMesh.
    meshInformationDescriptor.mIsEnabledSkeletalAnimation = true;
    this->__pReadBoneData (mesh, meshInformationDescriptor);
  }

  if (mesh.HasFaces())
  { // Create indice information  from aiMesh.
    this->__pReadIndiceData (mesh, meshInformationDescriptor);
  }

  if (aiNode.mParent)
  { // If parent is exist, calculate dultiplied matrix with parent node's transformation.
    meshInformationDescriptor.mBaseModelMatrix = aiNode.mParent->mTransformation * aiNode.mTransformation;
  }
  else { meshInformationDescriptor.mBaseModelMatrix = aiNode.mTransformation; }

  // Get Material information. IF not exists, just pass only mesh information.
  // Diffuse map, specular map, height map, ambient map, emissive map etc.

  // If material is not exist, just emplace mesh information.
  if (const auto materialIndex = mesh.mMaterialIndex; materialIndex == 0)
  {
    MDY_LOG_DEBUG_D(kWarnNotHaveMaterial, kModelInformation, kFunc__pProcessAssimpMesh);
    this->mSubmeshInformations.emplace_back(meshInformationDescriptor);
  }
  else
  {
    // If retrieving material name has been failed, just replace it with model name + "0", "1"...
    // Get texture informations from material with name?
    const aiMaterial& material  = *aiScene.mMaterials[materialIndex];
    auto materialDescriptor     = this->__pReadMaterialData(material);

    // Create DDySubmeshInformation with material descriptor.
    meshInformationDescriptor.mMaterialName = materialDescriptor.mSpecifierName;
    this->mSubmeshInformations.emplace_back(meshInformationDescriptor);

    if (const auto it = std::find(MDY_BIND_BEGIN_END(this->mOverallBindedMaterialName), materialDescriptor.mSpecifierName);
        it == this->mOverallBindedMaterialName.end())
    {
      this->mOverallBindedMaterialName.emplace_back(materialDescriptor.mSpecifierName);
    }
  }
}

void DDyModelInformation::__pReadVertexData(const aiMesh& mesh, PDySubmeshInformationDescriptor& desc)
{
  constexpr int32_t mtThresholdSize = 1'000;
  constexpr int32_t mtTaskCount     = 4;
  ///
  /// @brief Read geomtery information from mesh.
  /// Position, normal, texture coordination(UV0), --tangent, bitangent--.
  ///
  static auto pReadGeometryInformation = [](const aiMesh& mesh, PDySubmeshInformationDescriptor& desc, int32_t start, int32_t to) {
    for (int32_t i = start; i < to; ++i)
    {
      DDyVertexInformation vertexInformation;
      if (mesh.HasPositions()) vertexInformation.mPosition = mesh.mVertices[i];
      if (mesh.HasNormals())   vertexInformation.mNormal = mesh.mNormals[i];

      // Only supports texture coords 0 set (UV0) for now.
      if (mesh.HasTextureCoords(0))
      {
        const auto& assimpTextureCoord = mesh.mTextureCoords[0][i];
        vertexInformation.mTexCoords = DDyVector2{ assimpTextureCoord.x, assimpTextureCoord.y };
      }

      // また骨、Tangent, Bitangentはしない。

      // Insert vertex information to vertice container of descriptor container.
      desc.mVertices[i] = vertexInformation;
    }
    return true;
  };

  // Retrieve vertex buffer informations of this mesh.
  desc.mVertices.resize(mesh.mNumVertices);

  if (mesh.mNumVertices >= mtThresholdSize)
  { // Do parallel
    FDyThreadPool                   pool(mtTaskCount);
    std::vector<std::future<bool>>  results;

    const auto vertexCountSize = static_cast<int32_t>(std::ceil(static_cast<float>(mesh.mNumVertices) / mtTaskCount));
    {
      TI32 start = 0, to = vertexCountSize;
      for (int32_t i = 0; i < mtTaskCount; ++i)
      {
        results.emplace_back(pool.Enqueue(pReadGeometryInformation, std::ref(mesh), std::ref(desc), start, to));

        start += vertexCountSize;
        to    += vertexCountSize;
        if (to >= static_cast<int32_t>(mesh.mNumVertices)) { to = mesh.mNumVertices; }
      }
    }

    // Check result. (needs to check this routine not culled by optimization in release mode).
    for (auto& result : results)
    {
      if (auto flag = result.get()) { MDY_LOG_DEBUG_D("Vertex retriviation job task result {}", flag); }
    }
  }
  else
  { // Do sequential
    pReadGeometryInformation(mesh, desc, 0, mesh.mNumVertices);
  }
}

void DDyModelInformation::__pReadBoneData(const aiMesh& mesh, PDySubmeshInformationDescriptor& desc)
{
  ///
  /// @brief Add {boneId, weight} to vacant bone data slot of vertexBoneData.
  ///
  static auto pAddBoneDataToVertex = [](DDyVertexBoneData& vertexBoneData, int32_t boneId, float weight)
  {
    for (TU32 i = 0; i < 4; ++i)
    {
      if (vertexBoneData.mBoneId[i] == -1)
      {
        vertexBoneData.mBoneId[i]   = boneId;
        vertexBoneData.mWeights[i]  = weight;
        return;
      }
    }
  };

  // Make bone data to submesh's vertices.
  for (TU32 i = 0; i < mesh.mNumBones; ++i)
  {
    const aiBone&     bone      = *mesh.mBones[i];
    TI32              boneId    = MDY_INITIALIZE_DEFUINT;
    const std::string boneName  = bone.mName.C_Str();

    if (const auto it = this->mBoneIdMap.find(boneName); it != this->mBoneIdMap.end())
    { // Check if bond id is already inserted to map of model information.
      boneId = it->second;
    }
    else
    {
      boneId = static_cast<TI32>(this->mBoneIdMap.size());
      this->mBoneIdMap.try_emplace(boneName, boneId);

      //for (auto& mAnimationChannel : this->mAnimationInformations[0].mAnimationChannels)
      //{
        //if (boneName == mAnimationChannel.mName)
        { // If bondMap called boneName is not binded to the map yet,
          // create information and mapping pointer so insert it to the list.
          // aiMatrix is row but transported to column major of DDyMatrix4x4
          DDyGeometryBoneInformation boneTransformInfo;
          boneTransformInfo.mBoneOffsetMatrix = bone.mOffsetMatrix;
          this->mOverallModelBoneInformations.emplace_back(boneTransformInfo);
        }
      //}
    }

    // Insert bondId (to the mOverallModelBoneInformation) and weight to the submesh information descriptor.
    for (TU32 j = 0; j < bone.mNumWeights; ++j)
    {
      const int32_t applyVertexId = bone.mWeights[j].mVertexId;
      const float   applyWeight   = bone.mWeights[j].mWeight;
      pAddBoneDataToVertex(desc.mVertices[applyVertexId].mVertexBoneData, boneId, applyWeight);
    }
  }
}

void DDyModelInformation::__pReadIndiceData(const aiMesh& mesh, PDySubmeshInformationDescriptor& desc)
{
  desc.mIndices.reserve(mesh.mNumFaces * 3);
  for (uint32_t i = 0; i < mesh.mNumFaces; ++i)
  {
    const aiFace& assimpFace = mesh.mFaces[i];
    for (uint32_t j = 0; j < assimpFace.mNumIndices; ++j)
    {
      desc.mIndices.emplace_back(assimpFace.mIndices[j]);
    }
  }
  desc.mIndices.shrink_to_fit();
}

PDyMaterialInstanceMetaInfo DDyModelInformation::__pReadMaterialData(const aiMaterial& material)
{
  aiString materialName  = {};
  if (const auto ret = material.Get(AI_MATKEY_NAME, materialName); ret == AI_FAILURE)
  {
    MDY_UNEXPECTED_BRANCH();
  }

  // Get material textures information.
  PDyMaterialInstanceMetaInfo materialDescriptor;
  materialDescriptor.mIsShaderLazyInitialized_Deprecated = true;
  materialDescriptor.mSpecifierName            = materialName.C_Str();
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
  materialDescriptor.mTextureNames_Deprecated = textureNames;

  // Let InformationManager initialize material information instance.
  auto& manInfo = MDyIOData::GetInstance();
  if (const auto ptr = manInfo.GetMaterialInformation(materialDescriptor.mSpecifierName); !ptr)
  {
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateMaterialInformation_Deprecated(materialDescriptor));
  }
  else
  {
    MDY_LOG_WARNING_D(kWarnDuplicatedMaterialName, kModelInformation, kFunc__pReadMaterialData,
                      kFunc__pReadMaterialData, materialDescriptor.mSpecifierName);
  }

  return materialDescriptor;
}

std::optional<std::vector<std::string>>
DDyModelInformation::__pLoadMaterialTextures(const aiMaterial& material, EDyTextureMapType type)
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
  auto& manInfo = MDyIOData::GetInstance();

  std::vector<std::string> textureInformationString;
  const uint32_t textureMapCount = material.GetTextureCount(aiTextureType);
  for (uint32_t i = 0; i < textureMapCount; ++i)
  {
    aiString textureLocalPath;
    if (material.GetTexture(aiTextureType, i, &textureLocalPath) != AI_SUCCESS)
    {
      MDY_LOG_ERROR_D("{} | Failed to read texture information from assimp.", "DDyModelInformation::__pLoadMaterialTextures");
      return std::nullopt;
    }

    // Check there is duplicated texture by comparing texture local path.
    const auto dupTexturePath = std::find(MDY_BIND_BEGIN_END(this->mOverallTextureLocalPaths), textureLocalPath.C_Str());
    const auto textureName    = DyGetFileNameFromPath(textureLocalPath.C_Str());
    if (dupTexturePath == this->mOverallTextureLocalPaths.end())
    {
      PDyTextureInstanceMetaInfo textureDesc;
      // Set each texture name to texture file name except for file type like a .png .jpg.
      textureDesc.mSpecifierName                        = textureName;
      textureDesc.mExternalFilePath               = textureLocalPath.C_Str();
      textureDesc.mTextureFileAbsolutePath_Deprecated            = this->mModelRootPath + '/' + textureDesc.mExternalFilePath;
      textureDesc.mTextureType                        = EDyTextureStyleType::D2;
      textureDesc.mIsEnabledCustomedTextureParameter  = false;
      textureDesc.mIsEnabledAbsolutePath_Deprecated              = true;
      textureDesc.mIsUsingDefaultMipmapGeneration     = true;
      textureDesc.mTextureMapType_Deprecated                     = type;

      MDY_CALL_ASSERT_SUCCESS(manInfo.CreateTextureInformation_Deprecated(textureDesc));
      this->mOverallTextureLocalPaths.emplace_back(textureDesc.mExternalFilePath);
    }

    MDY_LOG_DEBUG_D("{}::{} | Texture Name : {}", "DDyModelInformation", "__pLoadMaterialTextures", textureName);
    MDY_LOG_DEBUG_D("{}::{} | Map Type : {}",     "DDyModelInformation", "__pLoadMaterialTextures", DyGetDebugStringFromEDyMapType(type).data());
    textureInformationString.emplace_back(textureName);
  }

  return textureInformationString;
}

void DDyModelInformation::pCreateNodeInformation(const aiNode& aiNode, DMoeBoneNodeInformation& nodeInfo)
{
  if (const auto idIt = this->mBoneIdMap.find(aiNode.mName.data); idIt != this->mBoneIdMap.end())
  {
    DMoeBoneNodeInformation tempBoneNode;
    tempBoneNode.mName          = aiNode.mName.data;
    tempBoneNode.mParentNodePtr = &nodeInfo;
    tempBoneNode.mNodeTransform = aiNode.mTransformation;
    tempBoneNode.mOffsetMatrix  = this->mOverallModelBoneInformations[idIt->second].mBoneOffsetMatrix;

    nodeInfo.mChildrenNodes.emplace_back(std::move(tempBoneNode));

    for (TU32 i = 0; i < aiNode.mNumChildren; ++i)
    { // If the node we just found was a bone node then pass it in.
      this->pCreateNodeInformation(*aiNode.mChildren[i], *nodeInfo.mChildrenNodes.rbegin());
    }
  }
  else
  {
    for (TU32 i = 0; i < aiNode.mNumChildren; ++i)
    { // Else
      this->pCreateNodeInformation(*aiNode.mChildren[i], nodeInfo);
    }
  }
}
#endif

void DDyModelInformation::__pOutputDebugInformationLog()
{
#if defined(_DEBUG) || !defined(NDEBUG)
  for (TU32 i = 0; i < this->mOverallBindedMaterialName.size(); ++i)
  {
    MDY_LOG_DEBUG_D(kModelInformationNumbTemplate, kModelInformation.data(), "base material name", i, this->mOverallBindedMaterialName[i]);
  }

  for (TU32 i = 0; i < this->mOverallTextureLocalPaths.size(); ++i)
  {
    MDY_LOG_DEBUG_D(kModelInformationNumbTemplate, kModelInformation.data(), "innate texture name", i, this->mOverallTextureLocalPaths[i]);
  }

  for (TU32 i = 0; i < this->mSubmeshInformations.size(); ++i)
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