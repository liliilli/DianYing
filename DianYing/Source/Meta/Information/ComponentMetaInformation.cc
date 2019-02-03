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
#include <Dy/Meta/Information/ComponentLightMetaInfo.h>

//!
//! Forward declaration & local translation unit functions.
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_Type,      "Type");
MDY_SET_IMMUTABLE_STRING(sHeader_Details,   "Details");
MDY_SET_IMMUTABLE_STRING(sHeader_Activated, "Activated");

///
/// @brief  Get viewport rectangle size from proper jsonAtlas, save it to metaInfo as input value.
/// @param  jsonAtlas
/// @param  metaInfo
///
void DyGetViewportRectFromJson(_MIN_ const nlohmann::json& jsonAtlas, _MOUT_ dy::PDyCameraComponentMetaInfo& metaInfo)
{
  // Calculate
  dy::DDyVector2 viewportRectXY = {};
  viewportRectXY.X = jsonAtlas.at("X").get<TF32>();
  viewportRectXY.Y = jsonAtlas.at("Y").get<TF32>();

  dy::DDyVector2 viewportRectWH = {};
  viewportRectWH.X = jsonAtlas.at("W").get<TF32>();
  viewportRectWH.Y = jsonAtlas.at("H").get<TF32>();

  // Update value.
  metaInfo.mDetails.mViewportSize.mLeftDown = viewportRectXY;
  metaInfo.mDetails.mViewportSize.mRightUp  = viewportRectWH;
}

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

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ TComponentMetaList& p)
{
  for (const auto& componentAtlas : j)
  {
    const auto type = DyJsonGetValueFrom<EDyComponentMetaType>(componentAtlas, sHeader_Type);
    switch (type)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyComponentMetaType::Transform:
      p.emplace_back(type, componentAtlas.get<PDyTransformComponentMetaInfo>());
      break;
    case EDyComponentMetaType::Script:
      p.emplace_back(type, componentAtlas.get<PDyScriptComponentMetaInfo>());
      break;
    case EDyComponentMetaType::DirectionalLight:
      p.emplace_back(type, componentAtlas.get<PDyDirLightComponentMetaInfo>());
      break;
    case EDyComponentMetaType::ModelFilter:
      p.emplace_back(type, componentAtlas.get<PDyModelFilterComponentMetaInfo>());
      break;
    case EDyComponentMetaType::ModelRenderer:
      p.emplace_back(type, componentAtlas.get<PDyModelRendererComponentMetaInfo>());
      break;
    case EDyComponentMetaType::ModelAnimator:
      p.emplace_back(type, componentAtlas.get<PDyModelAnimatorComponentMetaInfo>());
      break;
    case EDyComponentMetaType::Camera:
      p.emplace_back(type, componentAtlas.get<PDyCameraComponentMetaInfo>());
      break;
    case EDyComponentMetaType::SoundSource:
      p.emplace_back(type, componentAtlas.get<PDySoundSourceComponentMetaInfo>());
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
    {MSVSTR(sHeader_Type),    p.mType},
    {MSVSTR(sHeader_Details), p.mDetails},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTransformComponentMetaInfo& p)
{
  using TDDetails = PDyTransformComponentMetaInfo::DDetails;
  DyJsonGetValueFromTo<EDyComponentMetaType>(j, sHeader_Type, p.mType);
  DyJsonGetValueFromTo<TDDetails>           (j, sHeader_Details, p.mDetails);
}

void to_json(nlohmann::json& j, const PDyTransformComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeaderLocalPosition),  p.mLocalPosition},
    {MSVSTR(sHeaderLocalAngle),     p.mLocalRotation},
    {MSVSTR(sHeaderLocalScale),     p.mLocalScale},

    {MSVSTR(sHeaderWorldPosition),  p.mWorldPosition},
    {MSVSTR(sHeaderWorldAngle),     p.mWorldRotation},
    {MSVSTR(sHeaderWorldScale),     p.mWorldScale},
  };;
}

void from_json(const nlohmann::json& j, PDyTransformComponentMetaInfo::DDetails& p)
{
  DyJsonGetValueFromTo<DDyVector3>(j, sHeaderLocalPosition, p.mLocalPosition);
  DyJsonGetValueFromTo<DDyVector3>(j, sHeaderLocalAngle, p.mLocalRotation);
  DyJsonGetValueFromTo<DDyVector3>(j, sHeaderLocalScale, p.mLocalScale);

  DyJsonGetValueFromTo<DDyVector3>(j, sHeaderWorldPosition, p.mWorldPosition);
  DyJsonGetValueFromTo<DDyVector3>(j, sHeaderWorldAngle, p.mWorldRotation);
  DyJsonGetValueFromTo<DDyVector3>(j, sHeaderWorldScale, p.mWorldScale);
}

//!
//! PDyScriptComponentMetaInformation
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyScriptComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeader_Type),      p.mType},
    {MSVSTR(sHeader_Details),   p.mDetails},
    {MSVSTR(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyScriptComponentMetaInfo& p)
{
  using TDDetails = PDyScriptComponentMetaInfo::DDetails;
  DyJsonGetValueFromTo<EDyComponentMetaType>(j, sHeader_Type,       p.mType);
  DyJsonGetValueFromTo<TDDetails>           (j, sHeader_Details,    p.mDetails);
  DyJsonGetValueFromTo<bool>                (j, sHeader_Activated,  p.mInitiallyActivated);
}

//!
//! PDyModelFilterComponentMetaInfo
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelFilterComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeader_Type),      p.mType},
    {MSVSTR(sHeader_Details),   p.mDetails},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ PDyModelFilterComponentMetaInfo& p)
{
  using TDDetails = PDyModelFilterComponentMetaInfo::DDetails;
  DyJsonGetValueFromTo<EDyComponentMetaType>(j, sHeader_Type,       p.mType);
  DyJsonGetValueFromTo<TDDetails>           (j, sHeader_Details,    p.mDetails);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelFilterComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeaderModelName), p.mModelSpecifierName}
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelFilterComponentMetaInfo::DDetails& p)
{
  DyJsonGetValueFromTo<std::string>(j, sHeaderModelName, p.mModelSpecifierName);
}

