#include <precompiled.h>
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
#include <Dy/Core/Resource/Information/FDyModelInformation.h>
#include <Dy/Meta/Information/ModelMetaInformation.h>
#include <Dy/Core/Resource/Internal/ModelType.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

#ifdef false
FDyModelInformation::FDyModelInformation(_MIN_ const PDyModelInstanceMetaInfo_Deprecated& metaInfo)
{
  MDY_UNEXPECTED_BRANCH();
  if (metaInfo.mSourceType == EDyResourceSource::Builtin)
  { // If model is buitlin, just copy & paste.
    this->mSpecifierName  = metaInfo.mSpecifierName;
    auto& descriptor      = *metaInfo.mPtrBuiltinModelBuffer;
    for (const auto& submeshInformation : descriptor.mSubmeshConstructionInformations)
    {
      this->mMeshInformations.emplace_back(submeshInformation);
    }
  }
  else
  { // If model is external so should use assimp, parse file.
    MDY_NOT_IMPLEMENTED_ASSERT();
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
      MDY_LOG_DEBUG_D("DDyModelInformation_Deprecated | Model : {} Has animations", this->mModelName);
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
  }
}
#endif

FDyModelInformation::FDyModelInformation(_MIN_ const PDyModelInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName}
{
  for (const auto& [meshSpecifier, materialSpecifier] : metaInfo.mMeshList)
  {
    DySafeUniquePtrEmplaceBack(this->mMeshInformations, meshSpecifier);
  }
}

} /// ::dy namespace