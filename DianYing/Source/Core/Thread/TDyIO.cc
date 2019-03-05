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
#include <Dy/Core/Thread/TDyIO.h>

#include <assimp/Importer.hpp>

#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Internal/FDyModelVBOIntermediate.h>

#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Meta/Information/MetaInfoModelAnim.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>

#include <Dy/Management/IO/MDyIOData.h>
#include <Dy/Management/IO/MDyIOResource.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/WindowManager.h>

constexpr TU08 kDefaultPriority = 128;

namespace dy
{

TDyIO::TDyIO()
{
  this->mMetaInfoManager    = &MDyMetaInfo::GetInstance();
  this->mIODataManager      = &MDyIOData::GetInstance();
  this->mIOResourceManager  = &MDyIOResource::GetInstance();
}

TDyIO::~TDyIO()
{
  this->mIOResourceManager  = nullptr;
  this->mIODataManager      = nullptr;
  this->mMetaInfoManager    = nullptr;
}

EDySuccess TDyIO::Initialize()
{
  // Initialize IOWorkers with context.
  const auto& windowManager = MDyWindow::GetInstance();
  const auto& workerWndList = windowManager.GetGLWorkerWindowList();
  MDY_ASSERT_MSG(workerWndList.size() == this->mWorkerList.size(), "WndList and I/O Worker list size must be same.");

  for (auto i = 0u; i < this->mWorkerList.size(); ++i)
  {
    auto& [instance, thread] = this->mWorkerList[i];
    instance  = std::make_unique<TDyIOWorker>(MDyMetaInfo::GetInstance());
    thread    = std::thread(&TDyIOWorker::operator(), std::ref(*instance), DyMakeNotNull(workerWndList[i]));
  }

  return DY_SUCCESS;
}

void TDyIO::Release()
{ }

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
      if (this->mIsThreadStopped == true && this->mIOTaskQueue.empty() == true) 
      { 
        for (auto& [workerInstance, workerThread] : this->mWorkerList)
        { // Wait all worker thread is terminated.
          workerInstance->outTryStop();
          workerThread.join();
          workerInstance = nullptr;
        }
        break; 
      }
      task = this->mIOTaskQueue.top();
      this->mIOTaskQueue.pop();
    }

    { // Wait any worker is idle, if there is idle worker assign task to worker.
      this->mWorkerSemaphore.Wait();
      this->mIdleWorkerCounter.fetch_sub(1);
      inAssignTaskToWorkers(task, this->mWorkerList);
      MDY_SLEEP_FOR_ATOMIC_TIME();
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
  MDY_ASSERT_MSG(this->outIsIOThreadSlept() == true, "To stop io thread, IO Thread must be slept.");
  {
    MDY_SYNC_LOCK_GUARD(this->mQueueMutex);
    this->mIsThreadStopped = true;
  }
  this->mConditionVariable.notify_one();
  MDY_SLEEP_FOR_ATOMIC_TIME();
}

