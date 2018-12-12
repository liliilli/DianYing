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
#include <Dy/Core/Thread/TDyIO.h>
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>

#define MDY_CALL_BUT_NOUSE_RESULT(__MAExpression__) \
  { MDY_NOTUSED const auto _ = __MAExpression__; }

namespace dy
{

TDyIO::TDyIO()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyMetaInfo::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOData::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOResource::Initialize());

  this->mMetaInfoManager    = &MDyMetaInfo::GetInstance();
  this->mIODataManager      = &MDyIOData::GetInstance();
  this->mIOResourceManager  = &MDyIOResource::GetInstance();
}

TDyIO::~TDyIO()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOData::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyMetaInfo::Release());

  this->mIOResourceManager  = nullptr;
  this->mIODataManager      = nullptr;
  this->mMetaInfoManager    = nullptr;
}

EDySuccess TDyIO::Initialize()
{
  // Initialize IOWorkers with context.
  const auto& windowManager = MDyWindow::GetInstance();
  const auto& workerWndList = windowManager.GetGLWorkerWindowList();
  MDY_ASSERT(workerWndList.size() == this->mWorkerList.size(), "WndList and I/O Worker list size must be same.");

  for (auto i = 0u; i < this->mWorkerList.size(); ++i)
  {
    auto& [instance, thread] = this->mWorkerList[i];
    instance  = std::make_unique<TDyIOWorker>(MDyMetaInfo::GetInstance());
    thread    = std::thread(&TDyIOWorker::operator(), std::ref(*instance), DyMakeNotNull(workerWndList[i]));
  }

  return DY_SUCCESS;
}

void TDyIO::Release()
{
  // Stop and release IOWorkers
  for (auto& [instance, thread] : this->mWorkerList)
  {
    instance->outTryStop();
    thread.join();
    instance = nullptr;
  }

  MDY_NOT_IMPLEMENTED_ASSERT();
}

void TDyIO::operator()()
{
  /// @brief Waiting callback for condition variable for task queueing processing routine.
  static auto CbTaskQueueWaiting = [this]
  {
    return this->mIsThreadStopped == true || this->mIOTaskQueue.empty() == false;
  };

  /// @brief Assign picked to workers.
  static auto inAssignTaskToWorkers = [](_MIN_ const DDyIOTask& task, _MIO_ TDyIO::TWorkerList& workerList)
  {
    for (auto& [instance, thread] : workerList)
    {
      const auto isSuccessful = instance->outTryAssign(task);
      if (isSuccessful == DY_SUCCESS) { return; }
    }
    MDY_UNEXPECTED_BRANCH();
  };

  //!
  //! FUNCTION BODY
  //!

  MDY_CALL_ASSERT_SUCCESS(this->Initialize());
  while (true)
  {
    DDyIOTask task;
    { // Wait task in the queue, and try pop task when not empty.
      MDY_SYNC_WAIT_CONDITION(this->mQueueMutex, this->mConditionVariable, CbTaskQueueWaiting);
      if (this->mIsThreadStopped == true && this->mIOTaskQueue.empty() == true) { break; }
      task = this->mIOTaskQueue.top();
      this->mIOTaskQueue.pop();
    }

    { // Wait any worker is idle, if there is idle worker assign task to worker.
      this->mWorkerSemaphore.Wait();
      this->mIdleWorkerCounter.fetch_sub(1);
      inAssignTaskToWorkers(task, this->mWorkerList);
    }
  }

  this->Release();
}

void TDyIO::outInsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept
{
  MDY_SYNC_LOCK_GUARD(this->mResultListMutex);
  this->mWorkerResultList.emplace_back(result);
}

void TDyIO::outTryNotifyWorkerIsIdle()
{
  this->mWorkerSemaphore.Notify();
  this->mIdleWorkerCounter.fetch_add(1);
}

void TDyIO::outTryForwardToMainTaskList(_MIN_ const DDyIOTask& task) noexcept
{
  MDY_SYNC_LOCK_GUARD(this->mProcessTaskFromMainMutex);
  this->mIOProcessMainTaskList.emplace_back(task);
}

void TDyIO::outTryStop()
{
  {
    MDY_SYNC_LOCK_GUARD(this->mQueueMutex);
    this->mIsThreadStopped = true;
  }
  this->mConditionVariable.notify_one();
}

