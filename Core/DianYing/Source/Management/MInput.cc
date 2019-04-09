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
#include <Dy/Management/MInput.h>

#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/MTime.h>
#include <Dy/Management/MWindow.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/Type/Input/DAnalogInputItem.h>
#include <Dy/Management/Type/Render/DDyPixelInfo.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/GEngine.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Element/Internal/TIdDistributor.h>
#include <Dy/Element/FActor.h>

#include <Dy/Include/GlInclude.h>
#include <XInputEntry.h>

#ifdef DELETE
#undef DELETE
#endif

//!
//! Data
//!

namespace
{

dy::base::EInputState GetStatusFromGLFWEnum(int iLowLevelStatus) noexcept
{
  switch (iLowLevelStatus)
  {
  case GLFW_PRESS:    return dy::base::EInputState::Pressed;
  case GLFW_RELEASE:  return dy::base::EInputState::Released;
  case GLFW_REPEAT:   return dy::base::EInputState::Repeated;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(dy::base::EInputState::Released);
  }
};

constexpr TF32 kNegativeValue = -1.0f;
constexpr TF32 kPositiveValue = +1.0f;
constexpr TU32 kMaximumStickCount = 6;

//std::array<dy::DAnalogInputItem, kMaximumStickCount> mInputAnalogStickList = {};

dy::DVec2    sMouseLastPosition    = {};
dy::DVec2    sMousePresentPosition = {};
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
void DyCallbackInputKeyboard(MDY_NOTUSED GLFWwindow* window, _MIN_ int key, MDY_NOTUSED int scancode, _MIN_ int iAction, MDY_NOTUSED int mod)
{
  using namespace dy;
  using TEnum = EDyInputButton;
  const auto action = GetStatusFromGLFWEnum(iAction);
  
  switch (key)
  {
  case GLFW_KEY_RIGHT: ::dy::base::sLowLevelInputs[TEnum::Right].Update(action); break;
  case GLFW_KEY_LEFT: ::dy::base::sLowLevelInputs[TEnum::Left].Update(action); break;
  case GLFW_KEY_DOWN: ::dy::base::sLowLevelInputs[TEnum::Down].Update(action); break;
  case GLFW_KEY_UP:   ::dy::base::sLowLevelInputs[TEnum::Up].Update(action); break;

  case GLFW_KEY_A: ::dy::base::sLowLevelInputs[TEnum::A].Update(action); break;
  case GLFW_KEY_B: ::dy::base::sLowLevelInputs[TEnum::B].Update(action); break;
  case GLFW_KEY_C: ::dy::base::sLowLevelInputs[TEnum::C].Update(action); break;
  case GLFW_KEY_D: ::dy::base::sLowLevelInputs[TEnum::D].Update(action); break;
  case GLFW_KEY_E: ::dy::base::sLowLevelInputs[TEnum::E].Update(action); break;
  case GLFW_KEY_F: ::dy::base::sLowLevelInputs[TEnum::F].Update(action); break;
  case GLFW_KEY_G: ::dy::base::sLowLevelInputs[TEnum::G].Update(action); break;
  case GLFW_KEY_H: ::dy::base::sLowLevelInputs[TEnum::H].Update(action); break;
  case GLFW_KEY_I: ::dy::base::sLowLevelInputs[TEnum::I].Update(action); break;
  case GLFW_KEY_J: ::dy::base::sLowLevelInputs[TEnum::J].Update(action); break;
  case GLFW_KEY_K: ::dy::base::sLowLevelInputs[TEnum::K].Update(action); break;
  case GLFW_KEY_L: ::dy::base::sLowLevelInputs[TEnum::L].Update(action); break;
  case GLFW_KEY_M: ::dy::base::sLowLevelInputs[TEnum::M].Update(action); break;
  case GLFW_KEY_N: ::dy::base::sLowLevelInputs[TEnum::N].Update(action); break;
  case GLFW_KEY_O: ::dy::base::sLowLevelInputs[TEnum::O].Update(action); break;
  case GLFW_KEY_P: ::dy::base::sLowLevelInputs[TEnum::P].Update(action); break;
  case GLFW_KEY_Q: ::dy::base::sLowLevelInputs[TEnum::Q].Update(action); break;
  case GLFW_KEY_R: ::dy::base::sLowLevelInputs[TEnum::R].Update(action); break;
  case GLFW_KEY_S: ::dy::base::sLowLevelInputs[TEnum::S].Update(action); break;
  case GLFW_KEY_T: ::dy::base::sLowLevelInputs[TEnum::T].Update(action); break;
  case GLFW_KEY_U: ::dy::base::sLowLevelInputs[TEnum::U].Update(action); break;
  case GLFW_KEY_V: ::dy::base::sLowLevelInputs[TEnum::V].Update(action); break;
  case GLFW_KEY_X: ::dy::base::sLowLevelInputs[TEnum::X].Update(action); break;
  case GLFW_KEY_Y: ::dy::base::sLowLevelInputs[TEnum::Y].Update(action); break;
  case GLFW_KEY_Z: ::dy::base::sLowLevelInputs[TEnum::Z].Update(action); break;

  case GLFW_KEY_0: ::dy::base::sLowLevelInputs[TEnum::Num0].Update(action); break;
  case GLFW_KEY_1: ::dy::base::sLowLevelInputs[TEnum::Num1].Update(action); break;
  case GLFW_KEY_2: ::dy::base::sLowLevelInputs[TEnum::Num2].Update(action); break;
  case GLFW_KEY_3: ::dy::base::sLowLevelInputs[TEnum::Num3].Update(action); break;
  case GLFW_KEY_4: ::dy::base::sLowLevelInputs[TEnum::Num4].Update(action); break;
  case GLFW_KEY_5: ::dy::base::sLowLevelInputs[TEnum::Num5].Update(action); break;
  case GLFW_KEY_6: ::dy::base::sLowLevelInputs[TEnum::Num6].Update(action); break;
  case GLFW_KEY_7: ::dy::base::sLowLevelInputs[TEnum::Num7].Update(action); break;
  case GLFW_KEY_8: ::dy::base::sLowLevelInputs[TEnum::Num8].Update(action); break;
  case GLFW_KEY_9: ::dy::base::sLowLevelInputs[TEnum::Num9].Update(action); break;
    
  case GLFW_KEY_KP_0: ::dy::base::sLowLevelInputs[TEnum::NumKp0].Update(action); break;
  case GLFW_KEY_KP_1: ::dy::base::sLowLevelInputs[TEnum::NumKp1].Update(action); break;
  case GLFW_KEY_KP_2: ::dy::base::sLowLevelInputs[TEnum::NumKp2].Update(action); break;
  case GLFW_KEY_KP_3: ::dy::base::sLowLevelInputs[TEnum::NumKp3].Update(action); break;
  case GLFW_KEY_KP_4: ::dy::base::sLowLevelInputs[TEnum::NumKp4].Update(action); break;
  case GLFW_KEY_KP_5: ::dy::base::sLowLevelInputs[TEnum::NumKp5].Update(action); break;
  case GLFW_KEY_KP_6: ::dy::base::sLowLevelInputs[TEnum::NumKp6].Update(action); break;
  case GLFW_KEY_KP_7: ::dy::base::sLowLevelInputs[TEnum::NumKp7].Update(action); break;
  case GLFW_KEY_KP_8: ::dy::base::sLowLevelInputs[TEnum::NumKp8].Update(action); break;
  case GLFW_KEY_KP_9: ::dy::base::sLowLevelInputs[TEnum::NumKp9].Update(action); break;

  case GLFW_KEY_ESCAPE:       ::dy::base::sLowLevelInputs[TEnum::ESCAPE].Update(action); break;
  case GLFW_KEY_ENTER:        ::dy::base::sLowLevelInputs[TEnum::ENTER].Update(action); break;
  case GLFW_KEY_SPACE:        ::dy::base::sLowLevelInputs[TEnum::SPACE].Update(action); break;
  case GLFW_KEY_LEFT_SHIFT:   ::dy::base::sLowLevelInputs[TEnum::LSHIFT].Update(action); break;
  case GLFW_KEY_LEFT_CONTROL: ::dy::base::sLowLevelInputs[TEnum::LCTRL].Update(action); break;
  case GLFW_KEY_LEFT_ALT:     ::dy::base::sLowLevelInputs[TEnum::LALT].Update(action); break;

  case GLFW_KEY_INSERT:   ::dy::base::sLowLevelInputs[TEnum::INSERT].Update(action); break;
  case GLFW_KEY_DELETE:   ::dy::base::sLowLevelInputs[TEnum::DELETE].Update(action); break;
  case GLFW_KEY_PAGE_UP:  ::dy::base::sLowLevelInputs[TEnum::PAGEUP].Update(action); break;
  case GLFW_KEY_PAGE_DOWN: ::dy::base::sLowLevelInputs[TEnum::PAGEDOWN].Update(action); break;
  case GLFW_KEY_HOME:     ::dy::base::sLowLevelInputs[TEnum::HOME].Update(action); break;
  case GLFW_KEY_END:      ::dy::base::sLowLevelInputs[TEnum::END].Update(action); break;

  case GLFW_KEY_MINUS:          ::dy::base::sLowLevelInputs[TEnum::MINUS].Update(action); break;
  case GLFW_KEY_EQUAL:          ::dy::base::sLowLevelInputs[TEnum::EQUAL].Update(action); break;
  case GLFW_KEY_LEFT_BRACKET:   ::dy::base::sLowLevelInputs[TEnum::LBRACKET].Update(action); break;
  case GLFW_KEY_RIGHT_BRACKET:  ::dy::base::sLowLevelInputs[TEnum::RBRACKET].Update(action); break;

  case GLFW_KEY_SEMICOLON:  ::dy::base::sLowLevelInputs[TEnum::SEMICOLON].Update(action); break;
  case GLFW_KEY_APOSTROPHE: ::dy::base::sLowLevelInputs[TEnum::APOSTROPHE].Update(action); break;
  case GLFW_KEY_COMMA:      ::dy::base::sLowLevelInputs[TEnum::COMMA].Update(action); break;
  case GLFW_KEY_PERIOD:     ::dy::base::sLowLevelInputs[TEnum::PERIOD].Update(action); break;

  case GLFW_KEY_F1:  ::dy::base::sLowLevelInputs[TEnum::F1].Update(action); break;
  case GLFW_KEY_F2:  ::dy::base::sLowLevelInputs[TEnum::F2].Update(action); break;
  case GLFW_KEY_F3:  ::dy::base::sLowLevelInputs[TEnum::F3].Update(action); break;
  case GLFW_KEY_F4:  ::dy::base::sLowLevelInputs[TEnum::F4].Update(action); break;
  case GLFW_KEY_F5:  ::dy::base::sLowLevelInputs[TEnum::F5].Update(action); break;
  case GLFW_KEY_F6:  ::dy::base::sLowLevelInputs[TEnum::F6].Update(action); break;
  case GLFW_KEY_F7:  ::dy::base::sLowLevelInputs[TEnum::F7].Update(action); break;
  case GLFW_KEY_F8:  ::dy::base::sLowLevelInputs[TEnum::F8].Update(action); break;
  case GLFW_KEY_F9:  ::dy::base::sLowLevelInputs[TEnum::F9].Update(action); break;
  case GLFW_KEY_F10: ::dy::base::sLowLevelInputs[TEnum::F10].Update(action); break;
  case GLFW_KEY_F11: ::dy::base::sLowLevelInputs[TEnum::F11].Update(action); break;
  case GLFW_KEY_F12: ::dy::base::sLowLevelInputs[TEnum::F12].Update(action); break;

  case GLFW_KEY_CAPS_LOCK:    ::dy::base::sLowLevelInputs[TEnum::CAPSLOCK].Update(action); break;
  case GLFW_KEY_TAB:          ::dy::base::sLowLevelInputs[TEnum::TAB].Update(action); break;
  case GLFW_KEY_BACKSPACE:    ::dy::base::sLowLevelInputs[TEnum::BACKSPACE].Update(action); break;
  case GLFW_KEY_BACKSLASH:    ::dy::base::sLowLevelInputs[TEnum::BACKSLASH].Update(action); break;
  case GLFW_KEY_GRAVE_ACCENT: ::dy::base::sLowLevelInputs[TEnum::GBACCENT].Update(action); break;
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
void DyCallbackMouseInput(MDY_NOTUSED GLFWwindow* window, _MIN_ int button, _MIN_ int iAction, MDY_NOTUSED int mods)
{
  using namespace dy;
  using TEnum = EDyInputButton;
  const auto action = GetStatusFromGLFWEnum(iAction);

  switch (button)
  {
  case GLFW_MOUSE_BUTTON_1: ::dy::base::sLowLevelInputs[TEnum::Mouse0Lmb].Update(action); break;
  case GLFW_MOUSE_BUTTON_2: ::dy::base::sLowLevelInputs[TEnum::Mouse1Rmb].Update(action); break;
  case GLFW_MOUSE_BUTTON_3: ::dy::base::sLowLevelInputs[TEnum::Mouse2Mid].Update(action); break;

  case GLFW_MOUSE_BUTTON_4: ::dy::base::sLowLevelInputs[TEnum::Mouse3].Update(action); break;
  case GLFW_MOUSE_BUTTON_5: ::dy::base::sLowLevelInputs[TEnum::Mouse4].Update(action); break;
  case GLFW_MOUSE_BUTTON_6: ::dy::base::sLowLevelInputs[TEnum::Mouse5].Update(action); break;
  case GLFW_MOUSE_BUTTON_7: ::dy::base::sLowLevelInputs[TEnum::Mouse6].Update(action); break;
  case GLFW_MOUSE_BUTTON_8: ::dy::base::sLowLevelInputs[TEnum::Mouse7].Update(action); break;
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
  DyPushLogDebugDebug("Mouse button scrolling : X: {0}, Y: {1}", xoffset, yoffset);
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
    DyPushLogCritical("Joystick {0} Name : {1} Supported.", 0, glfwGetJoystickName(GLFW_JOYSTICK_1));
    mIsControllerConnected = true;
  }
  else if (event == GLFW_DISCONNECTED)
  {
    DyPushLogCritical("Joystick {0} Disconnected.", 0);
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
	const auto dt = dy::MTime::GetInstance().GetGameScaledTickedDeltaTimeValue();
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
class FActor;

EDySuccess MInput::pfInitialize()
{
  this->MDY_PRIVATE(pInitializeAxisNAction)();
  this->MDY_PRIVATE(pInitializeCallbacks)();

  if (DyCheckIsJoystickConnected() == true)
  { // Check joystick binding manually at first time.
    DyPushLogCritical("Joystick {0} Name : {1} Supported.", 0, glfwGetJoystickName(GLFW_JOYSTICK_1));
    mIsControllerConnected = true;
  }

  // Set mouse mode to normal.
  this->PushMouseMode(EDyMouseMode::Normal);
  this->PushMouseMode(EDyMouseMode::Picking);
  return EDySuccess::DY_SUCCESS;
}

void MInput::MDY_PRIVATE(pInitializeAxisNAction)()
{
   const auto& keyInformation = MSetting::GetInstance().GetInputSettingInformation();

  // AXIS MAP
  for (const auto& [specifierName, info] : keyInformation.mAxisMap)
  {
    MDY_ASSERT_MSG(this->IsAxisExist(specifierName) == false, "Duplicated axis-key specifier name is already binded.");
    auto [_, isSucceeded] = this->mBindedAxisMap.try_emplace(specifierName, info);
    MDY_ASSERT_MSG(isSucceeded == true, "");
  }

  // ACTION MAP
  for (const auto& [specifierName, info] : keyInformation.mActionMap)
  {
    MDY_ASSERT_MSG(this->IsActionExist(specifierName) == false, "Duplicated action-key specifier name is already binded.");
    auto [_, isSucceeded] = this->mBindedActionMap.try_emplace(specifierName, info);
    MDY_ASSERT_MSG(isSucceeded == true, "");
  }
}

void MInput::MDY_PRIVATE(pInitializeCallbacks)()
{
  this->mPtrGlfwWindowContext = MWindow::GetInstance().GetGLMainWindow();
  glfwSetKeyCallback        (this->mPtrGlfwWindowContext, DyCallbackInputKeyboard);
  glfwSetCursorPosCallback  (this->mPtrGlfwWindowContext, DyCallbackMouseMoving);
  glfwSetMouseButtonCallback(this->mPtrGlfwWindowContext, DyCallbackMouseInput);
  glfwSetScrollCallback     (this->mPtrGlfwWindowContext, DyCallbackMouseScroll);
  glfwSetInputMode          (this->mPtrGlfwWindowContext, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetJoystickCallback   (DyCallbackCheckJoystickConnection);
}

EDySuccess MInput::pfRelease()
{
  this->mBindedActionMap.clear();
  this->mBindedAxisMap.clear();
  return EDySuccess::DY_SUCCESS;
}

float MInput::GetAxisValue(_MIN_ const std::string& axisKeyName) noexcept
{
  // Validity test
  const auto keyIt = this->mBindedAxisMap.find(axisKeyName);
  if (keyIt == this->mBindedAxisMap.end())
  {
    DyPushLogDebugError("Failed to find {}.", axisKeyName);
		return 0.f;
	}

  return keyIt->second.mAxisValue;
}

const DVec2& MInput::GetPresentMousePosition() const noexcept
{
  return this->mMousePresentPosition;
}

const DVec2& MInput::GetPresentLastMousePosition() const noexcept
{
  return this->mMouseLastPosition;
}

TF32 MInput::GetJoystickStickValue(_MIN_ DClamp<TU32, 0, 5> index) const noexcept
{
  return ::dy::base::sLowLevelAnalogs[index].GetValue();
}

EInputButtonStatus MInput::GetButtonStatusValue(_MIN_ EDyButton button) const noexcept
{
  MDY_ASSERT_MSG(button != EDyButton::NoneError, "Button value must not be `NoneErorr`.");
  return ::dy::base::sLowLevelInputs[button].Get();
}

bool MInput::IsAxisPressed(_MIN_ const std::string& axisSpecifierName) noexcept
{
  // Validity test
  const auto keyIt = this->mBindedAxisMap.find(axisSpecifierName);
  if (keyIt == mBindedAxisMap.end())
  {
    DyPushLogDebugError("Failed to find {}.", axisSpecifierName);
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

bool MInput::IsAxisReleased(_MIN_ const std::string& axisSpecifierName) noexcept
{
  // Validity test
  const auto keyIt = this->mBindedAxisMap.find(axisSpecifierName);
  if (keyIt == mBindedAxisMap.end())
  {
    DyPushLogDebugError("Failed to find {}.", axisSpecifierName);
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

bool MInput::IsAxisExist(_MIN_ const std::string& axisSpecifierName) const noexcept
{
  return Contains(this->mBindedAxisMap, axisSpecifierName);
}

bool MInput::IsActionPressed(_MIN_ const std::string& actionSpecifier) const noexcept
{
  // Validity test
  const auto keyIt = this->mBindedActionMap.find(actionSpecifier);
  if (keyIt == mBindedActionMap.end())
  {
    DyPushLogDebugError("Failed to find {}.", actionSpecifier);
		return false;
  }

  return keyIt->second.mKeyStatus == DDyActionBindingInformation::EDyActionInputStatus::Pressed;
}

bool MInput::IsActionReleased(_MIN_ const std::string& actionSpecifier) const noexcept
{
  // Validity test
  const auto keyIt = this->mBindedActionMap.find(actionSpecifier);
  if (keyIt == mBindedActionMap.end())
  {
    DyPushLogDebugError(err_input_key_not_exist, actionSpecifier);
		return false;
  }

  return keyIt->second.mKeyStatus == DDyActionBindingInformation::EDyActionInputStatus::Released;
}

bool MInput::IsActionExist(_MIN_ const std::string& actionSpecifier) const noexcept
{
  return Contains(this->mBindedActionMap, actionSpecifier);
}

bool MInput::IsJoystickConnected() const noexcept
{
  return mIsControllerConnected;
}

EDyMouseMode MInput::GetMouseMode() const noexcept
{
  MDY_ASSERT_MSG(this->mPresentMouseMode.empty() == false, "Unexpected error occurred.");
  return this->mPresentMouseMode.top();
}

void MInput::PushMouseMode(_MIN_ EDyMouseMode iMouseMode) noexcept
{
  this->mPresentMouseMode.push(iMouseMode);
}

EDyMouseMode MInput::PopMouseMode() noexcept
{
  const auto mouseModeValue = this->GetMouseMode();
  this->mPresentMouseMode.pop();
  return mouseModeValue;
}

bool MInput::IsKeyPressed(_MIN_ EDyInputButton keyValue) const noexcept
{
  return ::dy::base::sLowLevelInputs[keyValue].Get() == EInputButtonStatus::Pressed; 
}

void MInput::pfInGameUpdate(_MIN_ TF32 dt) noexcept
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

void MInput::pfGlobalUpdate(_MIN_ TF32 dt) noexcept
{
  // DO NOTHING.
}

void MInput::MDY_PRIVATE(pUpdateJoystickSticks)()
{
  int supportingStickCount;
  const float* stickValueList = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &supportingStickCount);

  const auto stickCount = 
      static_cast<TU32>(supportingStickCount) < kMaximumStickCount 
    ? supportingStickCount 
    : kMaximumStickCount;
  for (TU32 i = 0; i < stickCount; ++i)
  {
    base::sLowLevelAnalogs[i].Update(stickValueList[i]);
  }
}

void MInput::MDY_PRIVATE(pUpdateJoystickButtons)()
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
    const auto state = GetStatusFromGLFWEnum(actionList[i]);
    ::dy::base::sLowLevelInputs[TEnum::Joystick0 + i].Update(state);
  }
}

void MInput::MDY_PRIVATE(pCheckAxisStatus)(_MIN_ TF32 dt)
{
  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckAxisStatusIfAny = [](_MINOUT_ DDyAxisBindingInformation& axisInfo, _MIN_ bool isPositive, _MIN_ EInputButtonStatus goalState)
  {
    return std::any_of(
        MDY_BIND_CBEGIN_CEND((isPositive == true ? axisInfo.mPositiveButtonId : axisInfo.mNegativeButtonId)),
        [goalState](_MIN_ const auto& id) { return ::dy::base::sLowLevelInputs[id].Get() == goalState; }
    );
  };

  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckAxisStatusIfAll = [](_MINOUT_ DDyAxisBindingInformation& axisInfo, _MIN_ bool isPositive, _MIN_ EInputButtonStatus goalState)
  {
    return std::all_of(
        MDY_BIND_CBEGIN_CEND((isPositive == true ? axisInfo.mPositiveButtonId : axisInfo.mNegativeButtonId)),
        [goalState](_MIN_ const auto& id) { return ::dy::base::sLowLevelInputs[id].Get() == goalState; }
    );
  };

  /// @brief Process axis update routine when axisInfo status is `Status::CommonNeutral`.
  /// @param axisInfo Axis information
  static auto ProcessAxis_CommonNeutral = [](_MINOUT_ DDyAxisBindingInformation& axisInfo)
  {
    if (CheckAxisStatusIfAny(axisInfo, false, EInputButtonStatus::Pressed) == true)
    { // Negative
      axisInfo.mAxisValue = kNegativeValue;
      axisInfo.mKeyStatus = EDyInputAxisStatus::NegativePressed;
      return;
    }
    if (CheckAxisStatusIfAny(axisInfo, true, EInputButtonStatus::Pressed) == true)
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
    if (CheckAxisStatusIfAll(axisInfo, false, EInputButtonStatus::Released) == false) { return; } 
    if (CheckAxisStatusIfAll(axisInfo, true, EInputButtonStatus::Released) == false)
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
    if (CheckAxisStatusIfAll(axisInfo, true, EInputButtonStatus::Released) == false) { return; }
    if (CheckAxisStatusIfAll(axisInfo, false, EInputButtonStatus::Released) == false)
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

void MInput::MDY_PRIVATE(pCheckActionStatus)(_MIN_ TF32 dt)
{
  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckActionStatusIfAny = [](_MINOUT_ DDyActionBindingInformation& actionInfo, _MIN_ EInputButtonStatus goalState)
  {
    return std::any_of(
        MDY_BIND_CBEGIN_CEND(actionInfo.mActionId),
        [goalState](_MIN_ const auto& id) { return ::dy::base::sLowLevelInputs[id].Get() == goalState; }
    );
  };

  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckActionStatusIfAll = [](_MINOUT_ DDyActionBindingInformation& actionInfo, _MIN_ EInputButtonStatus goalState)
  {
    return std::all_of(
        MDY_BIND_CBEGIN_CEND(actionInfo.mActionId),
        [goalState](_MIN_ const auto& id) { return ::dy::base::sLowLevelInputs[id].Get() == goalState; }
    );
  };

  /// @brief Process action update routine when actionInfo status is `Status::Released`.
  /// @param actionInfo action information
  static auto ProcessAction_Released = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    if (CheckActionStatusIfAny(actionInfo, EInputButtonStatus::Pressed) == true) 
    { actionInfo.mKeyStatus = EDyInputActionStatus::Pressed; }
  };

  /// @brief Process action update routine when actionInfo status is `Status::Pressed`.
  /// @param actionInfo action information
  static auto ProcessAction_Pressed = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    if (CheckActionStatusIfAll(actionInfo, EInputButtonStatus::Released) == true) 
    { actionInfo.mKeyStatus = EDyInputActionStatus::Released; }
    else
    { actionInfo.mKeyStatus = EDyInputActionStatus::Bottled; }
  };

  /// @brief Process action update routine when actionInfo status is `Status::Bottled`.
  /// @param actionInfo action information
  static auto ProcessAction_Bottled = [](_MINOUT_ DDyActionBindingInformation& actionInfo)
  {
    if (CheckActionStatusIfAll(actionInfo, EInputButtonStatus::Released) == true) 
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

void MInput::MDY_PRIVATE(pUpdateMouseMovement)(_MIN_ TF32 dt)
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

EDySuccess MInput::MDY_PRIVATE(TryRequireControllerUi)(_MIN_ AWidgetCppScript& iRefUiScript) noexcept
{
  return this->mDelegateManger.TryRequireControllerUi(iRefUiScript);
}

EDySuccess MInput::MDY_PRIVATE(TryDetachContollerUi)(_MIN_ AWidgetCppScript& iRefUiScript) noexcept
{
  return this->mDelegateManger.TryDetachContollerUi(iRefUiScript);
}

EDySuccess MInput::MDY_PRIVATE(TryBindAxisDelegate)(
    _MIN_ AWidgetCppScript& iRefUiScript, 
    _MIN_ std::function<void(TF32)> iFunction,
    _MIN_ const std::string& iAxisName)
{
  if (this->mDelegateManger.GetPtrUiScriptOnBinding() != &iRefUiScript)
  { // Check ui is binding to delegate now. If not matched, just return EDySuccess::DY_FAILURE with error log.
    DyPushLogError("Failed to binding axis function of UI script. Instance reference did not match.");
    return EDySuccess::DY_FAILURE;
  }

  if (this->IsAxisExist(iAxisName) == false)
  { // Check `Axis` is exist. if not, return EDySuccess::DY_FAILURE.
    DyPushLogError("Failed to binding axis function of UI script. Axis `{}` does not exist.", iAxisName);
    return EDySuccess::DY_FAILURE;
  }

  this->mDelegateManger.BindAxisDelegateUi(iFunction, this->mBindedAxisMap.at(iAxisName));
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MInput::MDY_PRIVATE(TryBindActionDelegate)(
    _MIN_ AWidgetCppScript& iRefUiScript, 
    _MIN_ EDyInputActionStatus iCondition,
    _MIN_ std::function<void()> iFunction,
    _MIN_ const std::string& iActionName)
{
  if (this->mDelegateManger.GetPtrUiScriptOnBinding() != &iRefUiScript)
  { // Check ui is binding to delegate now. If not matched, just return EDySuccess::DY_FAILURE with error log.
    DyPushLogError("Failed to binding action function of UI script. Instance reference did not match.");
    return EDySuccess::DY_FAILURE;
  }

  if (this->IsActionExist(iActionName) == false)
  { // Check `Action` is exist. if not, return EDySuccess::DY_FAILURE.
    DyPushLogError("Failed to binding action function of UI script. Action `{}` does not exist.", iActionName);
    return EDySuccess::DY_FAILURE;
  }

  this->mDelegateManger.BindActionDelegateUi(iFunction, iCondition, this->mBindedActionMap.at(iActionName));
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MInput::MDY_PRIVATE(TryBindAxisDelegate)(
    _MIN_ AActorCppScript& iRefUiScript, 
    _MIN_ std::function<void(TF32)> iFunction,
    _MIN_ const std::string& iAxisName)
{
  if (this->mDelegateManger.GetPtrActorScriptOnBinding() != &iRefUiScript)
  { // Check actor is binding to delegate now. If not matched, just return EDySuccess::DY_FAILURE with error log.
    DyPushLogError("Failed to binding axis function of Actor script. Instance reference did not match.");
    return EDySuccess::DY_FAILURE;
  }

  if (this->IsAxisExist(iAxisName) == false)
  { // Check `Axis` is exist. if not, return EDySuccess::DY_FAILURE.
    DyPushLogError("Failed to binding axis function of Actor script. Axis `{}` does not exist.", iAxisName);
    return EDySuccess::DY_FAILURE;
  }

  this->mDelegateManger.BindAxisDelegateActor(iFunction, this->mBindedAxisMap.at(iAxisName));
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MInput::MDY_PRIVATE(TryBindActionDelegate)(
    _MIN_ AActorCppScript& iRefUiScript, 
    _MIN_ EDyInputActionStatus iCondition,
    _MIN_ std::function<void()> iFunction, 
    _MIN_ const std::string& iActionName)
{
  if (this->mDelegateManger.GetPtrActorScriptOnBinding() != &iRefUiScript)
  { // Check Actor is binding to delegate now. If not matched, just return EDySuccess::DY_FAILURE with error log.
    DyPushLogError("Failed to binding action function of Actor script. Instance reference did not match.");
    return EDySuccess::DY_FAILURE;
  }

  if (this->IsActionExist(iActionName) == false)
  { // Check `Action` is exist. if not, return EDySuccess::DY_FAILURE.
    DyPushLogError("Failed to binding action function of Actor script. Action `{}` does not exist.", iActionName);
    return EDySuccess::DY_FAILURE;
  }

  this->mDelegateManger.BindActionDelegateActor(iFunction, iCondition, this->mBindedActionMap.at(iActionName));
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MInput::MDY_PRIVATE(TryRequireControllerActor)(_MIN_ AActorCppScript& iRefActor) noexcept
{
  return this->mDelegateManger.TryRequireControllerActor(iRefActor);
}

EDySuccess MInput::MDY_PRIVATE(TryDetachContollerActor)(_MIN_ AActorCppScript& iRefActor) noexcept
{
  return this->mDelegateManger.TryDetachContollerActor(iRefActor);
}

EInputButtonStatus MInput::MDY_PRIVATE(GetLowlevelKeyStatus)(_MIN_ EDyButton iId) noexcept
{
  return ::dy::base::sLowLevelInputs[iId].Get();
}

EDySuccess MInput::TryPickObject(_MIN_ const DVec2& iScreenPosition)
{
  // Get position of present frame.
  const auto& position = this->mMousePresentPosition;
  
  // Get actor id from rendering manager.
  auto* ptrFramebuffer = MIOResource::GetInstance().
      GetPtrInformation<EResourceType::GLFrameBuffer>("dyBtBasicRender");
  DDyPixelInfo pixel;
  pixel.ObjectID = TActorIdContainer::kExclusiveId;

  // Critical section of getting object id information from attachment.
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ptrFramebuffer->GetTargetFrameBufferId());
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
    auto& refWorld = MWorld::GetInstance();
    auto* ptrActor = refWorld.GetActorWithObjectId(static_cast<TU32>(pixel.ObjectID));
    if (ptrActor == nullptr)
    { /* Do nothing if not exist */
      return EDySuccess::DY_FAILURE;
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
        DyPushLogDebugInfo("Picked {}", ptrActor->ToString());
        ptrActor->MDY_PRIVATE(AttachPickingTargetFromSystem)(&this->mPtrActorPickingTarget);
      } break;
      }
    }
  }

  return EDySuccess::DY_SUCCESS;
}

void MInput::SetPickingTargetCallbackFunction(_MIN_ TPickingCallbackFunction iPtrGlobalFunction)
{
  this->mActorPickingCallback = iPtrGlobalFunction;
}

void MInput::ResetPickingTargetCallback() noexcept
{
  mActorPickingCallback = nullptr;
}

bool MInput::IsActorPicked() const noexcept
{
  return this->mPtrActorPickingTarget != nullptr;
}

FActor** MInput::MDY_PRIVATE(GetPPtrPickingTarget)() noexcept
{
  return &this->mPtrActorPickingTarget;
}

} /// ::dy namespace