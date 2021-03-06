#ifndef GUARD_MANAGEMENT_TYPE_FONTCHARACTERINFO_H
#define GUARD_MANAGEMENT_TYPE_FONTCHARACTERINFO_H
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

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/Type/DVector2.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @struct DDyTextureCoordBox
/// @brief
///
struct DDyTextureCoordBox
{
  ///
  TU32        mMapIndex           = MDY_INITIALIZE_DEFUINT;
  ///
  TU32        mChannel            = MDY_INITIALIZE_DEFUINT;
  ///
  DVec2  mLeftDown           = {};
  ///
  DVec2  mRightUp            = {};
};

void to_json  (_MOUT_ nlohmann::json& j,      _MIN_ const DDyTextureCoordBox& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyTextureCoordBox& p);

///
/// @struct DDyFontCharacterInfo
/// @brief
///
struct DDyFontCharacterInfo final
{
  ///
  TChr16        mCharCode           = MDY_INITIALIZE_DEFUINT;
  ///
  TF32        mHorizontalAdvance  = MDY_INITIALIZE_DEFUINT;
  ///
  DVec2  mHoritontalBearing  = {};
  ///
  DVec2  mScale              = {};
  ///
  DVec2  mSize               = {};
  ///
  DVec2  mTranslate          = {};
  ///
  DDyTextureCoordBox mTexCoordInfo = {};

  /// @brief  Create and return font character instance.
  /// @param  j Valid json atlas.
  /// @param  charCode character code UCS-2.
  /// @return Created character information instance.
  MDY_NODISCARD static DDyFontCharacterInfo CreateInstance(const nlohmann::json& j, TChr16 charCode);
};

void to_json  (_MOUT_ nlohmann::json& j,      _MIN_ const DDyFontCharacterInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyFontCharacterInfo& p);

extern DDyFontCharacterInfo sDefaultFontCharacterInfo;

} /// ::dy namespace

#endif /// GUARD_MANAGEMENT_TYPE_FONTCHARACTERINFO_H