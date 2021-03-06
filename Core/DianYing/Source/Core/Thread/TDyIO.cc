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
#include <Dy/Core/Thread/TRescIO.h>

#include <assimp/Importer.hpp>

#include <Dy/Core/Thread/SIOConnectionHelper.h>
#include <Dy/Core/Resource/Resource/FResourceModel.h>
#include <Dy/Core/Resource/Resource/FrameBuffer/FResourceFrameBufferGeneral.h>
#include <Dy/Core/Resource/Resource/FrameBuffer/FResourceFrameBufferPingPong.h>
#include <Dy/Core/Resource/Internal/FMeshVBOIntermediate.h>

#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Meta/Information/MetaInfoModelAnim.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>

#include <Dy/Management/IO/MIORescInfo.h>
#include <Dy/Management/IO/MIOResource.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/MWindow.h>
#include <Dy/Builtin/Constant/GeneralValue.h>

constexpr TU8 kDefaultPriority = 128;

namespace dy
{

TRescIO::TRescIO()
{
  this->MIOMetaManager = &MIOMeta::GetInstance();
  this->mIODataManager = &MIORescInfo::GetInstance();
  this->mIOResourceManager = &MIOResource::GetInstance();
}

TRescIO::~TRescIO()
{
  this->mIOResourceManager = nullptr;
  this->mIODataManager = nullptr;
  this->MIOMetaManager = nullptr;
}

EDySuccess TRescIO::Initialize()
{
  // Initialize IOWorkers with context.
  const auto& windowManager = MWindow::GetInstance();
  const auto& workerWndList = windowManager.GetGLWorkerWindowList();
  MDY_ASSERT_MSG(
    workerWndList.size() == this->mWorkerList.size(), 
    "WndList and I/O Worker list size must be same.");

  // Make worker instance list.
  for (TIndex i = 0, size = this->mWorkerList.size(); i < size; ++i)
  {
    auto& [instance, thread] = this->mWorkerList[i];
    instance = std::make_unique<TRescIOWorker>(MIOMeta::GetInstance());
    thread = std::thread(&TRescIOWorker::operator(), std::ref(*instance), std::ref(*workerWndList[i]));
  }

  return EDySuccess::DY_SUCCESS;
}

void TRescIO::Release()
{ 
  
}

void TRescIO::operator()()
{
  MDY_CALL_ASSERT_SUCCESS(this->Initialize());
  while (true)
  {
    DRescIOTask task;
    { 
      // Wait task in the queue, and try pop task when not empty.
      // Wait for condition variable for task queueing processing routine.
      MDY_SYNC_WAIT_CONDITION(
        this->mMutexTaskQueue, this->mConditionVariable, 
        [this] { return this->mIsThreadStopped == true || this->mIOTaskQueue.empty() == false; }
      );

      // If IO thread must be stopped, reset all task queue.
      if (this->mIsThreadStopped == false)
      {
        task = this->mIOTaskQueue.top();
        this->mIOTaskQueue.pop();
      }
      else
      {
        while (this->mIOTaskQueue.empty() == false) { this->mIOTaskQueue.pop(); }

        for (auto& [workerInstance, workerThread] : this->mWorkerList)
        { 
          // Wait all worker thread are terminated.
          // Wait worker instance to be ended and return `mMutexTask` signal.
          workerInstance->SyncTryStop();
          workerThread.join();
          workerInstance = nullptr;
        }
        break; 
      }
    }

    // Wait any worker is idle, if there is idle worker assign task to worker.  
    // We used spinlock, for avoiding complexity of control semaphore and counter.
    while (true)
    {
      for (auto& [instance, thread] : this->mWorkerList)
      {
        const auto isSucceeded = instance->outTryAssign(task);
        if (isSucceeded == EDySuccess::DY_SUCCESS) 
        { 
          // We use goto statement intentionally.
          goto LABEL_DY_AFTER_INSERT_TASK; 
        } 
      }
    }
LABEL_DY_AFTER_INSERT_TASK:
    MDY_SLEEP_FOR_ATOMIC_TIME();
  }

  this->Release();
}

void TRescIO::SyncInsertResult(const DRescIOWorkerResult& result) noexcept
{
  MDY_SYNC_LOCK_GUARD(this->mResultListMutex);
  this->mWorkerResultList.emplace_back(result);
}

void TRescIO::SyncTryForwardTaskToMainList(const DRescIOTask& forwardedMainTask) noexcept
{
  MDY_SYNC_LOCK_GUARD(this->mMutexMainProcessTask);
  this->mIOProcessMainTaskList.emplace_back(forwardedMainTask);
}

void TRescIO::SyncTryStop()
{
  MDY_ASSERT_MSG(this->outIsIOThreadSlept() == true, "To stop io thread, IO Thread must be slept.");
  {
    MDY_SYNC_LOCK_GUARD(this->mMutexTaskQueue);
    this->mIsThreadStopped = true;
  }
  this->mConditionVariable.notify_one();
  MDY_SLEEP_FOR_ATOMIC_TIME();
}

EDySuccess TRescIO::outTryEnqueueTask(
  const std::string& iSpecifier, 
  EResourceType iResourceType, 
  EResourceStyle iResourceStyle,
  EResourceScope iScope, 
  bool iIsDerivedFromResource)
{
  if (iResourceStyle == EResourceStyle::Information)
  {
    const auto specifier = TryRemovePostfix(iSpecifier, kInstancingPostfix);
    MDY_ASSERT_MSG_FORCE(
      this->outIsMetaInformationExist(specifier, iResourceType) == true, 
      "Meta information must be exist.");
  }

  // Query there is Reference Instance for myself. If found, just return do nothing.
  { 
    std::vector<PRIVerificationItem> itselfRIItem{};
    itselfRIItem.emplace_back(iSpecifier, iResourceType, iResourceStyle, iScope);
    const auto result = this->pCheckAndUpdateReferenceInstance(itselfRIItem);
    if (result.empty() == true || result.begin()->second != ERIState::NotExist) { return EDySuccess::DY_SUCCESS; }
  }

  // Make dependency list.
  DRescIODeferredTask::TConditionList conditionList = {};
  const auto checkList = this->pMakeDependenciesCheckList(iSpecifier, iResourceType, iResourceStyle, iScope);
  if (checkList.empty() == false)
  { 
    // And get not-found list from dependency list.
    const auto notFoundRIList = this->pCheckAndUpdateReferenceInstance(checkList);

    if (notFoundRIList.empty() == false)
    {
      for (const auto& [instance, status] : notFoundRIList)
      { // Require depende resource tasks only if NotValid but RI is exist.
        const auto& [specifier, type, style, scope] = instance;
        if (status == ERIState::NotExist) 
        { 
          outTryEnqueueTask(specifier, type, style, scope, true); 
        }

        conditionList.emplace_back(specifier, type, style);
      }
    }
  }

  // Require itself own resource task.
  MDY_CALL_ASSERT_SUCCESS(
    this->CreateReferenceInstance(iSpecifier, iResourceType, iResourceStyle, iScope)
  );

  // Construct IO Tasks.
  DRescIOTask task;
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
  if (task.mResourceType == EResourceType::Mesh && task.mResourcecStyle == EResourceStyle::Resource) 
  { 
    task.mResourceType = EResourceType::__MeshVBO;
  }

  // Make deferred task and forward deferred task to list (atomic)
  if (conditionList.empty() == false) 
  { 
    this->SyncInsertTaskToDeferredList({task, conditionList}); 
  }
  else
  {   // Just insert task to queue, if anything does not happen.
    { // Critical section.
      MDY_SYNC_LOCK_GUARD(this->mMutexTaskQueue);
      if (this->mIsThreadStopped == true) { return EDySuccess::DY_FAILURE; }
      this->mIOTaskQueue.emplace(task);
    }
    this->mConditionVariable.notify_one();
  }

  return EDySuccess::DY_SUCCESS;
}

EDySuccess TRescIO::CreateReferenceInstance(
  const std::string& specifier, 
  EResourceType type, 
  EResourceStyle style, 
  EResourceScope scope)
{
  switch (style)
  {
  case EResourceStyle::Information: 
    return this->mRIInformationMap.CreateReferenceInstance(specifier, type, style, scope);
  case EResourceStyle::Resource:    
    return this->mRIResourceMap.CreateReferenceInstance(specifier, type, style, scope);
  default: MDY_UNEXPECTED_BRANCH(); throw 0;
  }
}

std::vector<TRescIO::PRIVerificationItem> TRescIO::pMakeDependenciesCheckList(
  const std::string& iSpecifier,
  EResourceType iResourceType, EResourceStyle iResourceStyle, EResourceScope iScope) const
{
  std::vector<PRIVerificationItem> checkList = {};

  if (iResourceType == EResourceType::Material)
  { 
    // If resource type is `Material`, bind all dependencies of `Material` to checkList.
    const auto materialName   = TryRemovePostfix(iSpecifier, kInstancingPostfix);
    const auto& metaMaterial  = this->MIOMetaManager->GetMaterialMetaInformation(materialName);

    // Texture 
    for (const auto& bindingTextureItem : metaMaterial.mTextureNames) 
    {
      if (bindingTextureItem.mTextureSpecifier.empty() == true) { break; }
      checkList.emplace_back(
        bindingTextureItem.mTextureSpecifier, EResourceType::Texture, 
        iResourceStyle, iScope);
    }

    // Shader (If supports instancing, must have postfix "__inst".
    auto shaderName = metaMaterial.mShaderSpecifier;
    if (HasPostfix(iSpecifier, kInstancingPostfix) == true) 
    { 
      shaderName += kInstancingPostfix; 
    }

    checkList.emplace_back(shaderName, EResourceType::GLShader, iResourceStyle, iScope);
  }
  else if (iResourceType == EResourceType::GLFrameBuffer)
  { 
    // If resource type is `FrameBuffer`, bind all dependencies of `FrameBuffer` to checkList.
    const auto& metaInfo = this->MIOMetaManager->GetGlFrameBufferMetaInformation(iSpecifier);

    // Get dependent attachment specifier list and add.
    for (const auto& [specifier, type] : metaInfo.mColorAttachmentList)
    { checkList.emplace_back(specifier, EResourceType::GLAttachment, iResourceStyle, iScope); }

    // If framebuffer also use depth buffer, enqueue it. 
    if (metaInfo.mIsUsingDepthBuffer == true)
    { 
      MDY_ASSERT_MSG(
        metaInfo.mDepthAttachmentSpecifier.empty() == false, 
        "Depth attachment must be specified if use depth buffer.");
      checkList.emplace_back(
        metaInfo.mDepthAttachmentSpecifier, EResourceType::GLAttachment, iResourceStyle, iScope);
    }
  }
  else if (iResourceType == EResourceType::Model)
  { // If resource type is `Model` and if using builtin mesh specifier...
    const auto& metaInfo = this->MIOMetaManager->GetModelMetaInformation(iSpecifier);

    // Get dependent attachment specifier list and add.
    for (const auto& [meshSpecifier, materialSpecifier, isInstanced] : metaInfo.mMeshList)
    { 
      // If this list is need to be instanced...
      std::string requiredMeshSpecifier = meshSpecifier;
      std::string requiredMatSpecifier  = materialSpecifier;
      if (isInstanced == true)
      {
        // And mesh does not be a separated name if info.
        if (iResourceStyle == EResourceStyle::Resource)
        {
          requiredMeshSpecifier += kInstancingPostfix;
        }
        requiredMatSpecifier  += kInstancingPostfix;
      }

      // Mesh with instancing or not...
      // Material with instancing or not...
      checkList.emplace_back(requiredMeshSpecifier, EResourceType::Mesh, iResourceStyle, iScope);
      checkList.emplace_back(requiredMatSpecifier, EResourceType::Material, iResourceStyle, iScope);
    }

    // If this model will use skeleton, add skeleton (information) also.
    // Skeleton resource only has `information`.
    if (metaInfo.mSkeleton.mIsUsingSkeleton == true)
    {
      checkList.emplace_back(
        metaInfo.mSkeleton.mSkeletonSpecifier, 
        EResourceType::Skeleton, EResourceStyle::Information, iScope);
    }
  }
  else if (iResourceType == EResourceType::AnimationScrap)
  { 
    // If resource type is `AnimationScrap`, `Skeleton` must be populated also.
    const auto& metaInfo = this->MIOMetaManager->GetModelAnimScrapMetaInformation(iSpecifier);
    MDY_ASSERT_MSG_FORCE(
      metaInfo.mSkeletonSpeicfier.empty() == false, 
      "Skeleton specifier of animation scrap must not be empty.");

    // `AnimationScrap` could only be populated as `Information`,
    // so dependent Skeleton also be a `Information`.
    checkList.emplace_back(metaInfo.mSkeletonSpeicfier, EResourceType::Skeleton, iResourceStyle, iScope);
  }

  // Resource common dependencies.
  if (iResourceStyle == EResourceStyle::Resource)
  {
    switch (iResourceType)
    {
    case EResourceType::Mesh: 
    {
      auto specifier = TryRemovePostfix(iSpecifier, kInstancingPostfix);
      checkList.emplace_back(specifier, iResourceType, EResourceStyle::Information, iScope);
    } break;
    case EResourceType::Material:
    case EResourceType::Model: 
    case EResourceType::GLShader:
    case EResourceType::Texture:  
    case EResourceType::GLAttachment: 
    case EResourceType::GLFrameBuffer:
    {
      checkList.emplace_back(iSpecifier, iResourceType, EResourceStyle::Information, iScope);
    } break;
    default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(checkList);
    }
  }

  return checkList;
}

EDySuccess TRescIO::InstantPopulateMaterialResource(
  const PDyMaterialInstanceMetaInfo& iDesc,
  TResourceBinder<EResourceType::Material>& refMat, EResourceScope iScope, bool(*callback)())
{
  MDY_ASSERT_MSG(callback == nullptr,
    "Callback material resource population is not supported yet.");
  MDY_ASSERT_MSG(iScope == EResourceScope::Temporal, 
    "Temporary material resource population must be inputted.");

  // If resource type is `Material`, bind all dependencies of `Material` to checkList.
  std::vector<PRIVerificationItem> checkList = {};
  { 
    for (const auto& bindingTextureItem : iDesc.mTextureNames) 
    {
      if (bindingTextureItem.mTextureSpecifier.empty() == true) { break; }
      checkList.emplace_back(
        bindingTextureItem.mTextureSpecifier, EResourceType::Texture, 
        EResourceStyle::Resource, iScope);
    }

    checkList.emplace_back(
      iDesc.mShaderSpecifier, EResourceType::GLShader, 
      EResourceStyle::Resource, iScope);
  }

  // If `checkList` is not empty, check dependencies.
  DRescIODeferredTask::TConditionList conditionList = {};
  if (checkList.empty() == false)
  { // And get not-found list from dependency list.
    const auto notFoundRIList = this->pCheckAndUpdateReferenceInstance(checkList);
    if (notFoundRIList.empty() == false)
    { 
      for (const auto& [instance, status] : notFoundRIList)
      { // Require depende resource tasks only if NotValid but RI is exist.
        const auto& [specifier, type, style, scope] = instance;
        if (status == ERIState::NotExist) 
        {
          MDY_CALL_BUT_NOUSE_RESULT(outTryEnqueueTask(specifier, type, style, scope, true)); 
        }

        conditionList.emplace_back(specifier, type, style);
      }
    }
  }

  // Require itself own resource task.
  MDY_CALL_ASSERT_SUCCESS(this->CreateReferenceInstance(
      iDesc.mSpecifierName, 
      EResourceType::Material, EResourceStyle::Resource, iScope));

  // Construct IO Tasks.
  DRescIOTask task;
  {
    task.mSpecifierName       = iDesc.mSpecifierName;
    task.mResourceType        = EResourceType::Material;
    task.mResourcecStyle      = EResourceStyle::Resource;
    task.mScope               = EResourceScope::Temporal;
    task.mTaskPriority        = 192;
    task.mIsResourceDeferred  = false;
    task.mBoundObjectStyle    = EDyObject::Etc_NotBindedYet;
    task.mPtrBoundObject      = &refMat;
    task.mRawInstanceForUsingLater = iDesc;
  }

  // Make deferred task and forward deferred task to list (atomic)
  if (conditionList.empty() == false) 
  { 
    this->SyncInsertTaskToDeferredList({task, conditionList}); 
    return EDySuccess::DY_SUCCESS;
  }

  // Just insert task to queue, if anything does not happen.
  // Critical section.
  { 
    MDY_SYNC_LOCK_GUARD(this->mMutexTaskQueue);
    if (this->mIsThreadStopped == true) { return EDySuccess::DY_FAILURE; }
    this->mIOTaskQueue.emplace(task);
  }
  this->mConditionVariable.notify_one();

  return EDySuccess::DY_SUCCESS;
}

TRescIO::TDependencyList 
TRescIO::pCheckAndUpdateReferenceInstance(const std::vector<PRIVerificationItem>& dependencies) noexcept
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
      // Thread safety with RI container is okay, RI GC phase is held on Main thread.
    if (this->pIsReferenceInstanceExist(specifier, type, style) == true)
    {
      // Try enlarge scope of RI. RI scope is large following by Global > Level > Temporal.
      // Thread safety with RI container is okay, RI GC phase is held on Main thread.
      this->TryEnlargeResourceScope(scope, specifier, type, style);
      // Check if RI is bound by actual resource.
      if (this->pIsReferenceInstanceBound(specifier, type, style) == false)
      {
        resultNotFoundList.emplace_back(
          PRIVerificationItem{specifier, type, style, scope}, 
          ERIState::NotBoundYet);
      }
      continue;
    }

