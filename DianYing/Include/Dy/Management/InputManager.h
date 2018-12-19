#ifndef GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
#define GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
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

#include <string>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/KeyAxisBindingInformation.h>
#include <Dy/Management/Type/KeyActionBindingInformation.h>

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

  ///
  /// @brief Return present-frame mouse position.
  ///
  FORCEINLINE MDY_NODISCARD const DDyVector2& GetPresentMousePosition() const noexcept
  {
    return this->mMousePresentPosition;
  }

  ///
  /// @brief Return old-frame mouse position.
  ///
  FORCEINLINE MDY_NODISCARD const DDyVector2& GetPresentLastPosition() const noexcept
  {
    return this->mMouseLastPosition;
  }

  ///
  /// @brief Get whether or not specific axis was pressed.
  /// @param[in] axisSpecifier The axis name which axis instance has.
  /// @return boolean value, if specific axis was pressed, return true.
  ///
  MDY_NODISCARD bool IsAxisPressed(_MIN_ const std::string& axisSpecifier) noexcept;

  ///
  /// @brief Get whether or not specific axis was released.
  /// @param[in] axisSpecifier The axis name which axis instance has.
  /// @return boolean value, if specific axis was released, return true.
  ///
  MDY_NODISCARD bool IsAxisReleased(_MIN_ const std::string& axisSpecifier) noexcept;

  ///
  /// @brief Get whether or not speicfied axis is being repeated.
  /// @param axisSpecifier The axis name which axis instance has.
  /// @return boolean value, if given axis is being repeated return true.
  ///
  MDY_NODISCARD bool IsAxisRepeated(_MIN_ const std::string& axisSpecifier) noexcept;

  ///
  /// @brief Check if key (action, axis) exist.
  /// @param axisSpecifier key (action, axis) specifier name.
  ///
  MDY_NODISCARD bool IsAxisExist(_MIN_ const std::string& axisSpecifier) const noexcept;

  ///
  /// @brief
  /// @param actionSpecifier
  ///
  MDY_NODISCARD bool IsActionPressed(_MIN_ const std::string& actionSpecifier) const noexcept;

  ///
  /// @brief
  /// @param actionSpecifier
  ///
  MDY_NODISCARD bool IsActionReleased(_MIN_ const std::string& actionSpecifier) const noexcept;

  ///
  /// @brief Check whether or not specified action is being existed.
  /// @param actionSpecifier The action name which action instance has.
  ///
  MDY_NODISCARD bool IsActionExist(_MIN_ const std::string& actionSpecifier) const noexcept;

  /// @brief Check if mouse is moved on present frame, but false when mouse movement is not activated.
  MDY_NODISCARD bool IsMouseMoved() const noexcept { return this->mIsMouseMoved; }

  /// @brief Check joystick is connected (JOYSTICK 1)
  MDY_NODISCARD bool IsJoystickConnected() const noexcept;

private:
  void MDY_PRIVATE_FUNC_SPECIFIER(pInitializeAxisNAction)();
  void MDY_PRIVATE_FUNC_SPECIFIER(pInitializeCallbacks)();

  /// @brief Update input polling on present frame with delta time.
  /// This function must be called update phrase.
  void pfUpdate(_MIN_ TF32 dt) noexcept;

  void MDY_PRIVATE_FUNC_SPECIFIER(pCheckAxisStatus)(_MIN_ TF32 dt);
  void MDY_PRIVATE_FUNC_SPECIFIER(pCheckActionStatus)(_MIN_ TF32 dt);
  void MDY_PRIVATE_FUNC_SPECIFIER(pUpdateMouseMovement)(_MIN_ TF32 dt);
  void MDY_PRIVATE_FUNC_SPECIFIER(pUpdateJoystickSticks)();
  void MDY_PRIVATE_FUNC_SPECIFIER(pUpdateJoystickButtons)();

  using TAxisMap    = std::unordered_map<std::string, DDyAxisBindingInformation>;
  using TActionMap  = std::unordered_map<std::string, DDyActionBindingInformation>;

  // Window handle pointer (temporal)
  GLFWwindow*       mPtrGlfwWindowContext = nullptr;;
  GLFWcursor*       mGlfwWindowCursorPtr  = nullptr;

  TAxisMap          mBindedAxisMap        = {};
  TActionMap        mBindedActionMap      = {};

  DDyVector2        mMouseLastPosition    = {};
  DDyVector2        mMousePresentPosition = {};

  bool              mIsMouseMoved           = false;

  friend class DyEngine;
};

}

#endif /// GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
