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
#include <Dy/Core/Resource/Resource/FDyTextureGeneralResource.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Information/FDyTextureGeneralInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Management/Helper/SProfilingHelper.h>

namespace dy
{

FDyTextureGeneralResource::FDyTextureGeneralResource(_MIN_ const FDyTextureGeneralInformation& information) :
  FDyTextureResource(static_cast<const FDyTextureInformation&>(information))
{
  const auto& temp = static_cast<const FDyTextureGeneralInformation&>(information);
  this->mTextureSize = temp.GetSize();

  const auto optGlImageFormat = GlGetImageFormatFrom(information.GetFormat());
  MDY_ASSERT_MSG_FORCE(optGlImageFormat.has_value() == true, "Image format type must be valid.");
  const auto glImagePixelType = GlGetImagePixelTypeFrom(information.GetPixelReadType());
  MDY_ASSERT_MSG_FORCE(glImagePixelType != GL_NONE, "Image pixel format must be valid.");

  PDyGLTextureDescriptor descriptor {};
  { // Make internal descriptor for creating texture.
    descriptor.mBorderColor       = information.GetBorderColor();
    descriptor.mImageFormat       = *optGlImageFormat;
    descriptor.mImagePixelType    = glImagePixelType; 
    descriptor.mIsUsingCustomizedParameter  = temp.IsUsingCustomizedParamater();
    descriptor.mIsUsingDefaultMipmap        = temp.IsUsingDefaultMipmap();
    descriptor.mPtrBuffer         = &temp.GetBuffer();
    descriptor.mPtrParameterList  = &temp.GetParameterList();
    descriptor.mTextureSize       = this->mTextureSize;
    descriptor.mType              = this->mTextureType;
  }

  switch (this->mTextureType)
  { // Align size of texture following texture type.
  case ETextureStyleType::D1: this->mTextureSize.Y = 1; break;
  default: /* Do nothing */ break;
  case ETextureStyleType::NoneError: MDY_UNEXPECTED_BRANCH();
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

FDyTextureGeneralResource::~FDyTextureGeneralResource()
{
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    XGLWrapper::DeleteTexture(this->mTextureResourceId);
  }
  SProfilingHelper::DecreaseOnBindTextureCount(1);
}

} /// ::dy namespace