#ifndef GUARD_DY_CORE_INFORMATION_FInformationTextureGeneral_H
#define GUARD_DY_CORE_INFORMATION_FInformationTextureGeneral_H
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

/// @class FInformationTextureGeneral
/// @brief Texture general information class that not serve heap instance
/// but information to create heap instance.
class FInformationTextureGeneral final : public FInformationTexture
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FInformationTextureGeneral);
  FInformationTextureGeneral(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo);
  virtual ~FInformationTextureGeneral() = default;

  /// @brief Get buffer of image.
  MDY_NODISCARD const std::vector<TU8>& GetBuffer() const noexcept;

  /// @brief Get texture's size.
  MDY_NODISCARD const DIVec2& GetSize() const noexcept;

private:
  std::vector<TU8> mTextureImageBuffer = {};
  DIVec2     mTextureSize        = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_INFORMATION_FInformationTextureGeneral_H