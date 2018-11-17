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

#include <Dy/Core/Component/Internal/TextureType.h>
#include <Dy/Helper/Type/ColorRGBA32.h>
#include <Dy/Management/IO/IOResourceManager.h>

//!
//! Local variables
//!

namespace dy
{
constexpr const DDyColorRGBA32 _0 = DDyColorRGBA32(  0,   0, 255);
constexpr const DDyColorRGBA32 _1 = DDyColorRGBA32(255, 255, 255);
}

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinTextureErrorBlue::FDyBuiltinTextureErrorBlue()
{
  PDyTextureConstructionBufferChunkDescriptor desc = {};
  desc.mTextureSpecifierName       = FDyBuiltinTextureErrorBlue::sName;
  desc.mTextureType       = EDyTextureStyleType::D2;
  desc.mTextureMapType    = EDyTextureMapType::Diffuse;
  desc.mTextureColorType  = EDyImageColorFormatStyle::RGBA;
  desc.mWidth             = 16;
  desc.mHeight            = 16;

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
  desc.mBufferPtr = infoChunk.data();

  MDY_CALL_ASSERT_SUCCESS(MDyIOResource::GetInstance().CreateTextureResourceWithChunk(desc));
}

}