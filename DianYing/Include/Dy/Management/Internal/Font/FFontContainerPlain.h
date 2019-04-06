#ifndef GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H
#define GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H
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

#include <Dy/Meta/Information/MetaInfoFont.h>
#include <Dy/Management/Interface/AFontContainer.h>

namespace dy
{

/// @class FFontContainerPlain
/// @brief `ELoadingType` is `::ExternalPlain`.
class FFontContainerPlain : public AFontContainer
{
public:
  FFontContainerPlain(const PDyMetaFontInformation::DExternalPlain& details);

  /// @brief Release font container resources.
  virtual ~FFontContainerPlain();

  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FFontContainerPlain);

  MDY_NODISCARD bool IsCharacterGlyphExist(TChr16 fontCode) override;

  const DDyFontCharacterInfo& GetGlyphCharacter(TChr16 fontCode) override;

  TI32 GetLinefeedHeight(TI32 fontSize) const noexcept override;

  const DDyFontCharacterInfo& operator[](TChr16 fontCode) override;

  TI32 GetFontTextureArrayId() const noexcept override;

private:
  ///
  using TCharContainer = std::unordered_map<TChr16, DDyFontCharacterInfo>;

  ///
  TCharContainer  mCharContainer = {};
  ///
  TU32            mTexImageResId = MDY_INITIALIZE_DEFUINT;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H