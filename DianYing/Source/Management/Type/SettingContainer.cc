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
#include <Dy/Management/Type/SettingContainer.h>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>
#include <Dy/Helper/JsonHelper.h>

//!
//! Forward declaration
//!

namespace
{

//! DDySettingDescription

MDY_SET_IMMUTABLE_STRING(sProjectName,          "ProjectName");
MDY_SET_IMMUTABLE_STRING(sWindowName,           "WindowName");
MDY_SET_IMMUTABLE_STRING(sVersionHigh,          "VersionHigh");
MDY_SET_IMMUTABLE_STRING(sVersionMid,           "VersionMid");
MDY_SET_IMMUTABLE_STRING(sVersionLow,           "VersionLow");
MDY_SET_IMMUTABLE_STRING(sCompanyName,          "CompanyName");
MDY_SET_IMMUTABLE_STRING(sHomepage,             "Homepage");
MDY_SET_IMMUTABLE_STRING(sSupportContact,       "SupportContact");

//! DDySettingGameplay

MDY_SET_IMMUTABLE_STRING(sInitialScene,         "InitialScene");
MDY_SET_IMMUTABLE_STRING(sInitialResolution,    "InitialResolution");

MDY_SET_IMMUTABLE_STRING(sGamePlay_Shadow,                      "Shadow");
MDY_SET_IMMUTABLE_STRING(sGamePlay_Shadow_GlobalDefaultMapSize, "GlobalDefaultMapSize");

//! DDySettingInput

MDY_SET_IMMUTABLE_STRING(sAction, "Action");
MDY_SET_IMMUTABLE_STRING(sAxis,   "Axis");

MDY_SET_IMMUTABLE_STRING(sKey,    "Key");

MDY_SET_IMMUTABLE_STRING(sPositive, "+");
MDY_SET_IMMUTABLE_STRING(sNegative, "-");
MDY_SET_IMMUTABLE_STRING(sGravity,  "gravity");
MDY_SET_IMMUTABLE_STRING(sRepeat,   "repeat");

//! DDySettingTag

MDY_SET_IMMUTABLE_STRING(sObject,     "Object");
MDY_SET_IMMUTABLE_STRING(sCollision,  "Collision");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingDescription& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sProjectName),  p.mProjectName},
      {MSVSTR(sWindowName),   p.mWindowName},

      {MSVSTR(sVersionHigh),  p.mVersionHigh},
      {MSVSTR(sVersionMid),   p.mVersionMid},
      {MSVSTR(sVersionLow),   p.mVersionLow},

      {MSVSTR(sCompanyName),    p.mCompanyName},
      {MSVSTR(sHomepage),       p.mHomepage},
      {MSVSTR(sSupportContact), p.mSupportContact},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDySettingDescription& p)
{
  p.mProjectName  = DyGetValue<std::string>(j, sProjectName);
  p.mWindowName   = DyGetValue<std::string>(j, sWindowName);

  p.mVersionHigh  = DyGetValue<TI32>(j, sVersionHigh);
  p.mVersionMid   = DyGetValue<TI32>(j, sVersionMid);
  p.mVersionLow   = DyGetValue<TI32>(j, sVersionLow);

  p.mCompanyName      = DyGetValue<std::string>(j, sCompanyName);
  p.mHomepage         = DyGetValue<std::string>(j, sHomepage);
  p.mSupportContact   = DyGetValue<std::string>(j, sSupportContact);
}

//!
//! Gameplay
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingGameplay& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sInitialScene),       p.mInitialSceneSpecifier},
      {MSVSTR(sInitialResolution),  p.mInitialResolution},
      {MSVSTR(sGamePlay_Shadow),    p.mShadow},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay& p)
{
  p.mInitialSceneSpecifier  = DyGetValue<std::string>         (j, sInitialScene);
  p.mInitialResolution      = DyGetValue<DDyVectorInt2>       (j, sInitialResolution);
  p.mShadow                 = DyGetValue<decltype(p.mShadow)> (j, sGamePlay_Shadow);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingGameplay::DDyShadow& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sGamePlay_Shadow_GlobalDefaultMapSize), p.mShadowGlobalDefaultMap},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay::DDyShadow& p)
{
  p.mShadowGlobalDefaultMap = DyGetValue<DDyVectorInt2>(j, sGamePlay_Shadow_GlobalDefaultMapSize);
}

//!
//! Input
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingInput& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sAction),   p.mActionMap},
      {MSVSTR(sAxis),     p.mAxisMap},
  };
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingInput::DAction& p)
{
  if (p.mKey.empty() == false) { j[MSVSTR(sKey)] = p.mKey; }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingInput::DAxis& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sGravity),  p.mGravity},
      {MSVSTR(sRepeat),   p.mIsRepeatable},
  };

  if (p.mPositive.empty() == false) { j[MSVSTR(sPositive)] = p.mPositive; }
  if (p.mNegative.empty() == false) { j[MSVSTR(sNegative)] = p.mNegative; }
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput& p)
{
  p.mAxisMap    = DyGetValue<DDySettingInput::TAxisMap>   (j, sAxis);
  p.mActionMap  = DyGetValue<DDySettingInput::TActionMap> (j, sAction);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DAction& p)
{
  using TKeyListType = DDySettingInput::TKeyListType;
  if (DyIsJsonKeyExist(j, MSVSTR(sKey)) == true) { p.mKey = DyGetValue<TKeyListType>(j, sKey); }
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DAxis& p)
{
  using TKeyListType = DDySettingInput::TKeyListType;

  if (DyIsJsonKeyExist(j, MSVSTR(sPositive)) == true) { p.mPositive = DyGetValue<TKeyListType>(j, sPositive); }
  if (DyIsJsonKeyExist(j, MSVSTR(sNegative)) == true) { p.mNegative = DyGetValue<TKeyListType>(j, sNegative); }

  p.mGravity      = DyGetValue<TI32>(j, sGravity);
  p.mIsRepeatable = DyGetValue<bool>(j, sRepeat);
}

DDySettingInput::DAction
DDySettingInput::DAction::CreateInstance(_MIN_ const nlohmann::json& json, _MIN_ const std::string& specifierName)
{
  DDySettingInput::DAction p = json.get<DDySettingInput::DAction>();
  p.mSpecifierName = specifierName;

  return p;
}

DDySettingInput::DAxis
DDySettingInput::DAxis::CreateInstance(_MIN_ const nlohmann::json& json, _MIN_ const std::string& specifierName)
{
  DDySettingInput::DAxis p = json.get<DDySettingInput::DAxis>();
  p.mSpecifierName = specifierName;

  return p;
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingTag& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sObject),     p.mObjectTag},
      {MSVSTR(sCollision),  p.mCollisionTag},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingTag& p)
{
  p.mObjectTag    = DyGetValue<DDySettingTag::TObjectTagList>   (j, sObject);
  p.mCollisionTag = DyGetValue<DDySettingTag::TCollisionTagList>(j, sCollision);
}

} /// ::dy namespace