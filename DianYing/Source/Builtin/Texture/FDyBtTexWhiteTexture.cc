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
#include <Dy/Builtin/Texture/FDyBtTexWhiteTexture.h>

//!
//! Implementation
//!

namespace dy::builtin
{

void FDyBtTexWhiteTexture::ConstructBuffer(TBufferType& oBuffer, PDyTextureInstanceMetaInfo& oProperty) noexcept
{
  oProperty.mSpecifierName = sName;
  oProperty.mTextureType = ETextureStyleType::D2;
  oProperty.mTextureMapType_Deprecated = ETextureMapType::Diffuse;
  oProperty.mTextureColorType = EDyImageColorFormatStyle::RGB;
  oProperty.mBuiltinBufferSize.X = 1;
  oProperty.mBuiltinBufferSize.Y = 1;
  oProperty.mIsUsingDefaultMipmapGeneration = false;
  oProperty.mIsEnabledCustomedTextureParameter = false;

  // 1
  constexpr const TU08 sOn = 0b11111111;
  constexpr const std::array<TU08, 3> mOn = {sOn, sOn, sOn};
  constexpr const std::array<std::array<TU08, 3>, 1 * 1> infoChunk = { sOn };

  oBuffer = ConvertToTU08VectorList(infoChunk);
}

} /// ::dy::builtin namespace