    resultNotFoundList.emplace_back(
      PRIVerificationItem{specifier, type, style, scope}, 
      ERIState::NotExist);
  }

  return resultNotFoundList;
}

void TRescIO::TryEnlargeResourceScope(
  EResourceScope scope, const std::string& specifier, 
  EResourceType type, EResourceStyle style)
{
  switch (style)
  {
  case EResourceStyle::Information: 
    this->mRIInformationMap.TryEnlargeResourceScope(scope, specifier, type);  
    break;
  case EResourceStyle::Resource:    
    this->mRIResourceMap.TryEnlargeResourceScope(scope, specifier, type);     
    break;
  default: MDY_UNEXPECTED_BRANCH();
  }
}

void TRescIO::SyncInsertTaskToDeferredList(const DRescIODeferredTask& deferredTask)
{
  MDY_SYNC_LOCK_GUARD(this->mMutexDeferredTask);
  this->mIODeferredTaskList.emplace_back(deferredTask);
}

EDySuccess TRescIO::outTryRetrieveReferenceInstanceFromGC(
  const std::string& specifier, EResourceType type, EResourceStyle style)
{
  // Get RI from gc list.
  auto smtReferenceInstance = this->mGarbageCollector.MoveInstanceFromGC(specifier, type, style);
  if (smtReferenceInstance == nullptr) 
  { 
    DyPushLogError("Failed to get instance from IO GC, {}.", specifier);
    return EDySuccess::DY_FAILURE; 
  }

  // Reinsert RI to appropriate position.
  switch (style)
  {
  case EResourceStyle::Information: 
    return this->mRIInformationMap.MoveReferenceInstance(std::move(smtReferenceInstance));
  case EResourceStyle::Resource:    
    return this->mRIResourceMap.MoveReferenceInstance(std::move(smtReferenceInstance));
  default: MDY_UNEXPECTED_BRANCH(); throw;
  }
}

