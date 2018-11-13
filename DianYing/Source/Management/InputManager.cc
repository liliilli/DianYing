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
#include <Dy/Management/InputManager.h>

#include <nlohmann/json.hpp>

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/Helper/InputKeyString.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Constant/StringSettingFile.h>

//!
//! Data
//!

namespace
{

///
/// @enum EKeyPrimaryState
/// @brief Check key primary state only in this file.
///
enum class EKeyPrimaryState : char
{
  Released  = 0,
  Pressed   = 1,
  Repeated  = 2
};

constexpr const char err_input_key_not_exist[] = "Key axis is not exist. [Key axis : {}]";

EKeyPrimaryState  sPrimaryKeys[349];
dy::DDyVector2    sMouseLastPosition    = {};
dy::DDyVector2    sMousePresentPosition = {};
bool              sIsFirstMouseMovement = true;
bool              sMousePositionDirty   = false;

///
/// @brief
/// Polling notification of physical key input.
/// If key is notified glfw library execute this function as callback.
///
/// In this now, just print what key was pressed on console.
///
/// @param[in] window GLFW window instance.
/// @param[in] key Key information.
/// @param[in] scancode Not be used now.
/// @param[in] action Key pressed, released, keeping pushed states.
/// @param[in] mod Not be used now.
///
void DyCallbackInputKeyboard(MDY_NOTUSED GLFWwindow* window, _MIN_ int key, MDY_NOTUSED int scancode, _MIN_ int action, MDY_NOTUSED int mod)
{
  MDY_LOG_DEBUG_D("Key input : {0}, {1}", key, action);

  switch (action) {
  case GLFW_PRESS:
    sPrimaryKeys[key] = EKeyPrimaryState::Pressed;
    break;
  case GLFW_RELEASE:
    sPrimaryKeys[key] = EKeyPrimaryState::Released;
    break;
  case GLFW_REPEAT:
    sPrimaryKeys[key] = EKeyPrimaryState::Repeated;
    break;
  default: break;
  }
}

///
/// @brief Polling notification of mouse movement input.
/// @param xPos width position.
/// @param yPos height position.
///
void DyCallbackInputMouse(MDY_NOTUSED GLFWwindow* window, _MIN_ double xPos, _MIN_ double yPos)
{
  if (sIsFirstMouseMovement == true)
  {
    sMouseLastPosition.X = static_cast<float>(xPos);
    sMouseLastPosition.Y = static_cast<float>(yPos);
    sIsFirstMouseMovement = false;
  }
  else
  {
    sMouseLastPosition = sMousePresentPosition;
  }

  sMousePresentPosition.X = static_cast<float>(xPos);
  sMousePresentPosition.Y = static_cast<float>(yPos);
  sMousePositionDirty     = true;
}

///
/// @brief Polling notification of mouse button trigger input. \n
/// The action is one of `GLFW_PRESS` or `GLFW_RELEASE`.
/// @param window
/// @param button
/// @param action
/// @param mods
/// @link http://www.glfw.org/docs/latest/group__buttons.html
/// @link http://www.glfw.org/docs/latest/group__mods.html
///
void DyCallbackInputMouseButton(MDY_NOTUSED GLFWwindow* window, _MIN_ int button, _MIN_ int action, MDY_NOTUSED int mods)
{
  MDY_LOG_DEBUG_D("Mouse button input : {0}, {1}", button, action);
};

///
/// @brief Apply gravity of each key information, and if key axis value is in range of threshold value,
/// change key status to neutral status.
///
void DyProceedAxisGravity(_MINOUT_ dy::DDyAxisBindingInformation& axisInfo)
{
	const auto dt = dy::MDyTime::GetInstance().GetGameScaledTickedDeltaTimeValue();
  static constexpr TF32 kZeroValue = 0.0f;

	if (auto& value = axisInfo.mAxisValue; value < 0) // Negative
  {
		value += axisInfo.mGravity * dt;
		if (-axisInfo.mNeutralStatusThresholdValue <= value)
    {
			value = kZeroValue;
			axisInfo.mKeyStatus = dy::DDyAxisBindingInformation::EDyAxisInputStatus::CommonNeutral;
		}
	}
	else // Positive
  {
		value -= axisInfo.mGravity * dt;
		if (value <= axisInfo.mNeutralStatusThresholdValue)
    {
			value = kZeroValue;
			axisInfo.mKeyStatus = dy::DDyAxisBindingInformation::EDyAxisInputStatus::CommonNeutral;
		}
	}
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyInput::pfInitialize()
{
  const auto& settingManager = MDySetting::GetInstance();
  const auto& keyInformation = settingManager.GetInputSettingInformation();

  // AXIS MAP
  for (const auto& [specifierName, info] : keyInformation.mAxisMap)
  {
    MDY_ASSERT(this->IsAxisExist(specifierName) == false, "Duplicated axis-key specifier name is already binded.");
    auto [_, isSucceeded] = this->mBindedAxisMap.try_emplace(specifierName, info);
    MDY_ASSERT(isSucceeded == true, "");
  }
  // ACTION MAP
  for (const auto& [specifierName, info] : keyInformation.mActionMap)
  {
    MDY_ASSERT(this->IsActionExist(specifierName) == false, "Duplicated action-key specifier name is already binded.");
    auto [_, isSucceeded] = this->mBindedActionMap.try_emplace(specifierName, info);
    MDY_ASSERT(isSucceeded == true, "");
  }

  auto& winManager = MDyWindow::GetInstance();
  this->mTempGlfwWindowPtr = winManager.GetGlfwWindowContext();

  glfwSetKeyCallback        (this->mTempGlfwWindowPtr, DyCallbackInputKeyboard);
  glfwSetCursorPosCallback  (this->mTempGlfwWindowPtr, DyCallbackInputMouse);
  glfwSetMouseButtonCallback(this->mTempGlfwWindowPtr, DyCallbackInputMouseButton);

  glfwSetInputMode(this->mTempGlfwWindowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // @todo Implement joystick button and position callback function.

  return DY_SUCCESS;
}

EDySuccess MDyInput::pfRelease()
{
  return DY_SUCCESS;
}

float MDyInput::GetAxisValue(_MIN_ const std::string& axisKeyName) noexcept
{
  // Validity test
  const auto keyIt = this->mBindedAxisMap.find(axisKeyName);
  if (keyIt == this->mBindedAxisMap.end())
  {
    MDY_LOG_ERROR_D(err_input_key_not_exist, axisKeyName);
		return 0.f;
	}

  return keyIt->second.mAxisValue;
}

bool MDyInput::IsAxisPressed(_MIN_ const std::string& axisSpecifierName) noexcept
{
  // Validity test
  const auto keyIt = this->mBindedAxisMap.find(axisSpecifierName);
  if (keyIt == mBindedAxisMap.end())
  {
    MDY_LOG_ERROR_D(err_input_key_not_exist, axisSpecifierName);
		return false;
  }

  // If key is pressed on following repeated key flag, return true or false.
  if (DDyAxisBindingInformation& keyInformation = keyIt->second; keyInformation.mIsRepeatKey == false)
  {
    switch (keyInformation.mKeyStatus)
    {
    case DDyAxisBindingInformation::EDyAxisInputStatus::PositivePressed:
    case DDyAxisBindingInformation::EDyAxisInputStatus::NegativePressed:
      return true;
    default: return false;
    }
  }
  else
  {
    switch (keyInformation.mKeyStatus)
    {
    case DDyAxisBindingInformation::EDyAxisInputStatus::PositivePressed:
    case DDyAxisBindingInformation::EDyAxisInputStatus::NegativePressed:
    case DDyAxisBindingInformation::EDyAxisInputStatus::PositiveRepeated:
    case DDyAxisBindingInformation::EDyAxisInputStatus::NegativeRepeated:
      return true;
    default: return false;
    }
  }
}

bool MDyInput::IsAxisReleased(_MIN_ const std::string& axisSpecifierName) noexcept
{
  // Validity test
  const auto keyIt = this->mBindedAxisMap.find(axisSpecifierName);
  if (keyIt == mBindedAxisMap.end())
  {
    MDY_LOG_ERROR_D(err_input_key_not_exist, axisSpecifierName);
		return false;
  }

  switch (keyIt->second.mKeyStatus)
  {
  case DDyAxisBindingInformation::EDyAxisInputStatus::CommonNeutral:
  case DDyAxisBindingInformation::EDyAxisInputStatus::CommonReleased:
    return true;
  default: return false;
  }
}

bool MDyInput::IsAxisRepeated(_MIN_ const std::string& axisSpecifier) noexcept
{
  // Validity test
  const auto keyIt = this->mBindedAxisMap.find(axisSpecifier);
  if (keyIt == mBindedAxisMap.end())
  {
    MDY_LOG_ERROR_D(err_input_key_not_exist, axisSpecifier);
		return false;
  }

  switch (keyIt->second.mKeyStatus)
  {
  case DDyAxisBindingInformation::EDyAxisInputStatus::PositiveRepeated:
  case DDyAxisBindingInformation::EDyAxisInputStatus::NegativeRepeated:
    return true;
  default: return false;
  }
}

bool MDyInput::IsAxisExist(_MIN_ const std::string& axisSpecifierName) const noexcept
{
  return this->mBindedAxisMap.find(axisSpecifierName) != this->mBindedAxisMap.end();
}

bool MDyInput::IsActionPressed(_MIN_ const std::string& actionSpecifier) const noexcept
{
  // Validity test
  const auto keyIt = this->mBindedActionMap.find(actionSpecifier);
  if (keyIt == mBindedActionMap.end())
  {
    MDY_LOG_ERROR_D(err_input_key_not_exist, actionSpecifier);
		return false;
  }

  return keyIt->second.mKeyStatus == DDyActionBindingInformation::EDyActionInputStatus::Pressed;
}

bool MDyInput::IsActionReleased(_MIN_ const std::string& actionSpecifier) const noexcept
{
  // Validity test
  const auto keyIt = this->mBindedActionMap.find(actionSpecifier);
  if (keyIt == mBindedActionMap.end())
  {
    MDY_LOG_ERROR_D(err_input_key_not_exist, actionSpecifier);
		return false;
  }

  return keyIt->second.mKeyStatus == DDyActionBindingInformation::EDyActionInputStatus::Released;
}

MDY_NODISCARD bool MDyInput::IsActionExist(_MIN_ const std::string& actionSpecifier) const noexcept
{
  return this->mBindedActionMap.find(actionSpecifier) != this->mBindedActionMap.end();
}

void MDyInput::pfUpdate(_MIN_ TF32 dt) noexcept
{
  using EAxisStatus   = DDyAxisBindingInformation::EDyAxisInputStatus;
  using EActionStatus = DDyActionBindingInformation::EDyActionInputStatus;
  static constexpr TF32 kNegativeValue = -1.0f;
  static constexpr TF32 kPositiveValue = +1.0f;

  ///
  /// @brief
  /// @param axisInfo Axis information
  ///
  static auto CheckAxisStatus = [](_MINOUT_ DDyAxisBindingInformation& axisInfo,
                                   _MIN_ EKeyPrimaryState goalState,
                                   _MIN_ const bool isNegative,
                                   auto callback)
  {
    if (isNegative == true)
    {
      for (const auto id : axisInfo.mNegativeButtonId)
      {
        if (sPrimaryKeys[id] == goalState) { callback(axisInfo); return DY_SUCCESS; }
      }
    }
    else
    {
      for (const auto id : axisInfo.mPositiveButtonId)
      {
        if (sPrimaryKeys[id] == goalState) { callback(axisInfo); return DY_SUCCESS; }
      }
    }

    return DY_FAILURE;
  };

  ///
  /// @brief Process axis update routine when axisInfo status is `Status::CommonNeutral`.
  /// @param axisInfo Axis information
  ///
  static auto ProcessAxis_CommonNeutral = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Negative
    if (CheckAxisStatus(axisInfo, EKeyPrimaryState::Pressed, true, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kNegativeValue;
      axis.mKeyStatus = EAxisStatus::NegativePressed;
    }) == DY_SUCCESS) { return; }
    // Positive
    if (CheckAxisStatus(axisInfo, EKeyPrimaryState::Pressed, false, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kPositiveValue;
      axis.mKeyStatus = EAxisStatus::PositivePressed;
    }) == DY_SUCCESS) { return; }
  };

