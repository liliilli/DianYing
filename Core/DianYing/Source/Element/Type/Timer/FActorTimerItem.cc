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
#include <Dy/Element/Type/Timer/FActorTimerItem.h>
#include <utility>
#include <Dy/Element/Type/Timer/FTimerHandle.h>
#include <Dy/Component/Abstract/AActorCppScript.h>
#include <Dy/Core/GEngine.h>

namespace dy
{

FActorTimerItem::FActorTimerItem(
    _MIN_ FTimerHandle& iHandle,
    _MIN_ AActorCppScript& iRefScript,
    _MIN_ TF32 iFirstDelay, 
    _MIN_ TF32 iDelayTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ std::function<void()> iCbFunction) :
    mPtrHandle{&iHandle},
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

FActorTimerItem::~FActorTimerItem()
{
  this->MDY_PRIVATE(Abort)();
}

void FActorTimerItem::MDY_PRIVATE(ResetTimerProperties)(
    _MIN_ TF32 iFirstDelay, 
    _MIN_ TF32 iDelayTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ std::function<void()> iCbFunction)
{
  mStatus     = ETimerState::Play;
  mIsLooped   = iIsLooped;
  mFirstDelay = iFirstDelay;
  mDelayTime  = iDelayTime;
  mTimeGoal   = mFirstDelay + mDelayTime;
  mTimeElapsed = 0.0f;
  mCallbackFunction = std::move(iCbFunction);
}

void FActorTimerItem::MDY_PRIVATE(Abort)()
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

void FActorTimerItem::Update(_MIN_ TF32 iDt) noexcept
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

bool FActorTimerItem::Checked() const noexcept
{
  return this->mStatus == ETimerState::Play
      && this->mCallbackFunction != nullptr
      && this->mDeferredCallCount > 0;
}

void FActorTimerItem::CallFunction(_MIN_ bool iCallOnlyOnce) noexcept
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
        if (TEMP_CAST(gEngine)->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
      }
    }
  }
}

void FActorTimerItem::SetTimerStatus(_MIN_ ETimerState iStatus) noexcept
{
  this->mStatus = iStatus;
}

ETimerState FActorTimerItem::GetTimerStatus() const noexcept
{
  return this->mStatus;
}

TU32 FActorTimerItem::GetIndex() const noexcept
{
  return this->mIndex;
}

} /// ::dy namespace