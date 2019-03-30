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
#include <Dy/Core/Thread/IO/DDyIOTask.h>
#include <Dy/Core/Thread/IO/DDyIOReferenceContainer.h>
#include <Dy/Core/Thread/IO/FDyIOGC.h>
#include <Dy/Core/Thread/IO/TDyIOWorker.h>
#include <Dy/Core/Thread/IO/DDyIOTaskDeferred.h>
#include <Dy/Helper/Internal/FSemaphore.h>
#include <Dy/Component/Interface/IInitializeHelper.h>

//!
//! Forward declaration
//!

namespace dy
{
class MIOMeta;
class SDyIOConnectionHelper;
MDY_INTERFACE IBinderBase;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class TDyIO
/// @brief File IO Thread
///
class TDyIO final : public IInitializeHelper<void>
{
public:
  /// @struct PRIVerificationItem
  /// @brief
  struct PRIVerificationItem final
  {
    const std::string   mSpecifier  = MDY_INITIALIZE_EMPTYSTR;
    const EResourceType mType       = EResourceType::NoneError;
    const EDyResourceStyle mStyle   = EDyResourceStyle::NoneError;
    const EResourceScope   mScope   = EResourceScope::UserDefined;

    PRIVerificationItem(
      const std::string& specifier, EResourceType type, EDyResourceStyle style, EResourceScope scope) 
      : mSpecifier{specifier}, mType{type}, mStyle{style}, mScope{scope} {};
  };

  enum class EDyRIStatus : TU8
  {
    NotValid,
    NotBoundYet,
    Valid,
  };
  using TDependencyPair = std::pair<PRIVerificationItem, EDyRIStatus>;
  using TDependencyList = std::vector<TDependencyPair>;

public:
  TDyIO();
  ~TDyIO();

  /// @brief Entry point of Thread IO.
  void operator()();

  /// @brief Bind sleep callback function.
  /// This function can be called if only TDyIO Thread is slept. \n
  /// And callback is called, bound function will be disappeared.
  void BindSleepCallbackFunction(_MIN_ std::function<void(void)> iCbFunc);

  /// @brief Try call callback function when if only Thread I/O is slept.
  /// If not slept or exist callback function, just return DY_FAILURE.
  EDySuccess outTryCallSleptCallbackFunction();

  /// @brief Try Garbage collect of Reference Instance with resource as Scope and Style, which
  /// is only Valid resource but count is 0. \n
  /// This function may causes time consuming, call this carefully.
  void outTryForwardCandidateRIToGCList(_MIN_ EResourceScope iScope, _MIN_ EDyResourceStyle iStyle);

private:
  /// @struct FTaskQueueCmpFunctor
  /// @brief  IO Task queue comparsion function type.
  struct FTaskQueueCmpFunctor final
  {
    bool operator()(const DDyIOTask& lhs, const DDyIOTask& rhs) const noexcept
    {
      return lhs.mTaskPriority < rhs.mTaskPriority;
    };
  };

  /// @brief Initialize TDyIO.
  EDySuccess Initialize() override final;
  /// @brief Release TDyIO.
  void Release() override final;

  //!
  //! IO Worker side
  //!

  /// @brief Insert result instance from IO Worker safely.
  void outInsertResult(_MIN_ const DDyIOWorkerResult& result) noexcept;

  /// @brief Notify semaphore to proceed next queue.
  void outTryNotifyWorkerIsIdle();

  /// @brief Find that if any task which is same to `taskSpecifier` name in deferred task list,
  ///  so reinsert deferred task into Queue with high priority.
  /// This function use mutex. Performance would be afraid.
  bool outTryUpdateDeferredTaskListFromWorker(_MIN_ const std::string& taskSpecifier) noexcept;

  /// @brief
  ///
  void outTryForwardToMainTaskList(_MIN_ const DDyIOTask& task) noexcept;

  //!
  //! Logic & Render thread side.
  //!

  /// @brief Stop IO thread task and terminate thread.
  void outTryStop();

  /// @brief Enqueue IO Populating task without any binding to dy object.
  EDySuccess outTryEnqueueTask(
      const std::string& specifier,
      EResourceType resourceType, EDyResourceStyle resourceStyle,
      EResourceScope scope, bool isDerivedFromResource = false);

  /// @brief
  MDY_NODISCARD std::vector<PRIVerificationItem> pMakeDependenciesCheckList(
      _MIN_ const std::string& iSpecifier,
      _MIN_ EResourceType iResourceType,
      _MIN_ EDyResourceStyle iResourceStyle,
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
  MDY_NODISCARD bool pIsReferenceInstanceBound(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EDyResourceStyle style);

  /// @brief Check specified meta information is exist on meta information.
  /// @param specifier Resource specifier name.
  /// @param type Resource type.
  MDY_NODISCARD bool outIsMetaInformationExist(_MIN_ const std::string& specifier, _MIN_ EResourceType type);

  /// @brief Check whether resource's Reference Instance is exist on now as any kind of information.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  /// @param style Resource style mode.
  MDY_NODISCARD bool pIsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EDyResourceStyle style);

