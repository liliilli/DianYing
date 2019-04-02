#ifndef GUARD_DY_CORE_INFORMATION_FDYTEXTUREINFORMAITON_H
#define GUARD_DY_CORE_INFORMATION_FDYTEXTUREINFORMAITON_H
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

#include <Dy/Meta/Information/MetaInfoTexture.h>

namespace dy
{

/// @class FInformationTexture
/// @brief Texture information class that not serve heap instance
/// but information to create heap instance.
class FInformationTexture 
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FInformationTexture);
  FInformationTexture(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo);
  virtual ~FInformationTexture() = 0;

  /// @brief Get shader's specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get buffer format. (different from mTextureInformation.mTextureColorType)
  MDY_NODISCARD EDyImageColorFormatStyle GetFormat() const noexcept;

  /// @brief
  MDY_NODISCARD EDyGlImagePixelReadType GetPixelReadType() const noexcept;

  /// @brief Get texture's type (1d, 2d, etc)
  MDY_NODISCARD ETextureStyleType GetType() const noexcept;

  /// @brief Get Border color of texture.
  MDY_NODISCARD const DColorRGBA& GetBorderColor() const noexcept;

  /// @brief
  MDY_NODISCARD bool IsUsingCustomizedParamater() const noexcept;

  /// @brief
  MDY_NODISCARD bool IsUsingDefaultMipmap() const noexcept;

  /// @brief
  MDY_NODISCARD const TTextureParameterList& GetParameterList() const noexcept;

protected:
  ETextureStyleType       mTextureType            = ETextureStyleType::NoneError;
  std::string               mSpecifierName          = MDY_INITIALIZE_EMPTYSTR;

  EDyImageColorFormatStyle  mImageActualPixelFormat = EDyImageColorFormatStyle::NoneError;
  EDyGlImagePixelReadType   mInternalPixelReadType  = EDyGlImagePixelReadType::NoneError;
  DColorRGBA              mBorderColor;
  TTextureParameterList     mParameterOptionList;
  bool                      mIsUsingDefaultMipmap   = false;
  bool                      mIsUsingCustomParams    = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_INFORMATION_FDYTEXTUREINFORMAITON_H