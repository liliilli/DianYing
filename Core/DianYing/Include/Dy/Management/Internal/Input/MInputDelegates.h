#ifndef GUARD_DY_MANAGEMENT_INTERNAL_INPUT_FDYINPUTDELEGATEMANAGER_H
#define GUARD_DY_MANAGEMENT_INTERNAL_INPUT_FDYINPUTDELEGATEMANAGER_H
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

#include <Dy/Management/Type/KeyAxisBindingInformation.h>
#include <Dy/Management/Type/KeyActionBindingInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class AWidgetCppScript;
class AActorCppScript;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class MInputDelegates final
{
public:
  using TCallbackAction = std::function<void()>;
  using TCallbackAxis   = std::function<void(TF32)>;

  /// @brief Try require controller exlusive right for Actor. \n
  /// If there is any actor which is using controller delegate, Actor delegate will be reset and overwritten by this. \n
  /// And if there is same actor instance reference already, it just do nothing and return EDySuccess::DY_FAILURE.
  /// @TODO IMPLEMENT FOR ADYWIDGETLUASCRIPT
  MDY_NODISCARD EDySuccess TryRequireControllerActor(_MIN_ AActorCppScript& iRefActor) noexcept;

  /// @brief 
  /// @TODO IMPLEMENT FOR ADYACTORLUASCRIPT.
  MDY_NODISCARD EDySuccess TryDetachContollerActor(_MIN_ AActorCppScript& iRefActor) noexcept;

  /// @brief Try require controller exlusive right for UI Widget. \n
  /// If there is any actor which is using controller delegate, Actor delegate will be neglected. \n
  /// And if there is same ui script instance reference already, it just do nothing and return EDySuccess::DY_FAILURE.
  /// @TODO IMPLEMENT FOR ADYWIDGETLUASCRIPT.
  MDY_NODISCARD EDySuccess TryRequireControllerUi(_MIN_ AWidgetCppScript& iRefUiScript) noexcept;
  
  /// @brief 
  /// @TODO IMPLEMENT FOR ADYWIDGETLUASCRIPT.
  MDY_NODISCARD EDySuccess TryDetachContollerUi(_MIN_ AWidgetCppScript& iRefUiScript) noexcept;

  /// @brief Return the pointer address of `PtrUiScript` on binding.
  MDY_NODISCARD AWidgetCppScript* GetPtrUiScriptOnBinding() const noexcept;
  /// @brief Return the pointer address of `mPtrActorScript` on binding.
  MDY_NODISCARD AActorCppScript*  GetPtrActorScriptOnBinding() const noexcept;

  /// @brief
  void BindAxisDelegateUi(_MIN_ const TCallbackAxis& iFunction, _MIN_ DDyAxisBindingInformation& iRefAxis);
  /// @brief
  void BindActionDelegateUi(_MIN_ const TCallbackAction& iFunction, _MIN_ EDyInputActionStatus iStatus, _MIN_ DDyActionBindingInformation& iRefAction);

  /// @brief
  void BindAxisDelegateActor(
      _MIN_ const TCallbackAxis& iFunction, 
      _MIN_ DDyAxisBindingInformation& iRefAxis);
  /// @brief
  void BindActionDelegateActor(
      _MIN_ const TCallbackAction& iFunction, 
      _MIN_ EDyInputActionStatus iStatus, 
      _MIN_ DDyActionBindingInformation& iRefAction);

  /// @brief
  void CheckDelegateAxis(_MIN_ TF32 dt);
  /// @brief
  void CheckDelegateAction(_MIN_ TF32 dt);

private:
  template <typename TType>
  using TRef = std::reference_wrapper<TType>;
  using TAxisFuncBinder   = std::tuple<TRef<DDyAxisBindingInformation>, TCallbackAxis>;
  using TActionFuncBinder = std::tuple<TRef<DDyActionBindingInformation>, EDyInputActionStatus, TCallbackAction>;

  AActorCppScript*              mPtrActorScript       = MDY_INITIALIZE_NULL;
  std::vector<TAxisFuncBinder>    mActorAxisDelegateList    = {};
  std::vector<TActionFuncBinder>  mActorActionDelegateList  = {};

  AWidgetCppScript*             mPtrUiScript          = MDY_INITIALIZE_NULL;
  std::vector<TAxisFuncBinder>    mUiAxisDelegateList       = {};
  std::vector<TActionFuncBinder>  mUiActionDelegateList     = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_INPUT_FDYINPUTDELEGATEMANAGER_H