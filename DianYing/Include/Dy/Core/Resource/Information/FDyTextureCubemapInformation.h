#ifndef GUARD_DY_CORE_INFORMATION_FDyTextureCubemapInformation_H
#define GUARD_DY_CORE_INFORMATION_FDyTextureCubemapInformation_H
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
#include "FDyTextureInformation.h"

namespace dy
{

/// @enum class EDyCubemapFragment
/// @brief Cubemap fragment type.
enum class EDyCubemapFragment
{
  Top = 0, Bottom = 1, Left = 2, Right = 3, Front = 4, Back = 5
};

/// @class FDyTextureCubemapInformation
/// @brief Texture cubemap information class that not serve heap instance
/// but information to create heap instance.
class FDyTextureCubemapInformation : public FDyTextureInformation
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyTextureCubemapInformation);
  FDyTextureCubemapInformation(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo);
  virtual ~FDyTextureCubemapInformation() = default;
  
  /// @brief Get buffer of image.
  MDY_NODISCARD const std::vector<TU08>& GetBufferOf(_MIN_ EDyCubemapFragment iValue) const noexcept;
  /// @brief Get texture's size.
  MDY_NODISCARD const DVectorInt2& GetSizeOf(_MIN_ EDyCubemapFragment iValue) const noexcept;

private:
  std::vector<TU08> mTopTextureImageBuffer = {};
  std::vector<TU08> mBottomTextureImageBuffer = {};
  std::vector<TU08> mFrontTextureImageBuffer = {};
  std::vector<TU08> mBackTextureImageBuffer = {};
  std::vector<TU08> mLeftTextureImageBuffer = {};
  std::vector<TU08> mRightTextureImageBuffer = {};

  DVectorInt2     mTopTextureSize = {};
  DVectorInt2     mBottomTextureSize = {};
  DVectorInt2     mFrontTextureSize = {};
  DVectorInt2     mBackTextureSize = {};
  DVectorInt2     mLeftTextureSize = {};
  DVectorInt2     mRightTextureSize = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_INFORMATION_FDyTextureCubemapInformation_H