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
class ADyActorCppScript;
class ADyWidgetCppScript;
class FDyTimerHandle;
} /// ::dy namespace

//!
//! Implementation
//! 

namespace dy
{

class MDyGameTimer final : public IDySingleton<MDyGameTimer>
{
  MDY_SINGLETON_DERIVED(MDyGameTimer);
  MDY_SINGLETON_PROPERTIES(MDyGameTimer);
public:
  /// @Update binding, being played timers.
  void Update(_MIN_ TF32 iDt);

  /// @brief Set timer.
  template <typename TType>
  void SetTimer(
      _MIN_ FDyTimerHandle& iRefHandler, 
      _MIN_ TType& iRefScript, 
      void(TType::*iPtrCallback)(void), 
      _MIN_ TF32 iTickTime, 
      _MIN_ bool iIsLooped = false, 
      _MIN_ TF32 iDelayTime = 0.0f)
  {
    static_assert(
        IsInheritancedFrom<TType, ADyActorCppScript> == true
    ||  IsInheritancedFrom<TType, ADyWidgetCppScript> == true,
        "TType is not inheritenced from ADyActorCppScript or ADyWidgetCppScript");

    auto callback = std::bind(iPtrCallback, &iRefScript);
    this->MDY_PRIVATE(pSetTimer)(iRefHandler, iRefScript, callback, iTickTime, iIsLooped, iDelayTime);
  }

  /// @brief Pause timer, this function do nothing when Handler is not bound or paused already.
  void PauseActorTimer(_MIN_ FDyTimerHandle& iRefHandler);
  /// @brief Resume timer, this function do nothing when Handler is not bound or already played.
  void ResumeActorTimer(_MIN_ FDyTimerHandle& iRefHandler);
  /// @brief Stop actor timer.
  void StopActorTimer(_MIN_ FDyTimerHandle& iRefHandler);

  /// @brief Pause timer, this function do nothing when Handler is not bound or paused already.
  void PauseWidgetTimer(_MIN_ FDyTimerHandle& iRefHandler);
  /// @brief Resume timer, this function do nothing when Handler is not bound or already played.
  void ResumeWidgetTimer(_MIN_ FDyTimerHandle& iRefHandler);
  /// @brief Stop widget timer.
  void StopWidgetTimer(_MIN_ FDyTimerHandle& iRefHandler);

  void MDY_PRIVATE(TryGcRemoveAbortedTimerInstance)();

private:
  void MDY_PRIVATE(pSetTimer)(
      _MIN_ FDyTimerHandle& iRefHandler, 
      _MIN_ ADyActorCppScript& iRefScript,
      _MIN_ std::function<void(void)> iFunction,
      _MIN_ TF32 iTickTime, 
      _MIN_ bool iIsLooped, 
      _MIN_ TF32 iDelayTime);

  void MDY_PRIVATE(pSetTimer)(
      _MIN_ FDyTimerHandle& iRefHandler, 
      _MIN_ ADyWidgetCppScript& iRefScript,
      _MIN_ std::function<void(void)> iFunction,
      _MIN_ TF32 iTickTime, 
      _MIN_ bool iIsLooped, 
      _MIN_ TF32 iDelayTime);

  /// @brief Remove `::Aborted` actor timer instance list.
  void pTryGcRemoveAbortedActorTimerInstance();
  /// @brief Remove `::Aborted` widget timer instance list.
  void pTryGcRemoveAbortedWidgetTimerInstance();

  std::vector<FDyActorTimerItem>  mActorTimerList   = {};
  std::vector<FDyWidgetTimerItem> mWidgetTimerList  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TIMERMANAGER_H