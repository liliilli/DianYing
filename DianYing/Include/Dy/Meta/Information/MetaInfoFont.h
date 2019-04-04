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

  /// @brief 
  enum class ELoadingType 
  {
    ExternalPlain,
    ExternalCompressed,
    Runtime,
    Builtin,

    __Error, // ERROR TYPE. MUST NOT BE SEEN ON PLAY RUNTIME.
  };

  /// @enum   EFontType
  /// @brief  Font type for rendering.
  enum class EFontType
  { 
    SDF,     // Use Signed-distance field version (smooth, good but use more memory and performance).
    Plain,   // Use just plain bitmaped font texture. (fast but not good quality)

    __Error, // ERROR TYPE. MUST NOT BE SEEN ON PLAY RUNTIME.
  };
  
  /// @struct DExternalPlain
  /// @brief When `mLoadingType` is `ELoadingType::ExternalPlain`.
  struct DExternalPlain final
  {
    /// @brief Font type, This must not be __Error.
    EFontType mFontType = EFontType::__Error;
    /// @brief Font information path. 
    TFilePath mFontInformationPath;
    /// @brief Font texture path list.
    std::vector<TFilePath> mFontTexturePathList = {};
  };

  /// @struct DExternalCompressed
  struct DExternalCompressed final
  {
    /// @brief Font type, This must not be __Error.
    EFontType mFontType = EFontType::__Error;
    /// @brief Compressed font file path. 
    TFilePath mFilePath;
  };

  /// @struct DBuiltin
  /// @brief When 'mLoadingType' is 'ELoadingType::Builtin'.
  struct DBuiltin final
  {
    /// @brief Font type, This must not be __Error.
    EFontType mFontType = EFontType::__Error;
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
    TFilePath mFontFilePath;
    /// @brief If no font texture is specified, this must be enabled.
    /// and `mAlternativeFontFilePath` also be specified valid font path for runtime creation.
    bool mIsEnableRuntimeCreation = false;
  };

  /// @brief Font specifier name.
  std::string   mSpecifierName;
  /// @brief Uuid.
  DUuid         mUuid;
  /// @brief Loading type for handling variant font meta resource templates.
  ELoadingType  mLoadingType = ELoadingType::__Error;
  /// @brief 
  TDetails      mDetails;
};

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation& iFont);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation& oFont);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::ELoadingType& iVar);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::ELoadingType& oVar);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::EFontType& iVar);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::EFontType& oVar);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DExternalPlain& iDetail);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DExternalPlain& oDetail);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DExternalCompressed& iDetail);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DExternalCompressed& oDetail);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DBuiltin& iDetail);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DBuiltin& oDetail);

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DRuntime& iDetail);
void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DRuntime& oDetail);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_FONTMETAINFORMATION_H