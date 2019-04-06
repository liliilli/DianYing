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
#include <Dy/Builtin/Texture/CheckerBright.h>

//!
//! Implementation
//!

namespace dy::builtin
{

void FDyBtTexCheckerBright::ConstructBuffer(_MOUT_ TBufferType& buffer, _MOUT_ PDyTextureInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName             = sName;
  property.mTextureType               = ETextureStyleType::D2;
  property.mTextureMapType_Deprecated = ETextureMapType::Diffuse;
  property.mTextureColorType    = EDyImageColorFormatStyle::RGB;
  property.mBuiltinBufferSize.X = 8;
  property.mBuiltinBufferSize.Y = 8;
  property.mIsUsingDefaultMipmapGeneration = true;
  property.mIsEnabledCustomedTextureParameter = true;
  property.mParameterList = {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::Repeat},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::Repeat},
  };

  // 01010101
  // 10101010
  // 01010101
  // 10101010
  // 01010101
  // 10101010
  // 01010101
  // 10101010
  constexpr const TU8 sOn = 0b11000000;
  constexpr const TU8 sOf = 0b00110000;
  constexpr const std::array<TU8, 3> mOn = {sOn, sOn, sOn};
  constexpr const std::array<TU8, 3> mOf = {sOf, sOf, sOf};
  constexpr const std::array<std::array<TU8, 3>, 64> infoChunk =
  {
    mOf, mOn, mOf, mOn, mOf, mOn, mOf, mOn,
    mOn, mOf, mOn, mOf, mOn, mOf, mOn, mOf,
    mOf, mOn, mOf, mOn, mOf, mOn, mOf, mOn,
    mOn, mOf, mOn, mOf, mOn, mOf, mOn, mOf,
    mOf, mOn, mOf, mOn, mOf, mOn, mOf, mOn,
    mOn, mOf, mOn, mOf, mOn, mOf, mOn, mOf,
    mOf, mOn, mOf, mOn, mOf, mOn, mOf, mOn,
    mOn, mOf, mOn, mOf, mOn, mOf, mOn, mOf,
  };
  buffer = ConvertToTU08VectorList(infoChunk);
}

} /// ::dy::builtin namespace