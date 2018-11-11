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
#include <Dy/Management/TimeManager.h>

#include <chrono>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

EDySuccess MDyTime::IsGameFrameTicked() const noexcept
{
  if (this->__mIsEnabledVsync == true)
  {
    if (this->mGameElapsedDtFromLastTick < this->mGameTickFragment) { return DY_FAILURE; }

    this->mGameElapsedDtFromLastTick -= this->mGameTickFragment;
    return DY_SUCCESS;
  }
  else { return DY_SUCCESS; }
}

TI32 MDyTime::GetPresentFpsCountValue() const noexcept
{
  return this->mGameTickedFpsCountOld;
}

TF32 MDyTime::GetGameScaledTickedDeltaTimeValue() const noexcept
{
  const auto dt = this->mGameElapsedDtFromLastTick * this->mGameTimeScale;
  this->mGameElapsedDtFromLastTick = 0.0f;
  return dt;
}

TF32 MDyTime::GetGameScaledElapsedTimeValue() const noexcept
{
  return this->mGameElapsedTimeFromStartUp;
}

TF32 MDyTime::GetGameTimeScaleValue() const noexcept
{
  return this->mGameTimeScale;
}

EDySuccess MDyTime::SetGameTimeScale(_MIN_ const TF32 timeScale) noexcept
{
  if (timeScale <= 0.f)
  {
    this->mGameTimeScale = 0.0001f;
    MDY_LOG_WARNING_D("{} | Time scaling failed because of zero value or negative. Input timeScale : {}", "MDyTime::SetGameTimeScale", timeScale);
    return DY_FAILURE;
  }
  else
  {
    this->mGameTimeScale = timeScale;
    MDY_LOG_INFO_D("{} | MDyTime::mGameTimeScale : {}.", "MDyTime::SetGameTimeScale", this->mGameTimeScale);
    return DY_SUCCESS;
  }
}

void MDyTime::pUpdate() noexcept
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

void MDyTime::pfSetVsync(_MIN_ bool isVsyncEnabled) noexcept
{
  this->__mIsEnabledVsync = isVsyncEnabled;
}

EDySuccess MDyTime::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyTime::pfInitialize().", "FunctionCall");

  this->mGameTickFragment = 1.0f / static_cast<TF32>(this->mGameGoalFps);

  MDY_LOG_INFO_D("MDyTime::mGameGoalFps : {}."      , this->mGameGoalFps);
  MDY_LOG_INFO_D("MDyTime::mGameTickFragment : {}." , this->mGameTickFragment);
  MDY_LOG_INFO_D("MDyTime::mGameTimeScale : {}."    , this->mGameTimeScale);
  return DY_SUCCESS;
}

EDySuccess MDyTime::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyTime::pfRelease().", "FunctionCall");
  return DY_SUCCESS;
}

} /// ::dy namespace