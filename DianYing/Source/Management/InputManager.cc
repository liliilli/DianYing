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

#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/Helper/InputKeyString.h>
#include <Dy/Management/Type/Input/DDyInputButton.h>
#include <Dy/Management/Type/Input/DDyJoystickAnalog.h>

//!
//! Data
//!

namespace
{

constexpr const char err_input_key_not_exist[] = "Key axis is not exist. [Key axis : {}]";
constexpr TF32 kNegativeValue = -1.0f;
constexpr TF32 kPositiveValue = +1.0f;
constexpr TU32 kMaximumStickCount = 6;

std::array<dy::DDyInputButton, dy::kEDyInputButtonCount> mInputButtonList = {};
std::array<dy::DDyJoystickAnalog, kMaximumStickCount> mInputAnalogStickList = {};

dy::EDyInputButtonStatus  sPrimaryKeys[349];
dy::DDyVector2    sMouseLastPosition    = {};
dy::DDyVector2    sMousePresentPosition = {};
bool              sIsFirstMouseMovement = true;
bool              sMousePositionDirty   = false;
bool              mIsControllerConnected= false;

void DyCallbackCheckJoystickConnection(_MIN_ int joy, _MIN_ int event);

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
  using namespace dy;
  using TEnum = EDyInputButton;
  
