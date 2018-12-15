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
#include <Dy/Builtin/Texture/ErrorBlue.h>

#include <Dy/Core/Resource/Internal/TextureType.h>
#include <Dy/Helper/Type/ColorRGBA32.h>

//!
//! Local variables
//!

namespace
{

constexpr const dy::DDyColorRGBA32 _0 = dy::DDyColorRGBA32(  0,   0, 255);
constexpr const dy::DDyColorRGBA32 _1 = dy::DDyColorRGBA32(255, 255, 255);

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::builtin
{

void FDyBuiltinTextureErrorBlue::ConstructBuffer(_MOUT_ TBufferType& buffer, _MOUT_ PDyTextureInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName     = FDyBuiltinTextureErrorBlue::sName;
  property.mTextureType       = EDyTextureStyleType::D2;
  property.mTextureColorType  = EDyImageColorFormatStyle::RGBA;
  property.mBuiltinBufferSize = DDyVectorInt2{ 16, 16 };
  property.mTextureMapType_Deprecated = EDyTextureMapType::Diffuse;

  std::array<DDyColorRGBA32, 256> infoChunk =
  { //      0          4           8           12
    /* 0 */_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_0,_1,_1,_0,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_1,_1,_1,_1,_0,_0,_0,_0,_0,_0,
    /* 4 */_0,_0,_0,_0,_0,_1,_1,_1,_1,_1,_1,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_1,_1,_1,_1,_1,_1,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_1,_1,_1,_1,_1,_1,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_1,_1,_1,_1,_1,_1,_0,_0,_0,_0,_0,
    /* 8 */_0,_0,_0,_0,_0,_0,_1,_1,_1,_1,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_0,_1,_1,_0,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_0,_1,_1,_0,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,
    /* 12*/_0,_0,_0,_0,_0,_0,_0,_1,_1,_0,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_0,_1,_1,_0,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,
    /*   */_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,
  };

  buffer = ConvertToTU08VectorList(infoChunk);
}

}