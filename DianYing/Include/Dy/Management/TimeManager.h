#ifndef GUARD_DY_MANAGEMENT_TIME_MANAGER_H
#define GUARD_DY_MANAGEMENT_TIME_MANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>

namespace dy
{

///
/// @class MDyTime
/// @brief Time class manages time fragment.
///
class MDyTime final : public ISingleton<MDyTime>
{
  MDY_SINGLETON_DERIVED(MDyTime);
  MDY_SINGLETON_PROPERTIES(MDyTime);
public:
  ///
  /// @brief Check that frame is ticked.
  /// If frame is turned to the next frame and everything is ok, return DySuccess or DyFailure.
  ///
  [[nodiscard]] EDySuccess IsGameFrameTicked() const noexcept;

  ///
  /// @brief Return present frame per second.
  ///
  [[nodiscard]] int32_t GetPresentFpsValue() const noexcept;

  ///
  /// @brief Get just delta time.
  /// Format is integer part is seconds, and point part is millisecond region.
  ///
  /// @return Float value delta time value.
  ///
  [[nodiscard]] float GetGameScaledTickedDeltaTimeValue() const noexcept;

  ///
  /// @brief Get summed delta time from previous function call.
  /// Format is integer part is seconds, and point part is millisecond region.
  ///
  /// @return Float value delta time value.
  ///
  [[nodiscard]] float GetGameScaledElapsedTimeValue() const noexcept;

  ///
  /// @brief
  ///
  [[nodiscard]] float GetGameTimeScaleValue() const noexcept;

  ///
  /// @brief Get steady (not-scaled) delta time value.
  ///
  [[nodiscard]] FORCEINLINE float GetSteadyDeltaTimeValue() const noexcept
  {
    return this->mSteadyDeltaTime;
  }

  ///
  /// @brief Get steady (not-scaled) elapsed time value.
  ///
  [[nodiscard]] FORCEINLINE float GetSteadyElapsedTimeValue() const noexcept
  {
    return this->mSteadyElapsedTime;
  }

  ///
  /// @brief Set game time scale. This will affect game logic speed, timer, etc related to this
  /// but actual fps will not changed.
  ///
  /// timeScale should not be 0, or changed 0.0001f;
  ///
  [[maybe_unused]] EDySuccess SetGameTimeScale(float timeScale) noexcept;

private:
  ///
  /// @brief Update game time.
  ///
  void pUpdate() noexcept;

  ///
  /// @brief set vsync mode.
  ///
  void pfSetVsync(bool isVsyncEnabled) noexcept;

  float mSteadyDeltaTime              = 0.0f;
  float mSteadyElapsedTime            = 0.0f;

  float mTimeScale                    = 1.0f;
  mutable float mGameScaledDeltaTime    = 0.0f;
  mutable float mGameScaledElapsedTime  = 0.0f;

  mutable float mGameTickElapsedTime  = 0.0f;
  int32_t       mGameTickedFps        = 0;
  const int32_t mGameGoalFps          = 60;
  mutable float mGameTickFragment     = MDY_INITIALIZE_DEFINT;

  MDY_TRANSIENT float __mIsEnabledVsync = false;
  MDY_TRANSIENT float __mGameGoalFps    = 60;
  MDY_TRANSIENT float __mGameTimeScale  = 1.0f;

  friend class MDySetting;
  friend class MDyWindow;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TIME_MANAGER_H