EDySuccess TDyIO::outCreateReferenceInstance(
    _MIN_ const std::string& specifier,
    _MIN_ EDyResourceType type,
    _MIN_ EDyResourceStyle style,
    _MIN_ EDyScope scope)
{
  switch (style)
  {
  case EDyResourceStyle::Information: return this->mRIInformationMap.CreateReferenceInstance(specifier, type, style, scope);
  case EDyResourceStyle::Resource:    return this->mRIResourceMap.CreateReferenceInstance(specifier, type, style, scope);
  default: MDY_UNEXPECTED_BRANCH();   break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
}

void TDyIO::outMainForceProcessDeferredMainTaskList()
{
  MDY_SYNC_LOCK_GUARD(this->mProcessTaskFromMainMutex);
  for (const auto& task : this->mIOProcessMainTaskList)
  {
    SDyIOConnectionHelper::InsertResult(outMainProcessTask(task));
  }
}

DDyIOWorkerResult TDyIO::outMainProcessTask(_MIN_ const DDyIOTask& task)
{
  DDyIOWorkerResult result{};
  { // Copy properties to result instance.
    result.mResourceType        = task.mResourceType;
    result.mResourceStyle       = task.mResourcecStyle;
    result.mSpecifierName       = task.mSpecifierName;
    result.mIsHaveDeferredTask  = task.mIsResourceDeferred;
  }

  MDY_ASSERT(task.mResourcecStyle == EDyResourceStyle::Resource, "Main deferred task must be resource style.");
  const auto& infoManager = MDyIOData::GetInstance();

  switch (task.mResourceType) {
  case EDyResourceType::GLShader:
  { // Need to move it as independent function.
    auto instance = new CDyShaderResource_Deprecated();
    const auto error = instance->pfInitializeResource(*infoManager.GetShaderInformation(result.mSpecifierName));
    MDY_ASSERT(error != DY_FAILURE, "");
    result.mSmtPtrResultInstance = instance;
  } break;
  case EDyResourceType::Texture:
  case EDyResourceType::Model:
  case EDyResourceType::Material:
    MDY_UNEXPECTED_BRANCH();
  default: MDY_UNEXPECTED_BRANCH(); break;
  };

  return result;
}

EDySuccess TDyIO::outTryEnqueueTask(
    _MIN_ const std::string& specifier,
    _MIN_ EDyResourceType resourceType, _MIN_ EDyResourceStyle resourceStyle,
    _MIN_ EDyScope scope, _MIN_ bool isDerivedFromResource)
{
  /// @brief Check RI is exist, so enlarge scope and update properties etc.
  static auto CheckAndUpdateReferenceInstance = [scope, this](const std::string& specifier, const auto& resourceType, const auto& resourceStyle) -> bool
  {
    if (this->outIsReferenceInstanceExist(specifier, resourceType, resourceStyle) == true)
    {
      this->outTryEnlargeResourceScope(scope, specifier, resourceType, resourceStyle);
      return true;
    }
    if (this->mGarbageCollector.IsReferenceInstanceExist(specifier, resourceType, resourceStyle) == true)
    {
      MDY_CALL_ASSERT_SUCCESS(this->outTryRetrieveReferenceInstanceFromGC(specifier, resourceType, resourceStyle));
      //this->mGarbageCollector.MoveInstanceFromGC(specifier, resourceType, resourceStyle);
      return true;
    }

    return false;
  };

  MDY_ASSERT(this->outIsMetaInformationExist(specifier, resourceType) == true, "Meat information must be exist.");
  constexpr TU08 kDefaultPriority = 128;

  bool isShouldDeferred = false;
  if (resourceStyle == EDyResourceStyle::Resource)
  { // If `Resource` task, find `Information` is set up or Create `Information` task newly.
    const auto flag   = this->outTryEnqueueTask(specifier, resourceType, EDyResourceStyle::Information, scope, true);
    isShouldDeferred  = flag == DY_SUCCESS ? true : false;
  }
  if (const auto isExist = CheckAndUpdateReferenceInstance(specifier, resourceType, resourceStyle); isExist == true)
  { // This logic is intentional for insert `Resource` task to deferred task list when `Information` is not set up yet.
    if (isDerivedFromResource == false) { return DY_SUCCESS; }
    else                                { return DY_FAILURE; }
  }

  MDY_CALL_ASSERT_SUCCESS(this->outCreateReferenceInstance(specifier, resourceType, resourceStyle, scope));

  // Construct IO Task.
  DDyIOTask task = {};
  task.mSpecifierName = specifier;
  task.mObjectStyle   = EDyObject::Etc_NotBindedYet;
  task.mPtrBoundObject= nullptr;
  task.mResourceType  = resourceType;
  task.mResourcecStyle= resourceStyle;
  task.mScope         = scope;
  task.mTaskPriority  = kDefaultPriority;
  task.mIsResourceDeferred = isDerivedFromResource;

  if (isShouldDeferred == true)
  { // If `Resource` should be deferred task,
    // Insert deferred task and iterate list that there is something to be reinsert to Queue.
    // Because it does not guarantees this routine would be called prior to finding `Resource`
    // from IO Worker queue afterward creating `Information`. (It will damage performance, but there is no way...)
    this->outInsertDeferredTaskList(task);
  }
  else
  { // Just insert task to queue, if anything does not happen.
    {
      MDY_SYNC_LOCK_GUARD(this->mQueueMutex);
      if (this->mIsThreadStopped == true) { return DY_FAILURE; }
      this->mIOTaskQueue.emplace(task);
    }
    this->mConditionVariable.notify_one();
  }

  return DY_SUCCESS;
}

void TDyIO::outTryEnlargeResourceScope(_MIN_ EDyScope scope, _MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  // @TODO IMPELEMNT THIS
}

EDySuccess TDyIO::outTryRetrieveReferenceInstanceFromGC(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  // @TODO IMPELEMNT THIS
  return DY_SUCCESS;
}

bool TDyIO::outIsMetaInformationExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mMetaInfoManager), "MetaInformation manager must not be null.");
  switch (type)
  {
  case EDyResourceType::Script:     return this->mMetaInfoManager->IsScriptMetaInformationExist(specifier);
  case EDyResourceType::Model:      return this->mMetaInfoManager->IsModelMetaInfoExist(specifier);
  case EDyResourceType::GLShader:   return this->mMetaInfoManager->IsGLShaderMetaInfoExist(specifier);
  case EDyResourceType::Texture:    return this->mMetaInfoManager->IsTextureMetaInfoExist(specifier);
  case EDyResourceType::Material:   return this->mMetaInfoManager->IsMaterialMetaInfoExist(specifier);
  case EDyResourceType::WidgetMeta: return this->mMetaInfoManager->IsWidgetMetaInfoExist(specifier);
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
}

