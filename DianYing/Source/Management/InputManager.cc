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
#include <Dy/Management/InputManager.h>

#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/Type/Input/DDyInputButton.h>
#include <Dy/Management/Type/Input/DDyJoystickAnalog.h>
#include <Dy/Management/Type/Render/DDyPixelInfo.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Core/DyEngine.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Element/Internal/TDyIdDistributor.h>

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

dy::DDyVector2    sMouseLastPosition    = {};
dy::DDyVector2    sMousePresentPosition = {};
bool              sIsFirstMouseMovement = true;
bool              sMousePositionDirty   = false;
bool              mIsControllerConnected= false;

void DyCallbackCheckJoystickConnection(_MIN_ int joy, _MIN_ int event);

/// @brief
/// Polling notification of physical key input.
/// If key is notified glfw library execute this function as callback.
/// In this now, just print what key was pressed on console.
///
/// @param[in] window GLFW window instance.
/// @param[in] key Key information.
/// @param[in] scancode Not be used now.
/// @param[in] action Key pressed, released, keeping pushed states.
/// @param[in] mod Not be used now.
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

  case GLFW_KEY_0: mInputButtonList[TEnum::Num0].Update(action); break;
  case GLFW_KEY_1: mInputButtonList[TEnum::Num1].Update(action); break;
  case GLFW_KEY_2: mInputButtonList[TEnum::Num2].Update(action); break;
  case GLFW_KEY_3: mInputButtonList[TEnum::Num3].Update(action); break;
  case GLFW_KEY_4: mInputButtonList[TEnum::Num4].Update(action); break;
  case GLFW_KEY_5: mInputButtonList[TEnum::Num5].Update(action); break;
  case GLFW_KEY_6: mInputButtonList[TEnum::Num6].Update(action); break;
  case GLFW_KEY_7: mInputButtonList[TEnum::Num7].Update(action); break;
  case GLFW_KEY_8: mInputButtonList[TEnum::Num8].Update(action); break;
  case GLFW_KEY_9: mInputButtonList[TEnum::Num9].Update(action); break;
    
  case GLFW_KEY_KP_0: mInputButtonList[TEnum::NumKp0].Update(action); break;
  case GLFW_KEY_KP_1: mInputButtonList[TEnum::NumKp1].Update(action); break;
  case GLFW_KEY_KP_2: mInputButtonList[TEnum::NumKp2].Update(action); break;
  case GLFW_KEY_KP_3: mInputButtonList[TEnum::NumKp3].Update(action); break;
  case GLFW_KEY_KP_4: mInputButtonList[TEnum::NumKp4].Update(action); break;
  case GLFW_KEY_KP_5: mInputButtonList[TEnum::NumKp5].Update(action); break;
  case GLFW_KEY_KP_6: mInputButtonList[TEnum::NumKp6].Update(action); break;
  case GLFW_KEY_KP_7: mInputButtonList[TEnum::NumKp7].Update(action); break;
  case GLFW_KEY_KP_8: mInputButtonList[TEnum::NumKp8].Update(action); break;
  case GLFW_KEY_KP_9: mInputButtonList[TEnum::NumKp9].Update(action); break;

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

/// @brief Polling notification of mouse movement input.
/// @param xPos width position.
/// @param yPos height position.
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

/// @brief Polling notification of mouse button trigger input. \n
/// The action is one of `GLFW_PRESS` or `GLFW_RELEASE`.
/// @reference http://www.glfw.org/docs/latest/group__buttons.html
/// @reference http://www.glfw.org/docs/latest/group__mods.html
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
class FDyActor;

EDySuccess MDyInput::pfInitialize()
{
  this->MDY_PRIVATE(pInitializeAxisNAction)();
  this->MDY_PRIVATE(pInitializeCallbacks)();

  if (DyCheckIsJoystickConnected() == true)
  { // Check joystick binding manually at first time.
    MDY_LOG_CRITICAL("Joystick {0} Name : {1} Supported.", 0, glfwGetJoystickName(GLFW_JOYSTICK_1));
    mIsControllerConnected = true;
  }

  // Set mouse mode to normal.
  this->PushMouseMode(EDyMouseMode::Normal);
  this->PushMouseMode(EDyMouseMode::Picking);
  return DY_SUCCESS;
}

void MDyInput::MDY_PRIVATE(pInitializeAxisNAction)()
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

