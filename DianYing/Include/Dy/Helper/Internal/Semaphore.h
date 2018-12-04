#ifndef GUARD_DY_HELPER_INTENRAL_SEMAPHORE_H
#define GUARD_DY_HELPER_INTENRAL_SEMAPHORE_H
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
/// @credit https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads
///

#include <mutex>
#include <condition_variable>

///
/// @class DySemaphore
/// @brief Countable mutex (but not support priority inversion between threads) \n
/// This must be initialized with consturctor which has `count` parameter.
///
class DySemaphore final
{
public:
  DySemaphore() = default;
  DySemaphore(_MIN_ const TI32 count) : mCount {count} {};

  void Notify()
  {
    std::lock_guard<decltype(this->mMutex)> lock(this->mMutex);
    ++mCount;
    this->mConditionVariable.notify_one();
  }

  void Wait()
  {
    std::unique_lock<decltype(this->mMutex)> lock(this->mMutex);
    while (this->mCount <= 0)
    { // Handle spurious wake-ups.
      this->mConditionVariable.wait(lock);
    }

    --this->mCount;
  }

  MDY_NODISCARD EDySuccess TryWait()
  {
    std::lock_guard<decltype(this->mMutex)> lock(this->mMutex);
    if (this->mCount > 0)
    {
      --this->mCount;
      return DY_SUCCESS;
    }

    return DY_FAILURE;
  }

private:
  std::mutex              mMutex;
  std::condition_variable mConditionVariable;
  TI32                    mCount = 0; // Initialized as locked.
};

#endif /// GUARD_DY_HELPER_INTENRAL_SEMAPHORE_H