bool TDyIO::outIsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  switch (style)
  {
  case EDyResourceStyle::Information:
    return this->mRIInformationMap.IsReferenceInstanceExist(specifier, type);
  case EDyResourceStyle::Resource:
    return this->mRIResourceMap.IsReferenceInstanceExist(specifier, type);
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
}

void TDyIO::outInsertDeferredTaskList(_MIN_ const DDyIOTask& task)
{
  MDY_SYNC_LOCK_GUARD(this->mDeferredTaskMutex);
  this->mIODeferredTaskList.emplace_back(task);
}

void TDyIO::outTryUpdateDeferredTaskList(_MIN_ EDyResourceType type, _MIN_ const std::string& specifier)
{
  TDeferredTaskList::value_type result;
  {
    MDY_SYNC_LOCK_GUARD(this->mDeferredTaskMutex);
    auto resultIt = std::find_if(MDY_BIND_BEGIN_END(this->mIODeferredTaskList), [=](const TDeferredTaskList::value_type& instance)
    {
      return instance.mSpecifierName == specifier
          && instance.mResourceType == type;
    });
    MDY_ASSERT(resultIt != this->mIODeferredTaskList.end(), "IODeferredTaskList given iterator must be found.");
    result = *resultIt;
    *resultIt = std::move(this->mIODeferredTaskList.back());
    this->mIODeferredTaskList.pop_back();
  }

  result.mTaskPriority = 0xFF;

  {
    MDY_SYNC_LOCK_GUARD(this->mQueueMutex);
    if (this->mIsThreadStopped == true) { return; }
    this->mIOTaskQueue.emplace(result);
  }
  this->mConditionVariable.notify_one();
}

bool TDyIO::outCheckIOResultInCondition() noexcept
{
  MDY_SYNC_LOCK_GUARD(this->mResultListMutex);
  return this->mWorkerResultList.empty() == false;
}

void TDyIO::outForceProcessIOInsertPhase() noexcept
{
  // CRITICAL PERFORMANCE DOWN! MUST BE CALLED IN SYNCHRONIZATION PHASE.
  MDY_SYNC_LOCK_GUARD(this->mResultListMutex);
  for (auto& resultItem : this->mWorkerResultList)
  {
    MDY_ASSERT(resultItem.mResourceType != EDyResourceType::NoneError
            && resultItem.mResourceType != EDyResourceType::Script
            && resultItem.mResourceType != EDyResourceType::WidgetMeta, "Unexpected error occurred.");

    if (resultItem.mResourceStyle == EDyResourceStyle::Information)
    { // If Information, insert result into mIOData.
      mIODataManager->InsertResult(resultItem.mResourceType, resultItem.mSmtPtrResultInstance);

      if (resultItem.mIsHaveDeferredTask == true)
      { // If need to insert resource task of deferred list into queue, do this.
        outTryUpdateDeferredTaskList(resultItem.mResourceType, resultItem.mSpecifierName);
      }
    }
    else
    { // If Resource, insert result into mIOResource.
      if (resultItem.mSmtPtrResultInstance == nullptr) { continue; }
      mIOResourceManager->InsertResult(resultItem.mResourceType, resultItem.mSmtPtrResultInstance);
    }
  }
  this->mWorkerResultList.clear();
}

bool TDyIO::outIsIOThreadSlept() noexcept
{
  bool i = false;
  {
    MDY_SYNC_LOCK_GUARD(this->mQueueMutex);
    MDY_SYNC_LOCK_GUARD(this->mDeferredTaskMutex);
    MDY_SYNC_LOCK_GUARD(this->mResultListMutex);

    i = this->mIOTaskQueue.empty()
        && this->mIdleWorkerCounter.load() == dy::TDyIO::kWorkerThreadCount
        && this->mIODeferredTaskList.empty()
        && this->mWorkerResultList.empty();
  }

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(0ms);
  return i;
}

bool TDyIO::isoutIsMainTaskListIsEmpty() const noexcept
{
  return this->mIOProcessMainTaskList.empty();
}

} /// :: dy namesapace