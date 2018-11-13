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
#include <Dy/Core/Thread/TDyIO.h>
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>

namespace dy
{

TDyIO::TDyIO()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyMetaInfo::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOData::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOResource::Initialize());

  this->mIODataManager      = &MDyIOData::GetInstance();
  this->mIOResourceManager  = &MDyIOResource::GetInstance();
}

TDyIO::~TDyIO()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOData::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyMetaInfo::Release());

  this->mIOResourceManager  = nullptr;
  this->mIODataManager      = nullptr;
  this->mMetaInfoManager    = nullptr;
}

void TDyIO::operator()()
{
  while (true)
  {
    std::function<void(void)> designatedTask;

    {
      std::unique_lock<std::mutex> lock(this->mQueueMutex);
      this->mConditionVariable.wait(lock, [this] { return this->mIsStop == true || this->mTasks.empty() == false; });
      if (this->mIsStop && this->mTasks.empty()) return;

      designatedTask = std::move(this->mTasks.front());
      this->mTasks.pop();
    }

    designatedTask();
  }
}

} /// :: dy namesapace