#ifndef GUARD_DY_CORE_THREAD_TDYIO_H
#define GUARD_DY_CORE_THREAD_TDYIO_H
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

#include <future>
#include <queue>
#include <Dy/Management/IO/MIORescInfo.h>
#include <Dy/Management/IO/MIOResource.h>
#include <Dy/Core/Thread/IO/DRescIOTask.h>
#include <Dy/Core/Thread/IO/DIOReferenceContainer.h>
#include <Dy/Core/Thread/IO/FRescIOGC.h>
#include <Dy/Core/Thread/IO/TRescIOWorker.h>
#include <Dy/Core/Thread/IO/DRescIODeferredTask.h>
#include <Dy/Helper/Internal/FSemaphore.h>
#include <Dy/Component/Interface/IInitializeHelper.h>

//!
//! Forward declaration
//!

namespace dy
{
class MIOMeta;
class SIOConnectionHelper;
MDY_INTERFACE IBinderBase;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class TRescIO
/// @brief File IO Thread
class TRescIO final : public IInitializeHelper<void>
{
public:
  /// @struct PRIVerificationItem
  /// @brief
  struct PRIVerificationItem final
  {
    const std::string     mSpecifier;
    const EResourceType   mType  = EResourceType::NoneError;
    const EResourceStyle  mStyle = EResourceStyle::NoneError;
    const EResourceScope  mScope = EResourceScope::UserDefined;

    PRIVerificationItem(
      const std::string& specifier, 
      EResourceType type, 
      EResourceStyle style, 
      EResourceScope scope) 
      : mSpecifier{specifier}, 
        mType{type}, 
        mStyle{style}, 
        mScope{scope} {};
  };

  /// @enum ERIState 
  /// @brief Specifies Reference instance state.
  enum class ERIState : TU8
  {
    NotExist,     // Reference Instance is not exist at all.
    NotBoundYet,  // Reference Instance is exist but actual resource is not bound yet.
    Valid,        // Reference Instance is exist and actual resource is valid.
  };
  using TDependencyPair = std::pair<PRIVerificationItem, ERIState>;
  using TDependencyList = std::vector<TDependencyPair>;

public:
  TRescIO();
  ~TRescIO();

  /// @brief Entry point of Thread IO.
  void operator()();

  /// @brief Bind sleep callback function.
  /// This function can be called if only TRescIO Thread is slept. \n
  /// And callback is called, bound function will be disappeared.
  void BindSleepCallbackFunction(_MIN_ std::function<void(void)> iCbFunc);

  /// @brief Try call callback function when if only Thread I/O is slept.
  /// If not slept or exist callback function, just return EDySuccess::DY_FAILURE.
  EDySuccess outTryCallSleptCallbackFunction();

  /// @brief Try Garbage collect of Reference Instance with resource as Scope and Style, which
  /// is only Valid resource but count is 0. \n
  /// This function may causes time consuming, call this carefully.
  void outTryForwardCandidateRIToGCList(EResourceScope iScope, EResourceStyle iStyle);

  /// @brief Check IO GC candidate is exist on GC candidate list.
  MDY_NODISCARD bool IsGcCandidateExist() const noexcept;

  /// @brief Try Garbage collect of IO Reference Instance in GC list.
  void TryGC();

private:
  /// @struct FTaskQueueOrderCmpFunctor
  /// @brief  IO Task queue comparsion function type.
  struct FTaskQueueOrderCmpFunctor final
  {
    bool operator()(const DRescIOTask& lhs, const DRescIOTask& rhs) const noexcept
    {
      return lhs.mTaskPriority < rhs.mTaskPriority;
    };
  };

  /// @brief Initialize TRescIO.
  EDySuccess Initialize() override final;
  /// @brief Release TRescIO.
  void Release() override final;

  //!
  //! IO Worker side
  //!
  
  /// @brief Forward deferred task from IOWorker into Main task list of IO main thread.
  void SyncTryForwardTaskToMainList(const DRescIOTask& forwardedMainTask) noexcept;

  /// @brief Insert task that has dependencies into deferred task list.
  void SyncInsertTaskToDeferredList(const DRescIODeferredTask& deferredTask);

  /// @brief Insert result instance from IO Worker safely.
  void SyncInsertResult(const DRescIOWorkerResult& result) noexcept;

  /// @brief Find that if any task which is same to `taskSpecifier` name in deferred task list,
  ///  so reinsert deferred task into Queue with high priority.
  /// This function use mutex. Performance would be afraid.
  bool outTryUpdateDeferredTaskListFromWorker(_MIN_ const std::string& taskSpecifier) noexcept;

  //!
  //! Logic & Render thread side.
  //!

  /// @brief Stop IO thread task and terminate thread.
  void SyncTryStop();

  /// @brief Enqueue IO Populating task without any binding to dy object.
  EDySuccess outTryEnqueueTask(
      const std::string& specifier,
      EResourceType resourceType, EResourceStyle resourceStyle,
      EResourceScope scope, bool isDerivedFromResource = false);

  /// @brief
  MDY_NODISCARD std::vector<PRIVerificationItem> pMakeDependenciesCheckList(
      _MIN_ const std::string& iSpecifier,
      _MIN_ EResourceType iResourceType,
      _MIN_ EResourceStyle iResourceStyle,
      _MIN_ EResourceScope iScope) const;

  /// @brief Enqueue IO Populating `instant` material task.
  MDY_NODISCARD EDySuccess InstantPopulateMaterialResource(
      _MIN_ const PDyMaterialInstanceMetaInfo& iDesc,
      _MIN_ TResourceBinder<EResourceType::Material>& refMat, 
      _MIN_ EResourceScope scope,
      _MIN_ bool(*callback)());

  /// @brief Check RI is exist, so enlarge scope and update properties etc.
  MDY_NODISCARD TDependencyList
  pCheckAndUpdateReferenceInstance(_MIN_ const std::vector<PRIVerificationItem>& dependencies) noexcept;

  /// @brief Check Reference instance is bounded. (resource is bounded or not).
  /// If there is not specified instance in contianer, UB might be happened.
  MDY_NODISCARD bool pIsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EResourceStyle style);

  /// @brief Check specified meta information is exist on meta information.
  /// @param specifier Resource specifier name.
  /// @param type Resource type.
  MDY_NODISCARD bool outIsMetaInformationExist(_MIN_ const std::string& specifier, _MIN_ EResourceType type);

