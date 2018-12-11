#ifndef GUARD_DY_CORE_INFORMATION_FDYTEXTUREINFORMAITON_H
#define GUARD_DY_CORE_INFORMATION_FDYTEXTUREINFORMAITON_H
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

private:
  using TParameterList = PDyTextureInstanceMetaInfo::TParameterList;

  std::vector<TU08>         mTextureImageBuffer = {};
  DDyVectorInt2             mTextureSize = {};
  EDyImageColorFormatStyle  mImageActualPixelFormat = EDyImageColorFormatStyle::NoneError;
  DDyColorRGBA              mBorderColor;
  TParameterList            mParameterOptionList;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_INFORMATION_FDYTEXTUREINFORMAITON_H