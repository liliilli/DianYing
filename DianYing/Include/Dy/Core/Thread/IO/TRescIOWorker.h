#ifndef GUARD_DY_CORE_THREAD_IO_TDYIOWORKER_H
#define GUARD_DY_CORE_THREAD_IO_TDYIOWORKER_H
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

#include <atomic>
#include <Dy/Core/Thread/IO/DRescIOTask.h>
#include <Dy/Core/Thread/IO/DRescIOWorkerResult.h>
#include <Dy/Helper/System/Pointer.h>

namespace dy
{
class MIOMeta;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class TRescIOWorker
/// @brief IO Worker sub-thread class type.
///
class TRescIOWorker final
{
public:
  TRescIOWorker(const MIOMeta& metaManager) : mMetaManager(metaManager) {};
  ~TRescIOWorker()  = default;
  TRescIOWorker(const TRescIOWorker&)             = delete;
  TRescIOWorker(TRescIOWorker&&)                  = delete;
  TRescIOWorker& operator=(const TRescIOWorker&)  = delete;
  TRescIOWorker& operator=(TRescIOWorker&&)       = delete;

  /// @brief Operation of TDYIOWorker.
  void operator()(GLFWwindow& ptrWorkerWnd);

  /// @brief Assign Task to TDYIOWorker.
  EDySuccess outTryAssign(_MIN_ const DRescIOTask& DDyIOTask);

  /// @brief Check Worker is idle.
  bool SyncIsIdle() const noexcept;

  /// @brief Try stop this thread type.
  void SyncTryStop();

private:
  /// @brief DO IO Task.
  /// @param assignedTask assigned task to proceed.
  MDY_NODISCARD DRescIOWorkerResult PopulateIOResource(const DRescIOTask& assignedTask);

  /// @brief Process Information heap instance creation.
  MDY_NODISCARD DRescIOWorkerResult pPopulateIOResourceInformation(const DRescIOTask& assignedTask);

  /// @brief Process Resource heap instance creation.
  MDY_NODISCARD DRescIOWorkerResult pPopulateIOResourceResource(const DRescIOTask& assignedTask);

  /// @brief Routine
  void inWork();

  DRescIOTask mAssignedTask;

  mutable std::mutex mTaskMutex;
  mutable std::condition_variable mTaskCV;

  bool mIsAssigned   = false;
  bool mIsShouldStop = false;

  const MIOMeta& mMetaManager;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_IO_TDYIOWORKER_H