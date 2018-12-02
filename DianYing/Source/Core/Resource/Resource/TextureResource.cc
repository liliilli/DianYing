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
#include <Dy/Core/Resource/Resource/TextureResource.h>

#include <Dy/Core/Resource/Resource/MaterialResource.h>
#include <Dy/Core/Resource/Information/TextureInformation.h>
#include <Dy/Helper/Internal/ImageBinaryBuffer.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

CDyTextureResource::~CDyTextureResource()
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

EDySuccess CDyTextureResource::pfInitializeTextureResource(_MIN_ const DDyTextureInformation& textureInformation)
{
  // Make image binary data buffer.
  const auto& textureInfo         = textureInformation.GetInformation();
  const auto& textureBuffer       = textureInformation.GetBuffer();
  const auto  textureBufferFormat = textureInformation.GetFormat();

  TI32 glImageFormat = GL_NO_ERROR;
  switch (textureBufferFormat)
  {
  case EDyImageColorFormatStyle::R:     glImageFormat = GL_RED;   break;
  case EDyImageColorFormatStyle::RG:    glImageFormat = GL_RG;    break;
  case EDyImageColorFormatStyle::RGB:   glImageFormat = GL_RGB;   break;
  case EDyImageColorFormatStyle::RGBA:  glImageFormat = GL_RGBA;  break;
  default: MDY_UNEXPECTED_BRANCH(); return DY_FAILURE;
  }

  // Forward dataBuffer's retrieved information to data members.
  this->mTextureName   = textureInfo.mSpecifierName;
  this->mTextureType   = textureInfo.mTextureType;
  this->mTextureWidth  = textureInfo.mBuiltinBufferSize.X;
  switch (textureInfo.mTextureType)
  {
  case EDyTextureStyleType::D1: this->mTextureHeight = 1; break;
  case EDyTextureStyleType::D2: this->mTextureHeight = textureInfo.mBuiltinBufferSize.Y; break;
  default: MDY_UNEXPECTED_BRANCH();  break;
  }

  GLenum glTextureType = GL_NONE;
  // Get GL_TEXTURE_ TYPE from textureInfo.
  switch (this->mTextureType)
  {
  case EDyTextureStyleType::D1:
    glGenTextures(1, &this->mTextureResourceId);
    glBindTexture(GL_TEXTURE_1D, this->mTextureResourceId);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, this->mTextureWidth, 0, glImageFormat, GL_UNSIGNED_BYTE, textureBuffer.data());
    glTextureType = GL_TEXTURE_1D;
    break;
  case EDyTextureStyleType::D2:
    glGenTextures(1, &this->mTextureResourceId);
    glBindTexture(GL_TEXTURE_2D, this->mTextureResourceId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->mTextureWidth, this->mTextureHeight, 0, glImageFormat, GL_UNSIGNED_BYTE, textureBuffer.data());
    glTextureType = GL_TEXTURE_2D;
    break;
  default: MDY_UNEXPECTED_BRANCH(); return DY_FAILURE;
  }

  // Check PDyTextureConstructionBaseDesc::mIsUsingDefaultMipmapGeneration for generating mipmap.
  // Specifies the target to which the texture whose mimaps to generate is bound.
  // target​ must be GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_CUBE_MAP, or GL_TEXTURE_CUBE_MAP_ARRAY.
  if (textureInfo.mIsUsingDefaultMipmapGeneration == true) { glGenerateMipmap(glTextureType); }

  // Set texture parameters.
  if (textureInfo.mIsEnabledCustomedTextureParameter == true)
  {
    MDY_ASSERT(DyCheckTextureParameterList(textureInfo.mParameterList) == DY_SUCCESS, "Texture Parameter validation failed.");

    // Apply parameter option list to attachment.
    bool isThisAttachmentUsingClampToBorder = false;
    for (const auto& parameter : textureInfo.mParameterList)
    { // Check there is ClmapToBorder for border coloring.
      if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isThisAttachmentUsingClampToBorder = true; }
      // Set parameter
      glTexParameteri(glTextureType,
          DyGetParameterNameValue(parameter.mParameterOption),
          DyGetParameterValueValue(parameter.mParameterValue));
    }

    if (isThisAttachmentUsingClampToBorder == true)
    { // If isThisAttachmentUsingClampToBorder is true, apply border color to texture.
      glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, textureInfo.mBorderColor.Data());
    }
  }

  // Reset to default.
  glBindTexture(glTextureType, 0);
  return DY_SUCCESS;
}

EDySuccess CDyTextureResource::pfInitializeTextureResourceWithChunk(const PDyTextureConstructionBufferChunkDescriptor& descriptor)
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

void CDyTextureResource::__pfSetMaterialResourceLink(NotNull<CDyMaterialResource*> ptr) const noexcept
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
                  "CDyTextureResource::__pfSetMaterialResourceLink", ptr->GetMaterialName());
    MDY_UNEXPECTED_BRANCH();
  }
}

} /// ::dy namespace