  /// @brief Try bind binder instance to Resource Reference Instance.
  /// If not found RI, just return DY_FAILURE.
  MDY_NODISCARD EDySuccess TryBindBinderToResourceRI
  (const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  /// @brief Try bind binder instance to Information Reference Instance.
  /// If not found RI, just return DY_FAILURE.
  MDY_NODISCARD EDySuccess TryBindBinderToInformationRI
  (const std::string & iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  /// @brief Try detach binder instance from Resource Reference Instance.
  /// If nnot found RI, just return DY_FAILURE.
  MDY_NODISCARD EDySuccess TryDetachBinderFromResourceRI
  (const std::string& iSpecifier, EResourceType iType, IBinderBase& iPtrBinder);

  /// @brief Try detach binder instance from Information Reference Instance.
  /// If nnot found RI, just return DY_FAILURE.
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
  void pTryEnlargeResourceScope(
      _MIN_ EResourceScope scope,
      _MIN_ const std::string& specifier,
      _MIN_ EResourceType type, _MIN_ EDyResourceStyle style);

  ///
  /// @brief Try retrieve reference instance from gc.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  /// @param style Resource style mode.
  ///
  MDY_NODISCARD EDySuccess outTryRetrieveReferenceInstanceFromGC(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EDyResourceStyle style);

  /// @brief Create reference instance.
  MDY_NODISCARD EDySuccess outCreateReferenceInstance(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EDyResourceStyle style, _MIN_ EResourceScope scope);

  /// @brief Force Try process deferred task list which must be processed in main thread, \n
  /// so Insert created resource instance into result instance list for IO GC/IN Phase.
  void outMainForceProcessDeferredMainTaskList();
  MDY_NODISCARD static DDyIOWorkerResult outMainProcessTask(_MIN_ const DDyIOTask& task);

  /// @brief Insert deferred task list.
  void outInsertDeferredTaskList(_MIN_ const DDyIOTaskDeferred& task);

  /// @brief Insert valid const reference of RI into gc list as copied instance of original instance.
  /// Use this funciton carefully.
  void outInsertGcCandidate(std::unique_ptr<DDyIOReferenceInstance>& iRefRI);

  //!
  //! Common methods (always must use mutex)
  //!

  /// @brief Try update deferred task which can be insered to list insert into queue with more high priority.
  /// This function use mutex, so performance is afraid.
  void pTryUpdateDeferredTaskList(_MIN_ const std::string& specifier, _MIN_ EResourceType type, _MIN_ EDyResourceStyle style);

  /// @brief 
  void outForceProcessIOInsertPhase() noexcept;

  //!
  //! Condition Checking methods
  //!

  /// @brief Check IO Result list need to be inserted to each container.
  /// and so, need to be reinsert deferred queue list.
  MDY_NODISCARD bool outCheckIOResultInCondition() noexcept;

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
  using TWorkerPair = std::pair<std::unique_ptr<TDyIOWorker>, std::thread>;
  using TWorkerList = std::array<TWorkerPair, kWorkerThreadCount>;
  using TWorkerResultList = std::vector<DDyIOWorkerResult>;
  /// @brief Type for priority task queue.
  using TIOTaskQueue      = std::priority_queue<DDyIOTask, std::vector<DDyIOTask>, FTaskQueueCmpFunctor>;
  using TDeferredTaskList = std::vector<DDyIOTaskDeferred>;
  using TMainTaskList     = std::vector<DDyIOTask>;

  DDyIOReferenceContainer   mRIInformationMap = {};
  DDyIOReferenceContainer   mRIResourceMap = {};
  FDyIOGC                   mGarbageCollector = {};

  std::mutex                mQueueMutex;
  std::condition_variable   mConditionVariable;
  TIOTaskQueue              mIOTaskQueue = {};

  FSemaphore               mWorkerSemaphore{kWorkerThreadCount};
  TWorkerList               mWorkerList{};
  std::atomic<TI32>         mIdleWorkerCounter{kWorkerThreadCount};

  std::mutex                mResultListMutex;
  TWorkerResultList         mWorkerResultList{};

  /// @brief used to be synchronize with I/O-worker thread and I/O-thread.
  std::mutex                mDeferredTaskMutex;
  TDeferredTaskList         mIODeferredTaskList = {};

  /// @brief used to maintain task list to be processed from main
  /// because sharding context of arbitary resource between threads can not support.
  std::mutex                mProcessTaskFromMainMutex;
  /// @brief used to manage task list from mIOTaskQueue, which to be processed in main thread
  /// prior to IO GC/IN Phase if not empty. This list is atomic between IO Worker and Main thread.
  TMainTaskList             mIOProcessMainTaskList = {};

  bool                      mIsThreadStopped    = false;
  std::function<void(void)> mCbSleepFunction    = nullptr;
  std::function<void(void)> mCbNextSleepFunction= nullptr;

  MIOMeta*              MIOMetaManager    = nullptr;
  MIORescInfo*                mIODataManager      = nullptr;
  MIOResource*            mIOResourceManager  = nullptr;

  friend class SDyIOConnectionHelper;
  friend class SDyIOWorkerConnHelper;
  friend class SDyIOBindingHelper;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_TDYIO_H