  switch (key)
  {
  case GLFW_KEY_RIGHT:mInputButtonList[TEnum::Right].Update(action); break;
  case GLFW_KEY_LEFT: mInputButtonList[TEnum::Left].Update(action); break;
  case GLFW_KEY_DOWN: mInputButtonList[TEnum::Down].Update(action); break;
  case GLFW_KEY_UP:   mInputButtonList[TEnum::Up].Update(action); break;

  case GLFW_KEY_A: mInputButtonList[TEnum::A].Update(action); break;
  case GLFW_KEY_B: mInputButtonList[TEnum::B].Update(action); break;
  case GLFW_KEY_C: mInputButtonList[TEnum::C].Update(action); break;
  case GLFW_KEY_D: mInputButtonList[TEnum::D].Update(action); break;
  case GLFW_KEY_E: mInputButtonList[TEnum::E].Update(action); break;
  case GLFW_KEY_F: mInputButtonList[TEnum::F].Update(action); break;
  case GLFW_KEY_G: mInputButtonList[TEnum::G].Update(action); break;
  case GLFW_KEY_H: mInputButtonList[TEnum::H].Update(action); break;
  case GLFW_KEY_I: mInputButtonList[TEnum::I].Update(action); break;
  case GLFW_KEY_J: mInputButtonList[TEnum::J].Update(action); break;
  case GLFW_KEY_K: mInputButtonList[TEnum::K].Update(action); break;
  case GLFW_KEY_L: mInputButtonList[TEnum::L].Update(action); break;
  case GLFW_KEY_M: mInputButtonList[TEnum::M].Update(action); break;
  case GLFW_KEY_N: mInputButtonList[TEnum::N].Update(action); break;
  case GLFW_KEY_O: mInputButtonList[TEnum::O].Update(action); break;
  case GLFW_KEY_P: mInputButtonList[TEnum::P].Update(action); break;
  case GLFW_KEY_Q: mInputButtonList[TEnum::Q].Update(action); break;
  case GLFW_KEY_R: mInputButtonList[TEnum::R].Update(action); break;
  case GLFW_KEY_S: mInputButtonList[TEnum::S].Update(action); break;
  case GLFW_KEY_T: mInputButtonList[TEnum::T].Update(action); break;
  case GLFW_KEY_U: mInputButtonList[TEnum::U].Update(action); break;
  case GLFW_KEY_V: mInputButtonList[TEnum::V].Update(action); break;
  case GLFW_KEY_X: mInputButtonList[TEnum::X].Update(action); break;
  case GLFW_KEY_Y: mInputButtonList[TEnum::Y].Update(action); break;
  case GLFW_KEY_Z: mInputButtonList[TEnum::Z].Update(action); break;

  case GLFW_KEY_ESCAPE:       mInputButtonList[TEnum::ESCAPE].Update(action); break;
  case GLFW_KEY_ENTER:        mInputButtonList[TEnum::ENTER].Update(action); break;
  case GLFW_KEY_SPACE:        mInputButtonList[TEnum::SPACE].Update(action); break;
  case GLFW_KEY_LEFT_SHIFT:   mInputButtonList[TEnum::LSHIFT].Update(action); break;
  case GLFW_KEY_LEFT_CONTROL: mInputButtonList[TEnum::LCTRL].Update(action); break;
  case GLFW_KEY_LEFT_ALT:     mInputButtonList[TEnum::LALT].Update(action); break;

  case GLFW_KEY_INSERT:   mInputButtonList[TEnum::INSERT].Update(action); break;
  case GLFW_KEY_DELETE:   mInputButtonList[TEnum::DELETE].Update(action); break;
  case GLFW_KEY_PAGE_UP:  mInputButtonList[TEnum::PAGEUP].Update(action); break;
  case GLFW_KEY_PAGE_DOWN:mInputButtonList[TEnum::PAGEDOWN].Update(action); break;
  case GLFW_KEY_HOME:     mInputButtonList[TEnum::HOME].Update(action); break;
  case GLFW_KEY_END:      mInputButtonList[TEnum::END].Update(action); break;

  case GLFW_KEY_MINUS:          mInputButtonList[TEnum::MINUS].Update(action); break;
  case GLFW_KEY_EQUAL:          mInputButtonList[TEnum::EQUAL].Update(action); break;
  case GLFW_KEY_LEFT_BRACKET:   mInputButtonList[TEnum::LBRACKET].Update(action); break;
  case GLFW_KEY_RIGHT_BRACKET:  mInputButtonList[TEnum::RBRACKET].Update(action); break;

  case GLFW_KEY_SEMICOLON:  mInputButtonList[TEnum::SEMICOLON].Update(action); break;
  case GLFW_KEY_APOSTROPHE: mInputButtonList[TEnum::APOSTROPHE].Update(action); break;
  case GLFW_KEY_COMMA:      mInputButtonList[TEnum::COMMA].Update(action); break;
  case GLFW_KEY_PERIOD:     mInputButtonList[TEnum::PERIOD].Update(action); break;

  case GLFW_KEY_F1:  mInputButtonList[TEnum::F1].Update(action); break;
  case GLFW_KEY_F2:  mInputButtonList[TEnum::F2].Update(action); break;
  case GLFW_KEY_F3:  mInputButtonList[TEnum::F3].Update(action); break;
  case GLFW_KEY_F4:  mInputButtonList[TEnum::F4].Update(action); break;
  case GLFW_KEY_F5:  mInputButtonList[TEnum::F5].Update(action); break;
  case GLFW_KEY_F6:  mInputButtonList[TEnum::F6].Update(action); break;
  case GLFW_KEY_F7:  mInputButtonList[TEnum::F7].Update(action); break;
  case GLFW_KEY_F8:  mInputButtonList[TEnum::F8].Update(action); break;
  case GLFW_KEY_F9:  mInputButtonList[TEnum::F9].Update(action); break;
  case GLFW_KEY_F10: mInputButtonList[TEnum::F10].Update(action); break;
  case GLFW_KEY_F11: mInputButtonList[TEnum::F11].Update(action); break;
  case GLFW_KEY_F12: mInputButtonList[TEnum::F12].Update(action); break;

  case GLFW_KEY_CAPS_LOCK:    mInputButtonList[TEnum::CAPSLOCK].Update(action); break;
  case GLFW_KEY_TAB:          mInputButtonList[TEnum::TAB].Update(action); break;
  case GLFW_KEY_BACKSPACE:    mInputButtonList[TEnum::BACKSPACE].Update(action); break;
  case GLFW_KEY_BACKSLASH:    mInputButtonList[TEnum::BACKSLASH].Update(action); break;
  case GLFW_KEY_GRAVE_ACCENT: mInputButtonList[TEnum::GBACCENT].Update(action); break;
  default: break;
  }
}

///
/// @brief Polling notification of mouse movement input.
/// @param xPos width position.
/// @param yPos height position.
///
void DyCallbackMouseMoving(MDY_NOTUSED GLFWwindow* window, _MIN_ double xPos, _MIN_ double yPos)
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
/// @link http://www.glfw.org/docs/latest/group__buttons.html
/// @link http://www.glfw.org/docs/latest/group__mods.html
///
/// @TODO IMPLEMENT MOUSE BUTTON ALSO REPEATED, \n
/// USING `glfwGetMouseButton(window, button);` and callback.
///
void DyCallbackMouseInput(MDY_NOTUSED GLFWwindow* window, _MIN_ int button, _MIN_ int action, MDY_NOTUSED int mods)
{
  using namespace dy;
  using TEnum = EDyInputButton;

  switch (button)
  {
  case GLFW_MOUSE_BUTTON_1: mInputButtonList[TEnum::Mouse0Lmb].Update(action); break;
  case GLFW_MOUSE_BUTTON_2: mInputButtonList[TEnum::Mouse1Rmb].Update(action); break;
  case GLFW_MOUSE_BUTTON_3: mInputButtonList[TEnum::Mouse2Mid].Update(action); break;

  case GLFW_MOUSE_BUTTON_4: mInputButtonList[TEnum::Mouse3].Update(action); break;
  case GLFW_MOUSE_BUTTON_5: mInputButtonList[TEnum::Mouse4].Update(action); break;
  case GLFW_MOUSE_BUTTON_6: mInputButtonList[TEnum::Mouse5].Update(action); break;
  case GLFW_MOUSE_BUTTON_7: mInputButtonList[TEnum::Mouse6].Update(action); break;
  case GLFW_MOUSE_BUTTON_8: mInputButtonList[TEnum::Mouse7].Update(action); break;
  default: break;
  }
};

