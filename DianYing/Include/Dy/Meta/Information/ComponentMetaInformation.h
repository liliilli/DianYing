#ifndef GUARD_DY_META_INFORMATION_COMPONENTMETAINFORMATION_H
#define GUARD_DY_META_INFORMATION_COMPONENTMETAINFORMATION_H
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

#include <any>
#include <variant>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Meta/Type/EDyComponentTypes.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Helper/Type/Area2D.h>
#include <Dy/Helper/Type/Clamp.h>
#include <Dy/Management/Type/SettingContainer.h>
#include <Dy/Meta/Type/Physics/EDyColliderType.h>

namespace dy
{

///
/// @brief Component meta information type.
/// Used at \n
/// `PDyPrefabInstanceMetaInfo` \n
/// `PDyObjectMetaInfo`.
///
using TComponentMetaList  = std::vector<std::pair<EDyComponentMetaType, std::any>>;
void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const TComponentMetaList& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ TComponentMetaList& p);

///
/// @struct IDyMetaInformation
/// @brief Dependency information
///
struct IDyMetaInformation
{
  /// This mType must be EDyCDyComponentType::Script.
  EDyComponentMetaType mType = EDyComponentMetaType::NoneError;
};

///
/// @struct PDyTransformComponentMetaInfo
/// @brief  World transform information.
///
struct PDyTransformComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    /// Local position
    DDyVector3        mLocalPosition = {};
    /// Local rotation
    DDyVector3        mLocalRotation = {};
    /// Local scale
    DDyVector3        mLocalScale = DDyVector3{ 1, 1, 1 };
    /// World position
    DDyVector3        mWorldPosition = {};
    /// World rotation
    DDyVector3        mWorldRotation = {};
    /// World scale
    DDyVector3        mWorldScale = DDyVector3{ 1, 1, 1 };
  };

  /// Details
  DDetails mDetails = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyTransformComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTransformComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyTransformComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTransformComponentMetaInfo::DDetails& p);

///
/// @struct PDyScriptComponentMetaInfo
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo when mType is FDyPawn.
///
struct PDyScriptComponentMetaInfo final : public IDyMetaInformation
{
  using DDetails = PDyScriptReferenceMetaInfo;

  /// Details
  DDetails        mDetails            = {};
  /// Component is initially activated or not.
  bool            mInitiallyActivated = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyScriptComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyScriptComponentMetaInfo& p);

///
/// @struct PDyModelFilterComponentMetaInfo
/// @brief
///
struct PDyModelFilterComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    std::string mModelSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  };

  /// Details
  DDetails mDetails = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelFilterComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelFilterComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelFilterComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelFilterComponentMetaInfo::DDetails& p);

///
/// @struct PDyModelRendererComponentMetaInfo
/// @brief  Dependency information to CDYModelRenderer component.
/// (EDyComponentMetaType::ModelRenderer)
///
struct PDyModelRendererComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    /// Create shadow or not.
    bool                      mIsEnabledCreateShadow  = false;
  };

  /// Details
  DDetails mDetails = {};
  /// Component is initially activated or not.
  bool     mInitiallyActivated     = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelRendererComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelRendererComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelRendererComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelRendererComponentMetaInfo::DDetails& p);

///
/// @struct PDyModelAnimatorComponentMetaInfo
/// @brief  Dependency information to CDYModelAnimator component.
/// (EDyComponentMetaType::ModelAnimator)
///
struct PDyModelAnimatorComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    std::string mTempAnimationScrap = MDY_INITIALIZE_EMPTYSTR;
    std::string mSkeletonSpecifier  = MDY_INITIALIZE_EMPTYSTR;
  };

  /// Details
  DDetails mDetails = {};
  /// Component is initially activated or not.
  bool     mInitiallyActivated = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelAnimatorComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimatorComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelAnimatorComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimatorComponentMetaInfo::DDetails& p);

///
/// @struct PDyCameraComponentMetaInfo
/// @brief  Dependency information to CDyCamera (NEW! since 0.0.0~) component.
///
struct PDyCameraComponentMetaInfo final : public IDyMetaInformation
{
private:
  inline static TF32 sInitialNear    = 0.3f;
  inline static TF32 sInitialFar     = 1000.f;
public:
  struct DDetails final
  {
    /// Fov must be range from 0.1f to 180.f
    TF32       mInitialFieldOfView     = 70.f;
    ///
    EDyCameraProjectionType mProjectionType = EDyCameraProjectionType::NoneError;
    /// Near value of camera culling.
    TF32       mNear                   = PDyCameraComponentMetaInfo::sInitialNear;
    /// Far value of camera culling.
    TF32       mFar                    = PDyCameraComponentMetaInfo::sInitialFar;
    /// Viewport area
    /// Custom viewport size, but mIsCustomViewport must be enabled.
    DDyArea2D   mViewportSize           = {};
    /// Camera will be focused instantly.
    bool        mIsFocusInstantly       = false;
    /// Clipped camera will be unclipped.
    bool        mIsEnableMeshUnClipped  = false;
    /// This camera is 3D sound listener.
    bool        mIs3DListener = false;
  };
  /// Details
  DDetails    mDetails                = {};
  /// Component is initially activated or not.
  bool        mInitiallyActivated     = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyCameraComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCameraComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyCameraComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCameraComponentMetaInfo::DDetails& p);

