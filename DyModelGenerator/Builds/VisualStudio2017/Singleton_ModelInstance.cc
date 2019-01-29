///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
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
#include "Singleton_ModelInstance.h"
#include <filesystem>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include "Helper_JsonLz4.h"

EDySuccess Singleton_ModelInstance::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess Singleton_ModelInstance::pfRelease()
{
  this->ReleaseModel();
  return DY_SUCCESS;
}

EDySuccess Singleton_ModelInstance::ReadModelWithPath(const std::string& iPath)
{
  if (this->mAssimpModerImporter != nullptr) { this->mAssimpModerImporter = nullptr; }

  // Created `aiScene` will be removed automatically when smtptr of Importer ref counting is 0.
  this->mAssimpModerImporter = std::make_unique<Assimp::Importer>();
  
  const aiScene* ptrModelScene = this->mAssimpModerImporter->ReadFile(iPath,
      aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);
  // Check
  if (ptrModelScene == nullptr
  ||  ptrModelScene->mRootNode == nullptr 
  || (ptrModelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
  {
    this->ReleaseModel();
    return DY_FAILURE; 
  }

  // Insert
  for (unsigned i = 0; i < ptrModelScene->mNumMeshes; ++i)
  { this->mPtrAssimpModelMeshList.emplace_back(ptrModelScene->mMeshes[i]); }

  for (unsigned i = 0; i < ptrModelScene->mNumMaterials; ++i)
  { this->mPtrAssimpModelMaterialList.emplace_back(ptrModelScene->mMaterials[i]); }

  for (unsigned i = 0; i < ptrModelScene->mNumAnimations; ++i)
  { this->mPtrAssimpModelAnimList.emplace_back(ptrModelScene->mAnimations[i]); }

  for (unsigned i = 0; i < ptrModelScene->mNumTextures; ++i)
  { this->mPtrAssimpModelTextureList.emplace_back(ptrModelScene->mTextures[i]);}

  this->mAssimpModeNode    = std::make_unique<Data_AssimpModelNode>(DyMakeNotNull(ptrModelScene->mRootNode));
  this->mModelFileFullPath = iPath;
  return DY_SUCCESS;
}

EDySuccess Singleton_ModelInstance::ExportModelMesh(const std::string& iSpecifier, unsigned iMeshIndex, bool withSkeleton, bool isCompressed)
{
  if (iMeshIndex >= this->GetNumModelMeshes()) { return DY_FAILURE; }

  // Make serialized string form mesh instance.
  const auto meshInstance = CreateDyMesh(iMeshIndex, withSkeleton);
  
  // Get a directory path from model file full path.
  namespace fs = std::filesystem;
  const auto directoryPath = fs::path{this->mModelFileFullPath}.parent_path();

  if (isCompressed == true)
  {
#ifdef false
    const auto buffer = CompressStringBuffer(meshSerializedString);

    // Write file. `File` is RAII.
    const fs::path meshPath = fmt::format("{}/{}.{}", directoryPath.string(), iSpecifier, "dyMesh");
    std::FILE* fdFile = fopen(meshPath.string().c_str(), "wb");
    fwrite(&buffer.mRawBufferBytes, sizeof(unsigned), 1, fdFile);
    fwrite(&buffer.mCompressedBufferBytes, sizeof(unsigned), 1, fdFile);
    fwrite(buffer.mCompressedBuffer.data(), sizeof(char), buffer.mCompressedBufferBytes, fdFile);
    fclose(fdFile);
#endif
  }
  else
  {
    const fs::path meshPath = fmt::format("{}/{}.{}", directoryPath.string(), iSpecifier, "dybMesh");

    // Write file.
    std::FILE* fdFile = fopen(meshPath.string().c_str(), "wb");
    {
      // Write index count.
      const auto sz = static_cast<unsigned>(meshInstance.mIndexList.size());
      fwrite(&sz, sizeof(unsigned), 1, fdFile);
      // Write index list.
      for (unsigned i = 0; i < sz; ++i) { fwrite(&meshInstance.mIndexList[i], sizeof(unsigned), 1, fdFile); }
      // Write header of vertex list.
    }
    {
      // Write vertex count.
      const auto sz = static_cast<unsigned>(meshInstance.mVertexList.size());
      fwrite(&sz, sizeof(unsigned), 1, fdFile);
      // Write vertex list.
      for (unsigned v = 0; v < sz; ++v)
      {
        const auto& vi = meshInstance.mVertexList[v];
        fwrite(&vi.mPosition[0], sizeof(float), 3, fdFile);   // position.
        fwrite(&vi.mNormal[0], sizeof(float), 3, fdFile);     // normal.
        fwrite(&vi.mTexCoords0[0], sizeof(float), 2, fdFile); // uv0.
        fwrite(&vi.mTexCoords1[0], sizeof(float), 2, fdFile); // uv1.
        fwrite(&vi.mTangent[0], sizeof(float), 3, fdFile);    // tangent.
        fwrite(&vi.mBitangent[0], sizeof(float), 3, fdFile);  // bitangent.
        fwrite(&vi.mBoneId[0], sizeof(DDyVectorInt4), 1, fdFile); // boneId.
        fwrite(&vi.mWeights[0], sizeof(DDyVector4), 1, fdFile);   // weights.
      }
    }
    fclose(fdFile);
  }

  return DY_SUCCESS;
}

DMesh Singleton_ModelInstance::CreateDyMesh(unsigned iMeshIndex, bool withSkeleton)
{
  const auto& ptrAiMesh = this->mPtrAssimpModelMeshList[iMeshIndex];
  DMesh result;

  // If mesh has faces, so can create indices from this.
  if (ptrAiMesh->HasFaces() == true)
  { 
    result.mIndexList = CreateMeshIndices(ptrAiMesh);
  }

  //!
  //! Vertex
  //!

  if (ptrAiMesh->HasPositions() == true)
  { // If mesh has position (vert)
    const auto numVert = ptrAiMesh->mNumVertices;
    result.mVertexList.reserve(numVert);
    
    // Insert vertex information (vert, norm, tex0, tang, bitan)
    for (unsigned i = 0; i < numVert; ++i)
    { 
      DDyVertexInformation vertexInfo;
      
      this->TryInsertMeshVertex(i, ptrAiMesh, vertexInfo);  // Vertex
      this->TryInsertMeshNormal(i, ptrAiMesh, vertexInfo);  // Normal
      this->TryInsertMeshUV0(i, ptrAiMesh, vertexInfo);     // Texcoord0 (u,v)
      this->TryInsertMeshUV1(i, ptrAiMesh, vertexInfo);     // Texcoord1 (u,v)
      this->TryInsertMeshTanBt(i, ptrAiMesh, vertexInfo);   // Tangent & Bitangent
      // Insert item to result.
      result.mVertexList.emplace_back(std::move(vertexInfo));
    }

    // Bone & Weight
    if (withSkeleton == true && ptrAiMesh->HasBones() == true)
    {
      for (auto aiBondIndex = 0u, numBonds = ptrAiMesh->mNumBones; aiBondIndex < numBonds; ++aiBondIndex)
      {
        const auto* ptrAiBone = ptrAiMesh->mBones[aiBondIndex];

        // Find appropriate skeleton bone in ExportedSkeleton list using matching name.
        auto itSkeletonBone = std::find_if(
            this->mExportedSkeleton.cbegin(), this->mExportedSkeleton.cend(),
            [ptrAiBone](const DSkeletonBone& iBone) { return iBone.mSpecifier == ptrAiBone->mName.C_Str(); });
        jassert(itSkeletonBone != this->mExportedSkeleton.cend());

        // Get a proper id list from retrieved skeleton bone instance.
        const auto indexSkeletonBone = static_cast<signed>(std::distance(this->mExportedSkeleton.cbegin(), itSkeletonBone));
        for (unsigned idWeight = 0, numWeights = ptrAiBone->mNumWeights; idWeight < numWeights; ++idWeight)
        {
          // Traverse bone's weight.
          const auto& ptrWeight = ptrAiBone->mWeights[idWeight];
          const auto  idVertex  = ptrWeight.mVertexId;
          const auto  valWeight = ptrWeight.mWeight;

          // Insert each vertex with boneId (of `mExportedSkeleton`) and weights within 4 element list.
          auto& refVertex = result.mVertexList[idVertex];
          for (unsigned id = 0; id < 4; ++id)
          {
            if (refVertex.mBoneId[id] == -1)
            {
              refVertex.mBoneId[id]   = indexSkeletonBone;
              refVertex.mWeights[id]  = valWeight;
              break;
            }
          }
          //
        }
        //
      }
    }
  }

  return result;
}

std::vector<unsigned> Singleton_ModelInstance::CreateMeshIndices(NotNull<const aiMesh*> ptrAiMesh)
{
  const auto faceNum = ptrAiMesh->mNumFaces;
  std::vector<unsigned> result;

  // Because we have triangulate indexes, so have to set up memory space with 3 times.
  result.reserve(faceNum * 3);
  for (unsigned i = 0; i < faceNum; ++i)
  {
    const auto& ptrFace = ptrAiMesh->mFaces[i];
    jassert(ptrFace.mNumIndices == 3);

    // Insert face as indices (a, b, c).
    for (unsigned id = 0; id < 3; ++id)
    {
      result.emplace_back(ptrFace.mIndices[id]);
    }
  }

  return result;
}

void Singleton_ModelInstance::TryInsertMeshVertex(unsigned iIndex, NotNull<const aiMesh*> iPtrAiMesh, DDyVertexInformation& oResult)
{
  for (unsigned v = 0; v < 3; ++v)
  {
    oResult.mPosition[v] = iPtrAiMesh->mVertices[iIndex][v];
  }

}

void Singleton_ModelInstance::TryInsertMeshNormal(unsigned iIndex, NotNull<const aiMesh*> iPtrAiMesh, DDyVertexInformation& oResult)
{
  if (iPtrAiMesh->HasNormals() == true)
  {
    for (unsigned n = 0; n < 3; ++n)
    {
      oResult.mNormal[n] = iPtrAiMesh->mNormals[iIndex][n];
    }
  }
}

void Singleton_ModelInstance::TryInsertMeshUV0(unsigned iIndex, NotNull<const aiMesh*> iPtrAiMesh, DDyVertexInformation& oResult)
{
  if (iPtrAiMesh->HasTextureCoords(0) == true)
  {
    jassert(iPtrAiMesh->mNumUVComponents[0] == 2);
    for (unsigned uv = 0; uv < 2; ++uv)
    {
      oResult.mTexCoords0[uv] = iPtrAiMesh->mTextureCoords[0][iIndex][uv];
    }
  }
}

void Singleton_ModelInstance::TryInsertMeshUV1(unsigned iIndex, NotNull<const aiMesh*> iPtrAiMesh, DDyVertexInformation& oResult)
{
  if (iPtrAiMesh->HasTextureCoords(1) == true)
  {
    jassert(iPtrAiMesh->mNumUVComponents[1] == 2);
    for (unsigned uv = 0; uv < 2; ++uv)
    {
      oResult.mTexCoords1[uv] = iPtrAiMesh->mTextureCoords[1][iIndex][uv];
    }
  }
}

void Singleton_ModelInstance::TryInsertMeshTanBt(unsigned iIndex, NotNull<const aiMesh*> iPtrAiMesh, DDyVertexInformation& oResult)
{
  if (iPtrAiMesh->HasTangentsAndBitangents() == true)
  {
    for (unsigned t = 0; t < 3; ++t)
    {
      oResult.mTangent[t] = iPtrAiMesh->mTangents[iIndex][t];
      oResult.mBitangent[t] = iPtrAiMesh->mBitangents[iIndex][t];
    }
  }
}

EDySuccess Singleton_ModelInstance::ExportModelSkeleton(const std::string& iSpecifier, bool isCompressed)
{
  // Make serialized string form mesh instance.
  nlohmann::json jsonMeshAtlas    = this->mExportedSkeleton;
  const auto meshSerializedString = jsonMeshAtlas.dump();
  
  // Get a directory path from model file full path.
  namespace fs = std::filesystem;
  const auto directoryPath = fs::path{this->mModelFileFullPath}.parent_path();

  if (isCompressed == true)
  {
    const auto buffer = CompressStringBuffer(meshSerializedString);

    // Write file. `File` is RAII.
    const fs::path meshPath = fmt::format("{}/{}.{}", directoryPath.string(), iSpecifier, "dySkel");
    std::FILE* fdFile = fopen(meshPath.string().c_str(), "wb");
    fwrite(&buffer.mRawBufferBytes, sizeof(unsigned), 1, fdFile);
    fwrite(&buffer.mCompressedBufferBytes, sizeof(unsigned), 1, fdFile);
    fwrite(buffer.mCompressedBuffer.data(), sizeof(char), buffer.mCompressedBufferBytes, fdFile);
    fclose(fdFile);
  }
  else
  {
    const fs::path skelPath = fmt::format("{}/S_{}.{}", directoryPath.string(), iSpecifier, "json");

    // Write file.
    std::FILE* fdFile = fopen(skelPath.string().c_str(), "w");
    fwrite(meshSerializedString.c_str(), 
        sizeof(decltype(meshSerializedString)::value_type), 
        meshSerializedString.size(), 
        fdFile);
    fclose(fdFile);
  }

  return DY_SUCCESS;
}

EDySuccess Singleton_ModelInstance::ExportModelMaterials(const std::string& iSpecifier, unsigned iMatIndex, bool isCompressed)
{
  if (iMatIndex >= this->GetNumModelMeshes()) { return DY_FAILURE; }

  auto result = CreateDyMaterial(iMatIndex);
  // Add each texture specifier with prefix, `iSpecifier`.
  for (auto& [specifier, type] : result.mTextureSpecifierList)
  {
    specifier = fmt::format("{}_{}", iSpecifier, specifier);
  }
  // Make serialized string form mesh instance.
  nlohmann::json jsonMatAtlas    = result; 
  const auto matSerializedString = jsonMatAtlas.dump();
  
  // Get a directory path from model file full path.
  namespace fs = std::filesystem;
  const auto directoryPath = fs::path{this->mModelFileFullPath}.parent_path();

  if (isCompressed == true)
  {
    const auto buffer = CompressStringBuffer(matSerializedString);

    // Write file. `File` is RAII.
    const fs::path meshPath = fmt::format("{}/{}.{}", directoryPath.string(), iSpecifier, "dyMat");
    std::FILE* fdFile = fopen(meshPath.string().c_str(), "wb");
    fwrite(&buffer.mRawBufferBytes, sizeof(unsigned), 1, fdFile);
    fwrite(&buffer.mCompressedBufferBytes, sizeof(unsigned), 1, fdFile);
    fwrite(buffer.mCompressedBuffer.data(), sizeof(char), buffer.mCompressedBufferBytes, fdFile);
    fclose(fdFile);
  }
  else
  {
    const fs::path meshPath = fmt::format("{}/M_{}.{}", directoryPath.string(), iSpecifier, "json");

    // Write file.
    std::FILE* fdFile = fopen(meshPath.string().c_str(), "w");
    fwrite(matSerializedString.c_str(), sizeof(decltype(matSerializedString)::value_type), matSerializedString.size(), fdFile);
    fclose(fdFile);
  }

  return DY_SUCCESS;
}

EDySuccess Singleton_ModelInstance::ExportModelAnimation(const std::string& iSpecifier, unsigned iMatIndex, bool isCompressed)
{
  if (iMatIndex >= this->GetNumModelAnimations()) { return DY_FAILURE; }

  auto result = CreateDyAnimation(iMatIndex);

  // Get a directory path from model file full path.
  namespace fs = std::filesystem;
  const auto directoryPath = fs::path{this->mModelFileFullPath}.parent_path();

  if (isCompressed == true)
  {
    std::FILE* fdFile = fopen(iSpecifier.c_str(), "w");
    fclose(fdFile);
  }
  else
  {
    const auto ptr = iSpecifier.find_last_of('|');
    fs::path path;
    if (ptr != std::string::npos)
    {
      const auto specifier = iSpecifier.substr(ptr + 1, std::string::npos);
      path = fmt::format("{}/MA_{}.{}", directoryPath.string(), specifier, "dyMAnim");
    }
    else
    {
      path = fmt::format("{}/MA_{}.{}", directoryPath.string(), iSpecifier, "dyMAnim");
    }

    // Write file.
    std::FILE* fdFile = fopen(path.string().c_str(), "wb");
    
    // Write `mAnimationHeader` of given animation. 
    fwrite(&result.mAnimationHeader, sizeof(result.mAnimationHeader), 1, fdFile);

    // Write the number of animation node list.
    const auto numAnimNode = static_cast<unsigned>(result.mAnimationNodeList.size());
    fwrite(&numAnimNode, sizeof(unsigned), 1, fdFile);

    // Write `mAnimationNodeList` of given animation.
    for (unsigned i = 0; i < numAnimNode; ++i)
    {
      const auto& refAnimNode = result.mAnimationNodeList[i];
      // Write `mExportSkeleton` bone id.
      fwrite(&refAnimNode.mSkeletonBoneId, sizeof(unsigned), 1, fdFile);

      // Write the number of position, rotation (xyzw) and scale also.
      const auto numPosition  = static_cast<unsigned>(refAnimNode.mPositionList.size());
      const auto numRotation  = static_cast<unsigned>(refAnimNode.mRotationList.size());
      const auto numScale     = static_cast<unsigned>(refAnimNode.mScaleList.size());
      fwrite(&numPosition, sizeof(unsigned), 1, fdFile);
      fwrite(&numRotation, sizeof(unsigned), 1, fdFile);
      fwrite(&numScale, sizeof(unsigned), 1, fdFile);

      // Write position list of refAnimNode.
      for (unsigned idPos = 0, numPos = static_cast<unsigned>(refAnimNode.mPositionList.size()); idPos < numPos; ++idPos)
      {
        const auto& refPos = refAnimNode.mPositionList[idPos];
        fwrite(&refPos.mStartSecond, sizeof(refPos.mStartSecond), 1, fdFile);
        fwrite(&refPos.mTranslate[0], sizeof(float), 3, fdFile);
      }
      // Write rotation (x, y, z, w) quaternion list of refAnimNode.
      for (unsigned idRot = 0, numRot = static_cast<unsigned>(refAnimNode.mRotationList.size()); idRot < numRot; ++idRot)
      {
        const auto& refPos = refAnimNode.mRotationList[idRot];
        fwrite(&refPos, sizeof(refPos), 1, fdFile);
      }
      // Write scaling list of refAnimNode.
      for (unsigned idScl = 0, numScl = static_cast<unsigned>(refAnimNode.mScaleList.size()); idScl < numScl; ++idScl)
      {
        const auto& refPos = refAnimNode.mScaleList[idScl];
        fwrite(&refPos.mStartSecond, sizeof(refPos.mStartSecond), 1, fdFile);
        fwrite(&refPos.mScale[0], sizeof(float), 3, fdFile);
      }
    }
    fclose(fdFile);
  }

  return DY_SUCCESS;
}

DMaterial Singleton_ModelInstance::CreateDyMaterial(unsigned iMatIndex)
{
  const auto& ptrAiMaterial = this->mPtrAssimpModelMaterialList[iMatIndex];
  DMaterial result;

  // Get texture list.
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_DIFFUSE, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_SPECULAR, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_AMBIENT, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_EMISSIVE, result);
  // @TODO [CAUTION] Bump-map(Normal) is loaded as Height-map, when leading .obj file.
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_HEIGHT, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_NORMALS, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_SHININESS, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_OPACITY, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_DISPLACEMENT, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_LIGHTMAP, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_REFLECTION, result);
  TryInsertTextureSpecifier(ptrAiMaterial, aiTextureType_UNKNOWN, result);

  // Set blend mode when opacity map is applied.
  result.mBlendMode = decltype(result.mBlendMode)::Opaque;
  for (const auto& [specifier, type] : result.mTextureSpecifierList)
  {
    if (type == EDyTextureMapType::Opacity)
    { result.mBlendMode = decltype(result.mBlendMode)::TranslucentOIT; }
  }

  return result;
}

