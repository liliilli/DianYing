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
#include <Dy/Management/Type/SettingContainer.h>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>

#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Meta/Type/Input/JsonInputButton.h>
#include <Dy/Helper/System/Assertion.h>

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

MDY_SET_IMMUTABLE_STRING(sGamePlay_Graphics,                    "Graphics");
MDY_SET_IMMUTABLE_STRING(sGamePlay_Graphics_EnableDefaultSsao,  "EnableDefaultSsao");
MDY_SET_IMMUTABLE_STRING(sGamePlay_Graphics_EnableDefaultShadow,"EnableDefaultShadow");

//! DDySettingInput

MDY_SET_IMMUTABLE_STRING(sAction, "Action");
MDY_SET_IMMUTABLE_STRING(sAxis,   "Axis");

MDY_SET_IMMUTABLE_STRING(sKey,    "Key");

MDY_SET_IMMUTABLE_STRING(sPositive, "+");
MDY_SET_IMMUTABLE_STRING(sNegative, "-");
MDY_SET_IMMUTABLE_STRING(sGravity,  "gravity");

//! DDySettingTag

MDY_SET_IMMUTABLE_STRING(sObject,     "Object");
MDY_SET_IMMUTABLE_STRING(sCollision,  "Collision");

//! DDySettingMetaPath

MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Level,    "Level");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Widget,   "Widget");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Prefab,   "Prefab");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Script,   "Script");

MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Font,     "Font");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Texture,  "Texture");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Material, "Material");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Shader,   "Shader");

MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Model,"Model");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_ModelMesh,"ModelMesh");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_ModelAnim,"ModelAnim");
MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_ModelSkeleton,"ModelSkeleton");

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
      {(sProjectName),  p.mProjectName},
      {(sWindowName),   p.mWindowName},

      {(sVersionHigh),  p.mVersionHigh},
      {(sVersionMid),   p.mVersionMid},
      {(sVersionLow),   p.mVersionLow},

      {(sCompanyName),    p.mCompanyName},
      {(sHomepage),       p.mHomepage},
      {(sSupportContact), p.mSupportContact},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDySettingDescription& p)
{
  p.mProjectName  = json::GetValueFrom<std::string>(j, sProjectName);
  p.mWindowName   = json::GetValueFrom<std::string>(j, sWindowName);

  p.mVersionHigh  = json::GetValueFrom<TI32>(j, sVersionHigh);
  p.mVersionMid   = json::GetValueFrom<TI32>(j, sVersionMid);
  p.mVersionLow   = json::GetValueFrom<TI32>(j, sVersionLow);

  p.mCompanyName      = json::GetValueFrom<std::string>(j, sCompanyName);
  p.mHomepage         = json::GetValueFrom<std::string>(j, sHomepage);
  p.mSupportContact   = json::GetValueFrom<std::string>(j, sSupportContact);
}