void MDyInput::MDY_PRIVATE(pInitializeCallbacks)()
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
  this->mBindedActionMap.clear();
  this->mBindedAxisMap.clear();
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

const DDyVector2& MDyInput::GetPresentMousePosition() const noexcept
{
  return this->mMousePresentPosition;
}

const DDyVector2& MDyInput::GetPresentLastMousePosition() const noexcept
{
  return this->mMouseLastPosition;
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
  DDyAxisBindingInformation& keyInformation = keyIt->second; 
  switch (keyInformation.mKeyStatus)
  {
  case DDyAxisBindingInformation::EDyAxisInputStatus::PositivePressed:
  case DDyAxisBindingInformation::EDyAxisInputStatus::NegativePressed:
    return true;
  default: return false;
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

EDyMouseMode MDyInput::GetMouseMode() const noexcept
{
  MDY_ASSERT(this->mPresentMouseMode.empty() == false, "Unexpected error occurred.");
  return this->mPresentMouseMode.top();
}

void MDyInput::PushMouseMode(_MIN_ EDyMouseMode iMouseMode) noexcept
{
  this->mPresentMouseMode.push(iMouseMode);
}

EDyMouseMode MDyInput::PopMouseMode() noexcept
{
  const auto mouseModeValue = this->GetMouseMode();
  this->mPresentMouseMode.pop();
  return mouseModeValue;
}

bool MDyInput::IsKeyPressed(_MIN_ EDyInputButton keyValue) const noexcept
{
  return mInputButtonList[keyValue].Get() == EDyInputButtonStatus::Pressed; 
}

void MDyInput::pfInGameUpdate(_MIN_ TF32 dt) noexcept
{
  this->MDY_PRIVATE(pUpdateMouseMovement)(dt);
  if (this->IsJoystickConnected() == true)
  { // If joystick is connected, update values because GLFW 3.2.1 does not have callback for joystick.
    this->MDY_PRIVATE(pUpdateJoystickSticks)();
    this->MDY_PRIVATE(pUpdateJoystickButtons)();
  }

  this->MDY_PRIVATE(pCheckAxisStatus)(dt);
  this->MDY_PRIVATE(pCheckActionStatus)(dt);

  this->mDelegateManger.CheckDelegateAxis(dt);
  this->mDelegateManger.CheckDelegateAction(dt);
}

void MDyInput::pfGlobalUpdate(_MIN_ TF32 dt) noexcept
{
  // DO NOTHING.
}

void MDyInput::MDY_PRIVATE(pUpdateJoystickSticks)()
{
  int supportingStickCount;
  const float* stickValueList = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &supportingStickCount);

  const auto stickCount = 
      static_cast<TU32>(supportingStickCount) < kMaximumStickCount 
    ? supportingStickCount 
    : kMaximumStickCount;
  for (TU32 i = 0; i < stickCount; ++i)
  {
    mInputAnalogStickList[i].Update(stickValueList[i]);
  }
}

void MDyInput::MDY_PRIVATE(pUpdateJoystickButtons)()
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

void MDyInput::MDY_PRIVATE(pCheckAxisStatus)(_MIN_ TF32 dt)
{
  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckAxisStatusIfAny = [](_MINOUT_ DDyAxisBindingInformation& axisInfo, _MIN_ bool isPositive, _MIN_ EDyInputButtonStatus goalState)
  {
    return std::any_of(
        MDY_BIND_CBEGIN_CEND((isPositive == true ? axisInfo.mPositiveButtonId : axisInfo.mNegativeButtonId)),
        [goalState](_MIN_ const auto& id) { return mInputButtonList[id].Get() == goalState; }
    );
  };

  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckAxisStatusIfAll = [](_MINOUT_ DDyAxisBindingInformation& axisInfo, _MIN_ bool isPositive, _MIN_ EDyInputButtonStatus goalState)
  {
    return std::all_of(
        MDY_BIND_CBEGIN_CEND((isPositive == true ? axisInfo.mPositiveButtonId : axisInfo.mNegativeButtonId)),
        [goalState](_MIN_ const auto& id) { return mInputButtonList[id].Get() == goalState; }
    );
  };

  /// @brief Process axis update routine when axisInfo status is `Status::CommonNeutral`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_CommonNeutral = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  {
    if (CheckAxisStatusIfAny(axisInfo, false, EDyInputButtonStatus::Pressed) == true)
    { // Negative
      axisInfo.mAxisValue = kNegativeValue;
      axisInfo.mKeyStatus = EDyInputAxisStatus::NegativePressed;
      return;
    }
    if (CheckAxisStatusIfAny(axisInfo, true, EDyInputButtonStatus::Pressed) == true)
    { // Positive
      axisInfo.mAxisValue = kPositiveValue;
      axisInfo.mKeyStatus = EDyInputAxisStatus::PositivePressed;
    }
  };

  /// @brief Process axis update routine when axisInfo status is `Status::NegativePressed`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_NegativePressed = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { 
    // Negative
    if (CheckAxisStatusIfAll(axisInfo, false, EDyInputButtonStatus::Released) == false) { return; } 
    if (CheckAxisStatusIfAll(axisInfo, true, EDyInputButtonStatus::Released) == false)
    { // Positive
      axisInfo.mAxisValue = kPositiveValue;
      axisInfo.mKeyStatus = EDyInputAxisStatus::PositivePressed;
    }
    else { axisInfo.mKeyStatus = EDyInputAxisStatus::CommonReleased; }
  };

  /// @brief Process axis update routine when axisInfo status is `Status::NegativePressed`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_PositivePressed = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  { // Positive
    if (CheckAxisStatusIfAll(axisInfo, true, EDyInputButtonStatus::Released) == false) { return; }
    if (CheckAxisStatusIfAll(axisInfo, false, EDyInputButtonStatus::Released) == false)
    { // Negative
      axisInfo.mAxisValue = kPositiveValue;
      axisInfo.mKeyStatus = EDyInputAxisStatus::NegativePressed;
    }
    else { axisInfo.mKeyStatus = EDyInputAxisStatus::CommonReleased; }
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FUNCTION BODY ∨
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (auto& [_, axis] : this->mBindedAxisMap)
  { // AXIS
    switch (axis.mKeyStatus)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyInputAxisStatus::CommonReleased:
      DyProceedAxisGravity(axis);
      [[fallthrough]];
    case EDyInputAxisStatus::CommonNeutral:   ProcessAxis_CommonNeutral(axis);   break;
    case EDyInputAxisStatus::NegativePressed: ProcessAxis_NegativePressed(axis); break;
    case EDyInputAxisStatus::PositivePressed: ProcessAxis_PositivePressed(axis); break;
    }
  }
}