  ///
  /// @brief Process axis update routine when axisInfo status is `Status::NegativePressed`.
  /// @param axisInfo Axis information
  ///
  static auto ProcessAxis_NegativePressed = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Positive
    if (CheckAxisStatus(axisInfo, EKeyPrimaryState::Pressed, false, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kPositiveValue;
      axis.mKeyStatus = EAxisStatus::PositivePressed;
    }) == DY_SUCCESS) { return; }
    // Negative
    if (axisInfo.mIsRepeatKey == true)
    {
      CheckAxisStatus(axisInfo, EKeyPrimaryState::Repeated, true, [](DDyAxisBindingInformation& axis)
      {
        axis.mKeyStatus = EAxisStatus::NegativeRepeated;
      });
    }
    else
    {
      CheckAxisStatus(axisInfo, EKeyPrimaryState::Released, true, [](DDyAxisBindingInformation& axis)
      {
        axis.mKeyStatus = EAxisStatus::CommonReleased;
        DyProceedAxisGravity(axis);
      });
    }
  };

  ///
  /// @brief Process axis update routine when axisInfo status is `Status::NegativePressed`.
  /// @param axisInfo Axis information
  ///
  static auto ProcessAxis_PositivePressed = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Negative
    if (CheckAxisStatus(axisInfo, EKeyPrimaryState::Pressed, true, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kNegativeValue;
      axis.mKeyStatus = EAxisStatus::NegativePressed;
    }) == DY_SUCCESS) { return; }
    // Positive
    if (axisInfo.mIsRepeatKey == true)
    {
      CheckAxisStatus(axisInfo, EKeyPrimaryState::Repeated, false, [](DDyAxisBindingInformation& axis)
      {
        axis.mKeyStatus = EAxisStatus::PositiveRepeated;
      });
    }
    else
    {
      CheckAxisStatus(axisInfo, EKeyPrimaryState::Released, false, [](DDyAxisBindingInformation& axis)
      {
        axis.mKeyStatus = EAxisStatus::CommonReleased;
        DyProceedAxisGravity(axis);
      });
    }
  };

  ///
  /// @brief Process axis update routine when axisInfo status is `Status::PositiveRepeated`.
  /// @param axisInfo Axis information
  ///
  static auto ProcessAxis_PositiveRepeated = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Negative
    if (CheckAxisStatus(axisInfo, EKeyPrimaryState::Pressed, true, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kNegativeValue;
      axis.mKeyStatus = EAxisStatus::NegativePressed;
    }) == DY_SUCCESS) { return; }
    // Positive
    if (CheckAxisStatus(axisInfo, EKeyPrimaryState::Released, false, [](DDyAxisBindingInformation& axis)
    {
      axis.mKeyStatus = EAxisStatus::CommonReleased;
      DyProceedAxisGravity(axis);
    }) == DY_SUCCESS) { return; }
  };

  ///
  /// @brief Process axis update routine when axisInfo status is `Status::NegativeRepeated`.
  /// @param axisInfo Axis information
  ///
  static auto ProcessAxis_NegativeRepeated = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Positive
    if (CheckAxisStatus(axisInfo, EKeyPrimaryState::Pressed, false, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kPositiveValue;
      axis.mKeyStatus = EAxisStatus::PositivePressed;
    }) == DY_SUCCESS) { return; }
    // Negative
    if (CheckAxisStatus(axisInfo, EKeyPrimaryState::Released, true, [](DDyAxisBindingInformation& axis)
    {
      axis.mKeyStatus = EAxisStatus::CommonReleased;
      DyProceedAxisGravity(axis);
    }) == DY_SUCCESS) { return; }
  };

  ///
  /// @brief
  /// @param actionInfo action information
  ///
  static auto CheckActionStatus = [](_MINOUT_ DDyActionBindingInformation& actionInfo,
                                     _MIN_ EKeyPrimaryState goalState,
                                     auto callback)
  {
    for (const auto id : actionInfo.mActionId)
    {
      if (sPrimaryKeys[id] == goalState) { callback(actionInfo); return DY_SUCCESS; }
    }

    return DY_FAILURE;
  };

  ///
  /// @brief Process action update routine when actionInfo status is `Status::Released`.
  /// @param actionInfo action information
  ///
  static auto ProcessAction_Released = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    CheckActionStatus(actionInfo, EKeyPrimaryState::Released, [](DDyActionBindingInformation& action)
    {
      action.mKeyStatus = EActionStatus::Pressed;
    });
  };

  ///
  /// @brief Process action update routine when actionInfo status is `Status::Pressed`.
  /// @param actionInfo action information
  ///
  static auto ProcessAction_Pressed = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    if (CheckActionStatus(actionInfo, EKeyPrimaryState::Released, [](DDyActionBindingInformation& action)
    {
      action.mKeyStatus = EActionStatus::Released;
    }) == DY_SUCCESS) { return; }
    else
    {
      CheckActionStatus(actionInfo, EKeyPrimaryState::Pressed, [](DDyActionBindingInformation& action)
      {
        action.mKeyStatus = EActionStatus::Bottled;
      });
    }
  };

  ///
  /// @brief Process action update routine when actionInfo status is `Status::Bottled`.
  /// @param actionInfo action information
  ///
  static auto ProcessAction_Bottled = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    CheckActionStatus(actionInfo, EKeyPrimaryState::Released, [](DDyActionBindingInformation& action)
    {
      action.mKeyStatus = EActionStatus::Released;
    });
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (auto& [_, axis] : this->mBindedAxisMap)
  { // AXIS
    switch (axis.mKeyStatus)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EAxisStatus::CommonReleased:
      DyProceedAxisGravity(axis);
      [[fallthrough]];
    case EAxisStatus::CommonNeutral:     ProcessAxis_CommonNeutral(axis);     break;
    case EAxisStatus::NegativePressed:   ProcessAxis_NegativePressed(axis);   break;
    case EAxisStatus::PositivePressed:   ProcessAxis_PositivePressed(axis);   break;
    case EAxisStatus::PositiveRepeated:  ProcessAxis_PositiveRepeated(axis);  break;
    case EAxisStatus::NegativeRepeated:  ProcessAxis_NegativeRepeated(axis);  break;
    }
  }

  for (auto& [_, action] : this->mBindedActionMap)
  { // ACTION
    switch (action.mKeyStatus)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EActionStatus::Released: ProcessAction_Released(action);  break;
    case EActionStatus::Pressed:  ProcessAction_Pressed(action);   break;
    case EActionStatus::Bottled:  ProcessAction_Bottled(action);   break;
    }
  }

  if (sIsFirstMouseMovement == false && sMousePositionDirty == true)
  { // MOUSE
    this->mMousePresentPosition = sMousePresentPosition;
    this->mMouseLastPosition    = sMouseLastPosition;

    //MDY_LOG_DEBUG_D("Mouse position : {}, {}", this->mMousePresentPosition.X, this->mMousePresentPosition.Y);
    this->mIsMouseMoved = true;
    sMousePositionDirty = false;
  }
  else { this->mIsMouseMoved = false; }
}

} /// ::dy namespace