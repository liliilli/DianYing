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
#include <Dy/Meta/Information/MetaInfoTexture.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/ColorRGB24.h>
#include <Dy/Helper/Library/HelperFilesystem.h>

//!
//! Forward declaration & local translation unit data
//!

namespace
{



} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyTextureInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTextureInstanceMetaInfo& p)
{
  p.mSourceType  = EDyResourceSource::External;
  p.mTextureType = DyJsonGetValueFrom<EDyTextureStyleType>(j, "TextureType");
  
  // Cubemap need to read 6 textures.
  if (p.mTextureType == decltype(p.mTextureType)::D2Cubemap)
  {
    p.mExternalFilePath = DyJsonGetValueFrom<PDyTextureInstanceMetaInfo::DCubemapFilePath>(j, "ExternalPath");
  }
  else
  { // std::string.
    p.mExternalFilePath = DyJsonGetValueFrom<std::string>(j, "ExternalPath");
    MDY_ASSERT_MSG_FORCE(DyFsIsFileExist(std::get<std::string>(p.mExternalFilePath)) == true, "File path is not valid.");
  }

  p.mTextureColorType = DyJsonGetValueFrom<EDyImageColorFormatStyle>(j, "TextureColorType");
  p.mPixelReadType    = DyJsonGetValueFrom<EDyGlImagePixelReadType>(j, "PixelReadType");
  p.mParameterList    = DyJsonGetValueFrom<TTextureParameterList>(j, "TextureParameters");
  p.mIsEnabledCustomedTextureParameter = DyJsonGetValueFrom<bool>(j, "IsUsingCustomizedParameter");

  p.mIsUsingDefaultMipmapGeneration = DyJsonGetValueFrom<bool>(j, "IsUsingDefaultMipmap");
  p.mBorderColor                    = DyJsonGetValueFrom<DDyColorRGBA32>(j, "BorderColor");
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTextureInstanceMetaInfo::DCubemapFilePath& p)
{
  DyJsonGetValueFromTo(j, "Front", p.mFrontPath);
  DyJsonGetValueFromTo(j, "Back", p.mBackPath);
  DyJsonGetValueFromTo(j, "Right", p.mRightPath);
  DyJsonGetValueFromTo(j, "Left", p.mLeftPath);
  DyJsonGetValueFromTo(j, "Top", p.mTopPath);
  DyJsonGetValueFromTo(j, "Bottom", p.mBottomPath);

  MDY_ASSERT_MSG_FORCE(DyFsIsFileExist(p.mFrontPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(DyFsIsFileExist(p.mBackPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(DyFsIsFileExist(p.mRightPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(DyFsIsFileExist(p.mLeftPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(DyFsIsFileExist(p.mTopPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(DyFsIsFileExist(p.mBottomPath) == true, "File path is not exist.");
}

} /// ::dy namespace