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
/// @log
/// 2018-08-21 Create file.
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
  [[nodiscard]] float GetGameDeltaTimeValue() const noexcept;

  ///
  /// @brief Get summed delta time from previous function call.
  /// Format is integer part is seconds, and point part is millisecond region.
  ///
  /// @return Float value delta time value.
  ///
  [[nodiscard]] float GetGameSummedDeltaTimeValue() const noexcept;

  ///
  /// @brief
  ///
  [[nodiscard]] float GetGameTimeScale() const noexcept;

  ///
  /// @brief
  ///
  void SetGameTimeScale(float timeScale) noexcept;

  ///
  /// @brief
  ///
  void pUpdate() noexcept;

private:
  float mSteadyDeltaTime    = 0.f;
  float mSteadyElapsedTime  = 0.f;

  float mTimeScale = 1.0f;
  float mGameElapsedTime    = 0.f;
  mutable float mGameDeltaTime = 0.f;
  mutable float mGameSummedDeltaTime = 0.f;

  mutable float mGameTickElapsedTime = 0.f;
  int32_t       mGameTickedFps    = 0;
  const int32_t mGameGoalFps      = 60;
  mutable float mGameTickFragment = MDY_NOT_INITIALIZED_M1;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TIME_MANAGER_H