void Singleton_ModelInstance::TryInsertTextureSpecifier(NotNull<const aiMaterial*> iPtrAiMaterial, aiTextureType iTextureType, DMaterial& iRefMaterial)
{
  for (unsigned texId = 0, numTexture = iPtrAiMaterial->GetTextureCount(iTextureType); 
       texId < numTexture; ++texId)
  {
    aiString aiPathToTexture;
    const auto returnVal = iPtrAiMaterial->GetTexture(iTextureType, texId, &aiPathToTexture);
    
    // Get texture specifier name from given file path.
    const std::string pathToTexture = aiPathToTexture.C_Str();
    const auto idTypeSpecifierSep   = pathToTexture.find_last_of('.');
    const auto idTypeStartSep       = pathToTexture.find_last_of(R"(/\)") + 1;

    const std::string textureSpecifier = pathToTexture.substr(idTypeStartSep, idTypeSpecifierSep - idTypeStartSep);

    // Insert texture meta item.
    DMaterial::DTexture result;
    result.mTextureMapType    = ConvertAiTextureTypeToDyType(iTextureType);
    result.mTextureSpecifier  = textureSpecifier;
    iRefMaterial.mTextureSpecifierList.emplace_back(result);
  }
}

DDyAnimationSequence Singleton_ModelInstance::CreateDyAnimation(unsigned iAnimIndex)
{
  MDY_NOTUSED const auto& ptrAiAnimation = this->mPtrAssimpModelAnimList[iAnimIndex];

  DDyAnimationSequence result;

  // Make header information.
  {
    result.mAnimationHeader.mTickPerSecond  = static_cast<float>(ptrAiAnimation->mTicksPerSecond);
    result.mAnimationHeader.mNumChannels    = ptrAiAnimation->mNumChannels;
    result.mAnimationHeader.mDurationTicks  = static_cast<float>(ptrAiAnimation->mDuration);
    result.mAnimationHeader.mDurationSecond = result.mAnimationHeader.mDurationTicks / result.mAnimationHeader.mTickPerSecond;
    result.mAnimationHeader.mTickSecond     = 1 / result.mAnimationHeader.mTickPerSecond;
  }

  // Insert each channel (node) 's each pos, rot, scl informations.
  result.mAnimationNodeList.resize(result.mAnimationHeader.mNumChannels);
  for (unsigned idCh = 0, numCh = result.mAnimationHeader.mNumChannels; idCh < numCh; ++idCh)
  {
    // Get assimp internal channel (anim-node) information instance.
    const auto& ptrAiChannel = ptrAiAnimation->mChannels[idCh];

    // Find appropriate bone-node from `mExportedSkeleton` instance. Must be found.
    const auto channelString = ptrAiChannel->mNodeName.C_Str();
    const auto it = std::find_if(
        this->mExportedSkeleton.cbegin(), this->mExportedSkeleton.cend(), 
        [channelString](const DSkeletonBone& iBone) { return iBone.mSpecifier == channelString; });
    jassert(it != this->mExportedSkeleton.cend());

    // Apply index to given animation node.
    auto& node = result.mAnimationNodeList[idCh];
    node.mSkeletonBoneId = static_cast<unsigned>(std::distance(this->mExportedSkeleton.cbegin(), it));

    const auto dt = result.mAnimationHeader.mTickSecond;
    // Get position, and apply.
    node.mPositionList.resize(ptrAiChannel->mNumPositionKeys);
    for (unsigned idPos = 0, numPos = ptrAiChannel->mNumPositionKeys; idPos < numPos; ++idPos)
    {
      const auto& ptrAiPos = ptrAiChannel->mPositionKeys[idPos];
      auto& refPos = node.mPositionList[idPos];

      refPos.mStartSecond = static_cast<float>(ptrAiPos.mTime * dt);
      refPos.mTranslate   = ptrAiPos.mValue;
    }

    // Get rotation (quaternion), and apply only x, y, z, w.
    node.mRotationList.resize(ptrAiChannel->mNumRotationKeys);
    for (unsigned idRot = 0, numRot = ptrAiChannel->mNumRotationKeys; idRot < numRot; ++idRot)
    {
      const auto& ptrAiRot = ptrAiChannel->mRotationKeys[idRot];
      auto& refRot = node.mRotationList[idRot];

      refRot.mStartSecond  = static_cast<float>(ptrAiRot.mTime * dt);
      const auto& quaternion = ptrAiRot.mValue;
      refRot.mW = quaternion.w;
      refRot.mX = quaternion.x;
      refRot.mY = quaternion.y;
      refRot.mZ = quaternion.z;
    }

    // Get scale and apply.
    node.mScaleList.resize(ptrAiChannel->mNumScalingKeys);
    for (unsigned idScl = 0, numScl = ptrAiChannel->mNumScalingKeys; idScl < numScl; ++idScl)
    {
      const auto& ptrAiScl = ptrAiChannel->mScalingKeys[idScl];
      auto& refScl = node.mScaleList[idScl];
      refScl.mStartSecond = static_cast<float>(ptrAiScl.mTime * dt);
      refScl.mScale       = ptrAiScl.mValue;
    }
  }

  return result;
}

