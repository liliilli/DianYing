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
#include <Dy/Builtin/Texture/SSAONoiseMap.h>
#include <Dy/Helper/Math/Random.h>

//!
//! Local functions.
//!

namespace
{

MDY_NODISCARD dy::ATextureResource::TBufferType Convert(_MIN_ const std::array<dy::DVec2, 16> &iInfo)
{
  dy::ATextureResource::TBufferType result;
  for (const auto& [x, y] : iInfo)
  {
    // Insert x
    const TU8* ptr = reinterpret_cast<const TU8*>(&x);
    for (auto i = 0; i < sizeof(TF32); ++i) { result.push_back(*ptr++); }
    // Insert y
    ptr = reinterpret_cast<const TU8*>(&y);
    for (auto i = 0; i < sizeof(TF32); ++i) { result.push_back(*ptr++); }
  }
  return result;
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

void FDyBtTexSSAONoiseMap::ConstructBuffer(_MOUT_ TBufferType& buffer, _MOUT_ PDyTextureInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName             = sName;
  property.mTextureType               = ETextureStyleType::D2;
  property.mTextureMapType_Deprecated = ETextureMapType::Diffuse;
  property.mTextureColorType          = EDyImageColorFormatStyle::RG;
  property.mPixelReadType             = EDyGlImagePixelReadType::Float;
  property.mBuiltinBufferSize.X = 4;
  property.mBuiltinBufferSize.Y = 4;
  property.mIsUsingDefaultMipmapGeneration = false;
  property.mIsEnabledCustomedTextureParameter = true;
  property.mParameterList = {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::Repeat},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::Repeat},
  };

  // 0101
  // 0101
  // 1010
  // 1010
  std::array<DVec2, 16> infoChunk;
  for (auto& noiseItem : infoChunk) { noiseItem = random::RandomVector2Length(1); }

  buffer = Convert(infoChunk);
}

} /// ::dy::builtin namespace