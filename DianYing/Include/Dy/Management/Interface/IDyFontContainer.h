#ifndef GUARD_DY_MANAGEMENT_INTERFACE_IDYFONTCONTAINER_H
#define GUARD_DY_MANAGEMENT_INTERFACE_IDYFONTCONTAINER_H
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

#include <Dy/Management/Type/FontCharacterInfo.h>

//!
//! Implementation
//!

namespace dy
{

///
/// @interface IDyFontContainer
/// @brief Interface type of font container.
///
MDY_INTERFACE MDY_NO_VTABLE IDyFontContainer
{
public:
  virtual ~IDyFontContainer() = default;

  ///
  /// @brief  Check character glyph information is exist or not.
  /// @param  fontCode UCS-2 fontCode to verify.
  /// @return If found, return true or false.
  ///
  virtual MDY_NODISCARD bool IsCharacterGlyphExist(_MIN_ const TC16 fontCode) = 0;

  ///
  /// @brief  Get character glyph information instance from container. This function defered with derived type.
  /// @param  fontCode UCS-2 fontCode to verify.
  /// @return Valid font character instance.
  ///
  virtual MDY_NODISCARD const DDyFontCharacterInfo& GetGlyphCharacter(_MIN_ const TC16 fontCode) = 0;

  ///
  /// @brief  Helper operator function of glyph information.
  /// @param  fontCode UCS-2 font code to verify.
  /// @return Valid font character instance.
  ///
  virtual MDY_NODISCARD const DDyFontCharacterInfo& operator[](_MIN_ const TC16 fontCode) = 0;

  ///
  /// @brief  Get horizontal line feed height with font size.
  /// @param  fontSize
  /// @return Scaled line feed height.
  ///
  virtual MDY_NODISCARD TI32 GetLinefeedHeight(_MIN_ const TI32 fontSize) const noexcept = 0;

  ///
  /// @brief
  /// @return Texture array 2d id.
  ///
  virtual TI32 GetFontTextureArrayId() const noexcept = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERFACE_IDYFONTCONTAINER_H