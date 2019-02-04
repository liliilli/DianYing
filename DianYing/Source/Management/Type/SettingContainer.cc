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

MDY_SET_IMMUTABLE_STRING(sHeader_MetaPath_Scene,    "Scene");
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
  p.mProjectName  = DyJsonGetValueFrom<std::string>(j, sProjectName);
  p.mWindowName   = DyJsonGetValueFrom<std::string>(j, sWindowName);

  p.mVersionHigh  = DyJsonGetValueFrom<TI32>(j, sVersionHigh);
  p.mVersionMid   = DyJsonGetValueFrom<TI32>(j, sVersionMid);
  p.mVersionLow   = DyJsonGetValueFrom<TI32>(j, sVersionLow);

  p.mCompanyName      = DyJsonGetValueFrom<std::string>(j, sCompanyName);
  p.mHomepage         = DyJsonGetValueFrom<std::string>(j, sHomepage);
  p.mSupportContact   = DyJsonGetValueFrom<std::string>(j, sSupportContact);
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
      {MSVSTR(sGamePlay_Graphics),  p.mGraphics},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay& p)
{
  p.mInitialSceneSpecifier  = DyJsonGetValueFrom<std::string>         (j, sInitialScene);
  p.mInitialResolution      = DyJsonGetValueFrom<DDyVectorInt2>       (j, sInitialResolution);
  p.mShadow                 = DyJsonGetValueFrom<decltype(p.mShadow)> (j, sGamePlay_Shadow);
  p.mGraphics               = DyJsonGetValueFrom<decltype(p.mGraphics)>(j, sGamePlay_Graphics);
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
  p.mShadowGlobalDefaultMap = DyJsonGetValueFrom<DDyVectorInt2>(j, sGamePlay_Shadow_GlobalDefaultMapSize);
}

void to_json(nlohmann::json& j, const DDySettingGameplay::DDyGraphics& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sGamePlay_Graphics_EnableDefaultSsao),    p.mIsEnabledDefaultSsao},
    {MSVSTR(sGamePlay_Graphics_EnableDefaultShadow),  p.mIsEnabledDefaultShadow},
  };
}

void from_json(const nlohmann::json& j, DDySettingGameplay::DDyGraphics& p)
{
  p.mIsEnabledDefaultSsao   = DyJsonGetValueFrom<bool>(j, sGamePlay_Graphics_EnableDefaultSsao);
  p.mIsEnabledDefaultShadow = DyJsonGetValueFrom<bool>(j, sGamePlay_Graphics_EnableDefaultShadow);
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
  };

  if (p.mPositive.empty() == false) { j[MSVSTR(sPositive)] = p.mPositive; }
  if (p.mNegative.empty() == false) { j[MSVSTR(sNegative)] = p.mNegative; }
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput& p)
{
  p.mAxisMap    = DyJsonGetValueFrom<DDySettingInput::TAxisMap>   (j, sAxis);
  p.mActionMap  = DyJsonGetValueFrom<DDySettingInput::TActionMap> (j, sAction);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DAction& p)
{
  using TKeyListType = DDySettingInput::TKeyListType;
  
  if (DyIsJsonKeyExist(j, MSVSTR(sKey)) == true) { p.mKey = DyJsonGetValueFrom<TKeyListType>(j, sKey); }
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DAxis& p)
{
  using TKeyListType = DDySettingInput::TKeyListType;

  if (DyIsJsonKeyExist(j, MSVSTR(sPositive)) == true) { p.mPositive = DyJsonGetValueFrom<TKeyListType>(j, sPositive); }
  if (DyIsJsonKeyExist(j, MSVSTR(sNegative)) == true) { p.mNegative = DyJsonGetValueFrom<TKeyListType>(j, sNegative); }

  p.mGravity        = DyJsonGetValueFrom<TI32>(j, sGravity);
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
  p.mObjectTag    = DyJsonGetValueFrom<DDySettingTag::TObjectTagList>   (j, sObject);
  p.mCollisionTag = DyJsonGetValueFrom<DDySettingTag::TCollisionTagList>(j, sCollision);
}

