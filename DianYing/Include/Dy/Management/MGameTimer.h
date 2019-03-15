#ifndef GUARD_DY_MANAGEMENT_TIMERMANAGER_H
#define GUARD_DY_MANAGEMENT_TIMERMANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Element/Type/Timer/FDyActorTimerItem.h>
#include <Dy/Element/Type/Timer/FDyWidgetTimerItem.h>
#include <Dy/Helper/System/TypeTraits.h>

//!
//! Forward declaration
//!

namespace dy
{
class AActorCppScript;
class AWidgetCppScript;
class FDyTimerHandle;
} /// ::dy namespace

//!
//! Implementation
//! 

namespace dy
{

class MGameTimer final : public ISingleton<MGameTimer>
{
  MDY_SINGLETON_DERIVED(MGameTimer);
  MDY_SINGLETON_PROPERTIES(MGameTimer);
public:
  /// @Update binding, being played timers.
  void Update(TF32 iDt);

  /// @brief Set timer.
  template <typename TType>
  void SetTimer(
      FDyTimerHandle& iRefHandler, 
      TType& iRefScript, 
      void(TType::*iPtrCallback)(void), 
      TF32 iTickTime, 
      bool iIsLooped = false, 
      TF32 iDelayTime = 0.0f)
  {
    static_assert(
        IsInheritancedFrom<TType, AActorCppScript> == true
    ||  IsInheritancedFrom<TType, AWidgetCppScript> == true,
        "TType is not inheritenced from AActorCppScript or AWidgetCppScript");

    auto callback = std::bind(iPtrCallback, &iRefScript);
    this->MDY_PRIVATE(pSetTimer)(iRefHandler, iRefScript, callback, iTickTime, iIsLooped, iDelayTime);
  }

  /// @brief Pause timer, this function do nothing when Handler is not bound or paused already.
  void PauseActorTimer(FDyTimerHandle& iRefHandler);
  /// @brief Resume timer, this function do nothing when Handler is not bound or already played.
  void ResumeActorTimer(FDyTimerHandle& iRefHandler);
  /// @brief Stop actor timer.
  void StopActorTimer(FDyTimerHandle& iRefHandler);

  /// @brief Pause timer, this function do nothing when Handler is not bound or paused already.
  void PauseWidgetTimer(FDyTimerHandle& iRefHandler);
  /// @brief Resume timer, this function do nothing when Handler is not bound or already played.
  void ResumeWidgetTimer(FDyTimerHandle& iRefHandler);
  /// @brief Stop widget timer.
  void StopWidgetTimer(FDyTimerHandle& iRefHandler);

  void MDY_PRIVATE(TryGcRemoveAbortedTimerInstance)();

private:
  void MDY_PRIVATE(pSetTimer)(
      FDyTimerHandle& iRefHandler, 
      AActorCppScript& iRefScript,
      std::function<void(void)> iFunction,
      TF32 iTickTime, 
      bool iIsLooped, 
      TF32 iDelayTime);

  void MDY_PRIVATE(pSetTimer)(
      FDyTimerHandle& iRefHandler, 
      AWidgetCppScript& iRefScript,
      std::function<void(void)> iFunction,
      TF32 iTickTime, 
      bool iIsLooped, 
      TF32 iDelayTime);

  /// @brief Remove `::Aborted` actor timer instance list.
  void pTryGcRemoveAbortedActorTimerInstance();
  /// @brief Remove `::Aborted` widget timer instance list.
  void pTryGcRemoveAbortedWidgetTimerInstance();

  std::vector<FDyActorTimerItem>  mActorTimerList   = {};
  std::vector<FDyWidgetTimerItem> mWidgetTimerList  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TIMERMANAGER_H