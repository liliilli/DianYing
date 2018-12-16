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
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>

#include <Dy/Management/IO/MDyIOData.h>
#include <Dy/Management/IO/MDyIOResource.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/WindowManager.h>
#include "Dy/Core/Resource/Resource/FDyModelResource.h"
#include "Dy/Core/Resource/Internal/FDyModelVBOIntermediate.h"

#define MDY_CALL_BUT_NOUSE_RESULT(__MAExpression__) \
  { MDY_NOTUSED const auto MDY_TOKENPASTE2(_, __LINE__) = __MAExpression__; }

constexpr TU08 kDefaultPriority = 128;

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

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTION BODY
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

EDySuccess TDyIO::outCreateReferenceInstance(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style, _MIN_ EDyScope scope)
{
  switch (style)
  {
  case EDyResourceStyle::Information: return this->mRIInformationMap.CreateReferenceInstance(specifier, type, style, scope);
  case EDyResourceStyle::Resource:    return this->mRIResourceMap.CreateReferenceInstance(specifier, type, style, scope);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
}

EDySuccess TDyIO::outTryEnqueueTask(
    _MIN_ const std::string& iSpecifier,
    _MIN_ EDyResourceType iResourceType, _MIN_ EDyResourceStyle iResourceStyle,
    _MIN_ EDyScope iScope, _MIN_ bool iIsDerivedFromResource)
{
  MDY_ASSERT(this->outIsMetaInformationExist(iSpecifier, iResourceType) == true, "Meta information must be exist.");

  { // Query there is Reference Instance for myself. If found, just return do nothing.
    std::vector<PRIVerificationItem> itselfRIItem{};
    itselfRIItem.emplace_back(iSpecifier, iResourceType, iResourceStyle, iScope);
    const auto result = this->pCheckAndUpdateReferenceInstance(itselfRIItem);
    if (result.empty() == true || result.begin()->second != EDyRIStatus::NotValid) { return DY_SUCCESS; }
  }

  DDyIOTaskDeferred::TConditionList conditionList = {};
  const auto checkList = this->pMakeDependenciesCheckList(iSpecifier, iResourceType, iResourceStyle, iScope);
  if (checkList.empty() == false)
  { // Make dependency list.
    const auto notFoundRIList = this->pCheckAndUpdateReferenceInstance(checkList);

    if (notFoundRIList.empty() == false)
    {
      for (const auto& [instance, status] : notFoundRIList)
      { // Require depende resource tasks only if NotValid but RI is exist.
        const auto& [specifier, type, style, scope] = instance;
        if (status == EDyRIStatus::NotValid) { MDY_CALL_BUT_NOUSE_RESULT(outTryEnqueueTask(specifier, type, style, scope, true)); }

        conditionList.emplace_back(specifier, type, style);
      }
    }
  }

  // Require itself own resource task.
  MDY_CALL_ASSERT_SUCCESS(this->outCreateReferenceInstance(iSpecifier, iResourceType, iResourceStyle, iScope));
  // Construct IO Tasks.
  DDyIOTask task;
  {
    task.mSpecifierName = iSpecifier;
    task.mResourceType  = iResourceType;
    task.mResourcecStyle= iResourceStyle;
    task.mScope         = iScope;
    task.mTaskPriority  = kDefaultPriority;
    task.mIsResourceDeferred = iIsDerivedFromResource;
    task.mBoundObjectStyle= EDyObject::Etc_NotBindedYet;
    task.mPtrBoundObject= nullptr;
  }

  // If this is model & resource task, change `mResourceType` to `__ModelVBO` as intermediate task.
  // Because VAO can not be created and shared from other thread not main thread.
  if (task.mResourceType == EDyResourceType::Model && task.mResourcecStyle == EDyResourceStyle::Resource) { task.mResourceType = EDyResourceType::__ModelVBO; }

  // Make deferred task and forward deferred task to list (atomic)
  if (conditionList.empty() == false) { this->outInsertDeferredTaskList({task, conditionList}); }
  else
  {   // Just insert task to queue, if anything does not happen.
    { // Critical section.
      MDY_SYNC_LOCK_GUARD(this->mQueueMutex);
      if (this->mIsThreadStopped == true) { return DY_FAILURE; }
      this->mIOTaskQueue.emplace(task);
    }
    this->mConditionVariable.notify_one();
  }

  return DY_SUCCESS;
}

std::vector<TDyIO::PRIVerificationItem> TDyIO::pMakeDependenciesCheckList(
    _MIN_ const std::string& iSpecifier,
    _MIN_ EDyResourceType iResourceType,
    _MIN_ EDyResourceStyle iResourceStyle,
    _MIN_ EDyScope iScope)
{
  std::vector<PRIVerificationItem> checkList = {};

  if (iResourceType == EDyResourceType::Material)
  { // If resource type is `Material`, bind all dependencies of `Material` to checkList.
    const auto& metaMaterial = this->mMetaInfoManager->GetMaterialMetaInformation(iSpecifier);
    for (const auto& textureSpecifier : metaMaterial.mTextureNames) {
      if (textureSpecifier.empty() == true) { break; }
      checkList.emplace_back(textureSpecifier, EDyResourceType::Texture, iResourceStyle, iScope);
    }
    checkList.emplace_back(metaMaterial.mShaderSpecifier, EDyResourceType::GLShader, iResourceStyle, iScope);
  }

  if (iResourceType == EDyResourceType::GLFrameBuffer)
  { // If resource type is `FrameBuffer`, bind all dependencies of `FrameBuffer` to checkList.
    const auto& metaMaterial = this->mMetaInfoManager->GetGlFrameBufferMetaInformation(iSpecifier);
    for (const auto& [specifier, type] : metaMaterial.mAttachmentList)
    { // Get dependent attachment specifier list and add.
      checkList.emplace_back(specifier, EDyResourceType::GLAttachment, iResourceStyle, iScope);
    }
  }

  if (iResourceStyle == EDyResourceStyle::Resource)
  {
    switch (iResourceType)
    {
    case EDyResourceType::Model:    case EDyResourceType::GLShader:
    case EDyResourceType::Texture:  case EDyResourceType::Material:
    case EDyResourceType::GLAttachment: case EDyResourceType::GLFrameBuffer:
      checkList.emplace_back(iSpecifier, iResourceType, EDyResourceStyle::Information, iScope);
      break;
    default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(checkList);
    }
  }

  return checkList;
}

TDyIO::TDependencyList TDyIO::pCheckAndUpdateReferenceInstance(_MIN_ const std::vector<PRIVerificationItem>& dependencies) noexcept
{
  TDependencyList resultNotFoundList = {};

  for (const auto& [specifier, type, style, scope] : dependencies)
  { // Find dependencies is on memory (not GCed, and avoid duplicated task queing)
    if (this->pIsReferenceInstanceExist(specifier, type, style) == true)
    {
      this->pTryEnlargeResourceScope(scope, specifier, type, style);
      if (this->pIsReferenceInstanceBound(specifier, type, style) == true) { continue; }
      else
      {
        resultNotFoundList.emplace_back(PRIVerificationItem{specifier, type, style, scope}, EDyRIStatus::NotBoundYet);
        continue;
      }
    }

    if (this->mGarbageCollector.IsReferenceInstanceExist(specifier, type, style) == true)
    {
      MDY_CALL_ASSERT_SUCCESS(this->outTryRetrieveReferenceInstanceFromGC(specifier, type, style));
      //this->mGarbageCollector.MoveInstanceFromGC(specifier, resourceType, resourceStyle);
      continue;
    }

    resultNotFoundList.emplace_back(PRIVerificationItem{specifier, type, style, scope}, EDyRIStatus::NotValid);
  }

  return resultNotFoundList;
}

void TDyIO::pTryEnlargeResourceScope(_MIN_ EDyScope scope, _MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  // @TODO IMPELEMNT THIS
}

EDySuccess TDyIO::outTryRetrieveReferenceInstanceFromGC(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  // @TODO IMPELEMNT THIS
  return DY_SUCCESS;
}

void TDyIO::outInsertDeferredTaskList(_MIN_ const DDyIOTaskDeferred& task)
{
  MDY_SYNC_LOCK_GUARD(this->mDeferredTaskMutex);
  this->mIODeferredTaskList.emplace_back(task);
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
    if (resultItem.mSmtPtrResultInstance == nullptr) { continue; }

    if (resultItem.mResourceStyle == EDyResourceStyle::Information)
    {
      this->mIODataManager->InsertResult(resultItem.mResourceType, resultItem.mSmtPtrResultInstance);
      MDY_CALL_ASSERT_SUCCESS(this->mRIInformationMap.TryUpdateValidity(resultItem.mResourceType, resultItem.mSpecifierName, true));
    }
    else
    {
      this->mIOResourceManager->InsertResult(resultItem.mResourceType, resultItem.mSmtPtrResultInstance);
      MDY_CALL_ASSERT_SUCCESS(this->mRIResourceMap.TryUpdateValidity(resultItem.mResourceType, resultItem.mSpecifierName, true));
    }

    // If need to insert resource task of deferred list into queue, do this.
    this->pTryUpdateDeferredTaskList(resultItem.mSpecifierName, resultItem.mResourceType, resultItem.mResourceStyle);
  }

  this->mWorkerResultList.clear();
}

