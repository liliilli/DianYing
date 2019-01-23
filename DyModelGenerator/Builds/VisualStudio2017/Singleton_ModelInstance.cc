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

EDySuccess Singleton_ModelInstance::ExportModelMesh(const std::string& iSpecifier, unsigned iMeshIndex, bool isCompressed)
{
  if (iMeshIndex >= this->GetNumModelMeshes()) { return DY_FAILURE; }

  // Make serialized string form mesh instance.
  const auto mesh = CreateDyMesh(iMeshIndex);

  nlohmann::json jsonMeshAtlas = mesh;
  const auto meshSerializedString = jsonMeshAtlas.dump();
  
  // Get a directory path from model file full path.
  namespace fs = std::filesystem;
  const auto directoryPath  = fs::path{this->mModelFileFullPath}.parent_path();

  if (isCompressed == true)
  {
    const auto buffer = CompressStringBuffer(meshSerializedString);
    #ifdef _DEBUG
    { // Check in debug mode, if raw buffer and decompressed buffer is same.
      const auto rawBuffer = DecompressLz4Buffer(buffer);
      jassert(rawBuffer == meshSerializedString);
    }
    #endif
    //const auto byteBuffer = nlohmann::json{buffer}.dump(1);

    // Write file. `File` is RAII.
    const fs::path meshPath = fmt::format("{}/{}.{}", directoryPath.string(), iSpecifier, "dyMesh");
    juce::File fdMeshFile {meshPath.string()};

    fdMeshFile.replaceWithData(&buffer.mRawBufferBytes, sizeof(unsigned));
    fdMeshFile.appendData(&buffer.mCompressedBufferBytes, sizeof(unsigned));
    const auto flag = fdMeshFile.appendData(buffer.mCompressedBuffer.data(), buffer.mCompressedBufferBytes);

    // If writing file is failed, return with FAILURE flag.
    if (flag == false) { return DY_FAILURE; }
  }
  else
  {
    const fs::path meshPath = fmt::format("{}/{}.{}", directoryPath.string(), iSpecifier, "json");

    // Write file. `File` is RAII.
    juce::File fdMeshFile {meshPath.string()};
    const auto flag = fdMeshFile.replaceWithText(meshSerializedString, true);

    // If writing file is failed, return with FAILURE flag.
    if (flag == false) { return DY_FAILURE; }
  }

  return DY_SUCCESS;
}

void Singleton_ModelInstance::ReleaseModel()
{
  this->mAssimpModerImporter = nullptr;
  this->mPtrAssimpModelAnimList.clear();
  this->mPtrAssimpModelMeshList.clear();
  this->mPtrAssimpModelMaterialList.clear();
  this->mPtrAssimpModelTextureList.clear();
  this->mAssimpModeNode = nullptr;
  this->mModelFileFullPath.clear();
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

std::string Singleton_ModelInstance::GetExportedMeshSpecifierName(
    const std::string& iSpecifier,
    unsigned iMeshIndex)
{
  const std::string meshName = this->mPtrAssimpModelMeshList[iMeshIndex]->mName.C_Str();
  if (meshName.empty() == true)
  { // If mesh name is empty, just create string with inputted `iMeshIndex`.
    return fmt::format("{0}_{1}", iSpecifier, iMeshIndex);
  }
  else
  { // If mesh name is not emtpy, return with `iSpecifier_meshName`.
    return fmt::format("{0}_{1}", iSpecifier, meshName);
  }
}

DMesh Singleton_ModelInstance::CreateDyMesh(unsigned iMeshIndex)
{
  const auto& ptrAiMesh = this->mPtrAssimpModelMeshList[iMeshIndex];
  DMesh result;

  //!
  //! Indices
  //!

  if (ptrAiMesh->HasFaces() == true)
  { // If mesh has faces.
    const auto faceNum = ptrAiMesh->mNumFaces;

    // Because we have triangulate indexes, so have to set up memory space with 3 times.
    result.mIndexList.reserve(faceNum * 3);
    for (unsigned i = 0; i < faceNum; ++i)
    {
      const auto& ptrFace = ptrAiMesh->mFaces[i];
      jassert(ptrFace.mNumIndices == 3);

      // Insert face as indices (a, b, c).
      for (unsigned id = 0; id < 3; ++id)
      {
        result.mIndexList.emplace_back(ptrFace.mIndices[id]);
      }
    }
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
      
      // Vertex
      for (unsigned v = 0; v < 3; ++v) 
      { vertexInfo.mPosition[v] = ptrAiMesh->mVertices[i][v]; }

      // Normal
      if (ptrAiMesh->HasNormals() == true)
      {
        for (unsigned n = 0; n < 3; ++n) 
        { vertexInfo.mNormal[n] = ptrAiMesh->mNormals[i][n]; }
      }

      // Texcoord0 (u,v)
      if (ptrAiMesh->HasTextureCoords(0) == true)
      {
        jassert(ptrAiMesh->mNumUVComponents[0] == 2);
        for (unsigned uv = 0; uv < 2; ++uv) 
        { vertexInfo.mTexCoords0[uv] = ptrAiMesh->mTextureCoords[0][i][uv]; }
      }

      // Tangent & Bitangent
      if (ptrAiMesh->HasTangentsAndBitangents() == true)
      {
        for (unsigned t = 0; t < 3; ++t)
        {
          vertexInfo.mTangent[t]    = ptrAiMesh->mTangents[i][t];
          vertexInfo.mBitangent[t]  = ptrAiMesh->mBitangents[i][t];
        }
      }

      // Bone & Weight
      if (ptrAiMesh->HasBones() == true)
      {
        // @TODO IMPLEMENT BONE EXPORTING
      }

      // Insert item to result.
      result.mVertexList.emplace_back(std::move(vertexInfo));
    }
  }

  return result;
}
