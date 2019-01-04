#ifndef GUARD_DY_META_INFORMATION_FONTMETAINFORMATION_H
#define GUARD_DY_META_INFORMATION_FONTMETAINFORMATION_H
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

#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @struct PDyMetaFontInformation
/// @brief  Font meta-information container type.
///
struct PDyMetaFontInformation final
{
  ///
  /// @enum   EFontType
  /// @brief  Font type for rendering.
  ///
  enum class EFontType
  { /// Use Signed-distance field version (smooth, good but use more memory and performance).
    SDF,
    /// Use just plain bitmaped font texture. (fast but not good quality)
    Plain,
    /// ERROR TYPE. MUST NOT BE SEEN ON PLAY RUNTIME.
    NoneError,
  };

  ///
  std::string mSpecifierName        = MDY_INITIALIZE_EMPTYSTR;
  ///
  EFontType   mFontType             = EFontType::NoneError;
  ///
  std::string mFontInformationPath  = MDY_INITIALIZE_EMPTYSTR;
  ///
  std::vector<std::string> mFontTexturePathList = {};

  ///
  std::string mFontAlternativeFilePath = MDY_INITIALIZE_EMPTYSTR;
  ///
  bool        mIsUsingRuntimeCreateionWhenGlyphNotExist = false;

  ///
  /// @brief  Create and return instance from json atlas.
  /// @param  fontAtlas Json atlas.
  /// @return Valid created meta information instance.
  ///
  static MDY_NODISCARD PDyMetaFontInformation CreateWithJson(_MIN_ const nlohmann::json& fontAtlas);
};

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_FONTMETAINFORMATION_H