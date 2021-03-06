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
#include <Dy/Management/Interface/IFontContainer.h>

namespace dy
{

/// @class FFontContainerPlain
/// @brief
class FFontContainerPlain : public IFontContainer
{
public:
  /// @brief This constructor must success.
  /// @param fontInformation
  FFontContainerPlain(const PDyMetaFontInformation& fontInformation);

  /// @brief Release font container resources.
  virtual ~FFontContainerPlain();

  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FFontContainerPlain);

  MDY_NODISCARD bool IsCharacterGlyphExist(const TChr16 fontCode) override;

  MDY_NODISCARD const DDyFontCharacterInfo& GetGlyphCharacter(const TChr16 fontCode) override;

  MDY_NODISCARD TI32 GetLinefeedHeight(const TI32 fontSize) const noexcept override;

  MDY_NODISCARD const DDyFontCharacterInfo& operator[](const TChr16 fontCode) override ;

  MDY_NODISCARD TI32 GetFontTextureArrayId() const noexcept override
  {
    return this->mTexImageResId;
  }

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