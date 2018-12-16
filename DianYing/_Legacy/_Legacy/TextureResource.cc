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
/// @TODO IMPLEMENT CUSTOM PARAMETER OPTION FEATURE USING DESCRIPTOR LIKE.
///

/// Header file
#include <Dy/Core/Resource/Resource_Deprecated/TextureResource_Deprecated.h>

#include <Dy/Core/Resource/Resource_Deprecated/MaterialResource_Deprecated.h>
#include <Dy/Core/Resource/Information_Deprecated/TextureInformation_Deprecated.h>
#include <Dy/Helper/Internal/ImageBinaryBuffer.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>

namespace dy
{

CDyTextureResource_Deprecated::~CDyTextureResource_Deprecated()
{
  // Release heap resources
  if (mTextureResourceId)
  {
    glDeleteTextures(1, &mTextureResourceId);
  }

  // Unbind previous and next level.
  if (__mLinkedTextureInformationPtr)
  {
    __mLinkedTextureInformationPtr->__pfLinkTextureResource(nullptr);
  }
  for (auto& [notUsed, materialPtr] : __mBindMaterialPtrCounters)
  {
    materialPtr->__pfResetTextureResourcePtr(DyMakeNotNull(this));
  }
}

EDySuccess CDyTextureResource_Deprecated::pfInitializeTextureResource(_MIN_ const DDyTextureInformation_Deprecated& textureInformation)
{
  const auto& textureInfo     = textureInformation.GetInformation();
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
  return DY_SUCCESS;
}

EDySuccess CDyTextureResource_Deprecated::pfInitializeTextureResourceWithChunk(const PDyTextureConstructionBufferChunkDescriptor& descriptor)
{
  int32_t glImageFormat = GL_NO_ERROR;
  switch (descriptor.mTextureColorType)
  {
  case EDyImageColorFormatStyle::R:     glImageFormat = GL_RED;   break;
  case EDyImageColorFormatStyle::RG:    glImageFormat = GL_RG;    break;
  case EDyImageColorFormatStyle::RGB:   glImageFormat = GL_RGB;   break;
  case EDyImageColorFormatStyle::RGBA:  glImageFormat = GL_RGBA;  break;
  default: MDY_UNEXPECTED_BRANCH();  return DY_FAILURE;
  }

  GLenum glTextureType = GL_NONE;
  // Get GL_TEXTURE_ TYPE from textureInfo.
  switch (descriptor.mTextureType)
  {
  case EDyTextureStyleType::D1:
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_1D, mTextureResourceId);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, descriptor.mWidth, 0, glImageFormat, GL_UNSIGNED_BYTE, descriptor.mBufferPtr);
    glTextureType = GL_TEXTURE_1D;
    break;
  case EDyTextureStyleType::D2:
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, descriptor.mWidth, descriptor.mHeight, 0, glImageFormat, GL_UNSIGNED_BYTE, descriptor.mBufferPtr);
    glTextureType = GL_TEXTURE_2D;
    break;
  default: MDY_UNEXPECTED_BRANCH();  return DY_FAILURE;
  }

  // Check PDyTextureConstructionBaseDesc::mIsUsingDefaultMipmapGeneration for generating mipmap.
  // Specifies the target to which the texture whose mimaps to generate is bound.
  // target​ must be GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_CUBE_MAP, or GL_TEXTURE_CUBE_MAP_ARRAY.
  if (descriptor.mIsUsingDefaultMipmapGeneration == true) { glGenerateMipmap(glTextureType); }

  // Forward dataBuffer's retrieved information to data members.
  this->mTextureName    = descriptor.mTextureSpecifierName;
  this->mTextureType    = descriptor.mTextureType;
  this->mTextureWidth   = descriptor.mHeight;
  switch (descriptor.mTextureType)
  {
  case EDyTextureStyleType::D1: this->mTextureHeight = 1; break;
  case EDyTextureStyleType::D2: this->mTextureHeight = descriptor.mHeight; break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  // Set texture parameters.
  if (descriptor.mIsEnabledCustomedTextureParameter == true)
  { // Check for parameter types.
    MDY_ASSERT(DyCheckTextureParameterList(descriptor.mParameterList) == DY_SUCCESS, "FFFFFFFF");

    // Apply parameter option list to attachment.
    bool isThisAttachmentUsingClampToBorder = false;
    for (const auto& parameter : descriptor.mParameterList)
    { // Check there is ClmapToBorder for border coloring.
      if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isThisAttachmentUsingClampToBorder = true; }
      // Set parameter
      glTexParameteri(glTextureType,
          DyGetParameterNameValue(parameter.mParameterOption),
          DyGetParameterValueValue(parameter.mParameterValue));
    }

    if (isThisAttachmentUsingClampToBorder == true)
    { // If isThisAttachmentUsingClampToBorder is true, apply border color to texture.
      glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, descriptor.mBorderColor.Data());
    }
  }

  // Reset to default.
  glBindTexture(glTextureType, 0);
  return DY_SUCCESS;;
}

void CDyTextureResource_Deprecated::__pfSetMaterialResourceLink(NotNull<CDyMaterialResource_Deprecated*> ptr) const noexcept
{
  if (this->__mBindMaterialPtrCounters.find(ptr) != this->__mBindMaterialPtrCounters.end())
  { // If found, just neglect.
    return;
  }

  // If not found, create counter instance for valid mateiral resource.
  auto [it, result] = this->__mBindMaterialPtrCounters.try_emplace(ptr, ptr);
  if (result == false)
  {
    MDY_LOG_ERROR("{} | Failed to link material resource. | Model name : {}",
                  "CDyTextureResource_Deprecated::__pfSetMaterialResourceLink", ptr->GetMaterialName());
    MDY_UNEXPECTED_BRANCH();
  }
}

} /// ::dy namespace
