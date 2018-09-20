#include <precompiled.h>
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

/// Header file
#include <Dy/Management/SynchronizationManager.h>
#include <Dy/Helper/ThreadPool.h>

namespace dy
{

EDySuccess MDySync::pfInitialize()
{
  auto supportThreadNumber = std::thread::hardware_concurrency();
  if (supportThreadNumber < 4) { supportThreadNumber = 4; }

  this->mThreadPool = new FDyThreadPool(supportThreadNumber - 2);
  this->mIoThread   = std::thread(&MDySync::LoopIoJob, std::ref(*this));

#if defined(MDY_FLAG_IN_EDITOR)

#endif
  return DY_SUCCESS;
}

EDySuccess MDySync::pfRelease()
{
  bool stopFlagTrue = false;
  while(!this->mIsAllStop.compare_exchange_strong(stopFlagTrue, true));

  if (this->mThreadPool) { delete this->mThreadPool; }
  return DY_SUCCESS;
}

void MDySync::LoopIoJob()
{
  while (true)
  {
    std::function<void(void)> designatedIoTask;
    {
      std::unique_lock<std::mutex> lock(this->mIoQueueMutex);

      this->mIoCv.wait(lock, [this] { return this->mIsAllStop.load() || !this->mIoQueue.empty(); });
      if (this->mIsAllStop.load() && this->mIoQueue.empty()) return;

      designatedIoTask = std::move(this->mIoQueue.front());
      this->mIoQueue.pop();
    }

    designatedIoTask();
  }
}

} /// ::dy namespace