void Singleton_ModelInstance::ReleaseModel()
{
  this->mAssimpModerImporter = nullptr;
  this->mPtrAssimpModelAnimList.clear();
  this->mPtrAssimpModelMeshList.clear();
  this->mPtrAssimpModelMaterialList.clear();
  this->mPtrAssimpModelTextureList.clear();
  this->mAssimpModeNode = nullptr;
  this->mExportedSkeleton.clear();
  this->mModelFileFullPath.clear();
  this->mMeshNameContainer.clear();
}

const Assimp::Importer* Singleton_ModelInstance::GetPtrModelImporter() const noexcept
{
  if (this->mAssimpModerImporter == nullptr) { return nullptr; }

  return this->mAssimpModerImporter.get();
}

const aiScene* Singleton_ModelInstance::GetPtrModelScene() const noexcept
{
  if (this->mAssimpModerImporter == nullptr) { return nullptr; }

  return this->mAssimpModerImporter->GetScene();
}

const aiNode* Singleton_ModelInstance::GetPtrRootNodeOfModelScene() const noexcept
{
  if (this->mAssimpModerImporter == nullptr) { return nullptr; }
  
  return this->mAssimpModerImporter->GetScene()->mRootNode;
}

const std::string& Singleton_ModelInstance::GetModelFileFullPath() const noexcept
{
  return this->mModelFileFullPath;
}

