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
#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Core/Resource/Internal/TextureType.h>

//!
//! Implementation
//!

namespace dy::builtin
{

void FDyBuiltinTextureChecker::ConstructBuffer(_MOUT_ TBufferType& buffer, _MOUT_ PDyTextureInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName             = sName;
  property.mTextureType               = EDyTextureStyleType::D2;
  property.mTextureMapType_Deprecated = EDyTextureMapType::Diffuse;
  property.mTextureColorType    = EDyImageColorFormatStyle::RGB;
  property.mBuiltinBufferSize.X = 8;
  property.mBuiltinBufferSize.Y = 8;
  property.mIsUsingDefaultMipmapGeneration = true;
  property.mIsEnabledCustomedTextureParameter = true;
  property.mParameterList = {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToEdge},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToEdge},
  };

  // 01010101
  // 10101010
  // 01010101
  // 10101010
  // 01010101
  // 10101010
  // 01010101
  // 10101010
  constexpr const TU08 sOn = 0b11111111;
  constexpr const TU08 sOf = 0b00000000;
  constexpr const std::array<TU08, 3> mOn = {sOn, sOn, sOn};
  constexpr const std::array<TU08, 3> mOf = {sOf, sOf, sOf};
  constexpr const std::array<std::array<TU08, 3>, 64> infoChunk =
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