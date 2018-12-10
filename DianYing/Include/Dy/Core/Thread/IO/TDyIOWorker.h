#ifndef GUARD_DY_CORE_THREAD_IO_TDYIOWORKER_H
#define GUARD_DY_CORE_THREAD_IO_TDYIOWORKER_H
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

#include <Dy/Core/Thread/IO/DDyIOTask.h>
#include <Dy/Core/Thread/IO/DDyIOWorkerResult.h>
#include <Dy/Management/IO/MetaInfoManager.h>

namespace dy
{

///
/// @class TDyIOWorker
/// @brief IO Worker sub-thread class type.
///
class TDyIOWorker final
{
public:
  TDyIOWorker(const MDyMetaInfo& metaManager) : mMetaManager(metaManager) {};
  ~TDyIOWorker()  = default;
  TDyIOWorker(const TDyIOWorker&)             = delete;
  TDyIOWorker(TDyIOWorker&&)                  = delete;
  TDyIOWorker& operator=(const TDyIOWorker&)  = delete;
  TDyIOWorker& operator=(TDyIOWorker&&)       = delete;

  /// @brief Operation of TDYIOWorker.
  void operator()();

  /// @brief Assign Task to TDYIOWorker.
  EDySuccess outTryAssign(_MIN_ const DDyIOTask& DDyIOTask);

  /// @brief Try stop this thread type.
  void outTryStop();

private:
  ///
  /// @brief DO IO Task.
  /// @param assignedTask assigned task to proceed.
  ///
  MDY_NODISCARD DDyIOWorkerResult PopulateIOResource(_MIN_ const DDyIOTask& assignedTask);

  /// @brief Process Information heap instance creation.
  MDY_NODISCARD DDyIOWorkerResult pPopulateIOResourceInformation(_MIN_ const DDyIOTask& assignedTask);

  /// @brief Process Resource heap instance creation.
  MDY_NODISCARD DDyIOWorkerResult pPopulateIOResourceResource(_MIN_ const DDyIOTask& assignedTask);

  /// @brief Routine
  void inWork();

  DDyIOTask                 mAssignedTask;

  std::mutex                mTaskMutex;
  std::condition_variable   mTaskCV;

  std::mutex                mMutexAssigned;
  bool                      mIsAssigned   = false;
  bool                      mIsShouldStop = false;

  const MDyMetaInfo&        mMetaManager;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_IO_TDYIOWORKER_H