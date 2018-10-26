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

#include <Dy/Core/Component/Internal/TextureType.h>
#include <Dy/Management/HeapResourceManager.h>

//!
//! Local variables
//!

namespace dy
{
constexpr const TU08 sOn = 0b11111111;
constexpr const TU08 sOf = 0b00000000;
}

//!
//! Implementation
//!

namespace dy::builtin
{

FDyBuiltinTextureChecker::FDyBuiltinTextureChecker()
{
  // @TODO INITIALIZE WITH
  // 01010101
  // 10101010
  // 01010101
  // 10101010
  // 01010101
  // 10101010
  // 01010101
  // 10101010
  PDyTextureConstructionBufferChunkDescriptor desc = {};
  desc.mTextureName       = FDyBuiltinTextureChecker::sName;
  desc.mTextureType       = EDyTextureStyleType::D2;
  desc.mTextureMapType    = EDyTextureMapType::Diffuse;
  desc.mTextureColorType  = EDyImageColorFormatStyle::RGB;
  desc.mWidth             = 8;
  desc.mHeight            = 8;

  constexpr std::array<TU08, 3> mOn = {sOn, sOn, sOn};
  constexpr std::array<TU08, 3> mOf = {sOf, sOf, sOf};

  std::array<std::array<TU08, 3>, 64> infoChunk =
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
  desc.mBufferPtr = infoChunk.data();
  desc.mIsUsingDefaultMipmapGeneration = true;

  MDY_CALL_ASSERT_SUCCESS(MDyHeapResource::GetInstance().CreateTextureResourceWithChunk(desc));
}

} /// ::dy::builtin namespace