///
/// @brief Polling notification of mouse scrolling (x, y) input. \n
/// scrolling value have integer value, (-..., ...), not only just -1, 0, 1. \n
/// and, polling stopping scrolling does not supported, so need to checking.
///
void DyCallbackMouseScroll(MDY_NOTUSED GLFWwindow* window, _MIN_ double xoffset, _MIN_ double yoffset)
{
  MDY_LOG_DEBUG_D("Mouse button scrolling : X: {0}, Y: {1}", xoffset, yoffset);
}

/// 
/// @brief Check joystick is connected. \n
/// This function does not poll initial joystick connection status, so
/// need to be checked first time manually.
///
void DyCallbackCheckJoystickConnection(_MIN_ int joy, _MIN_ int event)
{
  // Only support joystick 1.
  if (joy != GLFW_JOYSTICK_1) { return; }

  if (event == GLFW_CONNECTED)
  {
    MDY_LOG_CRITICAL("Joystick {0} Name : {1} Supported.", 0, glfwGetJoystickName(GLFW_JOYSTICK_1));
    mIsControllerConnected = true;
  }
  else if (event == GLFW_DISCONNECTED)
  {
    MDY_LOG_CRITICAL("Joystick {0} Disconnected.", 0);
    mIsControllerConnected = false;
  }
}

/// @brief Check joystick manually whether joystick is connected.
MDY_NODISCARD bool DyCheckIsJoystickConnected() noexcept
{
  const auto isConnected = glfwJoystickPresent(GLFW_JOYSTICK_1);
  return isConnected == GLFW_TRUE ? true : false;
}

/// @brief Apply gravity of each key information, and if key axis value is in range of threshold value,
/// change key status to neutral status.
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
  this->MDY_PRIVATE_FUNC_SPECIFIER(pInitializeAxisNAction)();
  this->MDY_PRIVATE_FUNC_SPECIFIER(pInitializeCallbacks)();

  if (DyCheckIsJoystickConnected() == true)
  { // Check joystick binding manually at first time.
    MDY_LOG_CRITICAL("Joystick {0} Name : {1} Supported.", 0, glfwGetJoystickName(GLFW_JOYSTICK_1));
    mIsControllerConnected = true;
  }

  return DY_SUCCESS;
}

void MDyInput::MDY_PRIVATE_FUNC_SPECIFIER(pInitializeAxisNAction)()
{
   const auto& keyInformation = MDySetting::GetInstance().GetInputSettingInformation();

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
}