unsigned Singleton_ModelInstance::GetNumModelMeshes() const noexcept
{
  return static_cast<unsigned>(this->mPtrAssimpModelMeshList.size());
}

std::string Singleton_ModelInstance::GetExportedMeshSpecifierName(const std::string& iSpecifier, unsigned iMeshIndex)
{
  const std::string meshName = this->mPtrAssimpModelMeshList[iMeshIndex]->mName.C_Str();
  if (meshName.empty() == true)
  { // If mesh name is empty, just create string with inputted `iMeshIndex`.
    return fmt::format("{0}_{1}", iSpecifier, iMeshIndex);
  }
  else
  { // If mesh name is not emtpy, return with `iSpecifier_meshName`.
    return fmt::format("{0}_{1}", iSpecifier, this->TryGetGeneratedName(meshName));
  }
}

std::string Singleton_ModelInstance::TryGetGeneratedName(const std::string& iName) noexcept
{
  if (this->mMeshNameContainer.find(iName) == this->mMeshNameContainer.end())
  {
    this->mMeshNameContainer.try_emplace(iName, 1);
    return iName;
  }
  else
  {
    const auto index = this->mMeshNameContainer[iName];
    this->mMeshNameContainer[iName] += 1;

    return fmt::format("{}_{}", iName, index);
  }
}

