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
#include <Dy/Management/SceneManager.h>
#include <Dy/Management/SettingManager.h>

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/Editor/GuiSetting.h>

#include <Dy/Core/Component/Resource/MaterialResource.h>
#include <Dy/Core/Component/Resource/ModelResource.h>
#include <Dy/Core/Component/Resource/SubmeshResource.h>
#include <Dy/Core/Component/Resource/TextureResource.h>
#include <Dy/Core/Component/Object/Camera.h>

namespace dy
{

EDySuccess MDyRendering::pfInitialize()
{
  pCreateDeferredGeometryBuffers();
  mFinalRenderingMesh = std::make_unique<decltype(mFinalRenderingMesh)::element_type>();

  if (this->mTempIsEnabledSsao)
  {
    mTempSsaoObject = std::make_unique<decltype(mTempSsaoObject)::element_type>();
  }

  if (this->mTempIsEnabledShadow)
  {
    this->mTempShadowObject = std::make_unique<decltype(this->mTempShadowObject)::element_type>();
  }

#if defined(MDY_FLAG_IN_EDITOR)
  //! Grid rendering setting.
  this->mGridEffect = std::make_unique<decltype(this->mGridEffect)::element_type>();
#endif /// MDY_FLAG_IN_EDITOR

  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfRelease()
{
  mFinalRenderingMesh = nullptr;
  pReleaseGeometryBuffers();

  return DY_SUCCESS;
}

void MDyRendering::PushDrawCallTask(CDyMeshRenderer& rendererInstance)
{
  this->mDrawCallQueue.push(&rendererInstance);
}

void MDyRendering::RenderDrawCallQueue()
{
  const auto& setting = MDySetting::GetInstance();

  // Reset previous frame results of each framebuffers.
  this->pResetRenderingFramebufferInstances();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Draw
  while (!this->mDrawCallQueue.empty())
  {
    CDyMeshRenderer& drawInstance = *this->mDrawCallQueue.front();

    { // General deferred rendering
      glViewport(0, 0, setting.GetWindowSizeWidth(), setting.GetWindowSizeHeight());
      glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);
      this->pRenderDeferredFrameBufferWith(drawInstance);
    }

#ifdef false
    if (this->mTempIsEnabledShadow)
    { // Basic shadow (directional light etc)
      glViewport(0, 0, 512, 512);
      this->pRenderShadowFrameBufferWith(drawInstance);
    }
#endif

    this->mDrawCallQueue.pop();
  }

  glViewport(0, 0, setting.GetWindowSizeWidth(), setting.GetWindowSizeHeight());
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);

  // Only in editor effects
#if defined(MDY_FLAG_IN_EDITOR)
  if (editor::MDyEditorSetting::GetInstance().GetmIsEnabledViewportRenderGrid() && this->mGridEffect)
  {
    this->mGridEffect->RenderGrid();
  }
#endif /// MDY_FLAG_IN_EDITOR

  // Post processing effects
  if (this->mTempIsEnabledSsao)
  {
    this->mTempSsaoObject->RenderScreen();
  }

#if !defined(MDY_FLAG_IN_EDITOR)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  this->mFinalRenderingMesh->RenderScreen();
#endif /// !MDY_FLAG_IN_EDITOR
}

