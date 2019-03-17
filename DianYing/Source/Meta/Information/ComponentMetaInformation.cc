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
#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>
#include <Dy/Meta/Components/PCompDirLightMetaInfo.h>
#include <Dy/Meta/Components/PCompPointLightMetaInfo.h>
#include <Dy/Meta/Components/PCompSpotLightMetaInfo.h>

//!
//! Forward declaration & local translation unit functions.
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_Type,      "Type");
MDY_SET_IMMUTABLE_STRING(sHeader_Details,   "Details");
MDY_SET_IMMUTABLE_STRING(sHeader_Activated, "Activated");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const TComponentMetaList& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& iJson, TComponentMetaList& oMeta)
{
  for (const auto& componentAtlas : iJson)
  {
    const auto type = json::GetValueFrom<EDyComponentMetaType>(componentAtlas, sHeader_Type);
    switch (type)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyComponentMetaType::Transform:
      oMeta.emplace_back(type, componentAtlas.get<PDyTransformComponentMetaInfo>());
      break;
    case EDyComponentMetaType::Script:
      oMeta.emplace_back(type, componentAtlas.get<PDyScriptComponentMetaInfo>());
      break;
    case EDyComponentMetaType::DirectionalLight:
      oMeta.emplace_back(type, componentAtlas.get<PDirLightComponentMetaInfo>());
      break;
    case EDyComponentMetaType::PointLight:
      oMeta.emplace_back(type, componentAtlas.get<PCompPointLightMetaInfo>());
      break;
    case EDyComponentMetaType::SpotLight:
      oMeta.emplace_back(type, componentAtlas.get<PCompSpotLightMetaInfo>());
      break;
    case EDyComponentMetaType::ModelFilter:
      oMeta.emplace_back(type, componentAtlas.get<PDyModelFilterComponentMetaInfo>());
      break;
    case EDyComponentMetaType::ModelRenderer:
      oMeta.emplace_back(type, componentAtlas.get<PDyModelRendererComponentMetaInfo>());
      break;
    case EDyComponentMetaType::ModelAnimator:
      oMeta.emplace_back(type, componentAtlas.get<PDyModelAnimatorComponentMetaInfo>());
      break;
    case EDyComponentMetaType::Camera:
      oMeta.emplace_back(type, componentAtlas.get<PDyCameraComponentMetaInfo>());
      break;
    case EDyComponentMetaType::SoundSource:
      oMeta.emplace_back(type, componentAtlas.get<PDySoundSourceComponentMetaInfo>());
      break;
    case EDyComponentMetaType::Rigidbody:
      oMeta.emplace_back(type, componentAtlas.get<PDyRigidbodyComponentMetaInfo>());
      break;
    case EDyComponentMetaType::Collider:
      oMeta.emplace_back(type, componentAtlas.get<PDyColliderComponentMetaInfo>());
      break;
    case EDyComponentMetaType::Skybox:
      oMeta.emplace_back(type, componentAtlas.get<PDySkyboxComponentMetaInfo>());
      break;
    }
  }
}

//!
//! PDyTranformComponentMetaInfo
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyTransformComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {(sHeader_Type),    p.mType},
    {(sHeader_Details), p.mDetails},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTransformComponentMetaInfo& p)
{
  using TDDetails = PDyTransformComponentMetaInfo::DDetails;
  json::GetValueFromTo<EDyComponentMetaType>(j, sHeader_Type, p.mType);
  json::GetValueFromTo<TDDetails>           (j, sHeader_Details, p.mDetails);
}

void to_json(nlohmann::json& j, const PDyTransformComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {(sHeaderLocalPosition),  p.mLocalPosition},
    {(sHeaderLocalAngle),     p.mLocalRotation},
    {(sHeaderLocalScale),     p.mLocalScale},

    {(sHeaderWorldPosition),  p.mWorldPosition},
    {(sHeaderWorldAngle),     p.mWorldRotation},
    {(sHeaderWorldScale),     p.mWorldScale},
  };;
}

void from_json(const nlohmann::json& j, PDyTransformComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo<DVector3>(j, sHeaderLocalPosition, p.mLocalPosition);
  json::GetValueFromTo<DVector3>(j, sHeaderLocalAngle, p.mLocalRotation);
  json::GetValueFromTo<DVector3>(j, sHeaderLocalScale, p.mLocalScale);

  json::GetValueFromTo<DVector3>(j, sHeaderWorldPosition, p.mWorldPosition);
  json::GetValueFromTo<DVector3>(j, sHeaderWorldAngle, p.mWorldRotation);
  json::GetValueFromTo<DVector3>(j, sHeaderWorldScale, p.mWorldScale);
}