  /// @brief Check whether resource's Reference Instance is exist on now as any kind of information.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  /// @param style Resource style mode.
  MDY_NODISCARD bool pIsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EResourceStyle style);

  /// @brief Try bind binder instance to Resource Reference Instance.
  /// If not found RI, just return EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess TryBindBinderToResourceRI
  (const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  /// @brief Try bind binder instance to Information Reference Instance.
  /// If not found RI, just return EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess TryBindBinderToInformationRI
  (const std::string & iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  /// @brief Try detach binder instance from Resource Reference Instance.
  /// If nnot found RI, just return EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess TryDetachBinderFromResourceRI
  (const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  /// @brief Try detach binder instance from Information Reference Instance.
  /// If nnot found RI, just return EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess TryDetachBinderFromInformationRI
  (const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  ///
  /// @brief Try update scope of given style's specifier RI of resource type. \n
  /// This function is not checking RI(Reference Instance) is exist or not, so when RI is not exist
  /// may will cause UB (Undefined Behaviour).
  /// \n\n
  /// If found RI's scope is \n
  /// Global < Level \n
  /// Level  < Temporal \n
  /// RI's scope will be changed to big range.
  ///
  void TryEnlargeResourceScope(
      _MIN_ EResourceScope scope,
      _MIN_ const std::string& specifier,
      _MIN_ EResourceType type, _MIN_ EResourceStyle style);

  ///
  /// @brief Try retrieve reference instance from gc.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  /// @param style Resource style mode.
  ///
  MDY_NODISCARD EDySuccess outTryRetrieveReferenceInstanceFromGC(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EResourceStyle style);

  /// @brief Create reference instance.
  MDY_NODISCARD EDySuccess CreateReferenceInstance(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EResourceStyle style, _MIN_ EResourceScope scope);

  /// @brief Force Try process deferred task list which must be processed in main thread, \n
  /// so Insert created resource instance into result instance list for IO GC/IN Phase.
  void outMainForceProcessDeferredMainTaskList();
  MDY_NODISCARD static DRescIOWorkerResult outMainProcessTask(_MIN_ const DRescIOTask& task);

  /// @brief Insert valid const reference of RI into gc list as copied instance of original instance.
  /// Use this funciton carefully.
  void outInsertGcCandidate(std::unique_ptr<DIOReferenceInstance>& iRefRI);

  //!
  //! Common methods (always must use mutex)
  //!

  /// @brief Try update deferred task which can be insered to list insert into queue with more high priority.
  /// This function use mutex, so performance is afraid.
  void pTryUpdateDeferredTaskList(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EResourceStyle style);

  /// @brief 
  void outForceProcessIOInsertPhase() noexcept;

  //!
  //! Condition Checking methods
  //!

  /// @brief Check IO Result list need to be inserted to each container.
  /// and so, need to be reinsert deferred queue list.
  MDY_NODISCARD bool SyncIsWorkerResultExist() noexcept;

  /// @brief Check IO Thread is slept. \n
  /// To be satisfied condition, \n
  /// 1. Task queue & Deferred queue must be empty. \n
  /// 2. Worker must be slept (not worked). \n
  /// 3. Result list must be empty. \n\n
  /// This function do heavy process stopping IO Thread and Worker's process, so do not use frequently.
  MDY_NODISCARD bool outIsIOThreadSlept() noexcept;

  /// @brief Check main task list is empty.
  MDY_NODISCARD bool isoutIsMainTaskListIsEmpty() const noexcept;

  inline static constexpr TI32 kWorkerThreadCount = 2;
  using TWorkerPair = std::pair<std::unique_ptr<TRescIOWorker>, std::thread>;
  using TWorkerList = std::array<TWorkerPair, kWorkerThreadCount>;
  using TWorkerResultList = std::vector<DRescIOWorkerResult>;
  /// @brief Type for priority task queue.
  using TIOTaskQueue      = std::priority_queue<DRescIOTask, std::vector<DRescIOTask>, FTaskQueueOrderCmpFunctor>;
  using TDeferredTaskList = std::vector<DRescIODeferredTask>;
  using TMainTaskList     = std::vector<DRescIOTask>;

  DIOReferenceContainer   mRIInformationMap = {};
  DIOReferenceContainer   mRIResourceMap = {};
  FRescIOGC                   mGarbageCollector = {};

  std::mutex                mMutexTaskQueue;
  std::condition_variable   mConditionVariable;
  TIOTaskQueue              mIOTaskQueue = {};

  TWorkerList               mWorkerList{};

  std::mutex                mResultListMutex;
  TWorkerResultList         mWorkerResultList{};

  /// @brief used to be synchronize with I/O-worker thread and I/O-thread.
  std::mutex                mMutexDeferredTask;
  TDeferredTaskList         mIODeferredTaskList = {};

  /// @brief used to maintain task list to be processed from main
  /// because sharding context of arbitary resource between threads can not support.
  std::mutex                mMutexMainProcessTask;
  /// @brief used to manage task list from mIOTaskQueue, which to be processed in main thread
  /// prior to IO GC/IN Phase if not empty. This list is atomic between IO Worker and Main thread.
  TMainTaskList             mIOProcessMainTaskList = {};

  bool                      mIsThreadStopped    = false;
  std::function<void()>     mCbSleepFunction    = nullptr;
  std::function<void()>     mCbNextSleepFunction= nullptr;

  MIOMeta*              MIOMetaManager    = nullptr;
  MIORescInfo*                mIODataManager      = nullptr;
  MIOResource*            mIOResourceManager  = nullptr;

  friend class SIOConnectionHelper;
  friend class SIOWorkerConnHelper;
  friend class SIOBindingHelper;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_TDYIO_H