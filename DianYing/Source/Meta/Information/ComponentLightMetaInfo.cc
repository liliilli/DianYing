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

void to_json(nlohmann::json& oJson, const PDyDirLightComponentMetaInfo& iItem)
{
  oJson = nlohmann::json
  {
    {sHeader_Type,      iItem.mType},
    {sHeader_Details,   iItem.mDetails},
    {sHeader_Activated, iItem.mInitiallyActivated},
  };
}

void from_json(const nlohmann::json& iJson, PDyDirLightComponentMetaInfo& oItem)
{
  oItem.mType    = DyJsonGetValueFrom<EDyComponentMetaType>(iJson, sHeader_Type);
  oItem.mDetails = DyJsonGetValueFrom<PDyDirLightComponentMetaInfo::DDetails>(iJson, sHeader_Details);
  oItem.mInitiallyActivated = DyJsonGetValueFrom<bool>(iJson, sHeader_Activated);
}

void to_json(nlohmann::json& oJson, const PDyDirLightComponentMetaInfo::DDetails& iDetail)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& iJson, PDyDirLightComponentMetaInfo::DDetails& oDetail)
{
  DyJsonGetValueFromTo(iJson, sHeaderLightDirection, oDetail.mDirection);
  DyJsonGetValueFromTo(iJson, sHeaderLightIntensity, oDetail.mIntensity);

  oDetail.mDiffuse  = DyJsonGetValueFrom<DDyColorRGB>(iJson, sHeaderLightDiffuse);
  oDetail.mSpecular = DyJsonGetValueFrom<DDyColorRGB>(iJson, sHeaderLightSpecular);
  oDetail.mAmbient  = DyJsonGetValueFrom<DDyColorRGB>(iJson, sHeaderLightAmbient);

  DyJsonGetValueFromTo(iJson, sHeaderShadowType, oDetail.mShadowType);
  DyJsonGetValueFromTo(iJson, sHeaderShadowStrength, oDetail.mShadowStrength);
  DyJsonGetValueFromTo(iJson, sHeaderShadowBias, oDetail.mShadowBias);
  DyJsonGetValueFromTo(iJson, sHeaderShadowResolution,oDetail.mShadowResolution);
  DyJsonGetValueFromTo(iJson, sHeaderShadowCullingMaskLayer, oDetail.mShadowCullingMaskLayer);
  DyJsonGetValueFromTo(iJson, sHeaderIsUsingGlobalShadowResolution, oDetail.mIsUsingGlobalShadowResolution);
  DyJsonGetValueFromTo(iJson, sHeaderIsCastingLight, oDetail.mIsCastingLight);
  DyJsonGetValueFromTo(iJson, sHeaderIsCastingShadow, oDetail.mIsCastingShadow);
}

} /// ::dy namespace