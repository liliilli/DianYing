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
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>

namespace dy
{

FDyTextureResource::FDyTextureResource(_MIN_ const FDyTextureInformation& information)
{
  const auto optGlImageFormat = DyGLGetImageFormatFrom(information.GetFormat());
  MDY_ASSERT(optGlImageFormat.has_value() == true, "Image format type must be valid.");

  // Forward dataBuffer's retrieved information to data members.
  this->mTextureName   = information.GetSpecifierName();
  this->mTextureType   = information.GetType();
  this->mTextureSize   = information.GetSize();

  switch (this->mTextureType)
  { // Align size of texture following texture type.
  case EDyTextureStyleType::D1: this->mTextureSize.Y = 1; break;
  case EDyTextureStyleType::D2: break;
  default: MDY_UNEXPECTED_BRANCH();
  }

  PDyGLTextureDescriptor descriptor {};
  { // Make internal descriptor for creating texture.
    descriptor.mBorderColor       = information.GetBorderColor();
    descriptor.mImageFormat       = *optGlImageFormat;
    descriptor.mIsUsingCustomizedParameter  = information.IsUsingCustomizedParamater();
    descriptor.mIsUsingDefaultMipmap        = information.IsUsingDefaultMipmap();
    descriptor.mPtrBuffer         = &information.GetBuffer();
    descriptor.mPtrParameterList  = &information.GetParameterList();
    descriptor.mTextureSize       = this->mTextureSize;
    descriptor.mType              = this->mTextureType;
  }

  // Create texture from shared context.
  auto optTextureId = FDyGLWrapper::CreateTexture(descriptor);
  MDY_ASSERT(optTextureId.has_value() == true, "Texture id creation must be succeeded.");

  this->mTextureResourceId = *optTextureId;
}

FDyTextureResource::~FDyTextureResource()
{
  if (this->mTextureResourceId) { glDeleteTextures(1, &mTextureResourceId); }
}

} /// ::dy namespace