//!
//! PDyScriptComponentMetaInformation
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyScriptComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {(sHeader_Type),      p.mType},
    {(sHeader_Details),   p.mDetails},
    {(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyScriptComponentMetaInfo& p)
{
  using TDDetails = PDyScriptComponentMetaInfo::DDetails;
  json::GetValueFromTo<EDyComponentMetaType>(j, sHeader_Type,       p.mType);
  json::GetValueFromTo<TDDetails>           (j, sHeader_Details,    p.mDetails);
  json::GetValueFromTo<bool>                (j, sHeader_Activated,  p.mInitiallyActivated);
}

//!
//! PDyModelFilterComponentMetaInfo
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelFilterComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {(sHeader_Type),      p.mType},
    {(sHeader_Details),   p.mDetails},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ PDyModelFilterComponentMetaInfo& p)
{
  using TDDetails = PDyModelFilterComponentMetaInfo::DDetails;
  json::GetValueFromTo<EDyComponentMetaType>(j, sHeader_Type,       p.mType);
  json::GetValueFromTo<TDDetails>           (j, sHeader_Details,    p.mDetails);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelFilterComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {(sHeaderModelName), p.mModelSpecifierName}
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelFilterComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo<std::string>(j, sHeaderModelName, p.mModelSpecifierName);
}