void TRescIO::outForceProcessIOInsertPhase() noexcept
{
  // CRITICAL PERFORMANCE DOWN! MUST BE CALLED IN SYNCHRONIZATION PHASE.
  MDY_SYNC_LOCK_GUARD(this->mResultListMutex);
  for (auto& resultItem : this->mWorkerResultList)
  {
    if (resultItem.mRawResultInstance == nullptr) { continue; }

    if (resultItem.mResourceStyle == EResourceStyle::Information)
    {
      this->mIODataManager->InsertResult(resultItem.mResourceType, resultItem.mRawResultInstance);
      MDY_CALL_ASSERT_SUCCESS(this->mRIInformationMap.TryUpdateValidity(
        resultItem.mResourceType, 
        resultItem.mSpecifierName, 
        true,
        resultItem.mRawResultInstance));
    }
    else
    {
      this->mIOResourceManager->InsertResult(resultItem.mResourceType, resultItem.mRawResultInstance);
      MDY_CALL_ASSERT_SUCCESS(this->mRIResourceMap.TryUpdateValidity(
        resultItem.mResourceType, 
        resultItem.mSpecifierName, 
        true,
        resultItem.mRawResultInstance));
    }

    // If need to insert resource task of deferred list into queue, do this.
    this->pTryUpdateDeferredTaskList(
      resultItem.mSpecifierName, resultItem.mResourceType, resultItem.mResourceStyle);
  }

  this->mWorkerResultList.clear();
}

