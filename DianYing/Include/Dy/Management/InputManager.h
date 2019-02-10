#ifndef GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
#define GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
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

#include <string>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Meta/Type/Input/EDyInputButton.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/KeyAxisBindingInformation.h>
#include <Dy/Management/Type/KeyActionBindingInformation.h>
#include <Dy/Management/Type/Input/EDyInputButtonStatus.h>
#include <Dy/Management/Internal/Input/FDyInputDelegateManager.h>
#include <Dy/Helper/Type/Clamp.h>

namespace dy
{

///
/// @class MDyInput
/// @brief Manages input polling, mouse movement and joystick input signaling.
///
class MDyInput final : public IDySingleton<MDyInput>
{
  MDY_SINGLETON_PROPERTIES(MDyInput);
  MDY_SINGLETON_DERIVED(MDyInput);
public:
  ///
  /// @brief Get Axis value which is bound to axis container.
  /// This returns [-1, 1] floating values of axis information instance found.
  /// If the axis name you find didn't find in container,
  /// output error flag in debug mode, and return always 0.0.
  ///
  /// @param[in] axisKeyName The axis name which axis instance has.
  /// @return The axis value which has [-1, 1] range floating value.
  ///
  MDY_NODISCARD TF32 GetAxisValue(_MIN_ const std::string& axisKeyName) noexcept;

  /// @brief Return present-frame mouse position.
  MDY_NODISCARD const DDyVector2& GetPresentMousePosition() const noexcept
  {
    return this->mMousePresentPosition;
  }

  /// @brief Return old-frame mouse position.
  MDY_NODISCARD const DDyVector2& GetPresentLastPosition() const noexcept
  {
    return this->mMouseLastPosition;
  }

  /// @brief Return original value which is not calculated and calibrated at all.
  MDY_NODISCARD TF32 GetJoystickStickValue(_MIN_ DDyClamp<TU32, 0, 5> index) const noexcept;

  /// @brief Return button status of supported keyboard, mouse, and joystick button. \n
  /// button input value must not be `NoneError`.
  MDY_NODISCARD EDyInputButtonStatus GetButtonStatusValue(_MIN_ EDyButton button) const noexcept;

  /// @brief Get whether or not specific axis was pressed.
  /// @param[in] axisSpecifier The axis name which axis instance has.
  /// @return boolean value, if specific axis was pressed, return true.
  MDY_NODISCARD bool IsAxisPressed(_MIN_ const std::string& axisSpecifier) noexcept;
  /// @brief Check if key (action, axis) exist.
  /// @param axisSpecifier key (action, axis) specifier name.
  MDY_NODISCARD bool IsAxisExist(_MIN_ const std::string& axisSpecifier) const noexcept;
  /// @brief Get whether or not specific axis was released.
  /// @param[in] axisSpecifier The axis name which axis instance has.
  /// @return boolean value, if specific axis was released, return true.
  MDY_NODISCARD bool IsAxisReleased(_MIN_ const std::string& axisSpecifier) noexcept;

  /// @brief
  MDY_NODISCARD bool IsActionPressed(_MIN_ const std::string& actionSpecifier) const noexcept;
  /// @brief Check whether or not specified action is being existed.
  MDY_NODISCARD bool IsActionExist(_MIN_ const std::string& actionSpecifier) const noexcept;
  /// @brief
  MDY_NODISCARD bool IsActionReleased(_MIN_ const std::string& actionSpecifier) const noexcept;

  /// @brief Check if mouse is moved on present frame, but false when mouse movement is not activated.
  MDY_NODISCARD bool IsMouseMoved() const noexcept { return this->mIsMouseMoved; }
  /// @brief Check joystick is connected (JOYSTICK 1)
  MDY_NODISCARD bool IsJoystickConnected() const noexcept;
  
  /// @brief Low-level api for checking key is just pressed. Not recommeneded, use `Action` or `Axis` instead.
  MDY_NODISCARD bool IsKeyPressed(_MIN_ EDyInputButton keyValue) const noexcept;