EDySuccess TDyIO::outTryEnqueueTask(
    _MIN_ const std::string& iSpecifier,
    _MIN_ EDyResourceType iResourceType, _MIN_ EDyResourceStyle iResourceStyle,
    _MIN_ EDyScope iScope, _MIN_ bool iIsDerivedFromResource)
{
  MDY_ASSERT_MSG_FORCE(this->outIsMetaInformationExist(iSpecifier, iResourceType) == true, "Meta information must be exist.");

  { // Query there is Reference Instance for myself. If found, just return do nothing.
    std::vector<PRIVerificationItem> itselfRIItem{};
    itselfRIItem.emplace_back(iSpecifier, iResourceType, iResourceStyle, iScope);
    const auto result = this->pCheckAndUpdateReferenceInstance(itselfRIItem);
    if (result.empty() == true || result.begin()->second != EDyRIStatus::NotValid) { return DY_SUCCESS; }
  }

  // Make dependency list.
  DDyIOTaskDeferred::TConditionList conditionList = {};
  const auto checkList = this->pMakeDependenciesCheckList(iSpecifier, iResourceType, iResourceStyle, iScope);
  if (checkList.empty() == false)
  { // And get not-found list from dependency list.
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
    task.mSpecifierName       = iSpecifier;
    task.mResourceType        = iResourceType;
    task.mResourcecStyle      = iResourceStyle;
    task.mScope               = iScope;
    task.mTaskPriority        = kDefaultPriority;
    task.mIsResourceDeferred  = iIsDerivedFromResource;
    task.mBoundObjectStyle    = EDyObject::Etc_NotBindedYet;
    task.mPtrBoundObject      = nullptr;
  }

  // If this is model & resource task, change `mResourceType` to `__ModelVBO` as intermediate task.
  // Because VAO can not be created and shared from other thread not main thread.
  if (task.mResourceType == EDyResourceType::Mesh 
   && task.mResourcecStyle == EDyResourceStyle::Resource) 
  { 
    task.mResourceType = EDyResourceType::__MeshVBO;
  }

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

EDySuccess TDyIO::outCreateReferenceInstance(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style, _MIN_ EDyScope scope)
{
  switch (style)
  {
  case EDyResourceStyle::Information: return this->mRIInformationMap.CreateReferenceInstance(specifier, type, style, scope);
  case EDyResourceStyle::Resource:    return this->mRIResourceMap.CreateReferenceInstance(specifier, type, style, scope);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
}

std::vector<TDyIO::PRIVerificationItem> TDyIO::pMakeDependenciesCheckList(
    _MIN_ const std::string& iSpecifier,
    _MIN_ EDyResourceType iResourceType,
    _MIN_ EDyResourceStyle iResourceStyle,
    _MIN_ EDyScope iScope) const
{
  std::vector<PRIVerificationItem> checkList = {};

  if (iResourceType == EDyResourceType::Material)
  { // If resource type is `Material`, bind all dependencies of `Material` to checkList.
    const auto& metaMaterial = this->mMetaInfoManager->GetMaterialMetaInformation(iSpecifier);
    for (const auto& bindingTextureItem : metaMaterial.mTextureNames) 
    {
      if (bindingTextureItem.mTextureSpecifier.empty() == true) { break; }
      checkList.emplace_back(bindingTextureItem.mTextureSpecifier, EDyResourceType::Texture, iResourceStyle, iScope);
    }
    checkList.emplace_back(metaMaterial.mShaderSpecifier, EDyResourceType::GLShader, iResourceStyle, iScope);
  }
  else if (iResourceType == EDyResourceType::GLFrameBuffer)
  { // If resource type is `FrameBuffer`, bind all dependencies of `FrameBuffer` to checkList.
    const auto& metaInfo = this->mMetaInfoManager->GetGlFrameBufferMetaInformation(iSpecifier);

    // Get dependent attachment specifier list and add.
    for (const auto& [specifier, type] : metaInfo.mColorAttachmentList)
    { checkList.emplace_back(specifier, EDyResourceType::GLAttachment, iResourceStyle, iScope); }

    // If framebuffer also use depth buffer, enqueue it. 
    if (metaInfo.mIsUsingDepthBuffer == true)
    { 
      MDY_ASSERT_MSG(metaInfo.mDepthAttachmentSpecifier.empty() == false, "Depth attachment must be specified if use depth buffer.");
      checkList.emplace_back(metaInfo.mDepthAttachmentSpecifier, EDyResourceType::GLAttachment, iResourceStyle, iScope);
    }
  }
  else if (iResourceType == EDyResourceType::Model)
  { // If resource type is `Model` and if using builtin mesh specifier...
    const auto& metaInfo = this->mMetaInfoManager->GetModelMetaInformation(iSpecifier);
    for (const auto& [meshSpecifier, materialSpecifier] : metaInfo.mMeshList)
    { // Get dependent attachment specifier list and add.
      checkList.emplace_back(meshSpecifier,     EDyResourceType::Mesh,      iResourceStyle, iScope);
      checkList.emplace_back(materialSpecifier, EDyResourceType::Material,  iResourceStyle, iScope);
    }

    // If this model will use skeleton, add skeleton (information) also.
    // Skeleton resource only has `information`.
    if (metaInfo.mSkeleton.mIsUsingSkeleton == true)
    {
      checkList.emplace_back(
          metaInfo.mSkeleton.mSkeletonSpecifier, 
          EDyResourceType::Skeleton, EDyResourceStyle::Information, iScope);
    }
  }
  else if (iResourceType == EDyResourceType::AnimationScrap)
  { // If resource type is `AnimationScrap`, `Skeleton` must be populated also.
    const auto& metaInfo = this->mMetaInfoManager->GetModelAnimScrapMetaInformation(iSpecifier);
    MDY_ASSERT_MSG_FORCE(metaInfo.mSkeletonSpeicfier.empty() == false, "Skeleton specifier of animation scrap must not be empty.");

    // `AnimationScrap` could only be populated as `Information`, so dependent Skeleton also be a `Information`.
    checkList.emplace_back(metaInfo.mSkeletonSpeicfier, EDyResourceType::Skeleton, iResourceStyle, iScope);
  }

  // Resource common dependencies.
  if (iResourceStyle == EDyResourceStyle::Resource)
  {
    switch (iResourceType)
    {
    case EDyResourceType::Mesh:
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

EDySuccess TDyIO::InstantPopulateMaterialResource(
    _MIN_ const PDyMaterialInstanceMetaInfo& iDesc,
    _MIN_ TDyResourceBinder<EDyResourceType::Material>& refMat, 
    _MIN_ EDyScope iScope, 
    _MIN_ bool(*callback)())
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(callback),  "Callback material resource population is not supported yet.");
  MDY_ASSERT_MSG(iScope == EDyScope::Temporal, "Temporary material resource population must be inputted.");

  // If resource type is `Material`, bind all dependencies of `Material` to checkList.
  std::vector<PRIVerificationItem> checkList = {};
  { 
    for (const auto& bindingTextureItem : iDesc.mTextureNames) 
    {
      if (bindingTextureItem.mTextureSpecifier.empty() == true) { break; }
      checkList.emplace_back(bindingTextureItem.mTextureSpecifier, EDyResourceType::Texture, EDyResourceStyle::Resource, iScope);
    }
    checkList.emplace_back(iDesc.mShaderSpecifier, EDyResourceType::GLShader, EDyResourceStyle::Resource, iScope);
  }

  // If `checkList` is not empty, check dependencies.
  DDyIOTaskDeferred::TConditionList conditionList = {};
  if (checkList.empty() == false)
  { // And get not-found list from dependency list.
    const auto notFoundRIList = this->pCheckAndUpdateReferenceInstance(checkList);
    if (notFoundRIList.empty() == false)
    { 
      for (const auto& [instance, status] : notFoundRIList)
      { // Require depende resource tasks only if NotValid but RI is exist.
        const auto& [specifier, type, style, scope] = instance;
        if (status == EDyRIStatus::NotValid) 
        {
          MDY_CALL_BUT_NOUSE_RESULT(outTryEnqueueTask(specifier, type, style, scope, true)); 
        }

        conditionList.emplace_back(specifier, type, style);
      }
    }
  }

  // Require itself own resource task.
  MDY_CALL_ASSERT_SUCCESS(this->outCreateReferenceInstance(
      iDesc.mSpecifierName, 
      EDyResourceType::Material, EDyResourceStyle::Resource, iScope));

  // Construct IO Tasks.
  DDyIOTask task;
  {
    task.mSpecifierName       = iDesc.mSpecifierName;
    task.mResourceType        = EDyResourceType::Material;
    task.mResourcecStyle      = EDyResourceStyle::Resource;
    task.mScope               = EDyScope::Temporal;
    task.mTaskPriority        = 192;
    task.mIsResourceDeferred  = false;
    task.mBoundObjectStyle    = EDyObject::Etc_NotBindedYet;
    task.mPtrBoundObject      = &refMat;
    task.mRawInstanceForUsingLater = iDesc;
  }

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

TDyIO::TDependencyList TDyIO::pCheckAndUpdateReferenceInstance(_MIN_ const std::vector<PRIVerificationItem>& dependencies) noexcept
{
  TDependencyList resultNotFoundList = {};

  for (const auto& [specifier, type, style, scope] : dependencies)
  { 
    // Find if reference instance is on garbage collector.
    if (this->mGarbageCollector.IsReferenceInstanceExist(specifier, type, style) == true)
    {
      MDY_CALL_ASSERT_SUCCESS(this->outTryRetrieveReferenceInstanceFromGC(specifier, type, style));
      continue;
    }

    // Find dependencies is on memory (not GCed, and avoid duplicated task queing)
    if (this->pIsReferenceInstanceExist(specifier, type, style) == true)
    {
      // Try enlarge scope of RI. RI scope is large following by Global > Level > Temporal.
      this->pTryEnlargeResourceScope(scope, specifier, type, style);
      // Check if RI is bound by actual resource.
      if (this->pIsReferenceInstanceBound(specifier, type, style) == false)
      {
        resultNotFoundList.emplace_back(PRIVerificationItem{specifier, type, style, scope}, EDyRIStatus::NotBoundYet);
      }
      continue;
    }

    resultNotFoundList.emplace_back(PRIVerificationItem{specifier, type, style, scope}, EDyRIStatus::NotValid);
  }

  return resultNotFoundList;
}

void TDyIO::pTryEnlargeResourceScope(_MIN_ EDyScope scope, _MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  switch (style)
  {
  case EDyResourceStyle::Information: this->mRIInformationMap.TryEnlargeResourceScope(scope, specifier, type);  break;
  case EDyResourceStyle::Resource:    this->mRIResourceMap.TryEnlargeResourceScope(scope, specifier, type);     break;
  default: MDY_UNEXPECTED_BRANCH();
  }
}

EDySuccess TDyIO::outTryRetrieveReferenceInstanceFromGC(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  // Get RI from gc list.
  auto optRI = this->mGarbageCollector.MoveInstanceFromGC(specifier, type, style);
  if (optRI.has_value() == false) { return DY_FAILURE; }

  // Reinsert RI to appropriate position.
  switch (style)
  {
  case EDyResourceStyle::Information: return this->mRIInformationMap.MoveReferenceInstance(std::move(*optRI));
  case EDyResourceStyle::Resource:    return this->mRIResourceMap.MoveReferenceInstance(std::move(*optRI));
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }
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
    MDY_ASSERT_MSG(resultItem.mResourceType != EDyResourceType::NoneError
            && resultItem.mResourceType != EDyResourceType::Script
            && resultItem.mResourceType != EDyResourceType::WidgetMeta, "Unexpected error occurred.");
    if (resultItem.mSmtPtrResultInstance == nullptr) { continue; }

    if (resultItem.mResourceStyle == EDyResourceStyle::Information)
    {
      this->mIODataManager->InsertResult(resultItem.mResourceType, resultItem.mSmtPtrResultInstance);
      MDY_CALL_ASSERT_SUCCESS(this->mRIInformationMap.TryUpdateValidity(
          resultItem.mResourceType, 
          resultItem.mSpecifierName, 
          true,
          resultItem.mSmtPtrResultInstance));
    }
    else
    {
      this->mIOResourceManager->InsertResult(resultItem.mResourceType, resultItem.mSmtPtrResultInstance);
      MDY_CALL_ASSERT_SUCCESS(this->mRIResourceMap.TryUpdateValidity(
          resultItem.mResourceType, 
          resultItem.mSpecifierName, 
          true,
          resultItem.mSmtPtrResultInstance));
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

DDyIOWorkerResult TDyIO::outMainProcessTask(_MIN_ const DDyIOTask& task)
{
  DDyIOWorkerResult result{};
  { // Copy properties to result instance.
    result.mResourceType        = task.mResourceType;
    result.mResourceStyle       = task.mResourcecStyle;
    result.mSpecifierName       = task.mSpecifierName;
    result.mIsHaveDeferredTask  = task.mIsResourceDeferred;
  }

  MDY_ASSERT_MSG(task.mResourcecStyle == EDyResourceStyle::Resource, "Main deferred task must be resource style.");
  const auto& infoManager = MDyIOData::GetInstance();

  switch (task.mResourceType) {
  case EDyResourceType::GLShader:
  { // Need to move it as independent function.
    const auto instance = new FDyShaderResource(*infoManager.GetPtrInformation<EDyResourceType::GLShader>(result.mSpecifierName));
    result.mSmtPtrResultInstance = instance;
  } break;
  case EDyResourceType::Mesh:
  { // Get intermediate instance from task, and make mesh resource.
    Owner<FDyMeshVBOIntermediate*> ptrrawIntermediateInstance = std::any_cast<FDyMeshVBOIntermediate*>(task.mRawInstanceForUsingLater);
    const auto instance = new FDyMeshResource(*ptrrawIntermediateInstance);
    MDY_DELETE_RAWHEAP_SAFELY(ptrrawIntermediateInstance);
    result.mSmtPtrResultInstance = instance;
  } break;
  case EDyResourceType::GLFrameBuffer:
  { // Only Resource, create fbo with attachment.
    const auto instance = new FDyFrameBufferResource(*infoManager.GetPtrInformation<EDyResourceType::GLFrameBuffer>(result.mSpecifierName));
    result.mSmtPtrResultInstance = instance;
  } break;
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

EDySuccess TDyIO::TryBindBinderToResourceRI
(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  return this->mRIResourceMap.TryBindBinderToResourceRI(iSpecifier, iType, iPtrBinder);
}

EDySuccess TDyIO::TryBindBinderToInformationRI
(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  return this->mRIInformationMap.TryBindBinderToResourceRI(iSpecifier, iType, iPtrBinder);
}

EDySuccess TDyIO::TryDetachBinderFromResourceRI
(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  return this->mRIResourceMap.TryDetachBinderFromResourceRI(iSpecifier, iType, iPtrBinder);
}

EDySuccess TDyIO::TryDetachBinderFromInformationRI
(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ const __FDyBinderBase* iPtrBinder)
{
  return this->mRIInformationMap.TryDetachBinderFromResourceRI(iSpecifier, iType, iPtrBinder);
}

bool TDyIO::pIsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style)
{
  switch (style)
  {
  case EDyResourceStyle::Information: return this->mRIInformationMap.IsReferenceInstanceBound(specifier, type);
  case EDyResourceStyle::Resource:    return this->mRIResourceMap.IsReferenceInstanceBound(specifier, type);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool TDyIO::outIsMetaInformationExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mMetaInfoManager), "MetaInformation manager must not be null.");
  switch (type)
  {
  case EDyResourceType::Script:         return this->mMetaInfoManager->IsScriptMetaInformationExist(specifier);
  case EDyResourceType::Mesh:           return this->mMetaInfoManager->IsMeshMetaInfoExist(specifier);
  case EDyResourceType::Model:          return this->mMetaInfoManager->IsModelMetaInfoExist(specifier);
  case EDyResourceType::Skeleton:       return this->mMetaInfoManager->IsModelSkeletonMetaInfoExist(specifier);
  case EDyResourceType::AnimationScrap: return this->mMetaInfoManager->IsModelAnimScrapMetaInfoExist(specifier);
  case EDyResourceType::GLShader:       return this->mMetaInfoManager->IsGLShaderMetaInfoExist(specifier);
  case EDyResourceType::Texture:        return this->mMetaInfoManager->IsTextureMetaInfoExist(specifier);
  case EDyResourceType::Material:       return this->mMetaInfoManager->IsMaterialMetaInfoExist(specifier);
  case EDyResourceType::WidgetMeta:     return this->mMetaInfoManager->IsWidgetMetaInfoExist(specifier);
  case EDyResourceType::GLAttachment:   return this->mMetaInfoManager->IsAttachmentMetaInfoExist(specifier);
  case EDyResourceType::GLFrameBuffer:  return this->mMetaInfoManager->IsFrameBufferMetaInfoExist(specifier);
  case EDyResourceType::Sound:          return this->mMetaInfoManager->IsSoundMetaInfoExist(specifier);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

void TDyIO::BindSleepCallbackFunction(_MIN_ std::function<void()> iCbFunc)
{
  if (this->mCbSleepFunction == nullptr)
  {
    mCbSleepFunction = nullptr;
    mCbSleepFunction = iCbFunc;
  }
  else
  {
    this->mCbNextSleepFunction = nullptr;
    this->mCbNextSleepFunction = iCbFunc;
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

  MDY_SLEEP_FOR_ATOMIC_TIME();
  return sleptFlag;
}

EDySuccess TDyIO::outTryCallSleptCallbackFunction()
{
  if (this->outIsIOThreadSlept() == false)  { return DY_FAILURE; }
  if (this->mCbSleepFunction == nullptr)    { return DY_FAILURE; }

  this->mCbSleepFunction();
  this->mCbSleepFunction = nullptr;
  if (this->mCbNextSleepFunction != nullptr)
  {
    this->mCbSleepFunction = this->mCbNextSleepFunction;
    this->mCbNextSleepFunction = nullptr;
  }
  return DY_SUCCESS;
}

void TDyIO::outInsertGcCandidate(_MIN_ const DDyIOReferenceInstance& iRefRI)
{
  this->mGarbageCollector.InsertGcCandidate(iRefRI);
}

void TDyIO::outTryForwardCandidateRIToGCList(_MIN_ EDyScope iScope, _MIN_ EDyResourceStyle iStyle)
{
  switch (iStyle)
  {
  case EDyResourceStyle::Information: 
  { // Get GC-Candidate RI instance from list (condition is `mIsResourceValid == true` && `mReferenceCount == 0`.
    // and reinsert it to gc list.
    const auto gcCandidateList = this->mRIInformationMap.GetForwardCandidateRIAsList(iScope);
    this->mGarbageCollector.InsertGcCandidateList(gcCandidateList);
    this->mGarbageCollector.TryGarbageCollectCandidateList();
  } break;
  case EDyResourceStyle::Resource:    
  { // Get GC-Candidate RI instance from list (condition is `mIsResourceValid == true` && `mReferenceCount == 0`.
    // and reinsert it to gc list.
    const auto gcCandidateList = this->mRIResourceMap.GetForwardCandidateRIAsList(iScope);
    this->mGarbageCollector.InsertGcCandidateList(gcCandidateList);
    this->mGarbageCollector.TryGarbageCollectCandidateList();
  } break;
  default: MDY_UNEXPECTED_BRANCH();
  }
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