//!
//! PDyModelRendererComponentMetaInfo
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelRendererComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {(sHeader_Type),      p.mType},
    {(sHeader_Details),   p.mDetails},
    {(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelRendererComponentMetaInfo& p)
{
  using TDDetails = PDyModelRendererComponentMetaInfo::DDetails;
  json::GetValueFromTo<EDyComponentMetaType>(j, sHeader_Type,       p.mType);
  json::GetValueFromTo<TDDetails>           (j, sHeader_Details,    p.mDetails);
  json::GetValueFromTo<bool>                (j, sHeader_Activated,  p.mInitiallyActivated);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelRendererComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {(sHeaderShadow),      p.mIsEnabledCreateShadow},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelRendererComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo<bool>(j, sHeaderShadow, p.mIsEnabledCreateShadow);
}

//!
//! PDyModelAnimatorComponentMetaInfo
//!

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelAnimatorComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {(sHeader_Type),      p.mType},
    {(sHeader_Details),   p.mDetails},
    {(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimatorComponentMetaInfo& p)
{
  using TDDetails = PDyModelAnimatorComponentMetaInfo::DDetails;
  json::GetValueFromTo<EDyComponentMetaType>(j, sHeader_Type,       p.mType);
  json::GetValueFromTo<TDDetails>           (j, sHeader_Details,    p.mDetails);
  json::GetValueFromTo<bool>                (j, sHeader_Activated,  p.mInitiallyActivated);
}

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelAnimatorComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {"TempAnimation",     p.mTempAnimationScrap},
    {"SkeletonSpecifier", p.mSkeletonSpecifier},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimatorComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo(j, "TempAnimation",      p.mTempAnimationScrap);
  json::GetValueFromTo(j, "SkeletonSpecifier",  p.mSkeletonSpecifier);
}

//!
//! PDyCameraComponentMetaInfo
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyCameraComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {(sHeader_Type),      p.mType},
    {(sHeader_Details),   p.mDetails},
    {(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCameraComponentMetaInfo& p)
{
  json::GetValueFromTo(j, sHeader_Type,       p.mType);
  json::GetValueFromTo(j, sHeader_Details,    p.mDetails);
  json::GetValueFromTo(j, sHeader_Activated,  p.mInitiallyActivated);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyCameraComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {(sHeaderFieldOfView),      p.mInitialFieldOfView},
    {(sHeaderProjection),       p.mProjectionType},

    {(sHeaderClippingNear),     p.mNear},
    {(sHeaderClippingFar),      p.mFar},
    {(sHeaderViewportRect),     p.mViewportSize},

    {(sHeaderIsFocusInstantly), p.mIsFocusInstantly},
    {(sHeaderIsMeshUnclipped),  p.mIsEnableMeshUnClipped},
    {"Is3DListener",                  p.mIs3DListener},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCameraComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo(j, sHeaderFieldOfView,       p.mInitialFieldOfView);
  json::GetValueFromTo(j, sHeaderProjection,        p.mProjectionType);

  json::GetValueFromTo(j, sHeaderClippingNear,      p.mNear);
  json::GetValueFromTo(j, sHeaderClippingFar,       p.mFar);
  json::GetValueFromTo(j, sHeaderViewportRect,      p.mViewportSize);

  json::GetValueFromTo(j, sHeaderIsFocusInstantly,  p.mIsFocusInstantly);
  json::GetValueFromTo(j, sHeaderIsMeshUnclipped,   p.mIsEnableMeshUnClipped);
  json::GetValueFromTo(j, "Is3DListener",           p.mIs3DListener);
}

//!
//! PDySoundSourceComponentMetaInfo
//!

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDySoundSourceComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {(sHeader_Type),      p.mType},
    {(sHeader_Details),   p.mDetails},
    {(sHeader_Activated), p.mInitiallyActivated},
  };;
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo& p)
{
  json::GetValueFromTo(j, sHeader_Type,       p.mType);
  json::GetValueFromTo(j, sHeader_Details,    p.mDetails);
  json::GetValueFromTo(j, sHeader_Activated,  p.mInitiallyActivated);
}

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDySoundSourceComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo(j, "SoundSpecifier", p.mSoundSpecifier);

  json::GetValueFromTo(j, "2DSound",  p.m2DSound);
  json::GetValueFromTo(j, "Muted",    p.mMuted);
  json::GetValueFromTo(j, "Looped",   p.mLooped);

  json::GetValueFromTo(j, "VolumeMultiplier", p.mVolumeMultiplier);
  json::GetValueFromTo(j, "PitchMultiplier",  p.mPitchMultiplier);
  json::GetValueFromTo(j, "Channel",          p.mChannelSpecifier);

  json::GetValueFromTo(j, "Attenuation", p.mAttenuation);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo::DAttenuation& p)
{
  json::GetValueFromTo(j, "Activated", p.mActivated);
  json::GetValueFromTo(j, "Near", p.mNearDistance);
  json::GetValueFromTo(j, "Far",  p.mFarDistance);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyRigidbodyComponentMetaInfo& p)
{
  json::GetValueFromTo(j, sHeader_Type,       p.mType);
  json::GetValueFromTo(j, sHeader_Details,    p.mDetails);
  json::GetValueFromTo(j, sHeader_Activated,  p.mInitiallyActivated);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyRigidbodyComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo(j, "IsSimulatePhysics", p.mIsSimulatePhysics);
  json::GetValueFromTo(j, "IsEnableGravity", p.mIsEnableGravity);

  json::GetValueFromTo(j, "MassInKg", p.mMassInKg);
  
  json::GetValueFromTo(j, "LinearDamping", p.mLinearDamping);
  json::GetValueFromTo(j, "AngularDamping", p.mAngularDamping);

  json::GetValueFromTo(j, "LockPos", p.mLockPosition);
  json::GetValueFromTo(j, "LockRot", p.mLockPosition);
  json::GetValueFromTo(j, "LockPreset", p.mLockPreset);

  json::GetValueFromTo(j, "Type", p.mType);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyColliderComponentMetaInfo& p)
{
  json::GetValueFromTo(j, sHeader_Type,       p.mType);
  json::GetValueFromTo(j, sHeader_Details,    p.mDetails);
  json::GetValueFromTo(j, sHeader_Activated,  p.mInitiallyActivated);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyColliderComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo(j, "IsNotifyEvent", p.mNotifyHitEvent);
  json::GetValueFromTo(j, "IsOverlapEvent", p.mNotifyOverlapEvent);
  json::GetValueFromTo(j, "ColliderType", p.mColliderType);

  switch (p.mColliderType)
  {
  case EDyColliderType::Sphere: 
  { p.mColliderDetails = json::GetValueFrom<TColliderBindingType<EDyColliderType::Sphere>::Type>(j, "ColliderDetails");
  } break;
  case EDyColliderType::Capsule:
  { p.mColliderDetails = json::GetValueFrom<TColliderBindingType<EDyColliderType::Capsule>::Type>(j, "ColliderDetails");
  } break;
  case EDyColliderType::Box:
  { p.mColliderDetails = json::GetValueFrom<TColliderBindingType<EDyColliderType::Box>::Type>(j, "ColliderDetails");
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  json::GetValueFromTo(j, "FilterPresetSpecifier", p.mCollisionFilterPresetSpecifier);
  json::GetValueFromTo(j, "CollisionFilter", p.mCollisionFilter);
  json::GetValueFromTo(j, "LayerName", p.mCollisionLayerName);
}

//!
//! PDySkyboxComponentMetaInfo
//!

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySkyboxComponentMetaInfo& p)
{
  json::GetValueFromTo(j, sHeader_Type,       p.mType);
  json::GetValueFromTo(j, sHeader_Details,    p.mDetails);
  json::GetValueFromTo(j, sHeader_Activated,  p.mInitiallyActivated);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySkyboxComponentMetaInfo::DDetails& p)
{
  json::GetValueFromTo(j, "Exposure", p.mExposure);
  json::GetValueFromTo(j, "Rotation", p.mRotation);
  json::GetValueFromTo(j, "TintColor", p.mTintColor);

  json::GetValueFromTo(j, "CubemapSpecifier", p.mCubemapSpecifier);
}

} /// ::dy namespace