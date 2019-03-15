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
#include <Dy/Management/MTime.h>

#include <chrono>
#include <Dy/Management/MLog.h>

namespace dy
{

EDySuccess MTime::IsGameFrameTicked() const noexcept
{
  if (this->__mIsEnabledVsync == true
  &&  this->mGameElapsedDtFromLastTick < this->mGameTickFragment) { return DY_FAILURE; }

  this->mGameElapsedDtThisFrame = this->mGameElapsedDtFromLastTick;
  this->mGameElapsedDtFromLastTick = 0;
  this->mGameTickedFpsCount += 1;
  return DY_SUCCESS;
}

TI32 MTime::GetPresentFpsCountValue() const noexcept
{
  return this->mGameTickedFpsCountOld;
}

TF32 MTime::GetGameScaledTickedDeltaTimeValue() const noexcept
{
  const auto dt = this->mGameElapsedDtThisFrame * this->mGameTimeScale;
  return dt;
}

TF32 MTime::GetGameScaledElapsedTimeValue() const noexcept
{
  return this->mGameElapsedTimeFromStartUp;
}

TF32 MTime::GetGameTimeScaleValue() const noexcept
{
  return this->mGameTimeScale;
}

TF32 MTime::GetSteadyDeltaTimeValue() const noexcept
{
  return this->mSteadyDeltaTime;
}

TF32 MTime::GetSteadyElapsedTimeValue() const noexcept
{
  return this->mSteadyElapsedTimeFromStartup;
}

EDySuccess MTime::SetGameTimeScale(_MIN_ const TF32 timeScale) noexcept
{
  if (timeScale <= 0.f)
  {
    this->mGameTimeScale = 0.0001f;
    DyPushLogDebugWarning("{} | Time scaling failed because of zero value or negative. Input timeScale : {}", "MTime::SetGameTimeScale", timeScale);
    return DY_FAILURE;
  }
  else
  {
    this->mGameTimeScale = timeScale;
    DyPushLogDebugInfo("{} | MTime::mGameTimeScale : {}.", "MTime::SetGameTimeScale", this->mGameTimeScale);
    return DY_SUCCESS;
  }
}

MDY_NOTUSED DDyTimepoint MTime::GetCalendarTime() const noexcept
{
  auto cTime = std::time(nullptr);
  return DDyTimepoint(*std::localtime(&cTime));
}

void MTime::pUpdate() noexcept
{
  using TSeconds = std::chrono::duration<float, std::ratio<1>>;

  static std::chrono::steady_clock::time_point steadyOldTimestamp;
  static std::chrono::steady_clock::time_point steadyNewTimestamp;
  static TF32 steadyElapsedFromSecond = 0.f;
  static bool isInitialized = false;

  if (isInitialized == false)
  { // If first time, just update oldTime and newTime with same timestamp.
    steadyOldTimestamp = steadyNewTimestamp = std::chrono::steady_clock::now();
    isInitialized = true;
    return;
  }

  // Update `mSteadyDeltaTime`.
  steadyNewTimestamp            = std::chrono::steady_clock::now();
  const auto steadyTimeFragment = std::chrono::duration_cast<TSeconds>(steadyNewTimestamp - steadyOldTimestamp);
  this->mSteadyDeltaTime        = steadyTimeFragment.count();

  this->mSteadyElapsedTimeFromStartup += this->mSteadyDeltaTime;
  this->mGameElapsedDtFromLastTick    += this->mSteadyDeltaTime;

  // Check steady time elapsed stamp exceeded 1 second, so reset FPS count.
  if (steadyElapsedFromSecond += this->mSteadyDeltaTime; steadyElapsedFromSecond >= 1.0f)
  {
    static TF32 second            = 1.0f;
    steadyElapsedFromSecond       = std::modf(steadyElapsedFromSecond, &second);
    this->mGameTickedFpsCountOld  = this->mGameTickedFpsCount;
    this->mGameTickedFpsCount     = 0;
  }

  // At final, set value to old timestamp for next polling.
  steadyOldTimestamp = steadyNewTimestamp;
}

void MTime::pfSetVsync(_MIN_ bool isVsyncEnabled) noexcept
{
  this->__mIsEnabledVsync = isVsyncEnabled;
}

EDySuccess MTime::pfInitialize()
{
  DyPushLogDebugInfo("{} | MTime::pfInitialize().", "FunctionCall");

  this->mGameTickFragment = 1.0f / static_cast<TF32>(this->mGameGoalFps);

  DyPushLogDebugInfo("MTime::mGameGoalFps : {}."      , this->mGameGoalFps);
  DyPushLogDebugInfo("MTime::mGameTickFragment : {}." , this->mGameTickFragment);
  DyPushLogDebugInfo("MTime::mGameTimeScale : {}."    , this->mGameTimeScale);
  return DY_SUCCESS;
}

EDySuccess MTime::pfRelease()
{
  DyPushLogDebugInfo("{} | MTime::pfRelease().", "FunctionCall");
  return DY_SUCCESS;
}

} /// ::dy namespace