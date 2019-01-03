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
#include <Dy/Element/Type/Timer/FDyTimerItem.h>
#include <utility>
#include <Dy/Element/Type/Timer/FDyTimerHandle.h>
#include <Dy/Component/Abstract/ADyActorCppScript.h>

namespace dy
{

FDyTimerItem::FDyTimerItem(
    _MIN_ FDyTimerHandle& iHandle,
    _MIN_ ADyActorCppScript& iRefScript,
    _MIN_ TF32 iFirstDelay, 
    _MIN_ TF32 iDelayTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ std::function<void()> iCbFunction) :
    mPtrHandle{&iHandle},
    mPtrScript{&iRefScript},
    mIndex{MDY_PRIVATE_SPECIFIER(sIndex)++},
    mStatus{EStatus::Play},
    mIsLooped{iIsLooped},
    mFirstDelay{iFirstDelay},
    mDelayTime{iDelayTime},
    mTimeGoal{mFirstDelay + mDelayTime},
    mCallbackFunction{std::move(std::move(iCbFunction))}
{ 
  this->mPtrHandle->MDY_PRIVATE_SPECIFIER(SetBinding)(mIndex);
  this->mPtrScript->MDY_PRIVATE_SPECIFIER(BindPtrTimerHandle)(*this->mPtrHandle);
}

void FDyTimerItem::MDY_PRIVATE_SPECIFIER(ResetTimerProperties)(
    _MIN_ TF32 iFirstDelay, 
    _MIN_ TF32 iDelayTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ std::function<void()> iCbFunction)
{
  mStatus   = EStatus::Play;
  mIsLooped = iIsLooped;

}

void FDyTimerItem::__Abort()
{
  this->mCallbackFunction = nullptr;
  this->mStatus = EStatus::Aborted;
  this->mPtrHandle->MDY_PRIVATE_SPECIFIER(Unbind)();
  this->mPtrHandle = nullptr;
  this->mPtrScript = nullptr;
}

void FDyTimerItem::Update(_MIN_ TF32 iDt) noexcept
{
  if (this->mStatus != EStatus::Play) { return; }

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

bool FDyTimerItem::Checked() const noexcept
{
  return this->mStatus == EStatus::Play
      && this->mCallbackFunction != nullptr
      && this->mDeferredCallCount > 0;
}

void FDyTimerItem::CallFunction(_MIN_ bool iCallOnlyOnce) noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mCallbackFunction), "Callback function of timer must be valid.");
  if (this->mStatus != EStatus::Play) { return; }

  if (this->mIsLooped == false)
  {
    this->mCallbackFunction();
    this->mDeferredCallCount = 0;
    this->mStatus = EStatus::Aborted;
    this->mCallbackFunction = nullptr;
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
      while (this->mDeferredCallCount > 0 && this->mStatus == EStatus::Play)
      {
        this->mCallbackFunction();
        this->mDeferredCallCount--;
      }
    }
  }
}

FDyTimerItem::EStatus FDyTimerItem::GetTimerStatus() const noexcept
{
  return this->mStatus;
}

TU32 FDyTimerItem::GetIndex() const noexcept
{
  return this->mIndex;
}

} /// ::dy namespace