void MDyInput::MDY_PRIVATE_FUNC_SPECIFIER(pInitializeCallbacks)()
{
  this->mPtrGlfwWindowContext = MDyWindow::GetInstance().GetGLMainWindow();
  glfwSetKeyCallback        (this->mPtrGlfwWindowContext, DyCallbackInputKeyboard);
  glfwSetCursorPosCallback  (this->mPtrGlfwWindowContext, DyCallbackMouseMoving);
  glfwSetMouseButtonCallback(this->mPtrGlfwWindowContext, DyCallbackMouseInput);
  glfwSetScrollCallback     (this->mPtrGlfwWindowContext, DyCallbackMouseScroll);
  glfwSetInputMode          (this->mPtrGlfwWindowContext, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetJoystickCallback   (DyCallbackCheckJoystickConnection);
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

TF32 MDyInput::GetJoystickStickValue(_MIN_ DDyClamp<TU32, 0, 5> index) const noexcept
{
  return mInputAnalogStickList[index].GetValue();
}

EDyInputButtonStatus MDyInput::GetButtonStatusValue(_MIN_ EDyButton button) const noexcept
{
  MDY_ASSERT(button != EDyButton::NoneError, "Button value must not be `NoneErorr`.");
  return mInputButtonList[button].Get();
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
  return DyIsMapContains(this->mBindedAxisMap, axisSpecifierName);
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

bool MDyInput::IsActionExist(_MIN_ const std::string& actionSpecifier) const noexcept
{
  return DyIsMapContains(this->mBindedActionMap, actionSpecifier);
}

bool MDyInput::IsJoystickConnected() const noexcept
{
  return mIsControllerConnected;
}

void MDyInput::pfUpdate(_MIN_ TF32 dt) noexcept
{
  this->MDY_PRIVATE_FUNC_SPECIFIER(pUpdateMouseMovement)(dt);
  if (this->IsJoystickConnected() == true)
  {
    this->MDY_PRIVATE_FUNC_SPECIFIER(pUpdateJoystickSticks)();
    this->MDY_PRIVATE_FUNC_SPECIFIER(pUpdateJoystickButtons)();
  }

  this->MDY_PRIVATE_FUNC_SPECIFIER(pCheckAxisStatus)(dt);
  this->MDY_PRIVATE_FUNC_SPECIFIER(pCheckActionStatus)(dt);
}

void MDyInput::MDY_PRIVATE_FUNC_SPECIFIER(pUpdateJoystickSticks)()
{
  int supportingStickCount;
  const float* stickValueList = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &supportingStickCount);

  const auto stickCount = supportingStickCount < kMaximumStickCount ? supportingStickCount : kMaximumStickCount;
  for (TU32 i = 0; i < stickCount; ++i)
  {
    mInputAnalogStickList[i].Update(stickValueList[i]);
  }
}

void MDyInput::MDY_PRIVATE_FUNC_SPECIFIER(pUpdateJoystickButtons)()
{
  using TEnum = EDyInputButton;
  static constexpr auto dyJoystickBtnCount = TEnum::Joystick17 - TEnum::Joystick0 + 1;

  int buttonCount;
  const auto* actionList = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
  const auto supportedButtonCount = 
      buttonCount < dyJoystickBtnCount 
    ? buttonCount 
    : dyJoystickBtnCount;

  for (int i = 0; i < supportedButtonCount; ++i)
  {
    mInputButtonList[TEnum::Joystick0 + i].Update(actionList[i]);
  }
}

void MDyInput::MDY_PRIVATE_FUNC_SPECIFIER(pCheckAxisStatus)(_MIN_ TF32 dt)
{
  using EAxisStatus   = DDyAxisBindingInformation::EDyAxisInputStatus;
  
  /// @brief Check keyboard axis status.
  /// @param axisInfo Axis information
  static auto CheckAxisStatus = [](_MINOUT_ DDyAxisBindingInformation& axisInfo,
                                   _MIN_ EDyInputButtonStatus goalState,
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

  /// @brief Process axis update routine when axisInfo status is `Status::CommonNeutral`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_CommonNeutral = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Negative
    if (CheckAxisStatus(axisInfo, EDyInputButtonStatus::Pressed, true, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kNegativeValue;
      axis.mKeyStatus = EAxisStatus::NegativePressed;
    }) == DY_SUCCESS) { return; }
    // Positive
    if (CheckAxisStatus(axisInfo, EDyInputButtonStatus::Pressed, false, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kPositiveValue;
      axis.mKeyStatus = EAxisStatus::PositivePressed;
    }) == DY_SUCCESS) { return; }
  };

  /// @brief Process axis update routine when axisInfo status is `Status::NegativePressed`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_NegativePressed = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Positive
    if (CheckAxisStatus(axisInfo, EDyInputButtonStatus::Pressed, false, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kPositiveValue;
      axis.mKeyStatus = EAxisStatus::PositivePressed;
    }) == DY_SUCCESS) { return; }
    // Negative
    if (axisInfo.mIsRepeatKey == true)
    {
      CheckAxisStatus(axisInfo, EDyInputButtonStatus::Repeated, true, [](DDyAxisBindingInformation& axis)
      {
        axis.mKeyStatus = EAxisStatus::NegativeRepeated;
      });
    }
    else
    {
      CheckAxisStatus(axisInfo, EDyInputButtonStatus::Released, true, [](DDyAxisBindingInformation& axis)
      {
        axis.mKeyStatus = EAxisStatus::CommonReleased;
        DyProceedAxisGravity(axis);
      });
    }
  };

  /// @brief Process axis update routine when axisInfo status is `Status::NegativePressed`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_PositivePressed = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Negative
    if (CheckAxisStatus(axisInfo, EDyInputButtonStatus::Pressed, true, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kNegativeValue;
      axis.mKeyStatus = EAxisStatus::NegativePressed;
    }) == DY_SUCCESS) { return; }
    // Positive
    if (axisInfo.mIsRepeatKey == true)
    {
      CheckAxisStatus(axisInfo, EDyInputButtonStatus::Repeated, false, [](DDyAxisBindingInformation& axis)
      {
        axis.mKeyStatus = EAxisStatus::PositiveRepeated;
      });
    }
    else
    {
      CheckAxisStatus(axisInfo, EDyInputButtonStatus::Released, false, [](DDyAxisBindingInformation& axis)
      {
        axis.mKeyStatus = EAxisStatus::CommonReleased;
        DyProceedAxisGravity(axis);
      });
    }
  };

  /// @brief Process axis update routine when axisInfo status is `Status::PositiveRepeated`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_PositiveRepeated = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Negative
    if (CheckAxisStatus(axisInfo, EDyInputButtonStatus::Pressed, true, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kNegativeValue;
      axis.mKeyStatus = EAxisStatus::NegativePressed;
    }) == DY_SUCCESS) { return; }
    // Positive
    if (CheckAxisStatus(axisInfo, EDyInputButtonStatus::Released, false, [](DDyAxisBindingInformation& axis)
    {
      axis.mKeyStatus = EAxisStatus::CommonReleased;
      DyProceedAxisGravity(axis);
    }) == DY_SUCCESS) { return; }
  };

  /// @brief Process axis update routine when axisInfo status is `Status::NegativeRepeated`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_NegativeRepeated = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Positive
    if (CheckAxisStatus(axisInfo, EDyInputButtonStatus::Pressed, false, [](DDyAxisBindingInformation& axis)
    {
      axis.mAxisValue = kPositiveValue;
      axis.mKeyStatus = EAxisStatus::PositivePressed;
    }) == DY_SUCCESS) { return; }
    // Negative
    if (CheckAxisStatus(axisInfo, EDyInputButtonStatus::Released, true, [](DDyAxisBindingInformation& axis)
    {
      axis.mKeyStatus = EAxisStatus::CommonReleased;
      DyProceedAxisGravity(axis);
    }) == DY_SUCCESS) { return; }
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FUNCTION BODY ∨
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
}

