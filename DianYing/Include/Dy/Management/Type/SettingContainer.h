#ifndef GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H
#define GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H
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

#include <nlohmann/json_fwd.hpp>

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Meta/Type/EDyInput.h>

namespace dy
{

///
/// @struct DDySettingDescription
/// @brief  Setting description able to be serialized.
///
struct DDySettingDescription final
{
  /// Project name of this project.
  std::string mProjectName    = MDY_INITILAIZE_EMPTYSTR;
  /// Window name of this project. if empty, just use MDySetting::mProjectName as window name with version.
  std::string mWindowName     = MDY_INITILAIZE_EMPTYSTR;
  /// [High. Mid. Low]
  TI32 mVersionHigh           = MDY_INITIALIZE_DEFINT;
  TI32 mVersionMid            = MDY_INITIALIZE_DEFINT;
  TI32 mVersionLow            = MDY_INITIALIZE_DEFINT;
  ///
  std::string mCompanyName    = MDY_INITILAIZE_EMPTYSTR;
  std::string mHomepage       = MDY_INITILAIZE_EMPTYSTR;
  std::string mSupportContact = MDY_INITILAIZE_EMPTYSTR;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingDescription& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingDescription& p);

///
/// @struct DDySettingGameplay
/// @brief  Setting gameplay that able to be serialized.
///
struct DDySettingGameplay final
{
  struct DDyShadow final
  { /// Global default shadow map size.
    DDyVectorInt2 mShadowGlobalDefaultMap = {};
  };

  /// Initial scene name. (or uuid?)
  std::string   mInitialSceneSpecifier           = MDY_INITILAIZE_EMPTYSTR;
  /// Initial game resolution scene.
  DDyVectorInt2 mInitialResolution      = {};
  /// Shadow default map resolution or properties.
  DDyShadow     mShadow                 = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingGameplay& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingGameplay::DDyShadow& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay::DDyShadow& p);

///
/// @struct DDySettingInput
/// @brief  Setting input that able to be serialized.
/// @TODO NEED TO CHANGE STRUCTRUE TO `ACTION` `AXIS`, NOT MEDIA DEPENDENT.
///
struct DDySettingInput final
{
  struct DDyMode final
  {
    bool mIsEnabledKeyboard   = false;
    bool mIsEnabledMouse      = false;
    bool mIsEnabledJoystick    = false;
  };

  struct DDyKeyboard final
  {
    std::string mSpecifierName  = {};
    EDyKeyboard mPositiveValue  = EDyKeyboard::NoneError;
    EDyKeyboard mNegativeValue  = EDyKeyboard::NoneError;
    TF32        mGravity        = MDY_INITIALIZE_DEFINT;
    bool        mIsRepeatable   = false;

    ///
    /// @brief  Create `DDyKeyboard` with valid json atlas.
    /// @param  json valid json atlas.
    /// @param  specifierName `DDyKeyboard::mSpecifierName`.
    /// @return If succeeded, just return `DDyKeyboard` instance.
    ///
    static MDY_NODISCARD DDyKeyboard CreateInstance(_MIN_ const nlohmann::json& json, _MIN_ const std::string& specifierName);
  };

  using TKeyboardMap = std::unordered_map<std::string, DDyKeyboard>;

  DDyMode       mMode         = {};
  TKeyboardMap  mKeyboardMap  = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingInput& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingInput::DDyMode& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DDyMode& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingInput::DDyKeyboard& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DDyKeyboard& p);

///
/// @struct DDySettingTag
/// @brief  Setting tag that able to be serialized.
///
struct DDySettingTag final
{
  using TObjectTagList    = std::vector<std::string>;
  using TCollisionTagList = std::vector<std::string>;

  TObjectTagList    mObjectTag     = {};
  TCollisionTagList mCollisionTag  = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingTag& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingTag& p);

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H