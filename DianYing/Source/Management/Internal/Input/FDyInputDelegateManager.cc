#include <precompiled.h>
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

/// Header file
#include <Dy/Management/Internal/Input/FDyInputDelegateManager.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

EDySuccess FDyInputDelegateManager::TryRequireControllerActor(_MIN_ ADyActorCppScript& iRefActor) noexcept
{
  if (this->mPtrActorScript == &iRefActor) 
  { 
    MDY_LOG_WARNING("Controller Actor reference on input delegate and requiring is same. Process is neglected.");
    return DY_FAILURE; 
  }

  if (MDY_CHECK_ISNOTNULL(this->mPtrActorScript)) 
  { // If `mPtrActorScript` is not null, automatically remove delegates.
    MDY_CALL_ASSERT_SUCCESS(this->TryDetachContollerActor(*this->mPtrActorScript));
  }

  this->mPtrActorScript = &iRefActor;
  MDY_LOG_DEBUG_D("Controller UI binding succeeded. ptr : {:x}", std::ptrdiff_t(this->mPtrActorScript));
  return DY_SUCCESS;
}

EDySuccess FDyInputDelegateManager::TryDetachContollerActor(_MIN_ ADyActorCppScript& iRefActor) noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrActorScript))
  {
    MDY_LOG_WARNING("Failed to detach controller actor. Controller Actor reference on input delegate is not bound anything.");
    return DY_FAILURE;
  }
  if (this->mPtrActorScript != &iRefActor) 
  { // If `mPtrActorScript` is not null but not matched to inputted reference. 
    MDY_LOG_WARNING("Failed to detach controller actor. Controller UI reference on input delegate is not bound anything.");
    return DY_FAILURE;
  }

  this->mActorActionDelegateList.clear();
  this->mActorAxisDelegateList.clear();
  MDY_LOG_DEBUG_D("Controller Actor detachment succeeded. ptr : {:x}", std::ptrdiff_t(this->mPtrActorScript));
  this->mPtrActorScript = nullptr;
  return DY_SUCCESS;
}

EDySuccess FDyInputDelegateManager::TryRequireControllerUi(_MIN_ ADyWidgetCppScript& iRefUiScript) noexcept
{
  if (this->mPtrUiScript == &iRefUiScript) 
  { 
    MDY_LOG_WARNING("Controller UI reference on input delegate and requiring is same. Process is neglected.");
    return DY_FAILURE; 
  }

  if (MDY_CHECK_ISNOTNULL(this->mPtrUiScript)) 
  { // If `mPtrUiScript` is not null, automatically remove delegates.
    MDY_CALL_ASSERT_SUCCESS(this->TryDetachContollerUi(*this->mPtrUiScript));
  }

  this->mPtrUiScript = &iRefUiScript;
  MDY_LOG_DEBUG_D("Controller UI binding succeeded. ptr : {:x}", std::ptrdiff_t(this->mPtrUiScript));
  return DY_SUCCESS;
}

EDySuccess FDyInputDelegateManager::TryDetachContollerUi(_MIN_ ADyWidgetCppScript& iRefUiScript) noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrUiScript))
  {
    MDY_LOG_WARNING("Failed to detach controller ui. Controller UI reference on input delegate is not bound anything.");
    return DY_FAILURE;
  }
  if (this->mPtrUiScript != &iRefUiScript) 
  { // If `mPtrUiScript` is not null but not matched to inputted reference. 
    MDY_LOG_WARNING("Failed to detach controller ui. Controller UI reference on input delegate is not bound anything.");
    return DY_FAILURE;
  }

  this->mUiActionDelegateList.clear();
  this->mUiAxisDelegateList.clear();
  MDY_LOG_DEBUG_D("Controller UI detachment succeeded. ptr : {:x}", std::ptrdiff_t(this->mPtrUiScript));
  this->mPtrUiScript = nullptr;
  return DY_SUCCESS;
}

void FDyInputDelegateManager::BindAxisDelegateUi(
    _MIN_ const std::function<void(TF32)>& iFunction, 
    _MIN_ DDyAxisBindingInformation& iRefAxis)
{
  this->mUiAxisDelegateList.emplace_back(std::ref(iRefAxis), iFunction);
}

void FDyInputDelegateManager::BindActionDelegateUi(
    _MIN_ const std::function<void()>& iFunction, 
    _MIN_ EDyInputActionStatus iStatus, 
    _MIN_ DDyActionBindingInformation& iRefAction)
{
  this->mUiActionDelegateList.emplace_back(std::ref(iRefAction), iStatus, iFunction);
}

void FDyInputDelegateManager::BindAxisDelegateActor(
    _MIN_ const TCallbackAxis& iFunction,
    _MIN_ DDyAxisBindingInformation& iRefAxis)
{
  this->mActorAxisDelegateList.emplace_back(std::ref(iRefAxis), iFunction);
}

void FDyInputDelegateManager::BindActionDelegateActor(
    _MIN_ const TCallbackAction& iFunction, 
    _MIN_ EDyInputActionStatus iStatus,
    _MIN_ DDyActionBindingInformation& iRefAction)
{
  this->mActorActionDelegateList.emplace_back(std::ref(iRefAction), iStatus, iFunction);
}

void FDyInputDelegateManager::CheckDelegateAxis(_MIN_ TF32 dt)
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

void FDyInputDelegateManager::CheckDelegateAction(_MIN_ TF32 dt)
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

ADyWidgetCppScript* FDyInputDelegateManager::GetPtrUiScriptOnBinding() const noexcept
{
  return this->mPtrUiScript;
}

ADyActorCppScript* FDyInputDelegateManager::GetPtrActorScriptOnBinding() const noexcept
{
  return this->mPtrActorScript;
}

} /// ::dy namespace