unsigned Singleton_ModelInstance::GetNumModelMaterials() const noexcept
{
  return static_cast<unsigned>(this->mPtrAssimpModelMaterialList.size());
}

std::string Singleton_ModelInstance::GetExportedMaterialSpecifierName(const std::string& iSpecifier, unsigned iMatIndex)
{
  aiString name;
  this->mPtrAssimpModelMaterialList[iMatIndex]->Get(AI_MATKEY_NAME, name);

  if (name.length == 0)
  { // If mesh name is empty, just create string with inputted `iMeshIndex`.
    return fmt::format("{0}_{1}", iSpecifier, iMatIndex);
  }
  else
  { // If mesh name is not emtpy, return with `iSpecifier_meshName`.
    return fmt::format("{0}_{1}", iSpecifier, name.C_Str());
  }
}

bool Singleton_ModelInstance::IsModelHasBones() const noexcept
{
  // If model has not been loaded yet, it just return false.
  if (this->mAssimpModerImporter == nullptr) { return false; }

  // If any meshes have a bone, return true.
  const auto numMeshes = this->GetNumModelMeshes();
  for (unsigned i = 0; i < numMeshes; ++i)
  {
    if (this->mPtrAssimpModelMeshList[i]->HasBones() == true) 
    { 
      return true; 
    }
  }

  // If not, return false.
  return false;
}

