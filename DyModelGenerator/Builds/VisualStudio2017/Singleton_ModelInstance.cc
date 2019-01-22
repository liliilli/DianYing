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
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

  /// Created `aiScene` will be removed automatically when smtptr of Importer ref counting is 0.
  this->mAssimpModerImporter = std::make_unique<Assimp::Importer>();
  
  const aiScene* ptrModelScene = this->mAssimpModerImporter->ReadFile(iPath,
      aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);
  if (ptrModelScene == nullptr) 
  {
    this->ReleaseModel();
    return DY_FAILURE; 
  }
  else
  { // Check
    if (ptrModelScene->mRootNode == nullptr || (ptrModelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0) 
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
  else
  {
    return this->mAssimpModerImporter.get();
  }
}

const aiScene* Singleton_ModelInstance::GetPtrModelScene() const noexcept
{
  if (this->mAssimpModerImporter == nullptr) { return nullptr; }
  else
  {
    return this->mAssimpModerImporter->GetScene();
  }
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