//!
//! Gameplay
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingGameplay& p)
{
  j = nlohmann::json
  {
      {(sInitialScene),       p.mInitialSceneSpecifier},
      {(sInitialResolution),  p.mInitialResolution},
      {(sGamePlay_Shadow),    p.mShadow},
      {(sGamePlay_Graphics),  p.mGraphics},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay& p)
{
  p.mInitialSceneSpecifier  = json::GetValueFrom<std::string>         (j, sInitialScene);
  p.mInitialResolution      = json::GetValueFrom<DIVec2>       (j, sInitialResolution);
  p.mShadow                 = json::GetValueFrom<decltype(p.mShadow)> (j, sGamePlay_Shadow);
  p.mGraphics               = json::GetValueFrom<decltype(p.mGraphics)>(j, sGamePlay_Graphics);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingGameplay::DDyShadow& p)
{
  j = nlohmann::json
  {
      {(sGamePlay_Shadow_GlobalDefaultMapSize), p.mShadowGlobalDefaultMap},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay::DDyShadow& p)
{
  p.mShadowGlobalDefaultMap = json::GetValueFrom<DIVec2>(j, sGamePlay_Shadow_GlobalDefaultMapSize);
}

void to_json(nlohmann::json& j, const DDySettingGameplay::DDyGraphics& p)
{
  j = nlohmann::json
  {
    {(sGamePlay_Graphics_EnableDefaultSsao),    p.mIsEnabledDefaultSsao},
    {(sGamePlay_Graphics_EnableDefaultShadow),  p.mIsEnabledDefaultShadow},
  };
}

void from_json(const nlohmann::json& j, DDySettingGameplay::DDyGraphics& p)
{
  p.mIsEnabledDefaultSsao   = json::GetValueFrom<bool>(j, sGamePlay_Graphics_EnableDefaultSsao);
  p.mIsEnabledDefaultShadow = json::GetValueFrom<bool>(j, sGamePlay_Graphics_EnableDefaultShadow);
}

//!
//! Input
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingInput& p)
{
  j = nlohmann::json
  {
      {(sAction),   p.mActionMap},
      {(sAxis),     p.mAxisMap},
  };
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingInput::DAction& p)
{
  if (p.mKey.empty() == false) { j[(sKey)] = p.mKey; }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingInput::DAxis& p)
{
  j = nlohmann::json
  {
      {(sGravity),  p.mGravity},
  };

  if (p.mPositive.empty() == false) { j[(sPositive)] = p.mPositive; }
  if (p.mNegative.empty() == false) { j[(sNegative)] = p.mNegative; }
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput& p)
{
  p.mAxisMap    = json::GetValueFrom<DDySettingInput::TAxisMap>   (j, sAxis);
  p.mActionMap  = json::GetValueFrom<DDySettingInput::TActionMap> (j, sAction);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DAction& p)
{
  using TKeyListType = DDySettingInput::TKeyListType;
  
  if (json::HasJsonKey(j, (sKey)) == true) { p.mKey = json::GetValueFrom<TKeyListType>(j, sKey); }
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DAxis& p)
{
  using TKeyListType = DDySettingInput::TKeyListType;

  if (json::HasJsonKey(j, (sPositive)) == true) { p.mPositive = json::GetValueFrom<TKeyListType>(j, sPositive); }
  if (json::HasJsonKey(j, (sNegative)) == true) { p.mNegative = json::GetValueFrom<TKeyListType>(j, sNegative); }

  p.mGravity        = json::GetValueFrom<TI32>(j, sGravity);
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
    {(sObject), p.mObjectTag},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingTag& p)
{
  p.mObjectTag    = json::GetValueFrom<DDySettingTag::TObjectTagList>   (j, sObject);
}

///
/// @struct DDySettingSound
/// @brief  Setting sound that able to be serialized.
///

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound& p)
{
  json::GetValueFromTo(j, "3D", p.m3DSetting);
  json::GetValueFromTo(j, "Master",   p.mMaster);
  json::GetValueFromTo(j, "Group",    p.mGroup);
  json::GetValueFromTo(j, "Channel",  p.mChannel);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::D3DSetting& p)
{
  json::GetValueFromTo(j, "DopplerOffset", p.mDopplerOffset);
  json::GetValueFromTo(j, "DistanceUnit", p.mDistanceUnit);
  json::GetValueFromTo(j, "AttenuationFactor", p.mAttenuationFactor);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::DDetail& p)
{
  json::GetValueFromTo(j, "Muted", p.mMuted);
  json::GetValueFromTo(j, "Volume", p.mVolume);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::DChannelDetail& p)
{
  from_json(j, static_cast<DDySettingSound::DDetail&>(p));
  json::GetValueFromTo(j, "Group", p.mGroupSpecifier);

  MDY_ASSERT_MSG_FORCE(p.mGroupSpecifier.empty() == false, "Channel's group must be specified.");
}