bool Singleton_ModelInstance::IsModelHasAnimations() const noexcept
{
  // If model has not been loaded yet, it just return false.
  if (this->mAssimpModerImporter == nullptr) { return false; }

  return this->GetPtrModelScene()->HasAnimations();
}

unsigned Singleton_ModelInstance::GetNumModelAnimations() const noexcept
{
  return this->GetPtrModelScene()->mNumAnimations;
}

std::string Singleton_ModelInstance::GetExportedAnimationSpecifierName(const std::string& iSpecifier, unsigned iAnimIndex)
{
  const auto ptrAnimation = this->GetPtrModelScene()->mAnimations[iAnimIndex];
  const auto animName = ptrAnimation->mName;

  if (animName.length == 0)
  { // If animation name is empty, just create string with inputted `iMeshIndex`.
    return fmt::format("{0}_{1}", iSpecifier, iAnimIndex);
  }
  else
  { // If animation name is not emtpy, return with `iSpecifier_animName`.
    return fmt::format("{0}_{1}", iSpecifier, animName.C_Str());
  }
}

void Singleton_ModelInstance::SetExportFlag(EExportFlags iFlags, bool isActivated)
{
  if (isActivated == true)
  {
    this->mExportFlags = 
      static_cast<EExportFlags>(
          static_cast<std::underlying_type_t<EExportFlags>>(this->mExportFlags)
        | static_cast<std::underlying_type_t<EExportFlags>>(iFlags)
      );
  }
  else
  {
    this->mExportFlags = 
      static_cast<EExportFlags>(
          static_cast<std::underlying_type_t<EExportFlags>>(this->mExportFlags)
        & ~static_cast<std::underlying_type_t<EExportFlags>>(iFlags)
      );
  }
}

