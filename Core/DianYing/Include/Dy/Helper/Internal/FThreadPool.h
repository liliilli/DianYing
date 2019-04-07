#ifndef GUARD_DY_HELPER_THREAD_POOL_H
#define GUARD_DY_HELPER_THREAD_POOL_H
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

#include <condition_variable>
#include <future>
#include <functional>
#include <mutex>
#include <queue>
#include <type_traits>

namespace dy
{

/// @class FThreadPool
/// @brief Thread pool receives function (not functor instance) with arbitary parameters.
/// @reference https://github.com/progschj/ThreadPool
class FThreadPool final
{
public:
  /// @brief Make idle thread and wait arbiatary valid function is enqueued in thread pool.
  /// If thread pool is destructed, working thread are signaled with 'join'.
  FThreadPool(int32_t size);

  FThreadPool(const FThreadPool&)             = delete;
  FThreadPool(FThreadPool&&)                  = delete;
  FThreadPool& operator=(const FThreadPool&)  = delete;
  FThreadPool& operator=(FThreadPool&&)       = delete;

  /// @brief Wait all working thread is done.
  ~FThreadPool();

  /// @brief Enqueue valid function pointer or lambda, and member function pointer with
  /// any the number of arbitary parameters.
  template <typename TCallable, typename... TArgs>
  auto Enqueue(TCallable&& callable, TArgs&&... args) -> std::future<std::invoke_result_t<TCallable, TArgs...>>;

private:
  std::vector<std::thread>  mWorkers;
  std::queue<std::function<void(void)>> mTasks;
  std::mutex                mQueueMutex;
  std::condition_variable   mConditionVariable;
  bool                      mIsStop             = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_THREAD_POOL_H
#include <Dy/Helper/Internal/Inline/FThreadPoolTmp.inl>