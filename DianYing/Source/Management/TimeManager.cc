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
/// @log
/// 2018-08-21 Create file.
///

/// Header file
#include <Dy/Management/TimeManager.h>

#include <chrono>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

EDySuccess MDyTime::IsGameFrameTicked() const noexcept
{
  if (this->__mIsEnabledVsync)
  {
    if (this->mGameTickElapsedTime < this->mGameTickFragment)
      return DY_FAILURE;
    else
    {
      this->mGameTickElapsedTime -= this->mGameTickFragment;
      return DY_SUCCESS;
    }
  }
  else
  {
    return DY_SUCCESS;
  }
}

int32_t MDyTime::GetPresentFpsValue() const noexcept
{
  return this->mGameTickedFps;
}

float MDyTime::GetGameScaledTickedDeltaTimeValue() const noexcept
{
  if (this->__mIsEnabledVsync)
  {
    return this->mGameTickFragment * this->mTimeScale;
  }
  else
  {
    return this->mGameScaledDeltaTime;
  }
}

float MDyTime::GetGameScaledElapsedTimeValue() const noexcept
{
  return this->mGameScaledElapsedTime;
}

float MDyTime::GetGameTimeScaleValue() const noexcept
{
  return this->mTimeScale;
}

EDySuccess MDyTime::SetGameTimeScale(float timeScale) noexcept
{
  if (timeScale <= 0.f)
  {
    this->mTimeScale = 0.0001f;
    MDY_LOG_WARNING_D("{} | Time scaling failed because of zero value or negative. Input timeScale : {}", "MDyTime::SetGameTimeScale", timeScale);
    return DY_FAILURE;
  }
  else
  {
    this->mTimeScale = timeScale;
    MDY_LOG_INFO_D("{} | MDyTime::mTimeScale : {}.", "MDyTime::SetGameTimeScale", this->mTimeScale);
    return DY_SUCCESS;
  }
}

void MDyTime::pUpdate() noexcept
{
  using TSeconds = std::chrono::duration<float, std::ratio<1>>;

  static std::chrono::steady_clock::time_point oldTime;
  static std::chrono::steady_clock::time_point newTime;
  static float steadyElapsedFromSecond = 0.f;
  static bool isNotFirst = false;

  if (!isNotFirst) {
    oldTime = newTime = std::chrono::steady_clock::now();
    isNotFirst = true;
    return;
  }

  newTime = std::chrono::steady_clock::now();
  const auto timeFragment = std::chrono::duration_cast<TSeconds>(newTime - oldTime);

  this->mSteadyDeltaTime       = timeFragment.count();
  this->mSteadyElapsedTime    += this->mSteadyDeltaTime;

  this->mGameTickElapsedTime    += this->mSteadyDeltaTime;
  this->mGameScaledDeltaTime     = this->mSteadyDeltaTime * this->mTimeScale;
  this->mGameScaledElapsedTime  += this->mGameScaledDeltaTime;

  if (steadyElapsedFromSecond += this->mSteadyDeltaTime;
      steadyElapsedFromSecond > 1.0f)
  {
    steadyElapsedFromSecond -= 1.0f;
    this->mGameTickedFps = 0;
  }
  else
  {
    this->mGameTickedFps += 1;
  }

  oldTime = newTime;
}

void MDyTime::pfSetVsync(bool isVsyncEnabled) noexcept
{
  this->__mIsEnabledVsync = isVsyncEnabled;
}

EDySuccess MDyTime::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyTime::pfInitialize().", "FunctionCall");

  this->mGameTickFragment = 1.0f / static_cast<float>(this->mGameGoalFps);
  MDY_LOG_INFO_D("MDyTime::mGameGoalFps : {}."      , this->mGameGoalFps);
  MDY_LOG_INFO_D("MDyTime::mGameTickFragment : {}." , this->mGameTickFragment);
  MDY_LOG_INFO_D("MDyTime::mTimeScale : {}."        , this->mTimeScale);

  return DY_SUCCESS;
}

EDySuccess MDyTime::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyTime::pfRelease().", "FunctionCall");
  return DY_SUCCESS;
}

} /// ::dy namespace