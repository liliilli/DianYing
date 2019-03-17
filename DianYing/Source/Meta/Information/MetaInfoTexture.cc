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
#include <Dy/Helper/Type/DColorRGB24.h>
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
  p.mTextureType = json::GetValueFrom<ETextureStyleType>(j, "TextureType");
  
  // Cubemap need to read 6 textures.
  if (p.mTextureType == decltype(p.mTextureType)::D2Cubemap)
  {
    p.mExternalFilePath = json::GetValueFrom<PDyTextureInstanceMetaInfo::DCubemapFilePath>(j, "ExternalPath");
  }
  else
  { // std::string.
    p.mExternalFilePath = json::GetValueFrom<std::string>(j, "ExternalPath");
    MDY_ASSERT_MSG_FORCE(IsFileExist(std::get<std::string>(p.mExternalFilePath)) == true, "File path is not valid.");
  }

  p.mTextureColorType = json::GetValueFrom<EDyImageColorFormatStyle>(j, "TextureColorType");
  p.mPixelReadType    = json::GetValueFrom<EDyGlImagePixelReadType>(j, "PixelReadType");
  p.mParameterList    = json::GetValueFrom<TTextureParameterList>(j, "TextureParameters");
  p.mIsEnabledCustomedTextureParameter = json::GetValueFrom<bool>(j, "IsUsingCustomizedParameter");

  p.mIsUsingDefaultMipmapGeneration = json::GetValueFrom<bool>(j, "IsUsingDefaultMipmap");
  p.mBorderColor                    = json::GetValueFrom<DColorRGBA32>(j, "BorderColor");
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTextureInstanceMetaInfo::DCubemapFilePath& p)
{
  json::GetValueFromTo(j, "Front", p.mFrontPath);
  json::GetValueFromTo(j, "Back", p.mBackPath);
  json::GetValueFromTo(j, "Right", p.mRightPath);
  json::GetValueFromTo(j, "Left", p.mLeftPath);
  json::GetValueFromTo(j, "Top", p.mTopPath);
  json::GetValueFromTo(j, "Bottom", p.mBottomPath);

  MDY_ASSERT_MSG_FORCE(IsFileExist(p.mFrontPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(IsFileExist(p.mBackPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(IsFileExist(p.mRightPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(IsFileExist(p.mLeftPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(IsFileExist(p.mTopPath) == true, "File path is not exist.");
  MDY_ASSERT_MSG_FORCE(IsFileExist(p.mBottomPath) == true, "File path is not exist.");
}

} /// ::dy namespace