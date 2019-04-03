#ifndef GUARD_DY_ELEMENT_TYPE_TIMER_FDYWIDGETTIMERITEM_H
#define GUARD_DY_ELEMENT_TYPE_TIMER_FDYWIDGETTIMERITEM_H
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

#include <functional>
#include <Dy/Element/Type/Timer/ETimerState.h>

//!
//! Forward declaration
//!

namespace dy
{
class FTimerHandle;
class AWidgetCppScript;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FWidgetTimerItem
/// @brief Internal widget timer item that used in internal timer management system.
class FWidgetTimerItem final
{
public:
  /// Ctor.
  FWidgetTimerItem(
      FTimerHandle& iHandle,
      AWidgetCppScript& iRefScript,
      TF32 iFirstDelay, 
      TF32 iDelayTime, 
      bool iIsLooped, 
      std::function<void()> iCbFunction);
  ~FWidgetTimerItem();

  /// @brief Update timer item when it is `ETimerStatus::Play`.
  void Update(TF32 iDt) noexcept;

  /// @brief Check this item is deserved to call function.
  MDY_NODISCARD bool Checked() const noexcept;

  /// @brief Call function with `mDeferredCallCount` times.
  /// If `mCallOnlyOnce` is true, just call once and reset call count.
  void CallFunction(bool iCallOnlyOnce = false) noexcept;

  /// @brief Set status.
  void SetTimerStatus(ETimerState iStatus) noexcept;

  /// @brief Get status of this time item.
  MDY_NODISCARD ETimerState GetTimerStatus() const noexcept;

  /// @brief Get index.
  MDY_NODISCARD TU32 GetIndex() const noexcept;

  /// @brief Reset timer, retaining index and binding status. but change `mStatus` to `Play`.
  void MDY_PRIVATE(ResetTimerProperties)(
      TF32 iFirstDelay, 
      TF32 iDelayTime, 
      bool iIsLooped, 
      std::function<void()> iCbFunction);

  /// @brief Abort timer item.
  void MDY_PRIVATE(Abort)();

private:
  inline static TU32 MDY_PRIVATE(sIndex) = 0;

  FTimerHandle*     mPtrHandle = nullptr;
  AWidgetCppScript* mPtrScript = nullptr;
  TU32    mIndex      = 0;
  bool    mIsLooped   = false;
  TF32    mFirstDelay = 0.0f;
  TF32    mDelayTime  = 0.0f;
  ETimerState mStatus = ETimerState::Aborted;

  TF32    mTimeGoal   = 0.0f;
  TU32    mDeferredCallCount  = 0;
  TF32    mTimeElapsed        = 0.0f;
  std::function<void(void)> mCallbackFunction = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_TYPE_TIMER_FDYWIDGETTIMERITEM_H
