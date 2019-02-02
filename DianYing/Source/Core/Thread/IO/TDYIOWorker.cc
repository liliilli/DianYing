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
#include <Dy/Core/Thread/IO/TDyIOWorker.h>
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/Thread/SDyIOWorkerConnHelper.h>
#include <Dy/Core/Resource/Information/FDyModelInformation.h>
#include <Dy/Core/Resource/Internal/FDyModelVBOIntermediate.h>
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Core/Resource/Information/FDyAttachmentInformation.h>
#include <Dy/Core/Resource/Information/FDyFrameBufferInformation.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Information/FDyModelSkeletonInformation.h>
#include <Dy/Core/Resource/Information/FDyModelAnimScrapInformation.h>
#include <Dy/Core/Resource/Information/FDySoundInformation.h>
#include <Dy/Management/IO/MDyIOData.h>

namespace dy
{

void TDyIOWorker::operator()(_MIN_ NotNull<GLFWwindow*> ptrWorkerWnd)
{
  glfwMakeContextCurrent(ptrWorkerWnd);
  this->inWork();
}

void TDyIOWorker::inWork()
{
  static auto CbTaskWaiting = [this]
  {
    return this->mIsAssigned.load() == true || this->mIsShouldStop == true;
  };

  while (true)
  {
    {
      MDY_SYNC_WAIT_CONDITION(this->mTaskMutex, this->mTaskCV, CbTaskWaiting);
      if (this->mIsShouldStop == true) { break; }
      if (this->mIsAssigned.load() == false)  { continue; }

      // Do process
      // (result have void* (heap object) as created resource instance,
      // need to be moved into resultContainer carefully without data race.
      // @TODO NEED TO BE CHECK RESULT IS NOT POPULATED (FORWARD TO MAIN TASK LIST)
      SDyIOWorkerConnHelper::InsertResult(this->PopulateIOResource(this->mAssignedTask));
      // 만약에 `Deferred` 가 Checked 되어있다면, `SDyIOConnectionHelper` 을 사용해서 atomic 하게 specifier 을 리스트에 넣어야 함.
      // 그러면 리스트가 not empty 됨으로써 IO Reinsert 조건이 만족 됨. (IO Reinsert 는 IO IN 의 부수 페이즈임)
      // Synchronization 매니저에 의한 Sync 과정에서 리스트의 speicfier 에 대해 `Deferred List` 에서 해당하는 `Resource` 태스크를
      // 꺼내서 상위 우선순위로 Queue 에 넘겨야 함. 이 때 specifier 에 대해 리스트에서 가지고 있지 않은 경우는 없음. 그러면 로직이 X된거임.

      MDY_ATOMIC_COMPARE_SWAP_WEAK(this->mIsAssigned, true, false);
      this->mAssignedTask = {};
    }

    SDyIOWorkerConnHelper::TryNotify();
    MDY_SLEEP_FOR_ATOMIC_TIME();
  }
}

EDySuccess TDyIOWorker::outTryAssign(_MIN_ const DDyIOTask& inputTask)
{
  {
    using namespace std::chrono_literals;
    if (MDY_SYNC_WAIT_CONDITION_FOR(this->mTaskMutex, this->mTaskCV, 1ms, [this] { return this->mIsAssigned.load() == false; }))
    { // Check assigned flag mutually.
      this->mAssignedTask = inputTask;
      MDY_ATOMIC_COMPARE_SWAP_WEAK(this->mIsAssigned, false, true);
    }
    else { return DY_FAILURE; }
  }

  this->mTaskCV.notify_one();
  MDY_SLEEP_FOR_ATOMIC_TIME();
  return DY_SUCCESS;
}

void TDyIOWorker::outTryStop()
{
  MDY_SYNC_LOCK_GUARD(this->mTaskMutex);
  this->mIsShouldStop = true;
  this->mTaskCV.notify_one();
  MDY_SLEEP_FOR_ATOMIC_TIME();
}

DDyIOWorkerResult TDyIOWorker::PopulateIOResource(_MIN_ const DDyIOTask& assignedTask)
{
  // 여기서는 IO Information & Resource 컨테이너는 반드시 atomic 해야함.
  // RESOURCE 을 만드는데, 각각의 CDy~~ 을 조정해줄 필요가 있을 듯 함.
  switch (assignedTask.mResourcecStyle)
  {
  case EDyResourceStyle::Information: return this->pPopulateIOResourceInformation(assignedTask);
  case EDyResourceStyle::Resource:    return this->pPopulateIOResourceResource(assignedTask);
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(DDyIOWorkerResult{});
}

DDyIOWorkerResult TDyIOWorker::pPopulateIOResourceInformation(_MIN_ const DDyIOTask& assignedTask)
{
  DDyIOWorkerResult result{};
  result.mResourceType  = assignedTask.mResourceType;
  result.mResourceStyle = assignedTask.mResourcecStyle;
  result.mSpecifierName = assignedTask.mSpecifierName;
  result.mIsHaveDeferredTask = assignedTask.mIsResourceDeferred;

  switch (result.mResourceType)
  {
  case EDyResourceType::GLShader:
    result.mSmtPtrResultInstance = new FDyShaderInformation
    (this->mMetaManager.GetGLShaderMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::Texture:
    result.mSmtPtrResultInstance = new FDyTextureInformation
    (this->mMetaManager.GetTextureMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::Mesh:
    result.mSmtPtrResultInstance = new FDyMeshInformation
    (this->mMetaManager.GetBtMeshMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::Model:
    result.mSmtPtrResultInstance = new FDyModelInformation
    (this->mMetaManager.GetModelMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::Skeleton:
    result.mSmtPtrResultInstance = new FDyModelSkeletonInformation
    (this->mMetaManager.GetModelSkeletonMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::AnimationScrap:
    result.mSmtPtrResultInstance = new FDyModelAnimScrapInformation
    (this->mMetaManager.GetModelAnimScrapMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::Material:
    result.mSmtPtrResultInstance = new FDyMaterialInformation
    (this->mMetaManager.GetMaterialMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::GLAttachment:
    result.mSmtPtrResultInstance = new FDyAttachmentInformation
    (this->mMetaManager.GetGLAttachmentMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::GLFrameBuffer:
    result.mSmtPtrResultInstance = new FDyFrameBufferInformation
    (this->mMetaManager.GetGlFrameBufferMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::Sound:
    result.mSmtPtrResultInstance = new FDySoundInformation
    (this->mMetaManager.GetSoundMetaInformation(assignedTask.mSpecifierName));
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  return result;
}

DDyIOWorkerResult TDyIOWorker::pPopulateIOResourceResource(_MIN_ const DDyIOTask& assignedTask)
{
  DDyIOWorkerResult result{};
  result.mResourceType  = assignedTask.mResourceType;
  result.mResourceStyle = assignedTask.mResourcecStyle;
  result.mSpecifierName = assignedTask.mSpecifierName;
  result.mIsHaveDeferredTask = false;

  const auto& infoManager = MDyIOData::GetInstance();
  switch (result.mResourceType)
  {
  case EDyResourceType::GLShader:
  { // https://www.khronos.org/opengl/wiki/OpenGL_Object#Object_Sharing
    // Shader can not create from other thread.
    SDyIOWorkerConnHelper::TryForwardToMainTaskList(assignedTask);
  } break;
  case EDyResourceType::Texture:
  { // Texture buffer can be created on another context. (It can be shared)
    result.mSmtPtrResultInstance = new FDyTextureResource(*infoManager.GetPtrInformation<EDyResourceType::Texture>(result.mSpecifierName));
  } break;
  case EDyResourceType::__MeshVBO:
  { // Builtin mesh not be created on another context... so forward it to main thread.
    //result.mSmtPtrResultInstance = new FDyMeshResource(*infoManager.GetPtrInformation<EDyResourceType::Mesh>(result.mSpecifierName));
    auto task = assignedTask;
    task.mResourceType = EDyResourceType::Mesh;
    task.mRawInstanceForUsingLater = new FDyMeshVBOIntermediate(*infoManager.GetPtrInformation<EDyResourceType::Mesh>(result.mSpecifierName));
    SDyIOWorkerConnHelper::TryForwardToMainTaskList(task);
  } break;
  case EDyResourceType::Model:
  { // 
    result.mSmtPtrResultInstance = new FDyModelResource(*infoManager.GetPtrInformation<EDyResourceType::Model>(result.mSpecifierName));
  } break;
  case EDyResourceType::Material:
  { // Material resource is just for binding allocated textures and shader instance ptr list.
    if (assignedTask.mRawInstanceForUsingLater.has_value() == true)
    {
      const auto& _ = std::any_cast<const PDyMaterialInstanceMetaInfo&>(assignedTask.mRawInstanceForUsingLater);
      result.mSmtPtrResultInstance = new FDyMaterialResource(_);
    }
    else
    {
      result.mSmtPtrResultInstance = 
      new FDyMaterialResource(*infoManager.GetPtrInformation<EDyResourceType::Material>(result.mSpecifierName));
    }
  } break;
  case EDyResourceType::GLAttachment:
  { // Attachment resource can be created on another context. (It can be shared)
    result.mSmtPtrResultInstance = new FDyAttachmentResource(*infoManager.GetPtrInformation<EDyResourceType::GLAttachment>(result.mSpecifierName));
  } break;
  case EDyResourceType::GLFrameBuffer:
  { // Framebuffer object must be created on main thread. so forward it to main deferred list.
    SDyIOWorkerConnHelper::TryForwardToMainTaskList(assignedTask);
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  return result;
}

} /// ::dy namespace