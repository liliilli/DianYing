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
#include <Dy/Meta/Information/ComponentLightMetaInfo.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/ColorRGB.h>

//!
//! Forward declaration
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

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyDirLightComponentMetaInfo& p)
{
  j = nlohmann::json
  {
    {(sHeader_Type),      p.mType},
    {(sHeader_Details),   p.mDetails},
    {(sHeader_Activated), p.mInitiallyActivated},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ PDyDirLightComponentMetaInfo& p)
{
  p.mType               = DyJsonGetValueFrom<EDyComponentMetaType>(j, sHeader_Type);
  p.mDetails            = DyJsonGetValueFrom<PDyDirLightComponentMetaInfo::DDetails>(j, sHeader_Details);
  p.mInitiallyActivated = DyJsonGetValueFrom<bool>(j, sHeader_Activated);
}

void to_json(nlohmann::json& j, const PDyDirLightComponentMetaInfo::DDetails& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& j, PDyDirLightComponentMetaInfo::DDetails& p)
{
  DyJsonGetValueFromTo(j, sHeaderLightDirection, p.mDirection);
  DyJsonGetValueFromTo(j, sHeaderLightIntensity, p.mIntensity);

  p.mDiffuse  = DyJsonGetValueFrom<DDyColorRGB>(j, sHeaderLightDiffuse);
  p.mSpecular = DyJsonGetValueFrom<DDyColorRGB>(j, sHeaderLightSpecular);
  p.mAmbient  = DyJsonGetValueFrom<DDyColorRGB>(j, sHeaderLightAmbient);

  DyJsonGetValueFromTo(j, sHeaderShadowType,      p.mShadowType);
  DyJsonGetValueFromTo(j, sHeaderShadowStrength,  p.mShadowStrength);
  DyJsonGetValueFromTo(j, sHeaderShadowBias,      p.mShadowBias);
  DyJsonGetValueFromTo(j, sHeaderShadowResolution,p.mShadowResolution);
  DyJsonGetValueFromTo(j, sHeaderShadowCullingMaskLayer, p.mShadowCullingMaskLayer);
  DyJsonGetValueFromTo(j, sHeaderIsUsingGlobalShadowResolution, p.mIsUsingGlobalShadowResolution);
  DyJsonGetValueFromTo(j, sHeaderIsCastingLight,  p.mIsCastingLight);
  DyJsonGetValueFromTo(j, sHeaderIsCastingShadow, p.mIsCastingShadow);
}

} /// ::dy namespace