void TDyIO::pTryUpdateDeferredTaskList(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ EDyResourceStyle iStyle)
{
  std::vector<DDyIOTask> reinsertionTasklist = {};

  { // Critical Section
    MDY_SYNC_LOCK_GUARD(this->mDeferredTaskMutex);
    for (auto it = this->mIODeferredTaskList.begin(); it != this->mIODeferredTaskList.end();)
    {
      auto& deferredTask = *it;
      // Try remove condition item. If removed something, try it'is satisfied with reinsertion condition.
      if (deferredTask.TryRemoveDependenciesItem(iSpecifier, iType, iStyle) == DY_SUCCESS
      &&  deferredTask.IsSatisfiedReinsertCondition() == true)
      {
        reinsertionTasklist.emplace_back(deferredTask.mTask);
        it = this->mIODeferredTaskList.erase(it);
      }
      else { ++it; }
    }
  }

  for (auto& task : reinsertionTasklist) { task.mTaskPriority = 0xFF; }

  { // Critical Section
    MDY_SYNC_LOCK_GUARD(this->mQueueMutex);
    for (const auto& task : reinsertionTasklist)
    {
      if (this->mIsThreadStopped == true) { return; }
      this->mIOTaskQueue.emplace(task);
    }
  }
  this->mConditionVariable.notify_one();
}

