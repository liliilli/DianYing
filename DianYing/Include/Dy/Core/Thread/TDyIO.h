#ifndef GUARD_DY_CORE_THREAD_TDYIO_H
#define GUARD_DY_CORE_THREAD_TDYIO_H
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

#include <future>
#include <queue>
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Core/Thread/IO/DDyIOTask.h>
#include <Dy/Core/Thread/IO/DDyIOReferenceContainer.h>
#include <Dy/Core/Thread/IO/FDyIOGC.h>
#include "Dy/Helper/Internal/Semaphore.h"
#include "IO/TDyIOWorker.h"
#include "Dy/Component/Interface/IDyInitializeHelper.h"

//!
//! Forward declaration
//!

namespace dy
{
class MDyMetaInfo;
class SDyIOConnectionHelper;
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
class TDyIO final : public IDyInitializeHelper<void>
{
public:
  TDyIO();
  ~TDyIO();

  /// @brief Entry point of Thread IO.
  void operator()();

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

  //!
  //! Logic & Render thread side.
  //!

  /// @brief Stop IO thread task and terminate thread.
  void outTryStop();

  /// @brief Enqueue IO Populating task without any binding to dy object.
  MDY_NODISCARD EDySuccess outTryEnqueueTask(
      _MIN_ const std::string& specifier,
      _MIN_ EDyResourceType resourceType, _MIN_ EDyResourceStyle resourceStyle,
      _MIN_ EDyScope scope, _MIN_ bool isDerivedFromResource = false);

  ///
  /// @brief Check specified meta information is exist on meta information.
  /// @param specifier Resource specifier name.
  /// @param type Resource type.
  ///
  MDY_NODISCARD bool outIsMetaInformationExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type);

  ///
  /// @brief Check whether resource's Reference Instance is exist on now as any kind of information.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  /// @param style Resource style mode.
  ///
  MDY_NODISCARD bool outIsReferenceInstanceExist(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style);

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
  void outTryEnlargeResourceScope(
      _MIN_ EDyScope scope,
      _MIN_ const std::string& specifier,
      _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style);

  ///
  /// @brief Try retrieve reference instance from gc.
  /// @param specifier Resource specifier name.
  /// @param type  Resource type.
  /// @param style Resource style mode.
  ///
  MDY_NODISCARD EDySuccess outTryRetrieveReferenceInstanceFromGC(_MIN_ const std::string& specifier, _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style);

  /// @brief Create reference instance.
  MDY_NODISCARD EDySuccess outCreateReferenceInstance(
      _MIN_ const std::string& specifier,
      _MIN_ EDyResourceType type, _MIN_ EDyResourceStyle style,
      _MIN_ EDyScope scope);

  /// @brief Insert deferred task list.
  void outInsertDeferredTaskList(_MIN_ const DDyIOTask& task);

  //!
  //! Common methods (always must use mutex)
  //!

  /// @brief Try update deferred task which can be insered to list insert into queue with more high priority.
  /// This function use mutex, so performance is afraid.
  void inoutTryUpdateDeferredTaskList();

  inline static constexpr TI32 kWorkerThreadCount = 2;
  using TWorkerPair = std::pair<std::unique_ptr<TDyIOWorker>, std::thread>;
  using TWorkerList = std::array<TWorkerPair, kWorkerThreadCount>;
  using TWorkerResultList = std::vector<DDyIOWorkerResult>;
  /// @brief Type for priority task queue.
  using TIOTaskQueue      = std::priority_queue<DDyIOTask, std::vector<DDyIOTask>, FTaskQueueCmpFunctor>;
  using TDeferredTaskList = std::vector<DDyIOTask>;

  DDyIOReferenceContainer   mRIInformationMap = {};
  DDyIOReferenceContainer   mRIResourceMap = {};
  FDyIOGC                   mGarbageCollector = {};

  std::mutex                mQueueMutex;
  std::condition_variable   mConditionVariable;

  DySemaphore               mWorkerSemaphore{kWorkerThreadCount};
  TWorkerList               mWorkerList{};

  std::mutex                mResultListMutex;
  TWorkerResultList         mWorkerResultList{};

  TIOTaskQueue              mIOTaskQueue = {};
  /// @brief always thread-compatible.
  TDeferredTaskList         mIODeferredTaskList = {};
  /// @brief used to be synchronize with I/O-worker thread and I/O-thread.
  std::mutex                mDeferredTaskMutex;
  std::vector<std::string>  mIODeferredTaskSpecifierList = {};

  bool                      mIsThreadStopped    = false;
  MDyMetaInfo*              mMetaInfoManager    = nullptr;
  MDyIOData*                mIODataManager      = nullptr;
  MDyIOResource*            mIOResourceManager  = nullptr;

  friend class SDyIOConnectionHelper;
  friend class SDyIOWorkerConnHelper;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_TDYIO_H