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
#include <Dy/Management/Internal/FramebufferManager.h>

namespace dy
{

EDySuccess MDyFramebuffer::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyFramebuffer::pfRelease()
{
  return DY_SUCCESS;
}

EDySuccess MDyFramebuffer::SetAttachmentInformation(const PDyGlAttachmentInformation& attachmentInfo)
{
  if (this->mValidAttachmentList.find(attachmentInfo.mAttachmentName) != this->mValidAttachmentList.end())
  {
    MDY_ASSERT(false, "attachmentInfo.mAttachmentName must not be duplicated in container's any arbitary attachment instance.");
    return DY_FAILURE;
  }

  // First, make a vacant space for information.
  auto [it, result] = this->mValidAttachmentList.try_emplace(attachmentInfo.mAttachmentName, nullptr);
  MDY_ASSERT(result == true, "Unexpected error occurred.");

  // Make heap instance and swap to vacant space.
  auto heapInstance = std::make_unique<PDyGlAttachmentInformation>(attachmentInfo);
  it->second.swap(heapInstance);
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(it->second), "Failed to swapping address for attachmentInfo. Unexpected error occurred");

  return DY_SUCCESS;
}

EDySuccess MDyFramebuffer::InitializeNewFrameBuffer(const PDyGlFrameBufferInformation& bufferInfo)
{ // Lambda functions


  // FunctionBody∨

  // Integrity test
  if (this->mValidFramebufferList.find(bufferInfo.mFrameBufferName) != this->mValidFramebufferList.end())
  {
    MDY_ASSERT(false, "bufferInfo.mFrameBufferName must not be duplicated in container's any arbitary framebuffer instance.");
    return DY_FAILURE;
  }

  // First, make a vacant space for information.
  auto [it, result] = this->mValidFramebufferList.try_emplace(bufferInfo.mFrameBufferName, nullptr);
  MDY_ASSERT(result == true, "Unexpected error occurred.");

  // Make heap instance and swap to vacant space.
  auto heapInstance = std::make_unique<PDyGlFrameBufferInformation>(bufferInfo);
  it->second.swap(heapInstance);
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(it->second), "Failed to swapping address for framebufferInfo. Unexpected error occurred.");

  // Find if there is attachment information in container.
  // If not found any attachment, just return FAILURE.
  PDyGlFrameBufferInformation& framebuffer = *it->second;
  for (const auto& attachment : framebuffer.mAttachmentList)
  {
    MDY_ASSERT(this->mValidAttachmentList.find(attachment.mAttachmentName) != this->mValidAttachmentList.end(),
        "Failed to find specified attachment instance from container. Unexpected error occurred.");
  }

  // Make frame buffer resource and attachment.
  glGenFramebuffers(1, &framebuffer.mFrameBufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.mFrameBufferId);

  const auto attachmentBufferCount = static_cast<TI32>(framebuffer.mAttachmentList.size());
  std::vector<GLuint> attachmentBufferIdList(attachmentBufferCount);
  std::vector<GLenum> attachmentTypeList = {};
  glGenTextures(attachmentBufferCount, attachmentBufferIdList.data());

  // Create attachment buffer resource.
  for (TI32 i = 0; i < attachmentBufferCount; ++i)
  {
    const PDyGlAttachmentBinderInformation& attachmentCursor = framebuffer.mAttachmentList[i];
    PDyGlAttachmentInformation& attachmentInstance = *this->mValidAttachmentList[attachmentCursor.mAttachmentName];

    // Check for parameter types.
    MDY_ASSERT(DyCheckTextureParameterList(attachmentInstance.mParameterList) == DY_SUCCESS, "FFFFFFFF");
    attachmentInstance.mAttachmentId = attachmentBufferIdList[i];
    // Bind texture.
    glBindTexture(GL_TEXTURE_2D, attachmentInstance.mAttachmentId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, attachmentInstance.mAttachmentSize.X, attachmentInstance.mAttachmentSize.Y, 0, GL_RGBA, GL_FLOAT, nullptr);

    // Apply parameter option list to attachment.
    bool isThisAttachmentUsingClampToBorder = false;
    for (const auto& parameter : attachmentInstance.mParameterList)
    { //
      if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder)
      {
        isThisAttachmentUsingClampToBorder = true;
      }
      //
      glTexParameteri(GL_TEXTURE_2D,
          DyGetParameterNameValue(parameter.mParameterOption),
          DyGetParameterValueValue(parameter.mParameterValue));
    }

    // If isThisAttachmentUsingClampToBorder is true, apply border color to texture.
    if (isThisAttachmentUsingClampToBorder == true)
    {
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, attachmentInstance.mBorderColor.Data());
    }

    // bind attachment type value to attachment texture.
    const auto typeValue = DyGetAttachmentTypeValue(attachmentCursor.mAttachmentType);
    glFramebufferTexture2D(GL_FRAMEBUFFER, typeValue, GL_TEXTURE_2D, attachmentInstance.mAttachmentId, 0);
    // Add value.
    attachmentTypeList.push_back(typeValue);
  }

  // Default depth g-buffer
  if (framebuffer.mIsUsingDefaultDepthBuffer == true)
  {
    glGenRenderbuffers(1, &framebuffer.mDefaultDepthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.mDefaultDepthBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebuffer.mFrameBufferSize.X, framebuffer.mFrameBufferSize.Y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer.mDefaultDepthBufferId);
  }

  // Let framebuffer know that attachmentBuffer's id will be drawn at framebuffer.
  glDrawBuffers(attachmentBufferCount, attachmentTypeList.data());
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return DY_SUCCESS;
}

EDySuccess MDyFramebuffer::RemoveFrameBuffer(_MIN_ const std::string& framebufferName)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_FAILURE;
}

PDyGlFrameBufferInformation* MDyFramebuffer::GetFrameBufferPointer(_MIN_ const std::string& framebufferName)
{
  auto framebufferIt = this->mValidFramebufferList.find(framebufferName);
  if (framebufferIt == this->mValidFramebufferList.end()) { return nullptr; }

  return framebufferIt->second.get();
}

PDyGlAttachmentInformation* MDyFramebuffer::GetAttachmentPointer(_MIN_ const std::string& attachmentName)
{
  auto attachmentIt = this->mValidAttachmentList.find(attachmentName);
  if (attachmentIt == this->mValidAttachmentList.end()) { return nullptr; }

  return attachmentIt->second.get();
}

PDyGlAttachmentInformation* MDyFramebuffer::GetAttachmentPointer(const std::string_view& attachmentName)
{
  auto attachmentIt = this->mValidAttachmentList.find(MSVSTR(attachmentName));
  if (attachmentIt == this->mValidAttachmentList.end()) { return nullptr; }

  return attachmentIt->second.get();;
}

} /// ::dy namespace