void MDyInput::MDY_PRIVATE(pCheckActionStatus)(_MIN_ TF32 dt)
{
  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckActionStatusIfAny = [](_MINOUT_ DDyActionBindingInformation& actionInfo, _MIN_ EDyInputButtonStatus goalState)
  {
    return std::any_of(
        MDY_BIND_CBEGIN_CEND(actionInfo.mActionId),
        [goalState](_MIN_ const auto& id) { return mInputButtonList[id].Get() == goalState; }
    );
  };

  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckActionStatusIfAll = [](_MINOUT_ DDyActionBindingInformation& actionInfo, _MIN_ EDyInputButtonStatus goalState)
  {
    return std::all_of(
        MDY_BIND_CBEGIN_CEND(actionInfo.mActionId),
        [goalState](_MIN_ const auto& id) { return mInputButtonList[id].Get() == goalState; }
    );
  };

  /// @brief Process action update routine when actionInfo status is `Status::Released`.
  /// @param actionInfo action information
  static auto ProcessAction_Released = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    if (CheckActionStatusIfAny(actionInfo, EDyInputButtonStatus::Pressed) == true) 
    { actionInfo.mKeyStatus = EDyInputActionStatus::Pressed; }
  };

  /// @brief Process action update routine when actionInfo status is `Status::Pressed`.
  /// @param actionInfo action information
  static auto ProcessAction_Pressed = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    if (CheckActionStatusIfAll(actionInfo, EDyInputButtonStatus::Released) == true) 
    { actionInfo.mKeyStatus = EDyInputActionStatus::Released; }
    else
    { actionInfo.mKeyStatus = EDyInputActionStatus::Bottled; }
  };

  /// @brief Process action update routine when actionInfo status is `Status::Bottled`.
  /// @param actionInfo action information
  static auto ProcessAction_Bottled = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    if (CheckActionStatusIfAll(actionInfo, EDyInputButtonStatus::Released) == true) 
    { actionInfo.mKeyStatus = EDyInputActionStatus::Released; }
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FUNCTION BODY ∨
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (auto& [_, action] : this->mBindedActionMap)
  { // ACTION
    switch (action.mKeyStatus)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyInputActionStatus::Released: ProcessAction_Released(action);  break;
    case EDyInputActionStatus::Pressed:  ProcessAction_Pressed(action);   break;
    case EDyInputActionStatus::Bottled:  ProcessAction_Bottled(action);   break;
    }
  }
}