///
/// @struct PDySoundSourceComponentMetaInfo
/// @brief Dependent information to CDySoundSource component.
///
struct PDySoundSourceComponentMetaInfo final : public IDyMetaInformation
{
public:
  struct DAttenuation final
  {
    bool mActivated = false;
    DDyClamp<TF32, 0, 1'000'000> mNearDistance  = 0.5f;
    DDyClamp<TF32, 0, 1'000'000> mFarDistance   = 5000.f;
  };

  struct DDetails final
  {
    std::string mSoundSpecifier; // Sound specifier.

    bool m2DSound = false; // Check this component is 2d sound or 3d.
    bool mMuted   = false; // CHeck this component is muted.
    bool mLooped  = false; // Check this component's sound will be played looped.

    DDyClamp<TF32, 0, 5> mVolumeMultiplier  = 1.0f; // Sound channel's volume multiplication.
    DDyClamp<TF32, 0, 5> mPitchMultiplier   = 1.0f; // Sound channel's pitch multiplication.
    std::string mChannelSpecifier; // If blank, this sound just use master channel.

    DAttenuation mAttenuation; // 3D Atteunation properties.
  };

  /// Details
  DDetails    mDetails            = {};
  /// Component is initially activated or not.
  bool        mInitiallyActivated = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDySoundSourceComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDySoundSourceComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo::DDetails& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDySoundSourceComponentMetaInfo::DAttenuation& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo::DAttenuation& p);

///
/// @struct PDyRigidbodyComponentMetaInfo
/// @brief Dependent information to CDyRididbody (only one or not per one actor) component. 
///
struct PDyRigidbodyComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    /// Check this rigidbody (and collider) simulate physics.
    bool mIsSimulatePhysics = false;
    /// Enable gravity or not.
    bool mIsEnableGravity = false;
    /// Mass of sum of collider.
    DDyClamp<TF32, 0, 100'000>  mMassInKg = 0.001f;
    /// Linear damping of rigidbody
    DDyClamp<TF32, 0, 10'000>   mLinearDamping = 1.0f;
    /// Angular damping of rigidbody
    DDyClamp<TF32, 0, 10'000>   mAngularDamping = 1.0f; 
    /// Lock psoition axis.
    DLockPreset::D3DAxis mLockPosition{};
    /// Lock rotation axis.
    DLockPreset::D3DAxis mLockRotation{};
    /// Lock preset.
    std::string mLockPreset = MDY_INITIALIZE_EMPTYSTR;
  };
 
  /// @brief Details
  DDetails mDetails;
  /// @brief Component is initially activated or not.
  bool     mInitiallyActivated = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyRigidbodyComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyRigidbodyComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyRigidbodyComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyRigidbodyComponentMetaInfo::DDetails& p);

///
/// @struct PDyColliderComponentMetaInfo
/// @brief Dependent information to CDyCollider (only one or not per one actor) component. 
/// https://docs.unrealengine.com/portals/0/images/Engine/Physics/Collision/Overview/COL_collideNoEvent_Sphere.png
///
struct PDyColliderComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    /// If true, this component notify hit event (callback) when target hit.
    bool mNotifyHitEvent = false;
    /// If true, this component notify overlap event when target overlapped.
    bool mNotifyOverlapEvent = false;
    /// Collider type of this component.
    EDyColliderType mColliderType = EDyColliderType::NoneError;
    /// Collider details.
    std::variant<
        TColliderBindingType<EDyColliderType::Box>::Type,
        TColliderBindingType<EDyColliderType::Capsule>::Type,
        TColliderBindingType<EDyColliderType::Sphere>::Type
    > mColliderDetails;
    /// Collision filter specifier. If not empty, load collision filter and override value.
    std::string mCollisionFilterPresetSpecifier = MDY_INITIALIZE_EMPTYSTR;
    /// Collision filter. each value of id is correspond to Collision layer list's specifier.
    std::vector<EDyCollisionFilter> mCollisionFilter;
  };

  /// @brief Details
  DDetails mDetails;
  /// @brief Component is initially activated or not.
  bool     mInitiallyActivated = false;
};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyColliderComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyColliderComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyColliderComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyColliderComponentMetaInfo::DDetails& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_COMPONENTMETAINFORMATION_H