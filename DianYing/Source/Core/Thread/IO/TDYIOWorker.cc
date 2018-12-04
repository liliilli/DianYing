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
#include <Dy/Core/Thread/IO/TDyIOWorker.h>

namespace dy
{

void TDyIOWorker::operator()()
{
  this->inWork();
}

void TDyIOWorker::inWork()
{
  static auto CbTaskWaiting = [this]
  {
    return this->mIsAssigned == true || this->mIsShouldStop == true;
  };

  while (true)
  {
    {
      MDY_SYNC_WAIT_CONDITION(this->mTaskMutex, this->mTaskCV, CbTaskWaiting);

      if (this->mIsShouldStop == true) { return; }
      MDY_ASSERT(this->mIsAssigned == true, "IOWorker's task must be assigned to proceed.");
    }

    // DO SOMETHING
    this->PopulateIOResource(this->mAssignedTask);

    { // Change status mutually.
      MDY_SYNC_LOCK_GUARD(this->mMutexAssigned);
      this->mIsAssigned = false;
    }
  }
}

EDySuccess TDyIOWorker::outTryAssign(_MIN_ const DDyIOTask& inputTask)
{
  { // Check assigned flag mutually.
    MDY_SYNC_LOCK_GUARD(this->mMutexAssigned);
    if (this->mIsAssigned == true) { return DY_FAILURE; }
    this->mIsAssigned = true;
  }

  { // And..
    MDY_SYNC_LOCK_GUARD(this->mTaskMutex);
    this->mAssignedTask = inputTask;
    this->mTaskCV.notify_one();
  }
  return DY_SUCCESS;
}

void TDyIOWorker::outTryStop()
{
  MDY_SYNC_LOCK_GUARD(this->mTaskMutex);
  this->mIsShouldStop = true;
}

void TDyIOWorker::PopulateIOResource(_MIN_ const DDyIOTask& assignedTask)
{
  // 여기서는 IO Information & Resource 컨테이너는 반드시 atomic 해야함.
  // RESOURCE 을 만드는데, 각각의 CDy~~ 을 조정해줄 필요가 있을 듯 함.


  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace