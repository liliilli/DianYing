#ifndef GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H
#define GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H
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

#include <Dy/Meta/Information/FontMetaInformation.h>
#include <Dy/Management/Interface/IDyFontContainer.h>

namespace dy
{

///
/// @class FDyFontResourceContainer
/// @brief
///
class FDyFontResourceContainer : public IDyFontContainer
{
public:
  ///
  /// @brief This constructor must success.
  /// @param fontInformation
  ///
  FDyFontResourceContainer(_MIN_ const PDyMetaFontInformation& fontInformation);

  /// @brief Release font container resources.
  virtual ~FDyFontResourceContainer();

  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyFontResourceContainer);

  MDY_NODISCARD bool IsCharacterGlyphExist(const TC16 fontCode) override;

  MDY_NODISCARD const DDyFontCharacterInfo& GetGlyphCharacter(const TC16 fontCode) override;

  MDY_NODISCARD TI32 GetLinefeedHeight(const TI32 fontSize) const noexcept override;

  MDY_NODISCARD const DDyFontCharacterInfo& operator[](_MIN_ const TC16 fontCode) override ;

private:
  ///
  using TCharContainer = std::unordered_map<TC16, DDyFontCharacterInfo>;

  ///
  TCharContainer  mCharContainer = {};
  ///
  TU32            mTexImageResId = MDY_INITIALIZE_DEFUINT;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_FONTRESOURCECONTAINER_H