void TRescIO::pTryUpdateDeferredTaskList(
  const std::string& iSpecifier, EResourceType iType, EResourceStyle iStyle)
{
  std::vector<DRescIOTask> reinsertionTasklist = {};

  // Critical Section
  { 
    MDY_SYNC_LOCK_GUARD(this->mMutexDeferredTask);
    for (auto it = this->mIODeferredTaskList.begin(); it != this->mIODeferredTaskList.end();)
    {
      auto& deferredTask = *it;
      // Try remove condition item. If removed something, try it'is satisfied with reinsertion condition.
      if (deferredTask.TryRemoveDependentItem(iSpecifier, iType, iStyle) == EDySuccess::DY_SUCCESS
      &&  deferredTask.IsSatisfiedReinsertCondition() == true)
      {
        reinsertionTasklist.emplace_back(deferredTask.mTask);
        it = this->mIODeferredTaskList.erase(it);
      }
      else { ++it; }
    }
  }
 
  // Reinserted deferred task will have more high priority.
  for (auto& task : reinsertionTasklist) { task.mTaskPriority = task.mTaskPriority + 1; }

  // Critical Section
  { 
    MDY_SYNC_LOCK_GUARD(this->mMutexTaskQueue);
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

void TRescIO::outMainForceProcessDeferredMainTaskList()
{
  MDY_SYNC_LOCK_GUARD(this->mMutexMainProcessTask);
  for (const auto& task : this->mIOProcessMainTaskList)
  {
    SIOConnectionHelper::InsertWorkerResult(outMainProcessTask(task));
  }
  this->mIOProcessMainTaskList.clear();
}

DRescIOWorkerResult TRescIO::outMainProcessTask(_MIN_ const DRescIOTask& task)
{
  DRescIOWorkerResult result{};
  { // Copy properties to result instance.
    result.mResourceType        = task.mResourceType;
    result.mResourceStyle       = task.mResourcecStyle;
    result.mSpecifierName       = task.mSpecifierName;
    result.mIsHaveDeferredTask  = task.mIsResourceDeferred;
  }

  MDY_ASSERT_MSG(
    task.mResourcecStyle == EResourceStyle::Resource, 
    "Main deferred task must be resource style.");
  const auto& infoManager = MIORescInfo::GetInstance();

  switch (task.mResourceType) 
  {
  case EResourceType::GLShader:
  { 
    // Need to move it as independent function.
    const auto instance = new FResourceShader(
      *infoManager.GetPtrInformation<EResourceType::GLShader>(result.mSpecifierName));

    result.mRawResultInstance = instance;
  } break;
  case EResourceType::Mesh:
  { 
    // Get intermediate instance from task, and make mesh resource.
    Owner<FMeshVBOIntermediate*> ptrrawIntermediateInstance = 
      std::any_cast<FMeshVBOIntermediate*>(task.mRawInstanceForUsingLater);

    const auto instance = new FResourceMesh(*ptrrawIntermediateInstance);
    MDY_DELETE_RAWHEAP_SAFELY(ptrrawIntermediateInstance);

    result.mRawResultInstance = instance;
  } break;
  case EResourceType::GLFrameBuffer:
  { 
    // Only Resource, create fbo with attachment.
    const auto& refInfo =
      *infoManager.GetPtrInformation<EResourceType::GLFrameBuffer>(result.mSpecifierName);
    if (refInfo.IsPingPong() == true)
    {
      const auto instance = new FResourceFrameBufferPingPong(refInfo);
      result.mRawResultInstance = instance;
    }
    else
    {
      const auto instance = new FResourceFrameBufferGeneral(refInfo);
      result.mRawResultInstance = instance;
    }
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  };

  return result;
}

//!
//! Condition
//!

bool TRescIO::pIsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EResourceStyle style)
{
  switch (style)
  {
  case EResourceStyle::Information: return this->mRIInformationMap.IsReferenceInstanceExist(specifier, type);
  case EResourceStyle::Resource:    return this->mRIResourceMap.IsReferenceInstanceExist(specifier, type);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

EDySuccess TRescIO::TryBindBinderToResourceRI
(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder)
{
  return this->mRIResourceMap.TryBindBinderToResourceRI(iSpecifier, iType, iPtrBinder);
}

EDySuccess TRescIO::TryBindBinderToInformationRI
(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder)
{
  return this->mRIInformationMap.TryBindBinderToResourceRI(iSpecifier, iType, iPtrBinder);
}

EDySuccess TRescIO::TryDetachBinderFromResourceRI
(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder)
{
  return this->mRIResourceMap.TryDetachBinderFromResourceRI(iSpecifier, iType, iPtrBinder);
}

EDySuccess TRescIO::TryDetachBinderFromInformationRI
(const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder)
{
  return this->mRIInformationMap.TryDetachBinderFromResourceRI(iSpecifier, iType, iPtrBinder);
}

bool TRescIO::pIsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EResourceStyle style)
{
  switch (style)
  {
  case EResourceStyle::Information: return this->mRIInformationMap.IsReferenceInstanceBound(specifier, type);
  case EResourceStyle::Resource:    return this->mRIResourceMap.IsReferenceInstanceBound(specifier, type);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

bool TRescIO::outIsMetaInformationExist(_MIN_ const std::string& specifier, _MIN_ EResourceType type)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->MIOMetaManager), "MetaInformation manager must not be null.");
  switch (type)
  {
  case EResourceType::Script:         return this->MIOMetaManager->IsScriptMetaInformationExist(specifier);
  case EResourceType::Mesh:           return this->MIOMetaManager->IsMeshMetaInfoExist(specifier);
  case EResourceType::Model:          return this->MIOMetaManager->IsModelMetaInfoExist(specifier);
  case EResourceType::Skeleton:       return this->MIOMetaManager->IsModelSkeletonMetaInfoExist(specifier);
  case EResourceType::AnimationScrap: return this->MIOMetaManager->IsModelAnimScrapMetaInfoExist(specifier);
  case EResourceType::GLShader:       return this->MIOMetaManager->IsGLShaderMetaInfoExist(specifier);
  case EResourceType::Texture:        return this->MIOMetaManager->IsTextureMetaInfoExist(specifier);
  case EResourceType::Material:       return this->MIOMetaManager->IsMaterialMetaInfoExist(specifier);
  case EResourceType::WidgetMeta:     return this->MIOMetaManager->IsWidgetMetaInfoExist(specifier);
  case EResourceType::GLAttachment:   return this->MIOMetaManager->IsAttachmentMetaInfoExist(specifier);
  case EResourceType::GLFrameBuffer:  return this->MIOMetaManager->IsFrameBufferMetaInfoExist(specifier);
  case EResourceType::Sound:          return this->MIOMetaManager->IsSoundMetaInfoExist(specifier);
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(false);
  }
}

void TRescIO::BindSleepCallbackFunction(_MIN_ std::function<void()> iCbFunc)
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

bool TRescIO::outIsIOThreadSlept() noexcept
{
  bool sleptFlag;
  {
    MDY_SYNC_LOCK_GUARD(this->mMutexTaskQueue);
    MDY_SYNC_LOCK_GUARD(this->mMutexDeferredTask);
    MDY_SYNC_LOCK_GUARD(this->mResultListMutex);

    // Check all worker are idle.
    for (const auto& [instance, thread] : this->mWorkerList)
    {
      if (instance == nullptr) { continue; } 
      if (instance->SyncIsIdle() == false) 
      { 
        sleptFlag = false; break; 
      };
    }
    
    // Check queue and list are empty.
    sleptFlag = this->mIOTaskQueue.empty()
        && this->mIODeferredTaskList.empty()
        && this->mWorkerResultList.empty();
  }

  MDY_SLEEP_FOR_ATOMIC_TIME();
  return sleptFlag;
}

EDySuccess TRescIO::outTryCallSleptCallbackFunction()
{
  if (this->outIsIOThreadSlept() == false)  { return EDySuccess::DY_FAILURE; }
  if (this->mCbSleepFunction == nullptr)    { return EDySuccess::DY_FAILURE; }

  this->mCbSleepFunction();
  this->mCbSleepFunction = nullptr;
  if (this->mCbNextSleepFunction != nullptr)
  {
    this->mCbSleepFunction = this->mCbNextSleepFunction;
    this->mCbNextSleepFunction = nullptr;
  }
  return EDySuccess::DY_SUCCESS;
}

void TRescIO::outInsertGcCandidate(std::unique_ptr<DIOReferenceInstance>& iRefRI)
{
  this->mGarbageCollector.InsertGcCandidate(iRefRI);
}

void TRescIO::outTryForwardCandidateRIToGCList(EResourceScope iScope, EResourceStyle iStyle)
{
  switch (iStyle)
  {
  case EResourceStyle::Information: 
  { // Get GC-Candidate RI instance from list (condition is `mIsResourceValid == true` && `mReferenceCount == 0`.
    // and reinsert it to gc list.
    auto gcCandidateList = this->mRIInformationMap.GetForwardCandidateRIAsList(iScope);
    this->mGarbageCollector.InsertGcCandidateList(std::move(gcCandidateList));
  } break;
  case EResourceStyle::Resource:    
  { // Get GC-Candidate RI instance from list (condition is `mIsResourceValid == true` && `mReferenceCount == 0`.
    // and reinsert it to gc list.
    auto gcCandidateList = this->mRIResourceMap.GetForwardCandidateRIAsList(iScope);
    this->mGarbageCollector.InsertGcCandidateList(std::move(gcCandidateList));
  } break;
  default: MDY_UNEXPECTED_BRANCH();
  }

  this->mGarbageCollector.TryGarbageCollectCandidateList();
}

bool TRescIO::IsGcCandidateExist() const noexcept
{
  return this->mGarbageCollector.IsGcCandidateExist();
}

void TRescIO::TryGC()
{
  this->mGarbageCollector.TryGarbageCollectCandidateList();
}

bool TRescIO::isoutIsMainTaskListIsEmpty() const noexcept
{
  return this->mIOProcessMainTaskList.empty();
}

bool TRescIO::SyncIsWorkerResultExist() noexcept
{
  MDY_SYNC_LOCK_GUARD(this->mResultListMutex);
  return this->mWorkerResultList.empty() == false;
}

} /// :: dy namesapace