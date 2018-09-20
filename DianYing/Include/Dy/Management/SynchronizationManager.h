#ifndef GUARD_DY_SYNCRONIZATION_MANAGER_H
#define GUARD_DY_SYNCRONIZATION_MANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Helper/ThreadPool.h>
#include <Dy/Helper/Pointer.h>

namespace dy
{

class MDySync final : public ISingleton<MDySync>
{
  MDY_SINGLETON_PROPERTIES(MDySync);
  MDY_SINGLETON_DERIVED(MDySync);
public:

  ///
  /// @brief Enqueue valid function pointer or lambda, and member function pointer with
  /// any the number of arbitary parameters.
  ///
  template <typename TCallable, typename... TArgs>
  auto EnqueueIO(TCallable&& callable, TArgs&&... args) -> std::future<std::invoke_result_t<TCallable, TArgs...>>
  {
    using TFunctionReturnType = std::invoke_result_t<TCallable, TArgs...>;
    auto assignedTask = std::make_shared<std::packaged_task<TFunctionReturnType()>>(
        std::bind(std::forward<TCallable>(callable), std::forward<TArgs>(args)...)
    );

    std::future<TFunctionReturnType> taskResult = assignedTask->get_future();
    {
      std::unique_lock<std::mutex> lock(this->mIoQueueMutex);
      if (this->mIsAllStop.load()) { throw std::runtime_error("Enqueue on stopped ThreadPool."); }

      this->mIoQueue.emplace([assignedTask]() { (*assignedTask)(); });
    }

    this->mIoCv.notify_one();
    return taskResult;
  }

private:
  ///
  /// @brief
  ///
  void pExecuteIoThread();

  using TTaskQueue = std::queue<std::function<void(void)>>;

  Owner<FDyThreadPool*>   mThreadPool = nullptr;
#if defined(MDY_FLAG_IN_EDITOR)
  std::thread             mEditorThread;
  std::condition_variable mEditorUpdateCv;
#endif

  std::thread             mIoThread;
  std::mutex              mIoQueueMutex;
  std::condition_variable mIoCv;
  TTaskQueue              mIoQueue;

  std::atomic<bool> mIsAllStop = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_SYNCRONIZATION_MANAGER_H