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
/// @brief Time class manages time fragment. \n
/// This management type manages ticking of each frame.
///
class MDyTime final : public ISingleton<MDyTime>
{
  MDY_SINGLETON_DERIVED(MDyTime);
  MDY_SINGLETON_PROPERTIES(MDyTime);
public:
  ///
  /// @brief  Check that frame is ticked.
  /// If frame is turned to the next frame and everything is ok, return DySuccess or DyFailure.
  ///
  MDY_NODISCARD EDySuccess IsGameFrameTicked() const noexcept;

  ///
  /// @brief  Return present frame per second.
  ///
  MDY_NODISCARD TI32 GetPresentFpsCountValue() const noexcept;

  ///
  /// @brief  Get just delta time.
  /// Format is integer part is seconds, and point part is millisecond region.
  ///
  /// @return Float value delta time value.
  ///
  MDY_NODISCARD TF32 GetGameScaledTickedDeltaTimeValue() const noexcept;

  ///
  /// @brief  Get summed delta time from previous function call.
  /// Format is integer part is seconds, and point part is millisecond region.
  ///
  /// @return Float value delta time value.
  ///
  MDY_NODISCARD TF32 GetGameScaledElapsedTimeValue() const noexcept;

  ///
  /// @brief  Get time scale value.
  ///
  MDY_NODISCARD TF32 GetGameTimeScaleValue() const noexcept;

  ///
  /// @brief  Get steady (not-scaled) delta time value.
  ///
  MDY_NODISCARD FORCEINLINE TF32 GetSteadyDeltaTimeValue() const noexcept
  {
    return this->mSteadyDeltaTime;
  }

  ///
  /// @brief Get steady (not-scaled) elapsed time value.
  ///
  MDY_NODISCARD FORCEINLINE TF32 GetSteadyElapsedTimeValue() const noexcept
  {
    return this->mSteadyElapsedTimeFromStartup;
  }

  ///
  /// @brief Set game time scale. This will affect game logic speed, timer,
  /// etc related to this but actual fps will not changed.
  /// timeScale should not be 0, or changed 0.0001f;
  ///
  MDY_NOTUSED EDySuccess SetGameTimeScale(_MIN_ const TF32 timeScale) noexcept;

private:
  ///
  /// @brief Update game time fragment sequence.
  ///
  void pUpdate() noexcept;

  ///
  /// @brief set vsync mode.
  ///
  void pfSetVsync(_MIN_ bool isVsyncEnabled) noexcept;

  TF32 mSteadyDeltaTime                 = 0.0f;
  TF32 mSteadyElapsedTimeFromStartup    = 0.0f;

  TF32 mGameTimeScale                   = 1.0f;
  TU32 mGameGoalFps                     = 60u;

  MDY_TRANSIENT TF32 mGameElapsedTimeFromStartUp= 0.0f;
  MDY_TRANSIENT TF32 mGameElapsedDtFromLastTick = 0.0f;
  MDY_TRANSIENT TU32 mGameTickedFpsCountOld     = MDY_INITIALIZE_DEFUINT;
  MDY_TRANSIENT TU32 mGameTickedFpsCount        = MDY_INITIALIZE_DEFUINT;
  MDY_TRANSIENT TF32 mGameTickFragment          = MDY_INITIALIZE_DEFINT;
  MDY_TRANSIENT bool __mIsEnabledVsync          = false;

  friend class MDySetting;
  friend class MDyWindow;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TIME_MANAGER_H