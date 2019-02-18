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
#include <Dy/Element/Type/Timer/FDyWidgetTimerItem.h>

#include <Dy/Component/Abstract/ADyWidgetCppScript.h>
#include <Dy/Element/Type/Timer/FDyTimerHandle.h>
#include <Dy/Core/DyEngine.h>

namespace dy
{

FDyWidgetTimerItem::FDyWidgetTimerItem(
    _MIN_ FDyTimerHandle& iHandle,
    _MIN_ ADyWidgetCppScript& iRefScript,
    _MIN_ TF32 iFirstDelay, 
    _MIN_ TF32 iDelayTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ std::function<void()> iCbFunction) :
    mPtrHandle{&iHandle},
    mPtrScript{&iRefScript},
    mIndex{MDY_PRIVATE(sIndex)++},
    mStatus{EDyTimerStatus::Play},
    mIsLooped{iIsLooped},
    mFirstDelay{iFirstDelay},
    mDelayTime{iDelayTime},
    mTimeGoal{mFirstDelay + mDelayTime},
    mCallbackFunction{std::move(iCbFunction)}
{ 
  this->mPtrHandle->MDY_PRIVATE(SetBinding)(mIndex);
  this->mPtrScript->MDY_PRIVATE(BindPtrTimerHandle)(*this->mPtrHandle);
}

FDyWidgetTimerItem::~FDyWidgetTimerItem()
{
  this->MDY_PRIVATE(Abort)();
}

void FDyWidgetTimerItem::MDY_PRIVATE(ResetTimerProperties)(
    _MIN_ TF32 iFirstDelay, 
    _MIN_ TF32 iDelayTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ std::function<void()> iCbFunction)
{
  mStatus     = EDyTimerStatus::Play;
  mIsLooped   = iIsLooped;
  mFirstDelay = iFirstDelay;
  mDelayTime  = iDelayTime;
  mTimeGoal   = mFirstDelay + mDelayTime;
  mTimeElapsed = 0.0f;
  mCallbackFunction = std::move(iCbFunction);
}

void FDyWidgetTimerItem::MDY_PRIVATE(Abort)()
{
  this->mCallbackFunction = nullptr;
  this->mDeferredCallCount = 0;
  this->mStatus = EDyTimerStatus::Aborted;
  if (MDY_CHECK_ISNOTNULL(this->mPtrScript))
  {
    this->mPtrScript->MDY_PRIVATE(DetachPtrTimerHandle)(*this->mPtrHandle);
    this->mPtrScript = nullptr;
  }
  if (MDY_CHECK_ISNOTNULL(this->mPtrHandle))
  {
    this->mPtrHandle->MDY_PRIVATE(Unbind)();
    this->mPtrHandle = nullptr;
  }
}

void FDyWidgetTimerItem::Update(_MIN_ TF32 iDt) noexcept
{
  if (this->mStatus != EDyTimerStatus::Play) { return; }

  this->mTimeElapsed += iDt;
  if (this->mTimeElapsed >= this->mTimeGoal)
  {
    this->mDeferredCallCount += static_cast<TU32>(std::floor(this->mTimeElapsed / this->mTimeGoal));
    this->mTimeElapsed = std::fmod(this->mTimeElapsed, this->mTimeGoal);

    // Check if time goal is not same to delay time, because time goal is delay time + first delay,
    // If true, reset time goal to delay time.
    if (this->mTimeGoal != this->mDelayTime) { this->mTimeGoal = this->mDelayTime; }
  }
}

bool FDyWidgetTimerItem::Checked() const noexcept
{
  return this->mStatus == EDyTimerStatus::Play
      && this->mCallbackFunction != nullptr
      && this->mDeferredCallCount > 0;
}

void FDyWidgetTimerItem::CallFunction(_MIN_ bool iCallOnlyOnce) noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mCallbackFunction), "Callback function of timer must be valid.");
  if (this->mStatus != EDyTimerStatus::Play) { return; }

  if (this->mIsLooped == false)
  {
    this->mCallbackFunction();
    this->MDY_PRIVATE(Abort)();
  }
  else
  {
    if (iCallOnlyOnce == true) 
    { 
      this->mCallbackFunction();
      this->mDeferredCallCount = 0;
    }
    else
    {
      // It's likely to be changed to Abort or Pause due to change status of outside-world.
      // In that case, function call must be stopped.
      while (this->mDeferredCallCount > 0 && this->mStatus == EDyTimerStatus::Play)
      {
        this->mCallbackFunction();
        this->mDeferredCallCount--;
        
        // Check game end signal is called.
        if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
      }
    }
  }
}

void FDyWidgetTimerItem::SetTimerStatus(_MIN_ EDyTimerStatus iStatus) noexcept
{
  this->mStatus = iStatus;
}

EDyTimerStatus FDyWidgetTimerItem::GetTimerStatus() const noexcept
{
  return this->mStatus;
}

TU32 FDyWidgetTimerItem::GetIndex() const noexcept
{
  return this->mIndex;
}

} /// ::dy namespace