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
#include <Dy/Core/Thread/IO/TRescIOWorker.h>
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/Thread/SDyIOWorkerConnHelper.h>
#include <Dy/Core/Resource/Information/FDyModelInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureCubemapInformation.h>
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Core/Resource/Information/FDyAttachmentInformation.h>
#include <Dy/Core/Resource/Information/FDyFrameBufferInformation.h>
#include <Dy/Core/Resource/Resource/FDyTextureGeneralResource.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Information/FDyModelSkeletonInformation.h>
#include <Dy/Core/Resource/Information/FDyModelAnimScrapInformation.h>
#include <Dy/Core/Resource/Information/FDySoundInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureGeneralInformation.h>
#include <Dy/Core/Resource/Resource/FDyTextureCubemapResource.h>
#include <Dy/Core/Resource/Resource/Attachment/FDyAttachmentGeneralResource.h>
#include <Dy/Core/Resource/Resource/Attachment/FDyAttachmentPingpongResource.h>
#include <Dy/Management/IO/MIORescInfo.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Core/Resource/Internal/FMeshVBOIntermediate.h>
#include <Dy/Builtin/Constant/GeneralValue.h>
#include <Dy/Helper/Library/HelperString.h>

namespace dy
{

void TRescIOWorker::operator()(GLFWwindow& ptrWorkerWnd)
{
  glfwMakeContextCurrent(&ptrWorkerWnd);
  this->inWork();
}

void TRescIOWorker::inWork()
{
  while (true)
  {
    {
      MDY_SYNC_WAIT_CONDITION(
        this->mTaskMutex, this->mTaskCV, 
        [this] { return this->mIsAssigned == true || this->mIsShouldStop == true; }
      );
      if (this->mIsShouldStop == true) { break; }

      // Do process
      // (result have void* (heap object) as created resource instance,
      // need to be moved into resultContainer carefully without data race.
      SDyIOWorkerConnHelper::InsertResult(this->PopulateIOResource(this->mAssignedTask));
      // 만약에 `Deferred` 가 Checked 되어있다면, `SDyIOConnectionHelper` 을 사용해서 atomic 하게 specifier 을 리스트에 넣어야 함.
      // 그러면 리스트가 not empty 됨으로써 IO Reinsert 조건이 만족 됨. (IO Reinsert 는 IO IN 의 부수 페이즈임)
      // Synchronization 매니저에 의한 Sync 과정에서 리스트의 speicfier 에 대해 `Deferred List` 에서 해당하는 `Resource` 태스크를
      // 꺼내서 상위 우선순위로 Queue 에 넘겨야 함. 이 때 specifier 에 대해 리스트에서 가지고 있지 않은 경우는 없음. 그러면 로직이 X된거임.

      this->mIsAssigned = false;
      this->mAssignedTask = {};
    }

    //SDyIOWorkerConnHelper::TryNotify();
    this->mTaskCV.notify_one();
    MDY_SLEEP_FOR_ATOMIC_TIME();
  }
}

EDySuccess TRescIOWorker::outTryAssign(const DRescIOTask& inputTask)
{
  {
    using namespace std::chrono_literals;
    if (
      MDY_SYNC_WAIT_CONDITION_FOR(
        this->mTaskMutex, this->mTaskCV, 1ms, 
        [this] { return this->mIsAssigned == false; }))
    { 
      // Check assigned flag mutually.
      this->mAssignedTask = inputTask;
      this->mIsAssigned = true;
    }
    else 
    { 
      // If failed to assign task, just return with DY_FAILURE.
      return DY_FAILURE; 
    }
  }

  this->mTaskCV.notify_one();
  MDY_SLEEP_FOR_ATOMIC_TIME();
  return DY_SUCCESS;
}

bool TRescIOWorker::SyncIsIdle() const noexcept
{
  return this->mIsAssigned == false;
}

void TRescIOWorker::SyncTryStop()
{
  MDY_SYNC_LOCK_GUARD(this->mTaskMutex);
  this->mIsShouldStop = true;
  this->mTaskCV.notify_one();
  MDY_SLEEP_FOR_ATOMIC_TIME();
}

DRescIOWorkerResult TRescIOWorker::PopulateIOResource(const DRescIOTask& assignedTask)
{
  switch (assignedTask.mResourcecStyle)
  {
  case EResourceStyle::Information: return this->pPopulateIOResourceInformation(assignedTask);
  case EResourceStyle::Resource:    return this->pPopulateIOResourceResource(assignedTask);
  default: MDY_UNEXPECTED_BRANCH(); throw 0; // Intentional.
  }
}

DRescIOWorkerResult TRescIOWorker::pPopulateIOResourceInformation(const DRescIOTask& assignedTask)
{
  DRescIOWorkerResult result{};
  result.mResourceType  = assignedTask.mResourceType;
  result.mResourceStyle = assignedTask.mResourcecStyle;
  result.mSpecifierName = assignedTask.mSpecifierName;
  result.mIsHaveDeferredTask = assignedTask.mIsResourceDeferred;

  // Getting meta information from MetaManager is absolutely thread safe.
  switch (result.mResourceType)
  {
  case EResourceType::GLShader:
  {
    const auto shaderName = TryRemovePostfix(assignedTask.mSpecifierName, kInstancingPostfix);
    // Check this is dynamic instancing material
    auto isInstanced  = false;
    if (shaderName != assignedTask.mSpecifierName) { isInstanced = true; }

    result.mRawResultInstance = new FDyShaderInformation
    (this->mMetaManager.GetGLShaderMetaInformation(shaderName), isInstanced);
  } break;
  case EResourceType::Texture:
  { 
    const auto& metaInfo = this->mMetaManager.GetTextureMetaInformation(assignedTask.mSpecifierName);
    if (metaInfo.mTextureType == ETextureStyleType::D2Cubemap)
    { 
      // When cubemap, we should use separated information type.
      result.mRawResultInstance = new FDyTextureCubemapInformation(metaInfo);
    }
    else
    {
      result.mRawResultInstance = new FDyTextureGeneralInformation(metaInfo);
    }
  } break;
  case EResourceType::Mesh:
    result.mRawResultInstance = new FDyMeshInformation
    (this->mMetaManager.GetBtMeshMetaInformation(assignedTask.mSpecifierName));
    break;
  case EResourceType::Model:
    result.mRawResultInstance = new FDyModelInformation
    (this->mMetaManager.GetModelMetaInformation(assignedTask.mSpecifierName));
    break;
  case EResourceType::Skeleton:
    result.mRawResultInstance = new FDyModelSkeletonInformation
    (this->mMetaManager.GetModelSkeletonMetaInformation(assignedTask.mSpecifierName));
    break;
  case EResourceType::AnimationScrap:
    result.mRawResultInstance = new FDyModelAnimScrapInformation
    (this->mMetaManager.GetModelAnimScrapMetaInformation(assignedTask.mSpecifierName));
    break;
  case EResourceType::Material:
  {
    const auto materialName = TryRemovePostfix(assignedTask.mSpecifierName, kInstancingPostfix);
    // Check this is instancing material
    const auto isInstanced  = materialName != assignedTask.mSpecifierName;

    result.mRawResultInstance = new FDyMaterialInformation
    (this->mMetaManager.GetMaterialMetaInformation(materialName), isInstanced);
  } break;
  case EResourceType::GLAttachment:
    result.mRawResultInstance = new FDyAttachmentInformation
    (this->mMetaManager.GetGLAttachmentMetaInformation(assignedTask.mSpecifierName));
    break;
  case EResourceType::GLFrameBuffer:
    result.mRawResultInstance = new FDyFrameBufferInformation
    (this->mMetaManager.GetGlFrameBufferMetaInformation(assignedTask.mSpecifierName));
    break;
  case EResourceType::Sound:
    result.mRawResultInstance = new FDySoundInformation
    (this->mMetaManager.GetSoundMetaInformation(assignedTask.mSpecifierName));
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  return result;
}

DRescIOWorkerResult TRescIOWorker::pPopulateIOResourceResource(const DRescIOTask& assignedTask)
{
  DRescIOWorkerResult result{};
  result.mResourceType  = assignedTask.mResourceType;
  result.mResourceStyle = assignedTask.mResourcecStyle;
  result.mSpecifierName = assignedTask.mSpecifierName;
  result.mIsHaveDeferredTask = false;

  const auto& infoManager = MIORescInfo::GetInstance();
  switch (result.mResourceType)
  {
  case EResourceType::GLShader:
  { 
    // https://www.khronos.org/opengl/wiki/OpenGL_Object#Object_Sharing
    // Shader can not create from other thread.
    SDyIOWorkerConnHelper::TryForwardToMainTaskList(assignedTask);
  } break;
  case EResourceType::Texture:
  { 
    // Texture buffer can be created on another context. (It can be shared)
    const auto* ptr = infoManager.GetPtrInformation<EResourceType::Texture>(result.mSpecifierName);
    if (ptr->GetType() == ETextureStyleType::D2Cubemap)
    {
      result.mRawResultInstance = new FDyTextureCubemapResource
        (static_cast<const FDyTextureCubemapInformation&>(*ptr)); 
    }
    else
    {
      result.mRawResultInstance = new FDyTextureGeneralResource
        (static_cast<const FDyTextureGeneralInformation&>(*ptr)); 
    }
  } break;
  case EResourceType::__MeshVBO:
  { 
    // Builtin mesh not be created on another context... so forward it to main thread.
    auto task = assignedTask;
    task.mResourceType = EResourceType::Mesh;
    const auto meshName = TryRemovePostfix(assignedTask.mSpecifierName, kInstancingPostfix);
    const auto isInstanced  = meshName != assignedTask.mSpecifierName;

    task.mRawInstanceForUsingLater = new FMeshVBOIntermediate(
      *infoManager.GetPtrInformation<EResourceType::Mesh>(meshName), isInstanced);

    SDyIOWorkerConnHelper::TryForwardToMainTaskList(task);
  } break;
  case EResourceType::Model:
  { 
    // 
    result.mRawResultInstance = new FDyModelResource
      (*infoManager.GetPtrInformation<EResourceType::Model>(result.mSpecifierName));
  } break;
  case EResourceType::Material:
  { 
    // Material resource is just for binding allocated textures and shader instance ptr list.
    if (assignedTask.mRawInstanceForUsingLater.has_value() == true)
    {
      result.mRawResultInstance = new FDyMaterialResource
        (std::any_cast<const PDyMaterialInstanceMetaInfo&>(assignedTask.mRawInstanceForUsingLater));
    }
    else
    {
      result.mRawResultInstance = new FDyMaterialResource
        (*infoManager.GetPtrInformation<EResourceType::Material>(result.mSpecifierName));
    }
  } break;
  case EResourceType::GLAttachment:
  { 
    // Attachment resource can be created on another context. (It can be shared)
    const auto& refInfo = 
      *infoManager.GetPtrInformation<EResourceType::GLAttachment>(result.mSpecifierName);
    if (refInfo.IsPingPong() == false)
    { // Create general attachment.
      result.mRawResultInstance = new FDyAttachmentGeneralResource(refInfo);
    }
    else
    { // Create ping-pong attachment.
      result.mRawResultInstance = new FDyAttachmentPingpongResource(refInfo);
    }
  } break;
  case EResourceType::GLFrameBuffer:
  { 
    // Framebuffer object must be created on main thread. so forward it to main deferred list.
    SDyIOWorkerConnHelper::TryForwardToMainTaskList(assignedTask);
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  return result;
}

} /// ::dy namespace