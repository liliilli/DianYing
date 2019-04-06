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
#include <Dy/Management/Type/FontCharacterInfo.h>
#include <nlohmann/json.hpp>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_HoriAdvance, "HoriAdvance");
MDY_SET_IMMUTABLE_STRING(sHeader_HoriBearing, "HoriBearing");
MDY_SET_IMMUTABLE_STRING(sHeader_Scale,       "Scale");
MDY_SET_IMMUTABLE_STRING(sHeader_Size,        "Size");
MDY_SET_IMMUTABLE_STRING(sHeader_Translate,   "Translate");

MDY_SET_IMMUTABLE_STRING(sHeader_TexCoordBox, "TexCoordBox");
MDY_SET_IMMUTABLE_STRING(sHeader_Channel,     "Channel");
MDY_SET_IMMUTABLE_STRING(sHeader_MapIndex,    "MapIndex");
MDY_SET_IMMUTABLE_STRING(sHeader_LeftDown,    "LeftDown");
MDY_SET_IMMUTABLE_STRING(sHeader_RightUp,     "RightUp");

}

//!
//! Implementation
//!

namespace dy
{

void to_json(nlohmann::json& j, const DDyTextureCoordBox& p)
{
  j = nlohmann::json
  {
      {sHeader_Channel, p.mChannel},
      {sHeader_MapIndex, p.mMapIndex},
      {sHeader_LeftDown, p.mLeftDown},
      {sHeader_RightUp, p.mRightUp}
  };
}

void from_json(const nlohmann::json& j, DDyTextureCoordBox& p)
{
  p.mChannel          = j.at((sHeader_Channel)).get<TU32>();
  p.mMapIndex         = j.at((sHeader_MapIndex)).get<TU32>();
  p.mLeftDown         = j.at((sHeader_LeftDown)).get<DVec2>();
  p.mRightUp          = j.at((sHeader_RightUp)).get<DVec2>();
}

DDyFontCharacterInfo DDyFontCharacterInfo::CreateInstance(const nlohmann::json& j, TChr16 charCode)
{
  DDyFontCharacterInfo result = j.get<DDyFontCharacterInfo>();
  result.mCharCode            = charCode;
  return result;
}

void to_json(nlohmann::json& j, const DDyFontCharacterInfo& p)
{
  j = nlohmann::json
  {
      {sHeader_HoriAdvance, p.mHorizontalAdvance},
      {sHeader_HoriBearing, p.mHoritontalBearing},
      {sHeader_Size, p.mScale},
      {sHeader_Size, p.mSize},
      {sHeader_Translate, p.mTranslate},
      {sHeader_TexCoordBox, p.mTexCoordInfo},
  };
}

void from_json(const nlohmann::json& j, DDyFontCharacterInfo& p)
{
  p.mHorizontalAdvance  = j.at((sHeader_HoriAdvance)).get<TF32>();
  p.mHoritontalBearing  = j.at((sHeader_HoriBearing)).get<DVec2>();
  p.mScale              = j.at((sHeader_Scale)).get<DVec2>();
  p.mSize               = j.at((sHeader_Size)).get<DVec2>();
  p.mTranslate          = j.at((sHeader_Translate)).get<DVec2>();
  p.mTexCoordInfo       = j.at((sHeader_TexCoordBox)).get<DDyTextureCoordBox>();
}

DDyFontCharacterInfo sDefaultFontCharacterInfo;

} /// ::dy namespace