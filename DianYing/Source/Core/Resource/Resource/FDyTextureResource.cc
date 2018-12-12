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

namespace dy
{

FDyTextureResource::FDyTextureResource(_MIN_ const FDyTextureInformation& information)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
#ifdef false
  const auto& textureInfo     =
  const auto optGlImageFormat = DyGLGetImageFormatFrom(textureInformation.GetFormat());
  MDY_ASSERT(optGlImageFormat.has_value() == true, "Image format type must be valid.");

  // Forward dataBuffer's retrieved information to data members.
  this->mTextureName   = textureInfo.mSpecifierName;
  this->mTextureType   = textureInfo.mTextureType;
  this->mTextureWidth  = textureInfo.mBuiltinBufferSize.X;
  switch (textureInfo.mTextureType)
  { // Align size of texture following texture type.
  case EDyTextureStyleType::D1: this->mTextureHeight = 1; break;
  case EDyTextureStyleType::D2: this->mTextureHeight = textureInfo.mBuiltinBufferSize.Y; break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(DY_FAILURE);
  }

  PDyGLTextureDescriptor descriptor {};
  { // Make internal descriptor for creating texture.
    descriptor.mBorderColor       = textureInfo.mBorderColor;
    descriptor.mImageFormat       = *optGlImageFormat;
    descriptor.mIsUsingCustomizedParameter = textureInfo.mIsEnabledCustomedTextureParameter;
    descriptor.mIsUsingDefaultMipmap = textureInfo.mIsUsingDefaultMipmapGeneration;
    descriptor.mPtrBuffer         = &textureInformation.GetBuffer();
    descriptor.mPtrParameterList  = &textureInfo.mParameterList;
    descriptor.mTextureSize       = DDyVectorInt2{this->mTextureWidth, this->mTextureHeight};
    descriptor.mType              = textureInfo.mTextureType;
  }

  // Create texture from shared context.
  auto optTextureId = FDyGLWrapper::CreateTexture(descriptor);
  MDY_ASSERT(optTextureId.has_value() == true, "Texture id creation must be succeeded.");
  this->mTextureResourceId = *optTextureId;
  return DY_SUCCESS
#endif
}

FDyTextureResource::~FDyTextureResource()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace