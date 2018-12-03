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
class TDyIO final
{
public:
  TDyIO();
  ~TDyIO();

  /// @brief Entry point of Thread IO.
  void operator()();

private:
  /// @brief Stop IO thread task and terminate thread.
  void pfStop();

  ///
  /// @brief Enqueue valid function pointer or lambda, and member function pointer with any the number of arbitary parameters.
  ///
  template <typename TCallable, typename... TArgs>
  auto Enqueue(TCallable&& callable, TArgs&&... args) -> std::future<std::invoke_result_t<TCallable, TArgs...>>
  {
    using TFunctionReturnType = std::invoke_result_t<TCallable, TArgs...>;
    auto assignedTask = std::make_shared<std::packaged_task<TFunctionReturnType()>>(
        std::bind(std::forward<TCallable>(callable), std::forward<TArgs>(args)...)
    );

    std::future<TFunctionReturnType> taskResult = assignedTask->get_future();
    {
      std::unique_lock<std::mutex> lock(this->mQueueMutex);
      if (this->mIsStop)
      {
        throw std::runtime_error("Enqueue on stopped ThreadPool.");
      }

      this->mTasks.emplace([assignedTask]() { (*assignedTask)(); });
    }

    this->mConditionVariable.notify_one();
    return taskResult;
  }

  std::vector<std::thread>  mWorkers;
  std::queue<std::function<void(void)>> mTasks;
  std::mutex                mQueueMutex;
  std::condition_variable   mConditionVariable;

  bool                      mIsStop             = false;

  MDyMetaInfo*              mMetaInfoManager    = nullptr;
  MDyIOData*                mIODataManager      = nullptr;
  MDyIOResource*            mIOResourceManager  = nullptr;

  friend class SDyIOConnectionHelper;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_TDYIO_H