void MDyRendering::pCreateDeferredGeometryBuffers() noexcept
{
  auto& settingManager = MDySetting::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();

  glGenFramebuffers(1, &this->mDeferredFrameBufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);
  glGenTextures(this->mAttachmentBuffersCount, &this->mAttachmentBuffers[0]);

  // Unlit g-buffer
  glBindTexture(GL_TEXTURE_2D, this->mAttachmentBuffers[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, overallScreenWidth, overallScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mAttachmentBuffers[0], 0);

  // Normal g-buffer
  glBindTexture(GL_TEXTURE_2D, this->mAttachmentBuffers[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, overallScreenWidth, overallScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->mAttachmentBuffers[1], 0);

  // Specular g-buffer
  glBindTexture(GL_TEXTURE_2D, this->mAttachmentBuffers[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, overallScreenWidth, overallScreenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->mAttachmentBuffers[2], 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    PHITOS_UNEXPECTED_BRANCH();
  }

  // View position g-buffer
  glBindTexture(GL_TEXTURE_2D, this->mAttachmentBuffers[3]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, overallScreenWidth, overallScreenHeight, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, this->mAttachmentBuffers[3], 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    PHITOS_UNEXPECTED_BRANCH();
  }

  // Depth g-buffer
  TU32 depthBuffer = MDY_NOT_INITIALIZED_0;
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, overallScreenWidth, overallScreenHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

  // Let framebuffer know that attachmentBuffer's id will be drawn at framebuffer.
  std::array<GLenum, 4> attachmentEnumList = {
      GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
  };
  glDrawBuffers(static_cast<TI32>(attachmentEnumList.size()), &attachmentEnumList[0]);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  MDY_LOG_INFO("{}::{} | Geometry buffer created.", "MDyRendering", "pCreateDeferredGeometryBuffers");
}

void MDyRendering::pReleaseGeometryBuffers() noexcept
{
  glDeleteTextures(this->mAttachmentBuffersCount, &this->mAttachmentBuffers[0]);
  if (this->mDeferredFrameBufferId) { glDeleteFramebuffers(1, &this->mDeferredFrameBufferId); }

  MDY_LOG_INFO("{}::{} | Geometry buffer released.", "MDyRendering", "pCreateDeferredGeometryBuffers");
}

void MDyRendering::pResetRenderingFramebufferInstances() noexcept
{
  // Reset overall deferred framebuffer setting
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset all shadow framebuffer setting
  if (this->mTempIsEnabledShadow)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, this->mTempShadowObject->GetShadowFrameBufferId());
    glClear(GL_DEPTH_BUFFER_BIT);
  }
}

void MDyRendering::pRenderDeferredFrameBufferWith(const CDyMeshRenderer& renderer) noexcept
{
  for (const auto& bindedMeshMatInfo : renderer.mMeshMaterialPtrBindingList)
  {
    // Integrity test.
    const auto shaderResource = bindedMeshMatInfo.mMaterialResource->GetShaderResource();
    if (!shaderResource)
    {
      MDY_LOG_CRITICAL("{} | Shader resource of {} is not binded, Can not render mesh.", "CDyMeshRenderer::Render", bindedMeshMatInfo.mMaterialResource->GetMaterialName());
      continue;
    }
    // Activate shader of one material and bind submesh VAO id.
    shaderResource->UseShader();
    glBindVertexArray(bindedMeshMatInfo.mSubmeshResource->GetVertexArrayId());

    // @todo temporal Bind camera matrix.
    if (auto* camera = MDyScene::GetInstance().GetMainCameraPtr(); camera)
    {
      const auto viewMatrix = glGetUniformLocation(shaderResource->GetShaderProgramId(), "viewMatrix");
      const auto projMatirx = glGetUniformLocation(shaderResource->GetShaderProgramId(), "projectionMatrix");

      glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &camera->GetViewMatrix()[0].X);
      glUniformMatrix4fv(projMatirx, 1, GL_FALSE, &camera->GetProjectionMatrix()[0].X);
    }

    // If skeleton animation is enabled, get bone transform and bind to shader.
    const auto boneTransform = glGetUniformLocation(shaderResource->GetShaderProgramId(), "boneTransform");
    if (renderer.mModelReferencePtr && renderer.mModelReferencePtr->IsEnabledModelAnimated())
    {
      const auto& matrixList = renderer.mModelReferencePtr->GetModelAnimationTransformMatrixList();
      const auto  matrixSize = static_cast<int32_t>(matrixList.size());
      for (int32_t i = 0; i < matrixSize; ++i)
      {
        glUniformMatrix4fv(boneTransform + i, 1, GL_FALSE, &matrixList[i].mFinalTransformation[0].X);
      }
    }

    // Bind textures of one material.
    if (bindedMeshMatInfo.mMaterialResource)
    {
      const auto& textureResources        = bindedMeshMatInfo.mMaterialResource->GetBindedTextureResources();
      const auto  textureResourceListSize = static_cast<int32_t>(textureResources.size());
      for (int32_t i = 0; i < textureResourceListSize; ++i)
      {
        glUniform1i(glGetUniformLocation(shaderResource->GetShaderProgramId(), (std::string("uTexture") + std::to_string(i)).c_str()), i);

        const auto texturePointer = textureResources[i].mValidTexturePointer;
        glActiveTexture(GL_TEXTURE0 + i);
        switch (texturePointer->GetTextureType())
        {
        case EDyTextureStyleType::D1: glBindTexture(GL_TEXTURE_1D, texturePointer->GetTextureId()); break;
        case EDyTextureStyleType::D2: glBindTexture(GL_TEXTURE_2D, texturePointer->GetTextureId()); break;
        default: PHITOS_UNEXPECTED_BRANCH(); break;
        }
      }
    }

    // Call function call drawing array or element. (not support instancing yet)
    if (bindedMeshMatInfo.mSubmeshResource->IsEnabledIndices())
    {
      glDrawElements(GL_TRIANGLES, bindedMeshMatInfo.mSubmeshResource->GetIndicesCounts(), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
      glDrawArrays(GL_TRIANGLES, 0, bindedMeshMatInfo.mSubmeshResource->GetVertexCounts());
    }

    // Unbind, unset, deactivate settings for this submesh and material.
    if (bindedMeshMatInfo.mMaterialResource)
    {
      const auto& textureResources        = bindedMeshMatInfo.mMaterialResource->GetBindedTextureResources();
      const auto  textureResourceListSize = static_cast<int32_t>(textureResources.size());
      for (int32_t i = 0; i < textureResourceListSize; ++i)
      {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
    }

    glBindVertexArray(0);
    shaderResource->UnuseShader();
  }
}

void MDyRendering::pRenderShadowFrameBufferWith(const CDyMeshRenderer& renderer) noexcept
{
  this->mTempShadowObject->RenderScreen(renderer);
}

} /// ::dy namespace