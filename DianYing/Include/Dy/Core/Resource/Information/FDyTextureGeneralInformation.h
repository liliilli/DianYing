#ifndef GUARD_DY_CORE_INFORMATION_FDyTextureGeneralInformation_H
#define GUARD_DY_CORE_INFORMATION_FDyTextureGeneralInformation_H
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

/// @class FDyTextureGeneralInformation
/// @brief Texture general information class that not serve heap instance
/// but information to create heap instance.
class FDyTextureGeneralInformation final : public FDyTextureInformation
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyTextureGeneralInformation);
  FDyTextureGeneralInformation(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo);
  virtual ~FDyTextureGeneralInformation() = default;

  /// @brief Get buffer of image.
  MDY_NODISCARD const std::vector<TU08>& GetBuffer() const noexcept;

  /// @brief Get texture's size.
  MDY_NODISCARD const DVectorInt2& GetSize() const noexcept;

private:
  std::vector<TU08> mTextureImageBuffer = {};
  DVectorInt2     mTextureSize        = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_INFORMATION_FDyTextureGeneralInformation_H