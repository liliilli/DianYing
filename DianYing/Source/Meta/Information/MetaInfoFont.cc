#include <precompiled.h>
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

/// Header file
#include <Dy/Meta/Information/MetaInfoFont.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Library/HelperFilesystem.h>
#include <Dy/Helper/Internal/XStringSwitch.h>

namespace dy
{

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation& iFont)
{
  oJson = nlohmann::json
  {
    {"SpecifierName", iFont.mSpecifierName},
    {"Type", iFont.mLoadingType},
    {"Uuid", iFont.mUuid},
  };

  using EEnum = PDyMetaFontInformation::EXPR_E(ELoadingType);
  switch (iFont.mLoadingType)
  {
  case EEnum::Builtin: /* Do nothing */
  case EEnum::ExternalCompressed:
  case EEnum::ExternalPlain:
  case EEnum::Runtime:
    break;
  };
}

void from_json(const nlohmann::json& iJson, PDyMetaFontInformation& oFont)
{
  json::GetValueFromTo(iJson, "SpecifierName", oFont.mSpecifierName);
  json::GetValueFromTo(iJson, "Type", oFont.mLoadingType);
  json::GetValueFromTo(iJson, "Uuid", oFont.mUuid);

  using EEnum = PDyMetaFontInformation::EXPR_E(ELoadingType);
  using EType = PDyMetaFontInformation;
  switch (oFont.mLoadingType)
  {
  case EEnum::Builtin: /* Do nothing */ break;
  case EEnum::ExternalCompressed:
    oFont.mDetails = json::GetValueFrom<EType::DExternalCompressed>(iJson, "Details");
    break;
  case EEnum::ExternalPlain:
    oFont.mDetails = json::GetValueFrom<EType::DExternalPlain>(iJson, "Details");
    break;
  case EEnum::Runtime:
    oFont.mDetails = json::GetValueFrom<EType::DRuntime>(iJson, "Details");
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::EXPR_E(ELoadingType)& iVar)
{
  oJson = PDyMetaFontInformation::ELoadingType::ToString(iVar);
}

void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::EXPR_E(ELoadingType)& oVar)
{
  using EType = PDyMetaFontInformation::ELoadingType;
  oVar = EType::ToEnum(json::GetValue<std::string>(iJson));

  MDY_ASSERT_FORCE(oVar != EType::__Error);
}

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::EXPR_E(EFontType)& iVar)
{
  oJson = PDyMetaFontInformation::EFontType::ToString(iVar);
}

void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::EXPR_E(EFontType)& oVar)
{
  using EType = PDyMetaFontInformation::EFontType;
  oVar = EType::ToEnum(json::GetValue<std::string>(iJson));
}

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DExternalPlain& iDetail)
{
  oJson = nlohmann::json
  {
    {"FontType", iDetail.mFontType},
    {"FontInfoPath", iDetail.mFontInformationPath},
    {"FontTexturePath", iDetail.mFontTexturePathList},
  };
}

void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DExternalPlain& oDetail)
{
  json::GetValueFromTo(iJson, "FontType", oDetail.mFontType);
  json::GetValueFromTo(iJson, "FontInfoPath", oDetail.mFontInformationPath);
  json::GetValueFromTo(iJson, "FontTexutrePath", oDetail.mFontTexturePathList);
}

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DExternalCompressed& iDetail)
{
  oJson = nlohmann::json
  {
    { "FontType", iDetail.mFontType },
    { "FilePath", iDetail.mFilePath }
  };
}

void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DExternalCompressed& oDetail)
{
  json::GetValueFromTo(iJson, "FontType", oDetail.mFontType);
  json::GetValueFromTo(iJson, "FilePath", oDetail.mFilePath);
}

void to_json(nlohmann::json& oJson, const PDyMetaFontInformation::DRuntime& iDetail)
{
  oJson = nlohmann::json
  {
    {"FilePath", iDetail.mFontFilePath},
  };
}

