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
#include <Dy/Core/Resource/Resource/FDyTextureCubemapResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Information/FDyTextureCubemapInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>

namespace dy
{

FDyTextureCubemapResource::FDyTextureCubemapResource(_MIN_ const FDyTextureCubemapInformation& information) :
  FDyTextureResource(static_cast<const FDyTextureInformation&>(information))
{
  const auto& temp = static_cast<const FDyTextureCubemapInformation&>(information);

  const auto optGlImageFormat = DyGLGetImageFormatFrom(information.GetFormat());
  MDY_ASSERT_MSG_FORCE(optGlImageFormat.has_value() == true, "Image format type must be valid.");
  const auto glImagePixelType = DyGlGetImagePixelTypeFrom(information.GetPixelReadType());
  MDY_ASSERT_MSG_FORCE(glImagePixelType != GL_NONE, "Image pixel format must be valid.");

  PDyGLTextureCubemapDescriptor descriptor {};
  { // Make internal descriptor for creating texture.
    descriptor.mBorderColor       = information.GetBorderColor();
    descriptor.mImageFormat       = *optGlImageFormat;
    descriptor.mImagePixelType    = glImagePixelType; 
    descriptor.mPtrParameterList  = &temp.GetParameterList();
    descriptor.mType              = this->mTextureType;
    descriptor.mIsUsingCustomizedParameter = temp.IsUsingCustomizedParamater();
    descriptor.mIsUsingDefaultMipmap       = temp.IsUsingDefaultMipmap();
    // Cubemap properties.
    descriptor.mTopBuffer     = &temp.GetBufferOf(EDyCubemapFragment::Top);
    descriptor.mTopSize       = temp.GetSizeOf(EDyCubemapFragment::Top);
    descriptor.mBottomBuffer  = &temp.GetBufferOf(EDyCubemapFragment::Bottom);
    descriptor.mBottomSize    = temp.GetSizeOf(EDyCubemapFragment::Bottom);
    descriptor.mLeftBuffer    = &temp.GetBufferOf(EDyCubemapFragment::Left);
    descriptor.mLeftSize      = temp.GetSizeOf(EDyCubemapFragment::Left);
    descriptor.mRightBuffer   = &temp.GetBufferOf(EDyCubemapFragment::Right);
    descriptor.mRightSize     = temp.GetSizeOf(EDyCubemapFragment::Right);
    descriptor.mFrontBuffer   = &temp.GetBufferOf(EDyCubemapFragment::Front);
    descriptor.mFrontSize     = temp.GetSizeOf(EDyCubemapFragment::Front);
    descriptor.mBackBuffer    = &temp.GetBufferOf(EDyCubemapFragment::Back);
    descriptor.mBackSize      = temp.GetSizeOf(EDyCubemapFragment::Back);
  }

  // Create texture from shared context.
  std::optional<TU32> optTextureId;
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    optTextureId = FDyGLWrapper::CreateTexture(descriptor);
  }
  MDY_ASSERT_MSG(optTextureId.has_value() == true, "Texture id creation must be succeeded.");
  this->mTextureResourceId = *optTextureId;

  SDyProfilingHelper::IncreaseOnBindTextureCount(1);
}

FDyTextureCubemapResource::~FDyTextureCubemapResource()
{
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    FDyGLWrapper::DeleteTexture(this->mTextureResourceId);
  }
  SDyProfilingHelper::DecreaseOnBindTextureCount(1);
}

} /// ::dy namespace