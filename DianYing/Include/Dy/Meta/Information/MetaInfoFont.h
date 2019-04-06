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
#include <Expr/TEnumTypeBinding.h>
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

  EXPR_DEFINE_ENUMTYPE_BINDING(ELoadingType, false, 
    DExternalPlain, ExternalPlain,
    DExternalCompressed, ExternalCompressed,
    DRuntime, Runtime,
    DBuiltin, Builtin);

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

    /// @enum EBuffer
    /// @brief Buffer type
    enum class EBuffer
    {
      Plain,
      Index,
      __Error
    };

    /// @brief Font information json string literal. (optional)
    using TInfoBuffer = const char*;
    /// @brief Font information string buffer index. (optional)
    using TInfoIndex = TIndex;
    /// @struct XInfo
    /// @brief Internal structure.
    struct XInfo final
    {
      EXPR_DEFINE_ENUMTYPE_BINDING(EBuffer, true, TInfoBuffer, Plain, TInfoIndex, Index);
    };

    /// @brief
    EBuffer mFontInfoType = EBuffer::__Error;
    /// @brief
    std::variant<TInfoBuffer, TInfoIndex> mFontInfoBuffer;

    /// @brief Font builtin texture list. (optional)
    using TTexBuffers = std::vector<std::vector<TChr8>>;
    /// @brief Font builtin image index index list. (optional)
    using TTexIndexes = std::vector<TIndex>;
    /// @struct XTexture
    /// @brief Internal structrue.
    struct XTexture final
    {
      EXPR_DEFINE_ENUMTYPE_BINDING(EBuffer, true, TTexBuffers, Plain, TTexIndexes, Index);
    };

    /// @brief
    EBuffer mTextureType = EBuffer::__Error;
    /// @brief 
    std::variant<TTexBuffers, TTexIndexes> mTexureBuffers;
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

/// @struct PBuiltinMetaFontInfo
/// @brief Buitlin meta information of font.
struct PBuiltinMetaFontInfo final
{
  /// @brief Font specifier name.
  std::string mIdentifier;
  /// @brief Uuid.
  DUuid       mUuid;
  /// @brief Details;
  PDyMetaFontInformation::DBuiltin mDetails;

  explicit operator PDyMetaFontInformation() const noexcept
  {
    PDyMetaFontInformation result;
    result.mLoadingType = decltype(result.mLoadingType)::Builtin;
    result.mSpecifierName = this->mIdentifier;
    result.mUuid    = this->mUuid;
    result.mDetails = this->mDetails;

    return result;
  }
};

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_FONTMETAINFORMATION_H