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
#include <Dy/Element/Type/Timer/FWidgetTimerItem.h>

#include <Dy/Component/Abstract/AWidgetCppScript.h>
#include <Dy/Element/Type/Timer/FTimerHandle.h>
#include <Dy/Core/GDyEngine.h>

namespace dy
{

FWidgetTimerItem::FWidgetTimerItem(
  FTimerHandle& iHandle,
  AWidgetCppScript& iRefScript,
  TF32 iFirstDelay, 
  TF32 iDelayTime, 
  bool iIsLooped, 
  std::function<void()> iCbFunction) 
  : mPtrHandle{&iHandle},
    mPtrScript{&iRefScript},
    mIndex{MDY_PRIVATE(sIndex)++},
    mStatus{ETimerState::Play},
    mIsLooped{iIsLooped},
    mFirstDelay{iFirstDelay},
    mDelayTime{iDelayTime},
    mTimeGoal{mFirstDelay + mDelayTime},
    mCallbackFunction{std::move(iCbFunction)}
{ 
  this->mPtrHandle->MDY_PRIVATE(SetBinding)(mIndex);
  this->mPtrScript->MDY_PRIVATE(BindPtrTimerHandle)(*this->mPtrHandle);
}

FWidgetTimerItem::~FWidgetTimerItem()
{
  this->MDY_PRIVATE(Abort)();
}

void FWidgetTimerItem::MDY_PRIVATE(ResetTimerProperties)(
  TF32 iFirstDelay, 
  TF32 iDelayTime, 
  bool iIsLooped, 
  std::function<void()> iCbFunction)
{
  mStatus     = ETimerState::Play;
  mIsLooped   = iIsLooped;
  mFirstDelay = iFirstDelay;
  mDelayTime  = iDelayTime;
  mTimeGoal   = mFirstDelay + mDelayTime;
  mTimeElapsed = 0.0f;
  mCallbackFunction = std::move(iCbFunction);
}

void FWidgetTimerItem::MDY_PRIVATE(Abort)()
{
  this->mCallbackFunction = nullptr;
  this->mDeferredCallCount = 0;
  this->mStatus = ETimerState::Aborted;
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

void FWidgetTimerItem::Update(TF32 iDt) noexcept
{
  if (this->mStatus != ETimerState::Play) { return; }

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

bool FWidgetTimerItem::Checked() const noexcept
{
  return this->mStatus == ETimerState::Play
      && this->mCallbackFunction != nullptr
      && this->mDeferredCallCount > 0;
}

void FWidgetTimerItem::CallFunction(bool iCallOnlyOnce) noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mCallbackFunction), "Callback function of timer must be valid.");
  if (this->mStatus != ETimerState::Play) { return; }

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
      while (this->mDeferredCallCount > 0 && this->mStatus == ETimerState::Play)
      {
        this->mCallbackFunction();
        this->mDeferredCallCount--;
        
        // Check game end signal is called.
        if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
      }
    }
  }
}

void FWidgetTimerItem::SetTimerStatus(ETimerState iStatus) noexcept
{
  this->mStatus = iStatus;
}

ETimerState FWidgetTimerItem::GetTimerStatus() const noexcept
{
  return this->mStatus;
}

TU32 FWidgetTimerItem::GetIndex() const noexcept
{
  return this->mIndex;
}

} /// ::dy namespace