void from_json(const nlohmann::json& iJson, PDyMetaFontInformation::DRuntime& oDetail)
{
  json::GetValueFromTo(iJson, "FilePath", oDetail.mFontFilePath);
}

#ifdef false
PDyMetaFontInformation PDyMetaFontInformation::CreateWithJson(const nlohmann::json& fontAtlas)
{
  /// @brief  Check font meta object has headers correctly.
  /// @param  fontAtlas Input json atlas.
  static auto CheckHeaderValidity = [](const nlohmann::json& fontAtlas) -> EDySuccess
  {
    if (json::HasJsonKey(fontAtlas, header_SpecifierName) == false)           { return DY_FAILURE; }
    if (json::HasJsonKey(fontAtlas, header_FontType) == false)                { return DY_FAILURE; }
    if (json::HasJsonKey(fontAtlas, header_FontInformationPath) == false)     { return DY_FAILURE; }
    if (json::HasJsonKey(fontAtlas, header_FontTexturePathList) == false)     { return DY_FAILURE; }
    if (json::HasJsonKey(fontAtlas, header_FontAlternativeFilePath) == false) { return DY_FAILURE; }
    if (json::HasJsonKey(fontAtlas, header_IsEnableRuntimeCreation) == false) { return DY_FAILURE; }
    return DY_SUCCESS;
  };

  /// @brief  Return PDyMetaFontInformation::EFontType from string. when not exist, just return __Error value.
  /// @param  fontType
  /// @return EFontType type or error type.
  static auto GetEFontTypeFrom = [](const std::string& fontType) -> EFontType
  {
    if (fontType == fonttype_SDF)   { return EFontType::SDF; }
    if (fontType == fonttype_Plain) { return EFontType::Plain; }

    MDY_UNEXPECTED_BRANCH(); throw; // Intentional.
  };

  //!
  //! FunctionBody∨
  //!

  // (1) Pre-validity test.
  MDY_ASSERT_MSG(
    CheckHeaderValidity(fontAtlas) == DY_SUCCESS, 
    "Font meta object must have all headers correctly.");

  // (2) Get Information from json atlas.
  PDyMetaFontInformation resultInstance = {};

  resultInstance.mSpecifierName           = json::GetValueFrom<std::string>(fontAtlas, header_SpecifierName);
  resultInstance.mFontType                = GetEFontTypeFrom(json::GetValueFrom<std::string>(fontAtlas, header_FontType));
  resultInstance.mFontInformationPath     = json::GetValueFrom<std::string>(fontAtlas, header_FontInformationPath);
  resultInstance.mFontTexturePathList     = json::GetValueFrom<std::vector<std::string>>(fontAtlas, header_FontTexturePathList);
  resultInstance.mAlternativeFontFilePath = json::GetValueFrom<std::string>(fontAtlas, header_FontAlternativeFilePath);
  resultInstance.mIsEnableRuntimeCreation = json::GetValueFrom<bool>(fontAtlas, header_IsEnableRuntimeCreation);

  // (3) Post-validity test.
  using namespace std::filesystem;
  MDY_ASSERT_MSG(
    resultInstance.mFontType != EFontType::__Error,    
    "FontType must not be null type (error type).");
  MDY_ASSERT_MSG(
    exists(resultInstance.mFontInformationPath) == true, 
    "Font information `.dyFntRes` must be exist but not exist.");
  for (const auto& texturePath : resultInstance.mFontTexturePathList)
  {
    MDY_ASSERT_MSG(
      exists(texturePath) == true, 
      "Font texture specified but doesn't exist in filesystem.");
  }
  if (resultInstance.mIsEnableRuntimeCreation == true)
  {
    MDY_ASSERT_MSG(
      exists(resultInstance.mAlternativeFontFilePath) == true, 
      "Alternative font file path must be exist.");
  }

  return resultInstance;
}
#endif

} /// ::dy namespaceb