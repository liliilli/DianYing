#ifndef GUARD_MANAGEMENT_TYPE_FONTCONTAINER_H
#define GUARD_MANAGEMENT_TYPE_FONTCONTAINER_H
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

#include <Dy/Management/Type/FontCharacter_Deprecated.h>

namespace dy
{

///
/// @class FDyFontContainer_Deprecated
/// @brief
///
class FDyFontContainer_Deprecated final
{
public:
  FDyFontContainer_Deprecated(_MIN_ const std::string& fontFilePath);
  ~FDyFontContainer_Deprecated();

  ///
  /// @brief
  /// @param  fontCode
  /// @return
  ///
  bool IsFontGlyphExist(_MIN_ const TC16 fontCode);

  ///
  /// @brief
  /// @param  fontCode
  /// @return If succeeded to create font glyph, return DY_SUCCESS or DY_FAILURE.
  ///
  EDySuccess CreateFontGlyph(_MIN_ const TC16 fontCode);

  ///
  /// @brief
  /// @param  fontCode
  /// @return
  ///
  const DDyFontCharacter_Deprecated& operator[](_MIN_ const TC16 fontCode);

  ///
  /// @brief
  /// @param  fontSize
  /// @return
  ///
  MDY_NODISCARD TI32 GetScaledLinefeedHeight(_MIN_ const TI32 fontSize) const noexcept;

private:
  ///
  std::string mFontSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  ///
  std::string mFontFilePath       = MDY_INITIALIZE_EMPTYSTR;
  ///
  TF32 mUnscaledLinefeedHeight    = MDY_INITIALIZE_DEFINT;
  ///
  std::unordered_map<TC16, DDyFontCharacter_Deprecated> mFontGlyphContainer = {};
};

} /// ::dy namespace

#endif /// GUARD_MANAGEMENT_TYPE_FONTCONTAINER_H