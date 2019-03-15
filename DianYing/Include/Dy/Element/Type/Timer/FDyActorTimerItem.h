#ifndef GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERITEM_H
#define GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERITEM_H
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

#include <Dy/Element/Type/Timer/EDyTimerStatus.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyTimerHandle;
class AActorCppScript;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FDyActorTimerItem
/// @brief Internal timer item.
class FDyActorTimerItem final
{
public:
  /// Ctor.
  FDyActorTimerItem(
      _MIN_ FDyTimerHandle& iHandle,
      _MIN_ AActorCppScript& iRefScript,
      _MIN_ TF32 iFirstDelay, 
      _MIN_ TF32 iDelayTime, 
      _MIN_ bool iIsLooped, 
      _MIN_ std::function<void()> iCbFunction);
  ~FDyActorTimerItem();

  /// @brief Update timer item when it is `ETimerStatus::Play`.
  void Update(_MIN_ TF32 iDt) noexcept;

  /// @brief Check this item is deserved to call function.
  MDY_NODISCARD bool Checked() const noexcept;

  /// @brief Call function with `mDeferredCallCount` times.
  /// If `mCallOnlyOnce` is true, just call once and reset call count.
  void CallFunction(_MIN_ bool iCallOnlyOnce = false) noexcept;

  /// @brief Set status.
  void SetTimerStatus(_MIN_ EDyTimerStatus iStatus) noexcept;

  /// @brief Get status of this time item.
  MDY_NODISCARD EDyTimerStatus GetTimerStatus() const noexcept;

  /// @brief Get index.
  MDY_NODISCARD TU32 GetIndex() const noexcept;

  /// @brief Reset timer, retaining index and binding status. but change `mStatus` to `Play`.
  void MDY_PRIVATE(ResetTimerProperties)(
      _MIN_ TF32 iFirstDelay, 
      _MIN_ TF32 iDelayTime, 
      _MIN_ bool iIsLooped, 
      _MIN_ std::function<void()> iCbFunction);

  /// @brief Abort timer item.
  void MDY_PRIVATE(Abort)();

private:
  inline static TU32 MDY_PRIVATE(sIndex) = 0;

  FDyTimerHandle*     mPtrHandle = nullptr;
  AActorCppScript*  mPtrScript = nullptr;
  TU32    mIndex      = 0;
  bool    mIsLooped   = false;
  TF32    mFirstDelay = 0.0f;
  TF32    mDelayTime  = 0.0f;
  EDyTimerStatus mStatus = EDyTimerStatus::Aborted;

  TF32    mTimeGoal   = 0.0f;
  TU32    mDeferredCallCount  = 0;
  TF32    mTimeElapsed        = 0.0f;
  std::function<void(void)> mCallbackFunction = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERITEM_H