//!
//! PDyModelRendererComponentMetaInfo
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelRendererComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeader_Type),      p.mType},
    {MSVSTR(sHeader_Details),   p.mDetails},
    {MSVSTR(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelRendererComponentMetaInfo& p)
{
  using TDDetails = PDyModelRendererComponentMetaInfo::DDetails;
  DyJsonGetValueFromTo<EDyComponentMetaType>(j, sHeader_Type,       p.mType);
  DyJsonGetValueFromTo<TDDetails>           (j, sHeader_Details,    p.mDetails);
  DyJsonGetValueFromTo<bool>                (j, sHeader_Activated,  p.mInitiallyActivated);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelRendererComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeaderShadow),      p.mIsEnabledCreateShadow},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelRendererComponentMetaInfo::DDetails& p)
{
  DyJsonGetValueFromTo<bool>(j, sHeaderShadow, p.mIsEnabledCreateShadow);
}

//!
//! PDyModelAnimatorComponentMetaInfo
//!

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyModelAnimatorComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeader_Type),      p.mType},
    {MSVSTR(sHeader_Details),   p.mDetails},
    {MSVSTR(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelAnimatorComponentMetaInfo& p)
{
  using TDDetails = PDyModelAnimatorComponentMetaInfo::DDetails;
  DyJsonGetValueFromTo<EDyComponentMetaType>(j, sHeader_Type,       p.mType);
  DyJsonGetValueFromTo<TDDetails>           (j, sHeader_Details,    p.mDetails);
  DyJsonGetValueFromTo<bool>                (j, sHeader_Activated,  p.mInitiallyActivated);
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
  DyJsonGetValueFromTo(j, "TempAnimation",      p.mTempAnimationScrap);
  DyJsonGetValueFromTo(j, "SkeletonSpecifier",  p.mSkeletonSpecifier);
}

//!
//! PDyCameraComponentMetaInfo
//!

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyCameraComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeader_Type),      p.mType},
    {MSVSTR(sHeader_Details),   p.mDetails},
    {MSVSTR(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCameraComponentMetaInfo& p)
{
  DyJsonGetValueFromTo(j, sHeader_Type,       p.mType);
  DyJsonGetValueFromTo(j, sHeader_Details,    p.mDetails);
  DyJsonGetValueFromTo(j, sHeader_Activated,  p.mInitiallyActivated);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyCameraComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeaderClippingNear),     p.mNear},
    {MSVSTR(sHeaderClippingFar),      p.mFar},
    {MSVSTR(sHeaderIsFocusInstantly), p.mIsFocusInstantly},
    {MSVSTR(sHeaderFieldOfView),      p.mInitialFieldOfView},
    {MSVSTR(sHeaderViewportRect),     p.mViewportSize}
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCameraComponentMetaInfo::DDetails& p)
{
  DyJsonGetValueFromTo(j, sHeaderFieldOfView,       p.mInitialFieldOfView);
  DyJsonGetValueFromTo(j, sHeaderProjection,        p.mProjectionType);

  DyJsonGetValueFromTo(j, sHeaderClippingNear,      p.mNear);
  DyJsonGetValueFromTo(j, sHeaderClippingFar,       p.mFar);
  DyJsonGetValueFromTo(j, sHeaderViewportRect,      p.mViewportSize);

  DyJsonGetValueFromTo(j, sHeaderIsFocusInstantly,  p.mIsFocusInstantly);
  DyJsonGetValueFromTo(j, sHeaderIsMeshUnclipped,   p.mIsEnableMeshUnClipped);
}

//!
//! PDySoundSourceComponentMetaInfo
//!

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDySoundSourceComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {MSVSTR(sHeader_Type),      p.mType},
    {MSVSTR(sHeader_Details),   p.mDetails},
    {MSVSTR(sHeader_Activated), p.mInitiallyActivated},
  };;
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo& p)
{
  DyJsonGetValueFromTo(j, sHeader_Type,       p.mType);
  DyJsonGetValueFromTo(j, sHeader_Details,    p.mDetails);
  DyJsonGetValueFromTo(j, sHeader_Activated,  p.mInitiallyActivated);
}

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDySoundSourceComponentMetaInfo::DDetails& p)
{
  j = nlohmann::json
  {
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo::DDetails& p)
{
  DyJsonGetValueFromTo(j, "SoundSpecifier", p.mSoundSpecifier);

  DyJsonGetValueFromTo(j, "2DSound",  p.m2DSound);
  DyJsonGetValueFromTo(j, "Muted",    p.mMuted);
  DyJsonGetValueFromTo(j, "Looped",   p.mLooped);

  DyJsonGetValueFromTo(j, "VolumeMultiplier", p.mVolumeMultiplier);
  DyJsonGetValueFromTo(j, "PitchMultiplier",  p.mPitchMultiplier);
  DyJsonGetValueFromTo(j, "Channel",          p.mChannelSpecifier);

  DyJsonGetValueFromTo(j, "Attenuation", p.mAttenuation);
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDySoundSourceComponentMetaInfo::DAttenuation& p)
{
  DyJsonGetValueFromTo(j, "Activated", p.mActivated);
  DyJsonGetValueFromTo(j, "Near", p.mNearDistance);
  DyJsonGetValueFromTo(j, "Far",  p.mFarDistance);
}

} /// ::dy namespace