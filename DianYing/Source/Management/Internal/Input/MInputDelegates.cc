#include <precompiled.h>
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

/// Header file
#include <Dy/Management/Internal/Input/MInputDelegates.h>
#include <Dy/Management/MLog.h>

namespace dy
{

EDySuccess MInputDelegates::TryRequireControllerActor(_MIN_ AActorCppScript& iRefActor) noexcept
{
  if (this->mPtrActorScript == &iRefActor) 
  { 
    DyPushLogWarning("Controller Actor reference on input delegate and requiring is same. Process is neglected.");
    return DY_FAILURE; 
  }

  if (MDY_CHECK_ISNOTNULL(this->mPtrActorScript)) 
  { // If `mPtrActorScript` is not null, automatically remove delegates.
    MDY_CALL_ASSERT_SUCCESS(this->TryDetachContollerActor(*this->mPtrActorScript));
  }

  this->mPtrActorScript = &iRefActor;
  DyPushLogDebugDebug("Controller UI binding succeeded. ptr : {:x}", std::ptrdiff_t(this->mPtrActorScript));
  return DY_SUCCESS;
}

EDySuccess MInputDelegates::TryDetachContollerActor(_MIN_ AActorCppScript& iRefActor) noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrActorScript))
  {
    DyPushLogWarning("Failed to detach controller actor. Controller Actor reference on input delegate is not bound anything.");
    return DY_FAILURE;
  }
  if (this->mPtrActorScript != &iRefActor) 
  { // If `mPtrActorScript` is not null but not matched to inputted reference. 
    DyPushLogWarning("Failed to detach controller actor. Controller UI reference on input delegate is not bound anything.");
    return DY_FAILURE;
  }

  this->mActorActionDelegateList.clear();
  this->mActorAxisDelegateList.clear();
  DyPushLogDebugDebug("Controller Actor detachment succeeded. ptr : {:x}", std::ptrdiff_t(this->mPtrActorScript));
  this->mPtrActorScript = nullptr;
  return DY_SUCCESS;
}

EDySuccess MInputDelegates::TryRequireControllerUi(_MIN_ AWidgetCppScript& iRefUiScript) noexcept
{
  if (this->mPtrUiScript == &iRefUiScript) 
  { 
    DyPushLogWarning("Controller UI reference on input delegate and requiring is same. Process is neglected.");
    return DY_FAILURE; 
  }

  if (MDY_CHECK_ISNOTNULL(this->mPtrUiScript)) 
  { // If `mPtrUiScript` is not null, automatically remove delegates.
    MDY_CALL_ASSERT_SUCCESS(this->TryDetachContollerUi(*this->mPtrUiScript));
  }

  this->mPtrUiScript = &iRefUiScript;
  DyPushLogDebugDebug("Controller UI binding succeeded. ptr : {:x}", std::ptrdiff_t(this->mPtrUiScript));
  return DY_SUCCESS;
}

EDySuccess MInputDelegates::TryDetachContollerUi(_MIN_ AWidgetCppScript& iRefUiScript) noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrUiScript))
  {
    DyPushLogWarning("Failed to detach controller ui. Controller UI reference on input delegate is not bound anything.");
    return DY_FAILURE;
  }
  if (this->mPtrUiScript != &iRefUiScript) 
  { // If `mPtrUiScript` is not null but not matched to inputted reference. 
    DyPushLogWarning("Failed to detach controller ui. Controller UI reference on input delegate is not bound anything.");
    return DY_FAILURE;
  }

  this->mUiActionDelegateList.clear();
  this->mUiAxisDelegateList.clear();
  DyPushLogDebugDebug("Controller UI detachment succeeded. ptr : {:x}", std::ptrdiff_t(this->mPtrUiScript));
  this->mPtrUiScript = nullptr;
  return DY_SUCCESS;
}

void MInputDelegates::BindAxisDelegateUi(
    _MIN_ const std::function<void(TF32)>& iFunction, 
    _MIN_ DDyAxisBindingInformation& iRefAxis)
{
  this->mUiAxisDelegateList.emplace_back(std::ref(iRefAxis), iFunction);
}

void MInputDelegates::BindActionDelegateUi(
    _MIN_ const std::function<void()>& iFunction, 
    _MIN_ EDyInputActionStatus iStatus, 
    _MIN_ DDyActionBindingInformation& iRefAction)
{
  this->mUiActionDelegateList.emplace_back(std::ref(iRefAction), iStatus, iFunction);
}

void MInputDelegates::BindAxisDelegateActor(
    _MIN_ const TCallbackAxis& iFunction,
    _MIN_ DDyAxisBindingInformation& iRefAxis)
{
  this->mActorAxisDelegateList.emplace_back(std::ref(iRefAxis), iFunction);
}

void MInputDelegates::BindActionDelegateActor(
    _MIN_ const TCallbackAction& iFunction, 
    _MIN_ EDyInputActionStatus iStatus,
    _MIN_ DDyActionBindingInformation& iRefAction)
{
  this->mActorActionDelegateList.emplace_back(std::ref(iRefAction), iStatus, iFunction);
}

void MInputDelegates::CheckDelegateAxis(_MIN_ TF32 dt)
{
  if (MDY_CHECK_ISNOTNULL(this->mPtrUiScript))
  { // If Ui Script have control exclusive right, do UI controller's delegates.
    for (auto& [axisInstance, function] : this->mUiAxisDelegateList)
    {
      function(axisInstance.get().mAxisValue);
      if (MDY_CHECK_ISNULL(this->mPtrUiScript)) { break; }
    }
  }
  else if (MDY_CHECK_ISNOTNULL(this->mPtrActorScript))
  { // If not, and actor is being bound to controller, do Actor's or do nothing.
    for (auto& [axisInstance, function] : this->mActorAxisDelegateList)
    {
      function(axisInstance.get().mAxisValue);
      if (MDY_CHECK_ISNULL(this->mPtrActorScript)) { break; }
    }
  }
}

void MInputDelegates::CheckDelegateAction(_MIN_ TF32 dt)
{
  if (MDY_CHECK_ISNOTNULL(this->mPtrUiScript))
  { // If Ui Script have control exclusive right, do UI controller's delegates.
    for (auto& [action, condition, function] : this->mUiActionDelegateList)
    {
      if (action.get().mKeyStatus == condition) { function(); }
      if (MDY_CHECK_ISNULL(this->mPtrUiScript)) { break; }
    }
  }
  else if (MDY_CHECK_ISNOTNULL(this->mPtrActorScript))
  { // If not, and actor is being bound to controller, do Actor's or do nothing.
    for (auto& [action, condition, function] : this->mActorActionDelegateList)
    {
      if (action.get().mKeyStatus == condition) { function(); }
      if (MDY_CHECK_ISNULL(this->mPtrActorScript)) { break; }
    }
  }
}

AWidgetCppScript* MInputDelegates::GetPtrUiScriptOnBinding() const noexcept
{
  return this->mPtrUiScript;
}

AActorCppScript* MInputDelegates::GetPtrActorScriptOnBinding() const noexcept
{
  return this->mPtrActorScript;
}

} /// ::dy namespace