///
/// @struct DDySettingPhysics
/// @brief Physics setting.
///

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingPhysics& p)
{
  json::GetValueFromTo(j, "Layer",        p.mCollisionTag);
  json::GetValueFromTo(j, "Common",       p.mCommonProperty);
  json::GetValueFromTo(j, "FilterPreset", p.mFilterPresetContainer);
  json::GetValueFromTo(j, "LockPreset",   p.mLockPresetContainer);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingPhysics::DCommon& p)
{
  json::GetValueFromTo(j, "Gravity",                p.mGravity);
  json::GetValueFromTo(j, "DefaultStaticFriction",  p.mDefaultStaticFriction);
  json::GetValueFromTo(j, "DefaultDynamicFriction", p.mDefaultDynamicFriction);
  json::GetValueFromTo(j, "DefaultRestitution",     p.mDefaultRestitution);
  json::GetValueFromTo(j, "DefaultAngularDamping",  p.mDefaultAngularDamping);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyCollisionFilter& p)
{
  // Get integer value from json.
  const auto val = j.get<TU32>();
  // 0 is block, 1 is overlap, 2 is ignore.
  p = static_cast<EDyCollisionFilter>(val);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DLockPreset& p)
{
  json::GetValueFromTo(j, "Position", p.mPosition);
  json::GetValueFromTo(j, "Rotation", p.mRotation);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DLockPreset::D3DAxis& p)
{
  json::GetValueFromTo(j, "X", p.mX);
  json::GetValueFromTo(j, "Y", p.mY);
  json::GetValueFromTo(j, "Z", p.mZ);
}

///
/// @struct DDySettingMetaPath
/// @brief  Setting meta file path that able to be serialized.
///

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingMetaPath& p)
{
  j = nlohmann::json
  {
      {(sHeader_MetaPath_Level),  p.mLevelMetaPath},
      {(sHeader_MetaPath_Widget), p.mWidgetMetaPath},
      {(sHeader_MetaPath_Prefab), p.mPrefabMetaPath},
      {(sHeader_MetaPath_Script), p.mScriptMetaPath},

      {(sHeader_MetaPath_Font),     p.mFontMetaPath},
      {(sHeader_MetaPath_Texture),  p.mTextureMetaPath},
      {(sHeader_MetaPath_Material), p.mMaterialMetaPath},
      {(sHeader_MetaPath_Shader),   p.mGLShaderMetaPath},

      {(sHeader_MetaPath_Model),    p.mModelMetaPath},
      {(sHeader_MetaPath_ModelMesh),p.mModelMeshMetaPath},
      {(sHeader_MetaPath_ModelAnim),p.mModelAnimMetaPath},
      {(sHeader_MetaPath_ModelSkeleton),p.mModelSkeletonMetaPath},

      {"Sound", p.mSoundMetaPath},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingMetaPath& p)
{
  p.mLevelMetaPath  = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Level);
  p.mWidgetMetaPath = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Widget);
  p.mPrefabMetaPath = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Prefab);
  p.mScriptMetaPath = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Script);

  p.mFontMetaPath     = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Font);
  p.mTextureMetaPath  = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Texture);
  p.mGLShaderMetaPath = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Shader);
  p.mMaterialMetaPath = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Material);

  p.mModelMetaPath        = json::GetValueFrom<std::string>(j, sHeader_MetaPath_Model);
  p.mModelMeshMetaPath    = json::GetValueFrom<std::string>(j, sHeader_MetaPath_ModelMesh);
  p.mModelAnimMetaPath    = json::GetValueFrom<std::string>(j, sHeader_MetaPath_ModelAnim);
  p.mModelSkeletonMetaPath= json::GetValueFrom<std::string>(j, sHeader_MetaPath_ModelSkeleton);

  p.mSoundMetaPath = json::GetValueFrom<std::string>(j, "Sound");
}

} /// ::dy namespace