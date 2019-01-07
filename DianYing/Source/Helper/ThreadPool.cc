#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/ThreadPool.h>

namespace dy
{

FDyThreadPool::FDyThreadPool(int32_t size)
{
  for (int32_t i = 0; i < size; ++i)
  {
    this->mWorkers.emplace_back(
      [this]()
      {
        while(true)
        {
          std::function<void(void)> designatedTask;
          {
            std::unique_lock<std::mutex> lock(this->mQueueMutex);
            this->mConditionVariable.wait(lock, [this]{ return this->mIsStop || !this->mTasks.empty(); });
            if (this->mIsStop && this->mTasks.empty()) return;

            designatedTask = std::move(this->mTasks.front());
            this->mTasks.pop();
          }

          designatedTask();
        }
      }
    );
  }
}

FDyThreadPool::~FDyThreadPool()
{
  {
    std::unique_lock<std::mutex> lock(this->mQueueMutex);
    this->mIsStop = true;
  }
  this->mConditionVariable.notify_all();
  for (std::thread& worker : this->mWorkers)
  {
    if (worker.joinable()) worker.join();
  }
}

} /// ::dy namespace
