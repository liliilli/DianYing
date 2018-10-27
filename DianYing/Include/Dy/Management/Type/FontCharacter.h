#ifndef GUARD_DY_MANAGEMENT_TYPE_FONTCHARACTER_H
#define GUARD_DY_MANAGEMENT_TYPE_FONTCHARACTER_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Helper/Type/VectorInt2.h>

namespace dy
{

///
/// @class DDyFontCharacter
/// @brief Manages each texture of font glyphes.
///
class DDyFontCharacter final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_CUSTOM(DDyFontCharacter);

  /// texture id of each character.
  TU32          mTextureId  = MDY_INITIALIZE_DEFUINT;
  /// glyph size
  DDyVectorInt2 mSize       = {};
  /// glyph mBearing position
  DDyVectorInt2 mBearing    = {};
  /// glyph advance width
  TU32          mAdvance    = MDY_INITIALIZE_DEFUINT;

  DDyFontCharacter() = default;
  explicit DDyFontCharacter(const TU32 texture_id, const DDyVectorInt2 size, const DDyVectorInt2 bearing, const TU32 advance) :
      mTextureId{ texture_id },
      mSize{ size },
      mBearing{ bearing },
      mAdvance{ advance } {};

  ~DDyFontCharacter() { if (mTextureId != MDY_INITIALIZE_DEFUINT) { glDeleteTextures(1, &mTextureId); } }
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_FONTCHARACTER_H