#ifndef GUARD_DY_MANAGEMENT_TIMERMANAGER_H
#define GUARD_DY_MANAGEMENT_TIMERMANAGER_H
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
#include <Dy/Element/Type/Timer/FDyTimerItem.h>
#include <Dy/Helper/System/TypeTraits.h>

//!
//! Forward declaration
//!

namespace dy
{
class ADyActorCppScript;
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
        IsInheritancedFrom<TType, ADyActorCppScript> == true, 
        "TType is not inheritenced from ADyActorCppScript.");

    auto callback = std::bind(iPtrCallback, &iRefScript);
    this->MDY_PRIVATE_SPECIFIER(pSetTimer)(iRefHandler, static_cast<ADyActorCppScript&>(iRefScript), callback, iTickTime, iIsLooped, iDelayTime);
  }

  /// @brief Pause timer, this function do nothing when Handler is not bound or paused already.
  void PauseTimer(_MIN_ FDyTimerHandle& iRefHandler);

  /// @brief Resume timer, this function do nothing when Handler is not bound or already played.
  void ResumeTimer(_MIN_ FDyTimerHandle& iRefHandler);

  /// @brief Stop timer.
  void StopTimer(_MIN_ FDyTimerHandle& iRefHandler);

  /// @brief Remove `::Aborted` timer instance list.
  void MDY_PRIVATE_SPECIFIER(TryGcRemoveAbortedTimerInstance)();

private:
  void MDY_PRIVATE_SPECIFIER(pSetTimer)(
      _MIN_ FDyTimerHandle& iRefHandler, 
      _MIN_ ADyActorCppScript& iRefScript,
      _MIN_ std::function<void(void)> iFunction,
      _MIN_ TF32 iTickTime, 
      _MIN_ bool iIsLooped, 
      _MIN_ TF32 iDelayTime);

  std::vector<FDyTimerItem> mTimerList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TIMERMANAGER_H