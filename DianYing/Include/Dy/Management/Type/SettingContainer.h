#ifndef GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H
#define GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H
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

#include <vector>
#include <nlohmann/json_fwd.hpp>

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Meta/Type/Input/EDyInputButton.h>
#include <Dy/Meta/Type/Physics/EDyCollisionFilter.h>
#include <Dy/Helper/Type/Clamp.h>

namespace dy
{

///
/// @struct DDySettingDescription
/// @brief  Setting description able to be serialized.
///
struct DDySettingDescription final
{
  /// Project name of this project.
  std::string mProjectName    = MDY_INITIALIZE_EMPTYSTR;
  /// Window name of this project. if empty, just use MDySetting::mProjectName as window name with version.
  std::string mWindowName     = MDY_INITIALIZE_EMPTYSTR;
  /// [High. Mid. Low]
  TI32 mVersionHigh           = MDY_INITIALIZE_DEFINT;
  TI32 mVersionMid            = MDY_INITIALIZE_DEFINT;
  TI32 mVersionLow            = MDY_INITIALIZE_DEFINT;
  ///
  std::string mCompanyName    = MDY_INITIALIZE_EMPTYSTR;
  std::string mHomepage       = MDY_INITIALIZE_EMPTYSTR;
  std::string mSupportContact = MDY_INITIALIZE_EMPTYSTR;
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

  struct DDyGraphics final
  {
    /// @brief Enable default CSM shading mapping. Have a effect to performance.
    bool mIsEnabledDefaultShadow  = false;
    /// @brief Enable default post-process SSAO occlusion.
    bool mIsEnabledDefaultSsao    = false;
  };

  /// Initial scene name. (or uuid?)
  std::string   mInitialSceneSpecifier           = MDY_INITIALIZE_EMPTYSTR;
  /// Initial game resolution scene.
  DDyVectorInt2 mInitialResolution      = {};
  /// Shadow default map resolution or properties.
  DDyShadow     mShadow                 = {};
  /// Graphics properties.
  DDyGraphics   mGraphics               = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingGameplay& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingGameplay::DDyShadow& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay::DDyShadow& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingGameplay::DDyGraphics& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingGameplay::DDyGraphics& p);

///
/// @struct DDySettingInput
/// @brief
///
struct DDySettingInput
{
  using TKeyListType = std::vector<EDyButton>;

  struct DAction final
  {
    std::string  mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
    TKeyListType mKey            = {};

    ///
    /// @brief  Create `DAxis` with valid json atlas.
    /// @param  json valid json atlas.
    /// @param  specifierName `DAxis::mSpecifierName`.
    /// @return If succeeded, just return `DAxis` instance.
    ///
    MDY_NODISCARD static DAction CreateInstance(_MIN_ const nlohmann::json& json, _MIN_ const std::string& specifierName);
  };

  struct DAxis final
  {
    std::string  mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
    TKeyListType mPositive       = {};
    TKeyListType mNegative       = {};
    TF32         mGravity        = MDY_INITIALIZE_DEFINT;

    ///
    /// @brief  Create `DAxis` with valid json atlas.
    /// @param  json valid json atlas.
    /// @param  specifierName `DAxis::mSpecifierName`.
    /// @return If succeeded, just return `DAxis` instance.
    ///
    MDY_NODISCARD static DAxis CreateInstance(_MIN_ const nlohmann::json& json, _MIN_ const std::string& specifierName);
  };

  using TActionMap  = std::unordered_map<std::string, DAction>;
  using TAxisMap    = std::unordered_map<std::string, DAxis>;

  TActionMap mActionMap = {};
  TAxisMap   mAxisMap   = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingInput& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingInput::DAction& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DAction& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingInput::DAxis& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingInput::DAxis& p);

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

///
/// @struct DDySettingSound
/// @brief  Setting sound that able to be serialized.
///
struct DDySettingSound final
{
  struct DDetail
  {
    DDyClamp<TF32, 0, 1> mVolume = 0.0f;
    bool mMuted = false;
  };
  struct DChannelDetail final : public DDetail
  {
    std::string mGroupSpecifier = "";
  };
  struct D3DSetting final
  {
    TF32 mDopplerOffset = 1.0f;
    TF32 mDistanceUnit  = 1.0f;
    TF32 mAttenuationFactor = 1.0f;
  };

