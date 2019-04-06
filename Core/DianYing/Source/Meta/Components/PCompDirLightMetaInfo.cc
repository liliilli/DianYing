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
#include <Dy/Meta/Components/PCompDirLightMetaInfo.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/DColorRGB.h>

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

void to_json(nlohmann::json& oJson, const PDirLightComponentMetaInfo& iItem)
{
  oJson = nlohmann::json
  {
    {sHeader_Type,      iItem.mType},
    {sHeader_Details,   iItem.mDetails},
    {sHeader_Activated, iItem.mInitiallyActivated},
  };
}

void from_json(const nlohmann::json& iJson, PDirLightComponentMetaInfo& oItem)
{
  oItem.mType    = json::GetValueFrom<EDyComponentMetaType>(iJson, sHeader_Type);
  oItem.mDetails = json::GetValueFrom<PDirLightComponentMetaInfo::DDetails>(iJson, sHeader_Details);
  oItem.mInitiallyActivated = json::GetValueFrom<bool>(iJson, sHeader_Activated);
}

void to_json(nlohmann::json& oJson, const PDirLightComponentMetaInfo::DDetails& iDetail)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& iJson, PDirLightComponentMetaInfo::DDetails& oDetail)
{
  json::GetValueFromTo(iJson, sHeaderLightDirection, oDetail.mDirection);
  json::GetValueFromTo(iJson, sHeaderLightIntensity, oDetail.mIntensity);

  oDetail.mDiffuse  = json::GetValueFrom<DColorRGB>(iJson, sHeaderLightDiffuse);

  json::GetValueFromTo(iJson, sHeaderShadowType, oDetail.mShadowType);
  json::GetValueFromTo(iJson, sHeaderShadowStrength, oDetail.mShadowStrength);
  json::GetValueFromTo(iJson, sHeaderShadowBias, oDetail.mShadowBias);
  json::GetValueFromTo(iJson, sHeaderShadowResolution,oDetail.mShadowResolution);
  json::GetValueFromTo(iJson, sHeaderShadowCullingMaskLayer, oDetail.mShadowCullingMaskLayer);
  json::GetValueFromTo(iJson, sHeaderIsUsingGlobalShadowResolution, oDetail.mIsUsingGlobalShadowResolution);
  json::GetValueFromTo(iJson, sHeaderIsCastingLight, oDetail.mIsCastingLight);
  json::GetValueFromTo(iJson, sHeaderIsCastingShadow, oDetail.mIsCastingShadow);
}

} /// ::dy namespace