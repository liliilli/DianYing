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
#include <Dy/Management/Internal/MDyDebug.h>
#include <Dy/Management/Type/SettingContainer.h>
#include <Dy/Management/Type/Input/EDyInputButtonStatus.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Core/DyEngine.h>

//!
//! Forward declaration & local translation unit function.
//!

namespace 
{

MDY_SET_IMMUTABLE_STRING(sKeyBinding, R"dy(
{
  "PauseInGameUpdate": { "Key": [ "KB_F1" ] }
}
)dy");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyDebug::pfInitialize()
{
  // Set action map for input debug keys.
  const auto jsonAtlas = nlohmann::json::parse(sKeyBinding);
  DDySettingInput::TActionMap map = jsonAtlas;

  // Set for debug input keys.
  for (const auto& [specifierName, info] : map)
  {
    auto [_, isSucceeded] = this->mBindedActionMap.try_emplace(specifierName, info);
    MDY_ASSERT(isSucceeded == true, "Unexpected error occurred.");

    // Set specifier name.
    _->second.mAxisSpecifierName = specifierName;
  }

  return DY_SUCCESS;
}

EDySuccess MDyDebug::pfRelease()
{
  return DY_SUCCESS;
}

EDySuccess MDyDebug::CheckInput(_MIN_ MDY_NOTUSED TF32 dt) noexcept
{
  if (this->mIsExternalInputActivated == false)
  {
    this->pUpdateInputKeys();

    // Check F1.
    if (this->IsActionPressed("PauseInGameUpdate") == true)
    {
      if (gEngine->IsInGameUpdatePaused() == true)  { gEngine->SetInGameUpdatePause(false); }
      else                                          { gEngine->SetInGameUpdatePause(true); }

      return DY_SUCCESS;
    }

    // Check F2.

  }

  return DY_FAILURE;
}

void MDyDebug::pUpdateInputKeys()
{
  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckActionStatusIfAny = [](_MINOUT_ DDyActionBindingInformation& actionInfo, _MIN_ EDyInputButtonStatus goalState)
  {
    return std::any_of(
        MDY_BIND_CBEGIN_CEND(actionInfo.mActionId),
        [goalState](_MIN_ const auto& id) 
        { return MDyInput::GetInstance().MDY_PRIVATE_SPECIFIER(GetLowlevelKeyStatus(id)) == goalState; }
    );
  };

  /// @brief Check action key status to find if any one is satisfied goalState condition.
  /// @param actionInfo action information
  static auto CheckActionStatusIfAll = [](_MINOUT_ DDyActionBindingInformation& actionInfo, _MIN_ EDyInputButtonStatus goalState)
  {
    return std::all_of(
        MDY_BIND_CBEGIN_CEND(actionInfo.mActionId),
        [goalState](_MIN_ const auto& id) 
        { return MDyInput::GetInstance().MDY_PRIVATE_SPECIFIER(GetLowlevelKeyStatus(id)) == goalState; }
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

bool MDyDebug::IsActionPressed(_MIN_ const std::string& iSpecifier) const noexcept
{
  // Validity test
  const auto keyIt = this->mBindedActionMap.find(iSpecifier);
  if (keyIt == mBindedActionMap.end())
  {
    MDY_UNEXPECTED_BRANCH();
		return false;
  }

  return keyIt->second.mKeyStatus == DDyActionBindingInformation::EDyActionInputStatus::Pressed;
}

bool MDyDebug::IsActionReleased(_MIN_ const std::string& iSpecifier) const noexcept
{
  // Validity test
  const auto keyIt = this->mBindedActionMap.find(iSpecifier);
  if (keyIt == mBindedActionMap.end())
  {
    MDY_UNEXPECTED_BRANCH();
		return false;
  }

  return keyIt->second.mKeyStatus == DDyActionBindingInformation::EDyActionInputStatus::Released;
}

void MDyDebug::UpdateAndRender()
{
  // DO NOTHING 
}

} /// ::dy namespace