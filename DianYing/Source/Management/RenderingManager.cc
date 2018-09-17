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
#include <Dy/Management/RenderingManager.h>

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/SettingManager.h>

#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Management/DataInformationManager.h>

namespace dy
{

EDySuccess MDyRendering::pfInitialize()
{
  pCreateGeometryBuffers();
  mFinalRenderingMesh = std::make_unique<decltype(mFinalRenderingMesh)::element_type>();

  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfRelease()
{
  mFinalRenderingMesh = nullptr;
  pReleaseGeometryBuffers();

  return DY_SUCCESS;
}

void MDyRendering::pCreateGeometryBuffers() noexcept
{
  auto& settingManager = MDySetting::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();

  glGenFramebuffers(1, &this->mDeferredFrameBufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);
  glGenTextures(this->mAttachmentBuffersCount, &this->mAttachmentBuffers[0]);

  // Unlit g-buffer
  glBindTexture(GL_TEXTURE_2D, this->mAttachmentBuffers[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, overallScreenWidth, overallScreenHeight, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mAttachmentBuffers[0], 0);

  // Normal g-buffer
  glBindTexture(GL_TEXTURE_2D, this->mAttachmentBuffers[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, overallScreenWidth, overallScreenHeight, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->mAttachmentBuffers[1], 0);

  // Specular g-buffer
  glBindTexture(GL_TEXTURE_2D, this->mAttachmentBuffers[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, overallScreenWidth, overallScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->mAttachmentBuffers[2], 0);

  // Depth g-buffer
  TU32 depthBuffer = MDY_NOT_INITIALIZED_0;
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, overallScreenWidth, overallScreenHeight);
  glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

  // Let framebuffer know that attachmentBuffer's id will be drawn at framebuffer.
  std::array<GLenum, 3> attachmentEnumList = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
  glDrawBuffers(static_cast<TI32>(attachmentEnumList.size()), &attachmentEnumList[0]);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  MDY_LOG_INFO("{}::{} | Geometry buffer created.", "MDyRendering", "pCreateGeometryBuffers");
}

void MDyRendering::pReleaseGeometryBuffers() noexcept
{
  glDeleteTextures(this->mAttachmentBuffersCount, &this->mAttachmentBuffers[0]);
  if (this->mDeferredFrameBufferId) { glDeleteFramebuffers(1, &this->mDeferredFrameBufferId); }

  MDY_LOG_INFO("{}::{} | Geometry buffer released.", "MDyRendering", "pCreateGeometryBuffers");
}

} /// ::dy namespace