void MDyInput::MDY_PRIVATE_FUNC_SPECIFIER(pCheckActionStatus)(_MIN_ TF32 dt)
{
  using EActionStatus = DDyActionBindingInformation::EDyActionInputStatus;

  /// @brief Check action key status.
  /// @param actionInfo action information
  static auto CheckActionStatus = [](_MINOUT_ DDyActionBindingInformation& actionInfo,
                                     _MIN_ EDyInputButtonStatus goalState,
                                     auto callback)
  {
    for (const auto id : actionInfo.mActionId)
    {
      if (sPrimaryKeys[id] == goalState) { callback(actionInfo); return DY_SUCCESS; }
    }

    return DY_FAILURE;
  };

  /// @brief Process action update routine when actionInfo status is `Status::Released`.
  /// @param actionInfo action information
  static auto ProcessAction_Released = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    CheckActionStatus(actionInfo, EDyInputButtonStatus::Released, [](DDyActionBindingInformation& action)
    {
      action.mKeyStatus = EActionStatus::Pressed;
    });
  };

  /// @brief Process action update routine when actionInfo status is `Status::Pressed`.
  /// @param actionInfo action information
  static auto ProcessAction_Pressed = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    if (CheckActionStatus(actionInfo, EDyInputButtonStatus::Released, [](DDyActionBindingInformation& action)
    {
      action.mKeyStatus = EActionStatus::Released;
    }) == DY_SUCCESS) { return; }
    else
    {
      CheckActionStatus(actionInfo, EDyInputButtonStatus::Pressed, [](DDyActionBindingInformation& action)
      {
        action.mKeyStatus = EActionStatus::Bottled;
      });
    }
  };

  /// @brief Process action update routine when actionInfo status is `Status::Bottled`.
  /// @param actionInfo action information
  static auto ProcessAction_Bottled = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    CheckActionStatus(actionInfo, EDyInputButtonStatus::Released, [](DDyActionBindingInformation& action)
    {
      action.mKeyStatus = EActionStatus::Released;
    });
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FUNCTION BODY ∨
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
}

void MDyInput::MDY_PRIVATE_FUNC_SPECIFIER(pUpdateMouseMovement)(_MIN_ TF32 dt)
{
  if (sIsFirstMouseMovement == false && sMousePositionDirty == true)
  { 
    this->mMousePresentPosition = sMousePresentPosition;
    this->mMouseLastPosition    = sMouseLastPosition;

    this->mIsMouseMoved = true;
    sMousePositionDirty = false;
  }
  else { this->mIsMouseMoved = false; }
}

} /// ::dy namespace