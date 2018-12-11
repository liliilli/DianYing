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
#include <Dy/Core/Thread/IO/TDyIOWorker.h>
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/Thread/SDyIOWorkerConnHelper.h>
#include "Dy/Management/IO/IODataManager.h"
#include "Dy/Core/Resource/Resource_Deprecated/ShaderResource_Deprecated.h"
#include "Dy/Core/Resource/Resource_Deprecated/TextureResource_Deprecated.h"

namespace dy
{

void TDyIOWorker::operator()()
{
  this->mIsAssigned = false;
  this->mIsShouldStop = false;
  this->inWork();
}

void TDyIOWorker::inWork()
{
  static auto CbTaskWaiting = [this]
  {
    return this->mIsAssigned == true || this->mIsShouldStop == true;
  };

  while (true)
  {
    {
      MDY_SYNC_WAIT_CONDITION(this->mTaskMutex, this->mTaskCV, CbTaskWaiting);
      if (this->mIsShouldStop == true) { return; }
      MDY_ASSERT(this->mIsAssigned == true, "IOWorker's task must be assigned to proceed.");
    }

    // Do process
    // (result have void* (heap object) as created resource instance,
    // need to be moved into resultContainer carefully without data race.
    SDyIOWorkerConnHelper::InsertResult(this->PopulateIOResource(this->mAssignedTask));
    // 만약에 `Deferred` 가 Checked 되어있다면, `SDyIOConnectionHelper` 을 사용해서 atomic 하게 specifier 을 리스트에 넣어야 함.
    // 그러면 리스트가 not empty 됨으로써 IO Reinsert 조건이 만족 됨. (IO Reinsert 는 IO IN 의 부수 페이즈임)
    // Synchronization 매니저에 의한 Sync 과정에서 리스트의 speicfier 에 대해 `Deferred List` 에서 해당하는 `Resource` 태스크를
    // 꺼내서 상위 우선순위로 Queue 에 넘겨야 함. 이 때 specifier 에 대해 리스트에서 가지고 있지 않은 경우는 없음. 그러면 로직이 X된거임.

    { // Change status mutually.
      MDY_SYNC_LOCK_GUARD(this->mMutexAssigned);
      this->mIsAssigned = false;
      this->mAssignedTask = {};
    }
    SDyIOWorkerConnHelper::TryNotify();
    MDY_SLEEP_FOR_ATOMIC_TIME();
  }
}

EDySuccess TDyIOWorker::outTryAssign(_MIN_ const DDyIOTask& inputTask)
{
  { // Check assigned flag mutually.
    MDY_SYNC_LOCK_GUARD(this->mMutexAssigned);
    if (this->mIsAssigned == true) { return DY_FAILURE; }
    // And..
    {
      MDY_SYNC_LOCK_GUARD(this->mTaskMutex);
      this->mAssignedTask = inputTask;
      this->mIsAssigned = true;
    }
  }
  this->mTaskCV.notify_one();

  MDY_SLEEP_FOR_ATOMIC_TIME();
  return DY_SUCCESS;
}

void TDyIOWorker::outTryStop()
{
  MDY_SYNC_LOCK_GUARD(this->mTaskMutex);
  this->mIsShouldStop = true;
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
  { // TEMPORARY
  case EDyResourceType::GLShader:
    result.mSmtPtrResultInstance = new DDyShaderInformation_Deprecated(this->mMetaManager.GetGLShaderMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::Texture:
    result.mSmtPtrResultInstance = new DDyTextureInformation_Deprecated(this->mMetaManager.GetTextureMetaInformation(assignedTask.mSpecifierName));
    break;
  case EDyResourceType::Model:
  case EDyResourceType::Material:
    MDY_NOT_IMPLEMENTED_ASSERT(); break;
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
  { // TEMPORARY
    auto instance = new CDyShaderResource_Deprecated();
    const auto i = instance->pfInitializeResource(*infoManager.GetShaderInformation(result.mSpecifierName));
    MDY_ASSERT(i != DY_FAILURE, "");
    result.mSmtPtrResultInstance = instance;
  } break;
  case EDyResourceType::Texture:
  { // TEMPORARY
    auto instance = new CDyTextureResource_Deprecated();
    const auto i = instance->pfInitializeTextureResource(*infoManager.GetTextureInformation(result.mSpecifierName));
    MDY_ASSERT(i != DY_FAILURE, "");
    result.mSmtPtrResultInstance = instance;
  } break;
  case EDyResourceType::Model:
  case EDyResourceType::Material:
    MDY_NOT_IMPLEMENTED_ASSERT(); break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  return result;
}

} /// ::dy namespace