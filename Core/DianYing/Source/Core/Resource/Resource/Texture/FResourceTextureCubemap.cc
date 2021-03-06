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
#include <Dy/Core/Resource/Resource/Texture/FResourceTextureCubemap.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Information/FInformationTextureCubemap.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Management/Helper/SProfilingHelper.h>

#include <Dy/Include/GlInclude.h>

namespace dy
{

FResourceTextureCubemap::FResourceTextureCubemap(_MIN_ const FInformationTextureCubemap& information) :
  AResourceTextureBase(static_cast<const FInformationTexture&>(information))
{
  const auto& temp = static_cast<const FInformationTextureCubemap&>(information);

  const auto optGlImageFormat = GlGetImageFormatFrom(information.GetFormat());
  MDY_ASSERT_MSG_FORCE(optGlImageFormat.has_value() == true, "Image format type must be valid.");
  const auto glImagePixelType = GlGetImagePixelTypeFrom(information.GetPixelReadType());
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
    optTextureId = XGLWrapper::CreateTexture(descriptor);
  }
  MDY_ASSERT_MSG(optTextureId.has_value() == true, "Texture id creation must be succeeded.");
  this->mTextureResourceId = *optTextureId;

  SProfilingHelper::IncreaseOnBindTextureCount(1);
}

FResourceTextureCubemap::~FResourceTextureCubemap()
{
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    XGLWrapper::DeleteTexture(this->mTextureResourceId);
  }
  SProfilingHelper::DecreaseOnBindTextureCount(1);
}

} /// ::dy namespace