///
/// @struct DDySettingSound
/// @brief  Setting sound that able to be serialized.
///

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound& p)
{
  DyJsonGetValueFromTo(j, "3D", p.m3DSetting);
  DyJsonGetValueFromTo(j, "Master",   p.mMaster);
  DyJsonGetValueFromTo(j, "Group",    p.mGroup);
  DyJsonGetValueFromTo(j, "Channel",  p.mChannel);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::D3DSetting& p)
{
  DyJsonGetValueFromTo(j, "DopplerOffset", p.mDopplerOffset);
  DyJsonGetValueFromTo(j, "DistanceUnit", p.mDistanceUnit);
  DyJsonGetValueFromTo(j, "AttenuationFactor", p.mAttenuationFactor);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::DDetail& p)
{
  DyJsonGetValueFromTo(j, "Muted", p.mMuted);
  DyJsonGetValueFromTo(j, "Volume", p.mVolume);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::DChannelDetail& p)
{
  from_json(j, static_cast<DDySettingSound::DDetail&>(p));
  DyJsonGetValueFromTo(j, "Group", p.mGroupSpecifier);

  MDY_ASSERT_FORCE(p.mGroupSpecifier.empty() == false, "Channel's group must be specified.");
}

///
/// @struct DDySettingPhysics
/// @brief Physics setting.
///

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingPhysics& p)
{
  DyJsonGetValueFromTo(j, "Layer",        p.mCollisionTag);
  DyJsonGetValueFromTo(j, "Common",       p.mCommonProperty);
  DyJsonGetValueFromTo(j, "FilterPreset", p.mFilterPresetContainer);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingPhysics::DCommon& p)
{
  DyJsonGetValueFromTo(j, "Gravity",                p.mGravity);
  DyJsonGetValueFromTo(j, "DefaultStaticFriction",  p.mDefaultStaticFriction);
  DyJsonGetValueFromTo(j, "DefaultDynamicFriction", p.mDefaultDynamicFriction);
  DyJsonGetValueFromTo(j, "DefaultRestitution",     p.DefaultRestitution);
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyCollisionFilter& p)
{
  // Get integer value from json.
  const auto val = j.get<TU32>();
  // 0 is block, 1 is overlap, 2 is ignore.
  p = static_cast<EDyCollisionFilter>(val);
}

///
/// @struct DDySettingMetaPath
/// @brief  Setting meta file path that able to be serialized.
///

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingMetaPath& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sHeader_MetaPath_Scene),  p.mSceneMetaPath},
      {MSVSTR(sHeader_MetaPath_Widget), p.mWidgetMetaPath},
      {MSVSTR(sHeader_MetaPath_Prefab), p.mPrefabMetaPath},
      {MSVSTR(sHeader_MetaPath_Script), p.mScriptMetaPath},

      {MSVSTR(sHeader_MetaPath_Font),     p.mFontMetaPath},
      {MSVSTR(sHeader_MetaPath_Texture),  p.mTextureMetaPath},
      {MSVSTR(sHeader_MetaPath_Material), p.mMaterialMetaPath},
      {MSVSTR(sHeader_MetaPath_Shader),   p.mGLShaderMetaPath},

      {MSVSTR(sHeader_MetaPath_Model),    p.mModelMetaPath},
      {MSVSTR(sHeader_MetaPath_ModelMesh),p.mModelMeshMetaPath},
      {MSVSTR(sHeader_MetaPath_ModelAnim),p.mModelAnimMetaPath},
      {MSVSTR(sHeader_MetaPath_ModelSkeleton),p.mModelSkeletonMetaPath},

      {"Sound", p.mSoundMetaPath},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingMetaPath& p)
{
  p.mSceneMetaPath  = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Scene);
  p.mWidgetMetaPath = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Widget);
  p.mPrefabMetaPath = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Prefab);
  p.mScriptMetaPath = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Script);

  p.mFontMetaPath     = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Font);
  p.mTextureMetaPath  = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Texture);
  p.mGLShaderMetaPath = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Shader);
  p.mMaterialMetaPath = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Material);

  p.mModelMetaPath        = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_Model);
  p.mModelMeshMetaPath    = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_ModelMesh);
  p.mModelAnimMetaPath    = DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_ModelAnim);
  p.mModelSkeletonMetaPath= DyJsonGetValueFrom<std::string>(j, sHeader_MetaPath_ModelSkeleton);

  p.mSoundMetaPath = DyJsonGetValueFrom<std::string>(j, "Sound");
}

} /// ::dy namespace