EExportFlags Singleton_ModelInstance::GetExportFlags() const noexcept
{
  return this->mExportFlags;
}

std::optional<Singleton_ModelInstance::TPtrAiNodeMap> Singleton_ModelInstance::CreatePtrAiNodeMap()
{
  // If model is not loaded, it just return nullopt.
  if (this->mAssimpModerImporter == nullptr) { return std::nullopt; }

  TPtrAiNodeMap resultMap;
  const aiScene* ptrAiScene = this->mAssimpModerImporter->GetScene();

  const auto* ptrAiNode = ptrAiScene->mRootNode;
  this->RecursiveInsertAiNodeIntoNodeMap(DyMakeNotNull(ptrAiNode), resultMap);

  return resultMap;
}

std::optional<Singleton_ModelInstance::TBoneSpecifierMap>
Singleton_ModelInstance::CreatePtrBoneSpecifierSet(const TPtrAiNodeMap& iPtrAiNodeMap) const noexcept
{
  // If model is not loaded, it just return nullopt.
  if (this->mAssimpModerImporter == nullptr) { return std::nullopt; }

  TBoneSpecifierMap resultBoneSpecifierSet;

  for (unsigned i = 0, numModelMesh = this->GetNumModelMeshes(); i < numModelMesh; ++i)
  {
    const auto ptrAiModelMesh = this->mPtrAssimpModelMeshList[i];
    
    for (unsigned b = 0, numMeshBone = ptrAiModelMesh->mNumBones; b < numMeshBone; ++b)
    {
      const aiBone* ptrAiBone = ptrAiModelMesh->mBones[b];
      // Find any proper aiNode with aiBone's name, because in assimp, matched aiBone and aiNode has a same name.
      const auto itPtrAiNode  = iPtrAiNodeMap.find(ptrAiBone->mName.C_Str());
      if (itPtrAiNode == iPtrAiNodeMap.end()) { continue; }

      const auto [ptrAiNodeSpecifier, ptrAiNodeInstance] = *itPtrAiNode;
      const aiNode* node = ptrAiNodeInstance.Get();

      while (node != nullptr)
      {
        DBoneSpecifier result;
        result.mSpecifier     = node->mName.C_Str();
        result.mOffsetMatrix  = ptrAiBone->mOffsetMatrix;
        resultBoneSpecifierSet.try_emplace(node->mName.C_Str(), result);
        node = node->mParent;

        // Don't chase up until the scene root node.
        if (node->mParent == nullptr) 
        { 
          break; 
        }
      }
    }
  }

  return resultBoneSpecifierSet;
}

