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

#include <Dy/Meta/Components/PCompSpotLightMetaInfo.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/System/Assertion.h>

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

void to_json(nlohmann::json& oJson, const PCompSpotLightMetaInfo& iItem)
{
  oJson = nlohmann::json
  {
    {sHeader_Type,      iItem.mType},
    {sHeader_Details,   iItem.mDetails},
    {sHeader_Activated, iItem.mInitiallyActivated},
  };
}

void from_json(const nlohmann::json& iJson, PCompSpotLightMetaInfo& oItem)
{
  oItem.mType    = json::GetValueFrom<EDyComponentMetaType>(iJson, sHeader_Type);
  oItem.mDetails = json::GetValueFrom<PCompSpotLightMetaInfo::DDetails>(iJson, sHeader_Details);
  oItem.mInitiallyActivated = json::GetValueFrom<bool>(iJson, sHeader_Activated);
}

void to_json(nlohmann::json& oJson, const PCompSpotLightMetaInfo::DDetails& iDetail)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& iJson, PCompSpotLightMetaInfo::DDetails& oDetail)
{
  /*
  { "Type": "PointLight",
    "Activated" : true,
    "Details" : {
      "Intensity": 50.0,
      "Direction": { "X": 0.5, "Y": 0.5, "Z": 0.5 },
      "Range" : 50.0,
      "RangeAngle" : 45.0,
      "Color" : { "R": 1.0, "G" : 1.0, "B" : 1.0 },
      "IsCastingLight" : true
    }
  }
  */

  json::GetValueFromTo(iJson, "Intensity", oDetail.mIntensity);
  json::GetValueFromTo(iJson, "Direction", oDetail.mDirection);
  json::GetValueFromTo(iJson, "Range", oDetail.mRange);
  json::GetValueFromTo(iJson, "RangeAngle", oDetail.mRangeAngle);
  json::GetValueFromTo(iJson, "Color", oDetail.mColor);
  json::GetValueFromTo(iJson, "IsCastingLight", oDetail.mIsCastingLight);
}

} /// ::dy namespace