//!
//! Task resource population from main thread.
//!

void TDyIO::outMainForceProcessDeferredMainTaskList()
{
  MDY_SYNC_LOCK_GUARD(this->mProcessTaskFromMainMutex);
  for (const auto& task : this->mIOProcessMainTaskList)
  {
    SDyIOConnectionHelper::InsertResult(outMainProcessTask(task));
  }
  this->mIOProcessMainTaskList.clear();
}

#define MDY_DELETE_RAWHEAP_SAFELY(__MAHeapInstance__) \
  delete __MAHeapInstance__; \
  __MAHeapInstance__ = MDY_INITIALIZE_NULL

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
    const auto instance = new FDyShaderResource(*infoManager.GetPtrInformation<EDyResourceType::GLShader>(result.mSpecifierName));
    result.mSmtPtrResultInstance = instance;
  } break;
  case EDyResourceType::Texture:
  case EDyResourceType::Model:
  { // Get intemediate instance from task, and make model resource. (Information => `Immediate Instance` => Resource)
    Owner<FDyModelVBOIntermediate*> ptrrawIntermediateInstance = static_cast<FDyModelVBOIntermediate*>(task.mRawInstanceForUsingLater);
    const auto instance = new FDyModelResource(*ptrrawIntermediateInstance);
    MDY_DELETE_RAWHEAP_SAFELY(ptrrawIntermediateInstance);
    result.mSmtPtrResultInstance = instance;
  } break;
  case EDyResourceType::Material:
    MDY_UNEXPECTED_BRANCH();
  default: MDY_UNEXPECTED_BRANCH(); break;
  };

  return result;
}

//!
//! Condition
//!

bool TDyIO::pIsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  switch (style)
  {
  case EDyResourceStyle::Information: return this->mRIInformationMap.IsReferenceInstanceExist(specifier, type);
  case EDyResourceStyle::Resource:    return this->mRIResourceMap.IsReferenceInstanceExist(specifier, type);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

EDySuccess TDyIO::TryBindBinderToResourceRI(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  return this->mRIResourceMap.TryBindBinderToResourceRI(iSpecifier, iType, iPtrBinder);
}

bool TDyIO::pIsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  switch (style)
  {
  case EDyResourceStyle::Information:
    return this->mRIInformationMap.IsReferenceInstanceBound(specifier, type);
  case EDyResourceStyle::Resource:
    return this->mRIResourceMap.IsReferenceInstanceBound(specifier, type);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool TDyIO::outIsMetaInformationExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type)
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mMetaInfoManager), "MetaInformation manager must not be null.");
  switch (type)
  {
  case EDyResourceType::Script:         return this->mMetaInfoManager->IsScriptMetaInformationExist(specifier);
  case EDyResourceType::Model:          return this->mMetaInfoManager->IsModelMetaInfoExist(specifier);
  case EDyResourceType::GLShader:       return this->mMetaInfoManager->IsGLShaderMetaInfoExist(specifier);
  case EDyResourceType::Texture:        return this->mMetaInfoManager->IsTextureMetaInfoExist(specifier);
  case EDyResourceType::Material:       return this->mMetaInfoManager->IsMaterialMetaInfoExist(specifier);
  case EDyResourceType::WidgetMeta:     return this->mMetaInfoManager->IsWidgetMetaInfoExist(specifier);
  case EDyResourceType::GLAttachment:   return this->mMetaInfoManager->IsAttachmentMetaInfoExist(specifier);
  case EDyResourceType::GLFrameBuffer:  return this->mMetaInfoManager->IsFrameBufferMetaInfoExist(specifier);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool TDyIO::outIsIOThreadSlept() noexcept
{
  bool sleptFlag;
  {
    MDY_SYNC_LOCK_GUARD(this->mQueueMutex);
    MDY_SYNC_LOCK_GUARD(this->mDeferredTaskMutex);
    MDY_SYNC_LOCK_GUARD(this->mResultListMutex);

    sleptFlag = this->mIOTaskQueue.empty()
        && this->mIdleWorkerCounter.load() == dy::TDyIO::kWorkerThreadCount
        && this->mIODeferredTaskList.empty()
        && this->mWorkerResultList.empty();
  }

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(0ms);
  return sleptFlag;
}

bool TDyIO::isoutIsMainTaskListIsEmpty() const noexcept
{
  return this->mIOProcessMainTaskList.empty();
}

bool TDyIO::outCheckIOResultInCondition() noexcept
{
  MDY_SYNC_LOCK_GUARD(this->mResultListMutex);
  return this->mWorkerResultList.empty() == false;
}

} /// :: dy namesapace