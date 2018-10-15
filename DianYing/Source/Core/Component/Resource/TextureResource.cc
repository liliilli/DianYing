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
#include <Dy/Core/Component/Resource/TextureResource.h>

#include <Dy/Core/Component/Resource/MaterialResource.h>
#include <Dy/Core/Component/Information/TextureInformation.h>
#include <Dy/Helper/Internal/ImageBinaryBuffer.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

///
/// @brief Set texture default parameter setting.
/// GL_TEXTURE_MIN_FILTER to GL_NEAREST
/// GL_TEXTURE_MAG_FILTER to GL_NEAREST
/// GL_TEXTURE_WRAP_S to GL_REPEAT
/// GL_TEXTURE_WRAP_T to GL_REPEAT
///
void DyGlSetDefaultOptionSetting(uint32_t textureId) {
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

} /// ::unnamed namespace

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

EDySuccess CDyTextureResource::pfInitializeTextureResource(const DDyTextureInformation& textureInformation)
{
  // Make image binary data buffer.
  const auto& textureInfo = textureInformation.GetInformation();

  std::unique_ptr<DDyImageBinaryDataBuffer> dataBuffer = nullptr;
  if (textureInfo.mIsEnabledAbsolutePath)
  {
    dataBuffer = std::make_unique<DDyImageBinaryDataBuffer>(textureInfo.mTextureFileAbsolutePath);
  }
  else
  {
    dataBuffer = std::make_unique<DDyImageBinaryDataBuffer>(textureInfo.mTextureFileLocalPath);
  }

  if (!dataBuffer->IsBufferCreatedProperly())
  {
    return DY_FAILURE;
  }

  int32_t glImageFormat = GL_NO_ERROR;
  switch (dataBuffer->GetImageFormat())
  {
  case EDyImageColorFormatStyle::R:     glImageFormat = GL_RED;   break;
  case EDyImageColorFormatStyle::RG:    glImageFormat = GL_RG;    break;
  case EDyImageColorFormatStyle::RGB:   glImageFormat = GL_RGB;   break;
  case EDyImageColorFormatStyle::RGBA:  glImageFormat = GL_RGBA;  break;
  default: // Unexpected branch (Error)
    assert(false);
    return DY_FAILURE;
  }

  // Get GL_TEXTURE_ TYPE from textureInfo.
  switch (textureInfo.mTextureType)
  {
  case EDyTextureStyleType::D1:
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_1D, mTextureResourceId);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA,
                 dataBuffer->GetImageWidth(), 0,
                 glImageFormat, GL_UNSIGNED_BYTE,
                 dataBuffer->GetBufferStartPoint());
    break;
  case EDyTextureStyleType::D2:
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 dataBuffer->GetImageWidth(), dataBuffer->GetImageHeight(), 0,
                 glImageFormat, GL_UNSIGNED_BYTE,
                 dataBuffer->GetBufferStartPoint());
    break;
  default:
    assert(false);
    /// std;:cout << Not expected type. << '\n';
    return DY_FAILURE;
  }

  // Forward dataBuffer's retrieved information to data members.
  this->mTextureName    = textureInfo.mTextureName;
  this->mTextureType    = textureInfo.mTextureType;
  this->mTextureWidth   = dataBuffer->GetImageWidth();
  switch (textureInfo.mTextureType)
  {
  case EDyTextureStyleType::D1: this->mTextureHeight  = 1; break;
  case EDyTextureStyleType::D2: this->mTextureHeight  = dataBuffer->GetImageHeight(); break;
  default: break;
  }

  // Set texture parameters.
  if (!textureInfo.mIsEnabledCustomedTextureParameter)
  {
    DyGlSetDefaultOptionSetting(mTextureResourceId);
  }
  else
  {
#ifdef false
    const auto& textureOptions = textureConstructionDescriptor.mTextureParameterOptions;
    for (const auto& option : textureOptions)
    {
      const auto glOption = DyGlSpecifyOption(option.GetOption());
      if (glOption == GL_NO_ERROR)
      {
        // Output error log to sink...
        return DY_FAILURE;
      }

      const auto glOptionValue = DyGlSpecifyOption(option.GetValue());
      if (glOptionValue == GL_NO_ERROR)
      {
        // Output error log to sink...
        return DY_FAILURE;
      }
    }
#endif
  }

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
  default: PHITOS_UNEXPECTED_BRANCH();  return DY_FAILURE;
  }

  // Get GL_TEXTURE_ TYPE from textureInfo.
  switch (descriptor.mTextureType)
  {
  case EDyTextureStyleType::D1:
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_1D, mTextureResourceId);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, descriptor.mWidth, 0, glImageFormat, GL_UNSIGNED_BYTE, descriptor.mBufferPtr);
    break;
  case EDyTextureStyleType::D2:
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, descriptor.mWidth, descriptor.mHeight, 0, glImageFormat, GL_UNSIGNED_BYTE, descriptor.mBufferPtr);
    break;
  default: PHITOS_UNEXPECTED_BRANCH();  return DY_FAILURE;
  }

  // Forward dataBuffer's retrieved information to data members.
  this->mTextureName    = descriptor.mTextureName;
  this->mTextureType    = descriptor.mTextureType;
  this->mTextureWidth   = descriptor.mHeight;
  switch (descriptor.mTextureType)
  {
  case EDyTextureStyleType::D1: this->mTextureHeight  = 1; break;
  case EDyTextureStyleType::D2: this->mTextureHeight  = descriptor.mHeight; break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

  // Set texture parameters.
  DyGlSetDefaultOptionSetting(mTextureResourceId);
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
    PHITOS_UNEXPECTED_BRANCH();
  }
}

} /// ::dy namespace
