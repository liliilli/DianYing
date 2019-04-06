#ifndef GUARD_DY_HELPER_INTERNAL_INLINE_FTHREADPOOLTMP_INL
#define GUARD_DY_HELPER_INTERNAL_INLINE_FTHREADPOOLTMP_INL
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

#include <memory>

namespace dy
{

template <typename TCallable, typename ... TArgs>
auto FThreadPool::Enqueue(
  TCallable&& callable,
  TArgs&&... args) -> std::future<std::invoke_result_t<TCallable, TArgs...>>
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

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_INLINE_FTHREADPOOLTMP_INL