void MDyInput::MDY_PRIVATE(pUpdateMouseMovement)(_MIN_ TF32 dt)
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

EDySuccess MDyInput::MDY_PRIVATE(TryRequireControllerUi)(_MIN_ ADyWidgetCppScript& iRefUiScript) noexcept
{
  return this->mDelegateManger.TryRequireControllerUi(iRefUiScript);
}

EDySuccess MDyInput::MDY_PRIVATE(TryDetachContollerUi)(_MIN_ ADyWidgetCppScript& iRefUiScript) noexcept
{
  return this->mDelegateManger.TryDetachContollerUi(iRefUiScript);
}

EDySuccess MDyInput::MDY_PRIVATE(TryBindAxisDelegate)(
    _MIN_ ADyWidgetCppScript& iRefUiScript, 
    _MIN_ std::function<void(TF32)> iFunction,
    _MIN_ const std::string& iAxisName)
{
  if (this->mDelegateManger.GetPtrUiScriptOnBinding() != &iRefUiScript)
  { // Check ui is binding to delegate now. If not matched, just return DY_FAILURE with error log.
    MDY_LOG_ERROR("Failed to binding axis function of UI script. Instance reference did not match.");
    return DY_FAILURE;
  }

  if (this->IsAxisExist(iAxisName) == false)
  { // Check `Axis` is exist. if not, return DY_FAILURE.
    MDY_LOG_ERROR("Failed to binding axis function of UI script. Axis `{}` does not exist.", iAxisName);
    return DY_FAILURE;
  }

  this->mDelegateManger.BindAxisDelegateUi(iFunction, this->mBindedAxisMap.at(iAxisName));
  return DY_SUCCESS;
}

EDySuccess MDyInput::MDY_PRIVATE(TryBindActionDelegate)(
    _MIN_ ADyWidgetCppScript& iRefUiScript, 
    _MIN_ EDyInputActionStatus iCondition,
    _MIN_ std::function<void()> iFunction,
    _MIN_ const std::string& iActionName)
{
  if (this->mDelegateManger.GetPtrUiScriptOnBinding() != &iRefUiScript)
  { // Check ui is binding to delegate now. If not matched, just return DY_FAILURE with error log.
    MDY_LOG_ERROR("Failed to binding action function of UI script. Instance reference did not match.");
    return DY_FAILURE;
  }

  if (this->IsActionExist(iActionName) == false)
  { // Check `Action` is exist. if not, return DY_FAILURE.
    MDY_LOG_ERROR("Failed to binding action function of UI script. Action `{}` does not exist.", iActionName);
    return DY_FAILURE;
  }

  this->mDelegateManger.BindActionDelegateUi(iFunction, iCondition, this->mBindedActionMap.at(iActionName));
  return DY_SUCCESS;
}

EDySuccess MDyInput::MDY_PRIVATE(TryBindAxisDelegate)(
    _MIN_ ADyActorCppScript& iRefUiScript, 
    _MIN_ std::function<void(TF32)> iFunction,
    _MIN_ const std::string& iAxisName)
{
  if (this->mDelegateManger.GetPtrActorScriptOnBinding() != &iRefUiScript)
  { // Check actor is binding to delegate now. If not matched, just return DY_FAILURE with error log.
    MDY_LOG_ERROR("Failed to binding axis function of Actor script. Instance reference did not match.");
    return DY_FAILURE;
  }

  if (this->IsAxisExist(iAxisName) == false)
  { // Check `Axis` is exist. if not, return DY_FAILURE.
    MDY_LOG_ERROR("Failed to binding axis function of Actor script. Axis `{}` does not exist.", iAxisName);
    return DY_FAILURE;
  }

  this->mDelegateManger.BindAxisDelegateActor(iFunction, this->mBindedAxisMap.at(iAxisName));
  return DY_SUCCESS;
}

