#ifndef GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERITEM_H
#define GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERITEM_H
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

//!
//! Forward declaration
//!

namespace dy
{
class FDyTimerHandle;
class ADyActorCppScript;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FDyTimerItem
/// @brief Internal timer item.
class FDyTimerItem final
{
public:
  enum class EStatus : unsigned char
  {
    Play,   //
    Paused, // 
    Aborted // Status only will be changed to this when Script is moved to GClist.
  };

  /// Ctor.
  FDyTimerItem(
      _MIN_ FDyTimerHandle& iHandle,
      _MIN_ ADyActorCppScript& iRefScript,
      _MIN_ TF32 iFirstDelay, 
      _MIN_ TF32 iDelayTime, 
      _MIN_ bool iIsLooped, 
      _MIN_ std::function<void()> iCbFunction);
  ~FDyTimerItem();

  /// @brief Update timer item when it is `ETimerStatus::Play`.
  void Update(_MIN_ TF32 iDt) noexcept;

  /// @brief Check this item is deserved to call function.
  MDY_NODISCARD bool Checked() const noexcept;

  /// @brief Call function with `mDeferredCallCount` times.
  /// If `mCallOnlyOnce` is true, just call once and reset call count.
  void CallFunction(_MIN_ bool iCallOnlyOnce = false) noexcept;

  /// @brief Set status.
  void SetTimerStatus(_MIN_ EStatus iStatus) noexcept;

  /// @brief Get status of this time item.
  MDY_NODISCARD EStatus GetTimerStatus() const noexcept;

  /// @brief Get index.
  MDY_NODISCARD TU32 GetIndex() const noexcept;

  /// @brief Reset timer, retaining index and binding status. but change `mStatus` to `Play`.
  void MDY_PRIVATE_SPECIFIER(ResetTimerProperties)(
      _MIN_ TF32 iFirstDelay, 
      _MIN_ TF32 iDelayTime, 
      _MIN_ bool iIsLooped, 
      _MIN_ std::function<void()> iCbFunction);

  /// @brief Abort timer item.
  void MDY_PRIVATE_SPECIFIER(Abort)();

private:
  inline static TU32 MDY_PRIVATE_SPECIFIER(sIndex) = 0;

  FDyTimerHandle*     mPtrHandle = nullptr;
  ADyActorCppScript*  mPtrScript = nullptr;
  TU32    mIndex      = 0;
  EStatus mStatus     = EStatus::Aborted;
  bool    mIsLooped   = false;
  TF32    mFirstDelay = 0.0f;
  TF32    mDelayTime  = 0.0f;

  TF32    mTimeGoal   = 0.0f;
  TU32    mDeferredCallCount  = 0;
  TF32    mTimeElapsed        = 0.0f;
  std::function<void(void)> mCallbackFunction = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_TYPE_TIMER_FDYTIMERITEM_H