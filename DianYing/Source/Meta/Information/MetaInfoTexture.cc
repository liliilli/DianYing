#include <precompiled.h>
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

/// Header file
#include <Dy/Meta/Information/MetaInfoTexture.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/ColorRGB24.h>

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
  p.mSourceType = EDyResourceSource::External;
  DyJsonGetValueFromTo(j, "SpecifierName", p.mSpecifierName);
  DyJsonGetValueFromTo(j, "FilePath",      p.mExternalFilePath);
  p.mTextureType                    = DyJsonGetValueFrom<EDyTextureStyleType>(j, "TextureType");
  p.mIsUsingDefaultMipmapGeneration = DyJsonGetValueFrom<bool>(j, "IsUsingMipmapGeneration");
  p.mBorderColor                    = DyJsonGetValueFrom<DDyColorRGB24>(j, "BorderColor");
  p.mTextureColorType               = DyJsonGetValueFrom<EDyImageColorFormatStyle>(j, "ColorType");

  p.mParameterList = DyJsonGetValueFrom<TTextureParameterList>(j, "Parameters");
  p.mIsEnabledCustomedTextureParameter = DyJsonGetValueFrom<bool>(j, "IsUsingCustomizedTextureParameter");
}

} /// ::dy namespace