EDySuccess MDyInput::MDY_PRIVATE(TryBindActionDelegate)(
    _MIN_ ADyActorCppScript& iRefUiScript, 
    _MIN_ EDyInputActionStatus iCondition,
    _MIN_ std::function<void()> iFunction, 
    _MIN_ const std::string& iActionName)
{
  if (this->mDelegateManger.GetPtrActorScriptOnBinding() != &iRefUiScript)
  { // Check Actor is binding to delegate now. If not matched, just return DY_FAILURE with error log.
    MDY_LOG_ERROR("Failed to binding action function of Actor script. Instance reference did not match.");
    return DY_FAILURE;
  }

  if (this->IsActionExist(iActionName) == false)
  { // Check `Action` is exist. if not, return DY_FAILURE.
    MDY_LOG_ERROR("Failed to binding action function of Actor script. Action `{}` does not exist.", iActionName);
    return DY_FAILURE;
  }

  this->mDelegateManger.BindActionDelegateActor(iFunction, iCondition, this->mBindedActionMap.at(iActionName));
  return DY_SUCCESS;
}

EDySuccess MDyInput::MDY_PRIVATE(TryRequireControllerActor)(_MIN_ ADyActorCppScript& iRefActor) noexcept
{
  return this->mDelegateManger.TryRequireControllerActor(iRefActor);
}

EDySuccess MDyInput::MDY_PRIVATE(TryDetachContollerActor)(_MIN_ ADyActorCppScript& iRefActor) noexcept
{
  return this->mDelegateManger.TryDetachContollerActor(iRefActor);
}

EDyInputButtonStatus MDyInput::MDY_PRIVATE(GetLowlevelKeyStatus)(_MIN_ EDyButton iId) noexcept
{
  return mInputButtonList[iId].Get();
}

EDySuccess MDyInput::TryPickObject(_MIN_ const DDyVector2& iScreenPosition)
{
  // Get position of present frame.
  const auto& position = this->mMousePresentPosition;
  
  // Get actor id from rendering manager.
  auto* ptrFramebuffer = MDyIOResource::GetInstance().
      GetPtrInformation<EDyResourceType::GLFrameBuffer>("dyBtBasicRender");
  DDyPixelInfo pixel;
  pixel.ObjectID = TActorIdContainer::kExclusiveId;

  // Critical section of getting object id information from attachment.
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ptrFramebuffer->GetFrameBufferId());
    glReadBuffer(GL_COLOR_ATTACHMENT4);

    const auto& framebufferSize = ptrFramebuffer->GetFrameBufferSize();
    glReadPixels(position.X, framebufferSize.Y - position.Y, 1, 1, GL_RGB, GL_FLOAT, &pixel);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  }

  // Check whether this is successful of getting object id from framebuffer attachment.
  if (static_cast<TU32>(pixel.ObjectID) != TActorIdContainer::kExclusiveId)
  {
    // If successful, try to bind object into binder of this.
    auto& refWorld = MDyWorld::GetInstance();
    auto* ptrActor = refWorld.GetActorWithObjectId(static_cast<TU32>(pixel.ObjectID));
    if (ptrActor == nullptr)
    { /* Do nothing if not exist */
      return DY_FAILURE;
    }
    else
    {
      switch (this->GetMouseMode())
      {
      case EDyMouseMode::Normal: 
      {
        // If normal mode, when picking callback function is exist, 
        // call callback function with argument.
        if (this->mActorPickingCallback != nullptr)
        {
          this->mActorPickingCallback(this->mPtrActorPickingTarget);
        }
      } break;
      case EDyMouseMode::Picking: 
      {
        // If picking mode, do fixed routine.
        if (this->mPtrActorPickingTarget != nullptr)
        {
          this->mPtrActorPickingTarget->MDY_PRIVATE(DetachPickingTargetFromSystem)();
        }
        // Bind ptrActor as Binder type.
        MDY_LOG_INFO_D("Picked {}", ptrActor->ToString());
        ptrActor->MDY_PRIVATE(AttachPickingTargetFromSystem)(&this->mPtrActorPickingTarget);
      } break;
      }
    }
  }

  return DY_SUCCESS;
}

void MDyInput::SetPickingTargetCallbackFunction(_MIN_ TPickingCallbackFunction iPtrGlobalFunction)
{
  this->mActorPickingCallback = iPtrGlobalFunction;
}

void MDyInput::ResetPickingTargetCallback() noexcept
{
  mActorPickingCallback = nullptr;
}

FDyActor** MDyInput::MDY_PRIVATE(GetPPtrPickingTarget)() noexcept
{
  return &this->mPtrActorPickingTarget;
}

} /// ::dy namespace