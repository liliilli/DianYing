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
#include <Dy/Core/Resource/Internal/TextureType.h>
#include <Dy/Helper/Math/Random.h>

//!
//! Local functions.
//!

namespace
{

MDY_NODISCARD dy::ADyTextureResource::TBufferType Convert(_MIN_ const std::array<dy::DDyVector2, 16> &iInfo)
{
  dy::ADyTextureResource::TBufferType result;
  for (const auto& [x, y] : iInfo)
  {
    // Insert x
    const TU08* ptr = reinterpret_cast<const TU08*>(&x);
    for (auto i = 0; i < sizeof(TF32); ++i) { result.push_back(*ptr++); }
    // Insert y
    ptr = reinterpret_cast<const TU08*>(&y);
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
  property.mTextureType               = EDyTextureStyleType::D2;
  property.mTextureMapType_Deprecated = EDyTextureMapType::Diffuse;
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
  std::array<DDyVector2, 16> infoChunk;
  for (auto& noiseItem : infoChunk) { noiseItem = random::RandomVector2Length(1); }

  buffer = Convert(infoChunk);
}

} /// ::dy::builtin namespace