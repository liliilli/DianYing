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

///
/// @class FDyTextureInformation
/// @brief Texture information class that not serve heap instance
/// but information to create heap instance.
///
class FDyTextureInformation final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyTextureInformation);
  FDyTextureInformation(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo);
  ~FDyTextureInformation() = default;

  /// @brief Get shader's specifier name.
  FORCEINLINE MDY_NODISCARD const std::string& GetSpecifierName() const noexcept
  {
    return this->mSpecifierName;
  }

  /// @brief Get buffer of image.
  FORCEINLINE MDY_NODISCARD const std::vector<TU08>& GetBuffer() const noexcept
  {
    return this->mTextureImageBuffer;
  }

  /// @brief Get buffer format. (different from mTextureInformation.mTextureColorType)
  FORCEINLINE MDY_NODISCARD EDyImageColorFormatStyle GetFormat() const noexcept
  {
    return this->mImageActualPixelFormat;
  }

  /// @brief Get texture's size.
  FORCEINLINE MDY_NODISCARD const DDyVectorInt2& GetSize() const noexcept
  {
    return this->mTextureSize;
  }

  /// @brief Get texture's type (1d, 2d, etc)
  FORCEINLINE MDY_NODISCARD EDyTextureStyleType GetType() const noexcept
  {
    return this->mTextureType;
  }

  /// @brief Get Border color of texture.
  MDY_NODISCARD const DDyColorRGBA& GetBorderColor() const noexcept
  {
    return this->mBorderColor;
  }

  /// @brief
  MDY_NODISCARD bool IsUsingCustomizedParamater() const noexcept
  {
    return this->mIsUsingCustomParams;
  }

  /// @brief
  MDY_NODISCARD bool IsUsingDefaultMipmap() const noexcept
  {
    return this->mIsUsingDefaultMipmap;
  }

  /// @brief
  MDY_NODISCARD const auto& GetParameterList() const noexcept
  {
    return this->mParameterOptionList;
  }

private:
  std::string               mSpecifierName          = MDY_INITIALIZE_EMPTYSTR;
  std::vector<TU08>         mTextureImageBuffer     = {};
  DDyVectorInt2             mTextureSize            = {};
  EDyImageColorFormatStyle  mImageActualPixelFormat = EDyImageColorFormatStyle::NoneError;
  EDyTextureStyleType       mTextureType            = EDyTextureStyleType::NoneError;
  DDyColorRGBA              mBorderColor;
  TTextureParameterList     mParameterOptionList;
  bool                      mIsUsingDefaultMipmap   = false;
  bool                      mIsUsingCustomParams    = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_INFORMATION_FDYTEXTUREINFORMAITON_H