  /// @brief Setting properties for 3D sound.
  D3DSetting                      m3DSetting;
  /// @brief Master is master channel of sound system.
  DDetail                         mMaster;
  /// @brief Group is a collection of channels.
  TStringHashMap<DDetail>         mGroup;
  /// @brief Channel is a collection of instant & created sound.
  TStringHashMap<DChannelDetail>  mChannel;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingSound& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingSound::D3DSetting& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::D3DSetting& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingSound::DDetail& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::DDetail& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingSound::DChannelDetail& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingSound::DChannelDetail& p);

///
/// @struct DDySettingPhysics
/// @brief Physics setting.
///
struct DDySettingPhysics final
{
public:
  using TCollisionTagList = std::vector<std::string>;
  /// @brief Filter list's index is correspond to TCollisionTagList's index. \n
  /// If readen value is 0, Key layer => Dest layer is blocked. \n
  /// If readen value is 1, Key layer => Dest layer is overlapped. \n
  /// If readen value is 2, Key layer => Dest layer is ignored.
  using TFilterList = std::vector<EDyCollisionFilter>;
  /// @brief Filter layer response table. \n
  /// _A_\_B_ Block   Overlap Ignore \n
  /// Block   Block   Overlap Ignore \n
  /// Overlap Overlap Overlap Ignore \n
  /// Ignore  Ignore  Ignore  Ignore
  using TFilterPreset = std::unordered_map<std::string, TFilterList>;
  struct DCommon final
  {
    // Global gravity
    TF32 mGravity = 0.0f;
    // Static collider's friction (摩擦)
    TF32 mDefaultStaticFriction = 0.0f;
    // Dynamic collider's friction (動的摩擦)
    TF32 mDefaultDynamicFriction = 0.0f;
    // Global restitution (跳ね返す程度)
    TF32 DefaultRestitution = 0.0f;
  };

  DCommon           mCommonProperty = {};
  TCollisionTagList mCollisionTag  = {};
  TFilterPreset     mFilterPresetContainer = {};
};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingPhysics& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingPhysics& p);

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const DDySettingPhysics::DCommon& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingPhysics::DCommon& p);

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const EDyCollisionFilter& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyCollisionFilter& p);

///
/// @struct DDySettingMetaPath
/// @brief  Setting meta file path that able to be serialized.
///
struct DDySettingMetaPath final
{
  // Meta strings
  std::string mSceneMetaPath    = MDY_INITIALIZE_EMPTYSTR;
  std::string mWidgetMetaPath   = MDY_INITIALIZE_EMPTYSTR;
  std::string mPrefabMetaPath   = MDY_INITIALIZE_EMPTYSTR;
  std::string mScriptMetaPath   = MDY_INITIALIZE_EMPTYSTR;

  // Resources
  std::string mFontMetaPath     = MDY_INITIALIZE_EMPTYSTR;
  std::string mModelMetaPath    = MDY_INITIALIZE_EMPTYSTR;
  std::string mModelMeshMetaPath= MDY_INITIALIZE_EMPTYSTR;
  std::string mModelAnimMetaPath= MDY_INITIALIZE_EMPTYSTR;
  std::string mModelSkeletonMetaPath = MDY_INITIALIZE_EMPTYSTR;
  std::string mTextureMetaPath  = MDY_INITIALIZE_EMPTYSTR;
  std::string mGLShaderMetaPath = MDY_INITIALIZE_EMPTYSTR;
  std::string mMaterialMetaPath = MDY_INITIALIZE_EMPTYSTR;
  std::string mSoundMetaPath    = MDY_INITIALIZE_EMPTYSTR;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDySettingMetaPath& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySettingMetaPath& p);

} /// ::dy namespace


#endif /// GUARD_DY_MANAGEMENT_TYPE_SETTINGCONTAINER_H