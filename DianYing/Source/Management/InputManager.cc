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
#include <Phitos/Dbg/assert.h>

#include <Dy/Helper/JsonHelper.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/Helper/InputKeyString.h>

//!
//! Data
//!

namespace
{

///
/// @enum EKeyPrimaryState
/// @brief Check key primary state only in this file.
///
enum class EKeyPrimaryState : char {
  Released  = 0,
  Pressed   = 1,
  Repeated  = 2
};

constexpr const char err_input_not_initiated[]          = "Input Setting its not initiated yet.";
constexpr const char err_input_duplicated_init[]        = "Duplicated function call of ::opgs16::manager::input::Initiate(context) is prohibited.";
constexpr const char err_input_key_not_exist[]          = "Key axis is not exist. [Key axis : {}]";
constexpr const char err_input_failed_load_file[]       = "Failed to find project input setting file. [Path : {}]";
constexpr const char err_input_failed_json_file[]       = "Header {} is not found in json file. [Path : {}]";
constexpr const char err_input_failed_failed_bind_key[] = "Failed some operation on binding keyboard key.";
constexpr const char err_input_keyboard_not_exist[]     = "Key {} is not found in mode object. [Path : {}]";
constexpr const char err_input_disable_msg[]            = "{} input feature will be disabled.";

EKeyPrimaryState  sPrimaryKeys[349];
dy::DDyVector2    sMouseLastPosition    = {};
dy::DDyVector2    sMousePresentPosition = {};
bool              sIsFirstMouseMovement = true;
bool              sMousePositionDirty   = false;

} /// ::unnamed namespace

//!
//! Functions
//!

