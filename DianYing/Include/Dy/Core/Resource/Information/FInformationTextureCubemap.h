#ifndef GUARD_DY_CORE_INFORMATION_FInformationTextureCubemap_H
#define GUARD_DY_CORE_INFORMATION_FInformationTextureCubemap_H
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
#include "FInformationTexture.h"

namespace dy
{

/// @enum class EDyCubemapFragment
/// @brief Cubemap fragment type.
enum class EDyCubemapFragment
{
  Top = 0, Bottom = 1, Left = 2, Right = 3, Front = 4, Back = 5
};

/// @class FInformationTextureCubemap
/// @brief Texture cubemap information class that not serve heap instance
/// but information to create heap instance.
class FInformationTextureCubemap : public FInformationTexture
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FInformationTextureCubemap);
  FInformationTextureCubemap(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo);
  virtual ~FInformationTextureCubemap() = default;
  
  /// @brief Get buffer of image.
  MDY_NODISCARD const std::vector<TU8>& GetBufferOf(_MIN_ EDyCubemapFragment iValue) const noexcept;
  /// @brief Get texture's size.
  MDY_NODISCARD const DIVec2& GetSizeOf(_MIN_ EDyCubemapFragment iValue) const noexcept;

private:
  std::vector<TU8> mTopTextureImageBuffer = {};
  std::vector<TU8> mBottomTextureImageBuffer = {};
  std::vector<TU8> mFrontTextureImageBuffer = {};
  std::vector<TU8> mBackTextureImageBuffer = {};
  std::vector<TU8> mLeftTextureImageBuffer = {};
  std::vector<TU8> mRightTextureImageBuffer = {};

  DIVec2     mTopTextureSize = {};
  DIVec2     mBottomTextureSize = {};
  DIVec2     mFrontTextureSize = {};
  DIVec2     mBackTextureSize = {};
  DIVec2     mLeftTextureSize = {};
  DIVec2     mRightTextureSize = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_INFORMATION_FInformationTextureCubemap_H