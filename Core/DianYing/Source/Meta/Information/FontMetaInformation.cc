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

#include <filesystem>

//!
//! Local translation unit code
//!

namespace 
{

MDY_SET_IMMUTABLE_STRING(header_SpecifierName, "SpecifierName");
MDY_SET_IMMUTABLE_STRING(header_FontType, "FontType");
MDY_SET_IMMUTABLE_STRING(header_FontInformationPath, "FontInfoPath");
MDY_SET_IMMUTABLE_STRING(header_FontTexturePathList, "FontTexturePath");
MDY_SET_IMMUTABLE_STRING(header_FontAlternativeFilePath, "FontFilePath");
MDY_SET_IMMUTABLE_STRING(header_IsEnableRuntimeCreation, "IsUsingRuntimeCreationWhenNotExist");

MDY_SET_IMMUTABLE_STRING(fonttype_SDF,    "SDF");
MDY_SET_IMMUTABLE_STRING(fonttype_Plain,  "Plain");

} /// unnamed namespace

//!
//! Implementation
//!

namespace dy
{

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

} /// ::dy namespace