namespace
{

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
void __InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
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

void __InputMouseCallback(GLFWwindow* window, double xPos, double yPos)
{
  if (sIsFirstMouseMovement)
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
/// @brief Apply gravity of each key information, and if key axis value is in range of threshold value,
/// change key status to neutral status.
///
void DyProceedGravity(dy::DDyKeyBindingInformation& key_info);

///
/// @brief ff
///
bool DyModeVerifyInputStyle(const nlohmann::json& json, const std::string& key, const std::string& file_path);

///
/// @brief ff
///
EDySuccess DyBindKeyboardKeyInformation(const nlohmann::json& atlas_json);

///
/// @brief ff
///
EDySuccess DyKeyboardBindKey(const nlohmann::basic_json<>::const_iterator& it, dy::MDyInput& inputManager);

void DyProceedGravity(dy::DDyKeyBindingInformation& key_info) {
	const auto dt = dy::MDyTime::GetInstance().GetGameScaledTickedDeltaTimeValue();
  static constexpr float kZeroValue = 0.0f;

	if (auto& value = key_info.mAxisValue; value < 0) // Negative
  {
		value += key_info.mToNeutralGravity * dt;
		if (-key_info.mNeutralStatusThresholdValue <= value)
    {
			value = kZeroValue;
			key_info.mKeyStatus = dy::DDyKeyBindingInformation::EDyKeyInputStatus::CommonNeutral;
		}
	}
	else // Positive
  {
		value -= key_info.mToNeutralGravity * dt;
		if (value <= key_info.mNeutralStatusThresholdValue)
    {
			value = kZeroValue;
			key_info.mKeyStatus = dy::DDyKeyBindingInformation::EDyKeyInputStatus::CommonNeutral;
		}
	}
}

bool DyModeVerifyInputStyle(const nlohmann::json& json, const std::string& key, const std::string& file_path) {
  if (!dy::DyIsJsonKeyExist(json, key)) {
    MDY_LOG_ERROR(err_input_disable_msg, key);
    MDY_LOG_INFO("Mode key {} is {}", key, "OFF");
    return false;
  }

  if (json.count(key) != 1) {
    MDY_LOG_CRITICAL_D("Duplicated {} is found. [{} : {}]", key, "Path", file_path);
    MDY_LOG_ERROR_D(err_input_disable_msg, key);
    MDY_LOG_INFO("Mode key {} is {}", key, "OFF");
    return false;
  }

  MDY_LOG_INFO("Mode key {} is {}", key, "ON");
  return true;
}

bool DyKeyboardVerifyKey(const std::string& key, const nlohmann::json& key_value) {
  if (!dy::DyIsJsonKeyExist(key_value, "+") && !dy::DyIsJsonKeyExist(key_value, "-")) {
    MDY_LOG_CRITICAL("Keyboard key {} does not have any key binding.", key);
    return false;
  }

  if (!dy::DyIsJsonKeyExist(key_value, "gravity")) {
    MDY_LOG_CRITICAL("Keyboard key {} does not have gravity.", key);
    return false;
  }

  if (!dy::DyIsJsonKeyExist(key_value, "repeat")) {
    MDY_LOG_CRITICAL("Keyboard key {} does not have repeat.", key);
    return false;
  }

  return true;
}

EDySuccess DyBindKeyboardKeyInformation(const nlohmann::json& atlas_json) {
  if (!dy::DyIsJsonKeyExist(atlas_json, "keyboard"))
  {
    MDY_LOG_CRITICAL("Header {} is not found in json file.", "keyboard");
    MDY_LOG_ERROR("Keyboard input feature will be disabled.");
    PHITOS_NOT_IMPLEMENTED_ASSERT();
    return DY_FAILURE;
  }
  const auto keyboard = atlas_json["keyboard"];

  auto& inputManager = dy::MDyInput::GetInstance();

  for (auto it = keyboard.begin(); it != keyboard.end(); ++it)
  {
    const std::string& key = it.key();
    const auto& value = it.value();

    if (inputManager.pIsKeyExist(key))
    {
      MDY_LOG_ERROR("Keyboard key {} is duplicated. key {} will not be performed properly.", key);
      continue;
    }

    if (DyKeyboardVerifyKey(key, value) == DY_FAILURE)
    {
      MDY_LOG_ERROR("Failed to verify keyboard key {0}. Keyboard key {0} will not bind to input system.", key);
      continue;
    }

    if (DyKeyboardBindKey(it, inputManager) == DY_FAILURE)
    {
      MDY_LOG_ERROR("Failed to bind keyboard key {}.", key);
      return DY_FAILURE;
    }
  }

  return DY_SUCCESS;
}

EDySuccess DyKeyboardBindKey(const nlohmann::basic_json<>::const_iterator& it, dy::MDyInput& inputManager) {
  const auto key    = it.key();
  const auto& value = it.value();

  dy::PDyKeyBindingConstructionDescriptor desc;
  desc.mKeyType = dy::EDyInputType::Keyboard;
  desc.mKeyName = key;
  if (auto pos_it = value.find("+"); pos_it != value.end())
  {
    const auto& pos_it_value = pos_it.value();

    if (!pos_it_value.is_string())
    {
      MDY_LOG_ERROR("Keyboard key {} {} value is not number.", key, "positive");
      PHITOS_ASSERT(!pos_it_value.is_string(), "Key binding failed.");
      return DY_FAILURE;
    }
    const auto key_string = pos_it_value.get<std::string>();
    auto uid = dy::DyGetKeyUidValue(key_string);
    if (!uid.has_value())
    {
      MDY_LOG_ERROR("Keyboard key {} might be not supported on this version.", key_string);
      PHITOS_ASSERT(uid.has_value(), "Failed to bind spcified key value. Might be not supported key string.");
    }

    MDY_LOG_DEBUG_D("Key axis bind : {} Positive : {}", key, key_string);
    desc.mPositiveButtonId = uid.value();
  }

  if (auto neg_it = value.find("-"); neg_it != value.end())
  {
    const auto& neg_it_value = neg_it.value();

    if (!neg_it_value.is_string())
    {
      MDY_LOG_ERROR("Keyboard key {} {} value is not number.", key, "negative");
      PHITOS_ASSERT(!neg_it_value.is_string(), "Key binding failed.");
      return DY_FAILURE;
    }
    const auto key_string = neg_it_value.get<std::string>();
    auto uid = dy::DyGetKeyUidValue(key_string);
    if (!uid.has_value())
    {
      MDY_LOG_ERROR("Keyboard key {} might be not supported on this version.", key_string);
      PHITOS_ASSERT(uid.has_value(), "Failed to bind spcified key value. Might be not supported key string.");
    }

    MDY_LOG_DEBUG_D("Key axis bind : {} Negative : {}", key, key_string);
    desc.mNegativeButtonId = uid.value();
  }

  const auto& gravity_it_value = value.find("gravity");
  if (!gravity_it_value->is_number_unsigned())
  {
    MDY_LOG_ERROR("Keyboard key {} {} value is not number.", key, "gravity");
    return DY_FAILURE;
  }
  desc.mToNeutralGravity = static_cast<float>(gravity_it_value->get<unsigned>());

  if (const auto& stick_it_value = value.find("repeat"); stick_it_value->is_boolean())
  {
    desc.mIsEnabledRepeatKey = stick_it_value.value();
  }

  if (!dy::MDyInput::GetInstance().pInsertKey(desc))
    return DY_FAILURE;
  else
    return DY_SUCCESS;
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyInput::pfInitialize()
{
  if (!this->pReadInputFile("./TestResource/input.meta"))
  {
    PHITOS_UNEXPECTED_BRANCH();
  }

  auto& winManager = MDyWindow::GetInstance();
  this->mTempGlfwWindowPtr = winManager.GetGlfwWindowContext();

  if (this->mIsEnabledKeyboard)
  {
    glfwSetKeyCallback(this->mTempGlfwWindowPtr, __InputKeyCallback);
  }

  if (this->mIsEnabledMouse)
  {
    glfwSetCursorPosCallback(this->mTempGlfwWindowPtr, __InputMouseCallback);
    glfwSetInputMode(this->mTempGlfwWindowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  if (this->mIsEnabledJoystick)
  {
    // @todo Implement joystick button and position callback function.
  }

  return DY_SUCCESS;
}

EDySuccess MDyInput::pReadInputFile(const std::string& file_path) {
  auto opJsonAtlas = DyGetJsonAtlas(file_path);
  if (!opJsonAtlas.has_value())
  {
    MDY_LOG_CRITICAL(err_input_failed_load_file, file_path);
    MDY_LOG_ERROR("Input feature will be disabled.");
    PHITOS_NOT_IMPLEMENTED_ASSERT();
    return DY_FAILURE;
  }
  const auto& atlas_json = opJsonAtlas.value();

  if (!DyIsJsonKeyExist(atlas_json, "mode")) {
    MDY_LOG_CRITICAL(err_input_failed_json_file, "mode", file_path);
    MDY_LOG_ERROR("Input feature will be disabled.");
    PHITOS_NOT_IMPLEMENTED_ASSERT();
    return DY_FAILURE;
  }

  const auto input_mode = atlas_json["mode"];

  const bool isActivatedKeyboard = DyModeVerifyInputStyle(input_mode, "keyboard", file_path);
  const bool isActivatedMouse    = DyModeVerifyInputStyle(input_mode, "mouse", file_path);
  const bool isActivatedJoystick = DyModeVerifyInputStyle(input_mode, "joystick", file_path);

  // Joystick verification did not held, because not supported yet.

  if (isActivatedKeyboard)
  {
    if (DyBindKeyboardKeyInformation(atlas_json) == DY_FAILURE)
    {
      MDY_LOG_WARNING(err_input_failed_failed_bind_key);
      return DY_FAILURE;
    }
    else
    {
      this->mIsEnabledKeyboard = true;
    }
  }

  if (isActivatedMouse)
  {
    if (atlas_json.find("mouse") == atlas_json.end())
    {
      MDY_LOG_CRITICAL(err_input_failed_json_file, "mouse", file_path);
      MDY_LOG_ERROR("mouse input feature will be disabled.");
      PHITOS_NOT_IMPLEMENTED_ASSERT();
      return DY_FAILURE;
    }
    else
    {
      const auto mouse = atlas_json["mouse"];
      this->mIsEnabledMouse = true;
    }
  }

  if (isActivatedJoystick)
  {
    MDY_LOG_DEBUG_D("Joystick feature is not supported now.");
  }

  return DY_SUCCESS;
}

EDySuccess MDyInput::pfRelease()
{
  m_key_disposal.clear();
  return DY_SUCCESS;
}

float MDyInput::GetKeyValue(const std::string& axisKeyName) noexcept
{
  const auto keyIt = this->mBindedKeyList.find(axisKeyName);
  if (keyIt == this->mBindedKeyList.end()) {
    MDY_LOG_ERROR_D(err_input_key_not_exist, axisKeyName);
		return 0.f;
	}

  return keyIt->second.mAxisValue;
}

bool MDyInput::IsKeyPressed(const std::string& keyName) noexcept
{
  const auto keyIt = this->mBindedKeyList.find(keyName);
  if (keyIt == mBindedKeyList.end()) {
    MDY_LOG_ERROR_D(err_input_key_not_exist, keyName);
		return false;
  }

  if (DDyKeyBindingInformation& keyInformation = keyIt->second;
      !keyInformation.mIsRepeatKey)
  {
    switch (keyInformation.mKeyStatus)
    {
    case DDyKeyBindingInformation::EDyKeyInputStatus::PositivePressed:
    case DDyKeyBindingInformation::EDyKeyInputStatus::NegativePressed:
      return true;
    default: return false;
    }
  }
  else
  {
    switch (keyInformation.mKeyStatus)
    {
    case DDyKeyBindingInformation::EDyKeyInputStatus::PositivePressed:
    case DDyKeyBindingInformation::EDyKeyInputStatus::NegativePressed:
    case DDyKeyBindingInformation::EDyKeyInputStatus::PositiveRepeated:
    case DDyKeyBindingInformation::EDyKeyInputStatus::NegativeRepeated:
      return true;
    default: return false;
    }
  }
}

bool MDyInput::IsKeyReleased(const std::string& keyName) noexcept
{
  const auto keyIt = this->mBindedKeyList.find(keyName);
  if (keyIt == mBindedKeyList.end()) {
    MDY_LOG_ERROR_D(err_input_key_not_exist, keyName);
		return false;
  }

  switch (keyIt->second.mKeyStatus) {
  case DDyKeyBindingInformation::EDyKeyInputStatus::CommonNeutral:
  case DDyKeyBindingInformation::EDyKeyInputStatus::CommonReleased:
    return true;
  default: return false;
  }
}

bool MDyInput::pIsKeyExist(const std::string& keyName) const noexcept
{
  return this->mBindedKeyList.find(keyName) != this->mBindedKeyList.end();
}

EDySuccess MDyInput::pInsertKey(const PDyKeyBindingConstructionDescriptor& bindingKey) noexcept
{
  if (this->pIsKeyExist(bindingKey.mKeyName))
  {
    MDY_LOG_ERROR_D("{} | Key is already binded. Name : {}", "MDyInput::pInsertKey", bindingKey.mKeyName);
    return DY_FAILURE;
  }

  try
  {
    auto [it, result] = this->mBindedKeyList.try_emplace(bindingKey.mKeyName, bindingKey);
    if (!result)
    {
      MDY_LOG_CRITICAL_D("{} | Unexpected error happened. Name : {}", "MDyInput::pInsertKey", bindingKey.mKeyName);
      return DY_FAILURE;
    }
  }
  catch (const std::runtime_error& e)
  {
    static auto DyGetDebugStringFromKeyStyle = [](EDyInputType type) -> std::string_view
    {
      switch (type)
      {
      case EDyInputType::Keyboard:  return "Keyboard";
      case EDyInputType::Mouse:     return "Mouse";
      case EDyInputType::Joystick:  return "Joystick";
      default:                      return "ErrorNone";
      }
    };

    MDY_LOG_CRITICAL_D("{} | Key binding failed because {}. | Name : {}", "MDyInput", e.what(), bindingKey.mKeyName);
    MDY_LOG_CRITICAL_D("{} | Key name : {}", "MDyInput", bindingKey.mKeyName);
    MDY_LOG_CRITICAL_D("{} | Key style : {}", "MDyInput", DyGetDebugStringFromKeyStyle(bindingKey.mKeyType));
    MDY_LOG_CRITICAL_D("{} | Key positive : {}", "MDyInput", bindingKey.mPositiveButtonId);
    MDY_LOG_CRITICAL_D("{} | Key negative : {}", "MDyInput", bindingKey.mNegativeButtonId);
    MDY_LOG_CRITICAL_D("{} | Key gravity : {}", "MDyInput", bindingKey.mToNeutralGravity);
    MDY_LOG_CRITICAL_D("{} | Key threshold : {}", "MDyInput", bindingKey.mNeturalThreshold);
    MDY_LOG_CRITICAL_D("{} | Key repeat key : {}", "MDyInput", bindingKey.mIsEnabledRepeatKey ? "ON" : "OFF");
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

void MDyInput::pfUpdate(float dt) noexcept
{
  static constexpr float kNegativeValue = -1.0f;
  static constexpr float kPositiveValue = +1.0f;

  // PRESSED, CommonNeutral checks key pressed event.
  // If key released in state PRESSED, change it to CommonReleased.

  if (this->mIsEnabledKeyboard)
  {
    for (auto&[keyName, key] : this->mBindedKeyList) {
      using Status = DDyKeyBindingInformation::EDyKeyInputStatus;

      switch (key.mKeyStatus) {
      case Status::CommonReleased:
        DyProceedGravity(key);
        [[fallthrough]];
      case Status::CommonNeutral:
        if (key.mNegativeButtonId != MDY_NOT_INITIALIZED_M1 && sPrimaryKeys[key.mNegativeButtonId] == EKeyPrimaryState::Pressed)
        {
          key.mAxisValue = kNegativeValue;
          key.mKeyStatus = Status::NegativePressed;
        }
        else if (key.mPositiveButtonId != MDY_NOT_INITIALIZED_M1 && sPrimaryKeys[key.mPositiveButtonId] == EKeyPrimaryState::Pressed)
        {
          key.mAxisValue = kPositiveValue;
          key.mKeyStatus = Status::PositivePressed;
        }
        break;
      case Status::NegativePressed:
        if (key.mPositiveButtonId != MDY_NOT_INITIALIZED_M1 && sPrimaryKeys[key.mPositiveButtonId] == EKeyPrimaryState::Pressed)
        {
          key.mAxisValue = kPositiveValue;
          key.mKeyStatus = Status::PositivePressed;
        }
        else if (key.mNegativeButtonId != MDY_NOT_INITIALIZED_M1) {
          const auto keyPrimaryState = sPrimaryKeys[key.mNegativeButtonId];
          if (key.mIsRepeatKey && keyPrimaryState == EKeyPrimaryState::Repeated) { key.mKeyStatus = Status::NegativeRepeated; }
          else if (keyPrimaryState == EKeyPrimaryState::Released)
          {
            key.mKeyStatus = Status::CommonReleased;
            DyProceedGravity(key);
          }
        }
        break;
      case Status::PositivePressed:
        if (key.mNegativeButtonId != MDY_NOT_INITIALIZED_M1 && sPrimaryKeys[key.mNegativeButtonId] == EKeyPrimaryState::Pressed)
        {
          key.mAxisValue = kNegativeValue;
          key.mKeyStatus = Status::NegativePressed;
        }
        else if (key.mPositiveButtonId != MDY_NOT_INITIALIZED_M1) {
          const auto keyPrimaryState = sPrimaryKeys[key.mPositiveButtonId];
          if (key.mIsRepeatKey && keyPrimaryState == EKeyPrimaryState::Repeated) { key.mKeyStatus = Status::PositiveRepeated; }
          else if (keyPrimaryState == EKeyPrimaryState::Released)
          {
            key.mKeyStatus = Status::CommonReleased;
            DyProceedGravity(key);
          }
        }
        break;
      case Status::PositiveRepeated:
        if (key.mNegativeButtonId != MDY_NOT_INITIALIZED_M1 && sPrimaryKeys[key.mNegativeButtonId] == EKeyPrimaryState::Pressed)
        {
          key.mAxisValue = kNegativeValue;
          key.mKeyStatus = Status::NegativePressed;
        }
        else if (key.mPositiveButtonId != MDY_NOT_INITIALIZED_M1)
        {
          const auto key_md = sPrimaryKeys[key.mPositiveButtonId];
          if (key_md == EKeyPrimaryState::Released)
          {
            key.mKeyStatus = Status::CommonReleased;
            DyProceedGravity(key);
          }
        }
        break;
      case Status::NegativeRepeated:
        if (key.mPositiveButtonId != MDY_NOT_INITIALIZED_M1 && sPrimaryKeys[key.mPositiveButtonId] == EKeyPrimaryState::Pressed)
        {
          key.mAxisValue = kPositiveValue;
          key.mKeyStatus = Status::PositivePressed;
        }
        else if (key.mNegativeButtonId != MDY_NOT_INITIALIZED_M1)
        {
          const auto key_md = sPrimaryKeys[key.mPositiveButtonId];
          if (key_md == EKeyPrimaryState::Released)
          {
            key.mKeyStatus = Status::CommonReleased;
            DyProceedGravity(key);
          }
        }
        break;
      }
    }
  }

  if (this->mIsEnabledMouse)
  {
    if (!sIsFirstMouseMovement && sMousePositionDirty)
    {
      this->mMousePresentPosition = sMousePresentPosition;
      this->mMouseLastPosition    = sMouseLastPosition;

      //MDY_LOG_DEBUG_D("Mouse position : {}, {}", this->mMousePresentPosition.X, this->mMousePresentPosition.Y);
      this->mIsMouseMoved = true;
      sMousePositionDirty = false;
    }
    else
    {
      this->mIsMouseMoved = false;
    }
  }
}

} /// ::dy namespace