  /// @brief Try require controller exlusive right for UI Widget. \n
  /// If there is any actor which is using controller delegate, Actor delegate will be neglected. \n
  /// And if there is same ui script instance reference already, it just do nothing and return DY_FAILURE.
  /// @TODO IMPLEMENT FOR ADYWIDGETLUASCRIPT
  MDY_NODISCARD EDySuccess MDY_PRIVATE_SPECIFIER(TryRequireControllerUi)(_MIN_ ADyWidgetCppScript& iRefUiScript) noexcept;

  /// @brief
  /// @TODO IMPLEMENT FOR ADYWIDGETLUASCRIPT
  MDY_NODISCARD EDySuccess MDY_PRIVATE_SPECIFIER(TryDetachContollerUi)(_MIN_ ADyWidgetCppScript& iRefUiScript) noexcept;

  /// @brief
  /// @TODO IMPLEMENT FOR ADYWIDGETLUASCRIPT
  MDY_NODISCARD EDySuccess MDY_PRIVATE_SPECIFIER(TryBindAxisDelegate)(
      _MIN_ ADyWidgetCppScript& iRefUiScript, 
      _MIN_ std::function<void(TF32)> iFunction,
      _MIN_ const std::string& iAxisName);

  /// @brief
  /// @TODO IMPLEMENT FOR ADYWIDGETLUASCRIPT
  MDY_NODISCARD EDySuccess MDY_PRIVATE_SPECIFIER(TryBindActionDelegate)(
      _MIN_ ADyWidgetCppScript& iRefUiScript, 
      _MIN_ EDyInputActionStatus iCondition,
      _MIN_ std::function<void()> iFunction,
      _MIN_ const std::string& iAxisName);
  
  /// @brief
  /// @TODO IMPLEMENT FOR ADYACTORLUASCRIPT
  MDY_NODISCARD EDySuccess MDY_PRIVATE_SPECIFIER(TryBindAxisDelegate)(
      _MIN_ ADyActorCppScript& iRefUiScript, 
      _MIN_ std::function<void(TF32)> iFunction,
      _MIN_ const std::string& iAxisName);

  /// @brief
  /// @TODO IMPLEMENT FOR ADYACTORLUASCRIPT
  MDY_NODISCARD EDySuccess MDY_PRIVATE_SPECIFIER(TryBindActionDelegate)(
      _MIN_ ADyActorCppScript& iRefUiScript, 
      _MIN_ EDyInputActionStatus iCondition,
      _MIN_ std::function<void()> iFunction,
      _MIN_ const std::string& iAxisName);

  /// @brief Try require controller exlusive right for Actor. \n
  /// If there is any actor which is using controller delegate, Actor delegate will be reset and overwritten by this. \n
  /// And if there is same actor instance reference already, it just do nothing and return DY_FAILURE.
  /// @TODO IMPLEMENT FOR ADYWIDGETLUASCRIPT
  MDY_NODISCARD EDySuccess MDY_PRIVATE_SPECIFIER(TryRequireControllerActor)(_MIN_ ADyActorCppScript& iRefActor) noexcept;

  /// @brief
  /// @TODO IMPLEMENT FOR ADYACTORLUASCRIPT
  MDY_NODISCARD EDySuccess MDY_PRIVATE_SPECIFIER(TryDetachContollerActor)(_MIN_ ADyActorCppScript& iRefActor) noexcept;

private:
  void MDY_PRIVATE_SPECIFIER(pInitializeAxisNAction)();
  void MDY_PRIVATE_SPECIFIER(pInitializeCallbacks)();

  /// @brief Update in-game input polling on present frame with delta time.
  /// This function must be called update phrase when in-game pause is not set.
  void pfInGameUpdate(_MIN_ TF32 dt) noexcept;

