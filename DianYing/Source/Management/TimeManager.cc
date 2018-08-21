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

namespace dy
{

EDySuccess MDyTime::IsGameFrameTicked() const noexcept
{
  if (this->mGameTickElapsedTime < this->mGameTickFragment)
    return DY_FAILURE;
  else
  {
    this->mGameTickElapsedTime -= this->mGameTickFragment;
    return DY_SUCCESS;
  }
}

int32_t MDyTime::GetPresentFpsValue() const noexcept
{
  return this->mGameTickedFps;
}

float MDyTime::GetGameDeltaTimeValue() const noexcept
{
  return this->mGameDeltaTime;
}

float MDyTime::GetGameSummedDeltaTimeValue() const noexcept
{
  const auto summedDeltaTimeValue = this->mGameSummedDeltaTime;
  this->mGameSummedDeltaTime = 0;
  return summedDeltaTimeValue;
}

  float MDyTime::GetGameTimeScale() const noexcept
{
  return this->mTimeScale;
}

  void MDyTime::SetGameTimeScale(float timeScale) noexcept
{
  mTimeScale = timeScale;
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
  const float steadyDelta = timeFragment.count();

  this->mSteadyDeltaTime       = steadyDelta;
  this->mSteadyElapsedTime    += this->mSteadyDeltaTime;

  this->mGameTickElapsedTime  += this->mSteadyDeltaTime;
  this->mGameDeltaTime         = this->mSteadyDeltaTime * this->mTimeScale;
  this->mGameSummedDeltaTime  += this->mGameDeltaTime;
  this->mGameElapsedTime      += this->mGameDeltaTime;

  if (steadyElapsedFromSecond  += this->mGameDeltaTime;
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

EDySuccess MDyTime::pfInitialize()
{
  mGameTickFragment = 1.0f / static_cast<float>(mGameGoalFps);
  return DY_SUCCESS;
}

EDySuccess MDyTime::pfRelease()
{
  return DY_SUCCESS;
}

} /// ::dy namespace