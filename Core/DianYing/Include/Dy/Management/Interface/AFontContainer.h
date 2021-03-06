#ifndef GUARD_DY_MANAGEMENT_INTERFACE_IDYFONTCONTAINER_H
#define GUARD_DY_MANAGEMENT_INTERFACE_IDYFONTCONTAINER_H
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

#include <Dy/Management/Type/FontCharacterInfo.h>
#include <Dy/Meta/Information/MetaInfoFont.h>

//!
//! Implementation
//!

namespace dy
{

/// @interface AFontContainer
/// @brief Interface type of font container.
class AFontContainer
{
public:
  AFontContainer(
    EXPR_E(PDyMetaFontInformation::ELoadingType) loadingType,
    EXPR_E(PDyMetaFontInformation::EFontType) fontType)
    : mLoadingType{loadingType},
      mFontType{fontType} {};
  virtual ~AFontContainer() = default;

  /// @brief  Check character glyph information is exist or not.
  /// @param  fontCode UCS-2 fontCode to verify.
  /// @return If found, return true or false.
  MDY_NODISCARD virtual bool IsCharacterGlyphExist(TChr16 fontCode) = 0;

  /// @brief  Get character glyph information instance from container. This function defered with derived type.
  /// @param  fontCode UCS-2 fontCode to verify.
  /// @return Valid font character instance.
  virtual const DDyFontCharacterInfo& GetGlyphCharacter(TChr16 fontCode) = 0;

  /// @brief  Helper operator function of glyph information.
  /// @param  fontCode UCS-2 font code to verify.
  /// @return Valid font character instance.
  virtual const DDyFontCharacterInfo& operator[](TChr16 fontCode) = 0;

  /// @brief  Get horizontal line feed height with font size.
  /// @param  fontSize
  /// @return Scaled line feed height.
  virtual TI32 GetLinefeedHeight(TI32 fontSize) const noexcept = 0;

  /// @brief
  /// @return Texture array 2d id.
  virtual TI32 GetFontTextureArrayId() const noexcept = 0;

  /// @brief Get font loading type of this instance.
  EXPR_E(PDyMetaFontInformation::ELoadingType) GetLoadingType() const noexcept;

  /// @brief Get font type of this font container instance.
  EXPR_E(PDyMetaFontInformation::EFontType) GetFontType() const noexcept;

private:
  EXPR_E(PDyMetaFontInformation::ELoadingType) mLoadingType = decltype(mLoadingType)::__Error;
  EXPR_E(PDyMetaFontInformation::EFontType) mFontType = decltype(mFontType)::__Error;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERFACE_IDYFONTCONTAINER_H