EDySuccess Singleton_ModelInstance::CreateModelSkeleton(
    MDY_NOTUSED const TPtrAiNodeMap& iPtrAiNodeMap,
    const TBoneSpecifierMap& iBoneSpecifierSet)
{
  // Get root-node and root-matrix (identity matrix)
  const aiNode*       rootNode    = this->GetPtrRootNodeOfModelScene();
  const DDyMatrix4x4  rootMatrix  = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

  // Make skeleton bone list recursively, traversing each node.
  // Created skeleton bone list is deterministic.
  this->RecursiveInsertSkeletonBoneIntoList(
      DyMakeNotNull(rootNode), iBoneSpecifierSet, 
      rootMatrix, -1, 
      this->mExportedSkeleton);
  return DY_SUCCESS;
}

void Singleton_ModelInstance::RecursiveInsertSkeletonBoneIntoList(
    const NotNull<const aiNode*> iPtrAiNode,
    const TBoneSpecifierMap& iRefBoneSpecifierSet, 
    const DDyMatrix4x4& iRefParentGlobalTransform,
    const signed int iParentSkeletonBoneId,
    std::vector<DSkeletonBone>& iSkeletonList)
{
  // Make global transform (parent * this-node) and potential parent index.
  const DDyMatrix4x4 globalTransform      = iRefParentGlobalTransform.Multiply(iPtrAiNode->mTransformation);
  const signed int skeletonBoneNodeIndex  = static_cast<signed int>(iSkeletonList.size()) - 1;

  // Remove redundant node(bone) so insert valid and activated node(bone).
  if (auto it = iRefBoneSpecifierSet.find(iPtrAiNode->mName.C_Str()); it != iRefBoneSpecifierSet.end())
  {
    iSkeletonList.emplace_back(DSkeletonBone{});
    auto& skeletonInstance = this->mExportedSkeleton.back();
    skeletonInstance.mPtrAiNode       = iPtrAiNode.Get();
    skeletonInstance.mSpecifier       = iPtrAiNode->mName.C_Str();
    skeletonInstance.mLocalTransform  = iPtrAiNode->mTransformation;
    skeletonInstance.mGlobalTransform = globalTransform;
    skeletonInstance.mOffsetMatrix    = it->second.mOffsetMatrix;
    skeletonInstance.mParentSkeletonBoneIndex = iParentSkeletonBoneId;
  }

  // Recursively request insertion to children.
  for (unsigned childIndex = 0, numChild = iPtrAiNode->mNumChildren; childIndex < numChild; ++childIndex)
  {
    const aiNode* ptrChildNode = iPtrAiNode->mChildren[childIndex];
    this->RecursiveInsertSkeletonBoneIntoList(
        DyMakeNotNull(ptrChildNode), iRefBoneSpecifierSet, 
        globalTransform, skeletonBoneNodeIndex, 
        iSkeletonList);
  }
}

void Singleton_ModelInstance::RemoveModelSkeleton()
{
  this->mExportedSkeleton.clear();
}

void Singleton_ModelInstance::RecursiveInsertAiNodeIntoNodeMap(NotNull<const aiNode*> iPtrAiNode, TPtrAiNodeMap& iMap)
{
  auto [_, isSucceeeded] = iMap.try_emplace(iPtrAiNode->mName.C_Str(), iPtrAiNode);
  jassert(isSucceeeded == true);

  for (unsigned i = 0, numChildren = iPtrAiNode->mNumChildren; i < numChildren; ++i)
  {
    this->RecursiveInsertAiNodeIntoNodeMap(DyMakeNotNull(iPtrAiNode->mChildren[i]), iMap);
  }
}