  /// @brief Update global (debug, outside world from in-game) input polling on present frame with delta time.
  /// This function must be called update phrase.
  void pfGlobalUpdate(_MIN_ TF32 dt) noexcept;

  void MDY_PRIVATE_SPECIFIER(pCheckAxisStatus)(_MIN_ TF32 dt);
  void MDY_PRIVATE_SPECIFIER(pCheckActionStatus)(_MIN_ TF32 dt);

  void MDY_PRIVATE_SPECIFIER(pUpdateMouseMovement)(_MIN_ TF32 dt);
  void MDY_PRIVATE_SPECIFIER(pUpdateJoystickSticks)();
  void MDY_PRIVATE_SPECIFIER(pUpdateJoystickButtons)();

  using TAxisMap    = std::unordered_map<std::string, DDyAxisBindingInformation>;
  using TActionMap  = std::unordered_map<std::string, DDyActionBindingInformation>;

  // Window handle pointer (temporal)
  GLFWwindow*       mPtrGlfwWindowContext = nullptr;;
  GLFWcursor*       mGlfwWindowCursorPtr  = nullptr;

  TAxisMap          mBindedAxisMap        = {};
  TActionMap        mBindedActionMap      = {};

  DDyVector2        mMouseLastPosition    = {};
  DDyVector2        mMousePresentPosition = {};

  FDyInputDelegateManager mDelegateManger = {};

  bool              mIsMouseMoved           = false;

  friend class DyEngine;
};

///
/// @macro MDY_ACQUIRE_CONTROLLER_UI
/// @brief Use this for acquire controller exclusive right for UI.
///
#define MDY_ACQUIRE_CONTROLLER_UI() \
  ::dy::MDyInput::GetInstance().MDY_PRIVATE_SPECIFIER(TryRequireControllerUi)(*this);

///
/// @macro MDY_DETACH_CONTROLLER_UI
/// @brief Detach ui script from controller delegate management.
///
#define MDY_DETACH_CONTROLLER_UI() \
  ::dy::MDyInput::GetInstance().MDY_PRIVATE_SPECIFIER(TryDetachContollerUi)(*this);

///
/// @macro MDY_ACQUIRE_CONTROLLER
/// @brief Use this for acquire controller exclusive right for Actor.
///
#define MDY_ACQUIRE_CONTROLLER() \
  ::dy::MDyInput::GetInstance().MDY_PRIVATE_SPECIFIER(TryRequireControllerActor)(*this);

///
/// @macro MDY_DETACH_CONTROLLER
/// @brief Detach actor script from controller delegate management.
///
#define MDY_DETACH_CONTROLLER() \
  ::dy::MDyInput::GetInstance().MDY_PRIVATE_SPECIFIER(TryDetachContollerActor)(*this);

///
/// @macro MDY_BIND_INPUT_AXIS
/// @brief Try register axis input delegate to manager.
///
#define MDY_BIND_INPUT_AXIS(__MAAxisSpecifier__, __MAScriptMemberFunctionPtr__) \
  { \
    auto& inputManager = ::dy::MDyInput::GetInstance(); \
    MDY_NOTUSED const auto flag = inputManager.MDY_PRIVATE_SPECIFIER(TryBindAxisDelegate) \
        (*this, std::bind(__MAScriptMemberFunctionPtr__, this, std::placeholders::_1), __MAAxisSpecifier__); \
  }

///
/// @macro MDY_BIND_INPUT_ACTION
/// @brief Try register axis input delegate to manager.
///
#define MDY_BIND_INPUT_ACTION(__MAAxisSpecifier__, __MACondition__, __MAScriptMemberFunctionPtr__) \
  { \
    auto& inputManager = ::dy::MDyInput::GetInstance(); \
    MDY_NOTUSED const auto flag = inputManager.MDY_PRIVATE_SPECIFIER(TryBindActionDelegate) \
        (*this, __MACondition__, std::bind(__MAScriptMemberFunctionPtr__, this), __MAAxisSpecifier__); \
  }

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
