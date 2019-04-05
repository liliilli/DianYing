#ifndef GUARD_DY_META_INFORMATION_FONTMETAINFORMATION_H
#define GUARD_DY_META_INFORMATION_FONTMETAINFORMATION_H
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

#include <variant>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>

#include <Expr/TEnumString.h>
#include <Dy/Helper/Type/DUuid.h>
#include <Dy/Helper/Library/HelperFilesystem.h>

namespace dy
{

/// @struct PDyMetaFontInformation
/// @brief  Font meta-information container type.
struct PDyMetaFontInformation final
{
  // Forward declarations 
  struct DExternalPlain;
  struct DExternalCompressed;
  struct DBuiltin;
  struct DRuntime;

  using TDetails = std::variant<DExternalPlain, DExternalCompressed, DBuiltin, DRuntime>;

  /// @enum  ELoadingType
  /// @brief Loading type.
  EXPR_DEFINE_ENUM(ELoadingType, 
    ExternalPlain, 
    ExternalCompressed, 
    Runtime, 
    Builtin);

  template <typename T> struct ToLoadingType_T;
  template <> struct ToLoadingType_T<DExternalPlain> { static constexpr auto value = ELoadingType::ExternalPlain; };
  template <> struct ToLoadingType_T<DExternalCompressed> { static constexpr auto value = ELoadingType::ExternalCompressed; };
  template <> struct ToLoadingType_T<DRuntime> { static constexpr auto value = ELoadingType::Runtime; };
  template <> struct ToLoadingType_T<DBuiltin> { static constexpr auto value = ELoadingType::Builtin; };

  template <EXPR_E(ELoadingType) E> struct ToDetailType_T;
  template <> struct ToDetailType_T<EXPR_E(ELoadingType)::ExternalPlain> { using Type = DExternalPlain; };
  template <> struct ToDetailType_T<EXPR_E(ELoadingType)::ExternalCompressed> { using Type = DExternalCompressed; };
  template <> struct ToDetailType_T<EXPR_E(ELoadingType)::Runtime> { using Type = DRuntime; };
  template <> struct ToDetailType_T<EXPR_E(ELoadingType)::Builtin> { using Type = DBuiltin; };

  template <typename T> static constexpr auto ToLoadingType = ToLoadingType_T<T>::value;
  template <EXPR_E(ELoadingType) E> using ToDetailType = typename ToDetailType_T<E>::Type;

  /// @enum   EFontType
  /// @brief  Font type for rendering.
  EXPR_DEFINE_ENUM(EFontType,
    SDF,    // Use Signed-distance field version (smooth, good but use more memory and performance).
    Plain); // Use just plain bitmaped font texture. (fast but not good quality)
  
  /// @struct DExternalPlain
  /// @brief When `mLoadingType` is `ELoadingType::ExternalPlain`.
  struct DExternalPlain final
  {
    /// @brief Font type, This must not be __Error.
    EXPR_E(EFontType) mFontType = EFontType::__Error;
    /// @brief Font information path. 
    DFilePath mFontInformationPath;
    /// @brief Font texture path list.
    std::vector<DFilePath> mFontTexturePathList = {};
  };

  /// @struct DExternalCompressed
  struct DExternalCompressed final
  {
    /// @brief Font type, This must not be __Error.
    EXPR_E(EFontType) mFontType = EFontType::__Error;
    /// @brief Compressed font file path. 
    DFilePath mFilePath;
  };

  /// @struct DBuiltin
  /// @brief When 'mLoadingType' is 'ELoadingType::Builtin'.
  struct DBuiltin final
  {
    /// @brief Font type, This must not be __Error.
    EXPR_E(EFontType) mFontType = EFontType::__Error;
    /// @brief Font information json string literal.
    const char* mFontInformation = nullptr;
    /// @brief Font builtin texture list.
    std::vector<std::vector<TChr8>> mFontTextureBuffers;
  };

  /// @struct DRuntime
  /// @brief When `mLoadingType` is `ELoadingType::Runtime`.
  /// This type only create Plain (Not SDF) textures.
  struct DRuntime final
  {
    /// @brief Font file to create runtime plain glyph texture.
    DFilePath mFontFilePath;
  };

  /// @brief Font specifier name.
  std::string   mSpecifierName;
  /// @brief Uuid.
  DUuid         mUuid;
  /// @brief Loading type for handling variant font meta resource templates.
  EXPR_E(ELoadingType) mLoadingType = ELoadingType::__Error;
  /// @brief 
  TDetails      mDetails;
};

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation& iFont);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation& oFont);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::EXPR_E(ELoadingType)& iVar);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::EXPR_E(ELoadingType)& oVar);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::EXPR_E(EFontType)& iVar);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::EXPR_E(EFontType)& oVar);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DExternalPlain& iDetail);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DExternalPlain& oDetail);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DExternalCompressed& iDetail);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DExternalCompressed& oDetail);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DRuntime& iDetail);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DRuntime& oDetail);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_FONTMETAINFORMATION_H