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
#include <Dy/Management/WorldManager.h>
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

void MDyRendering::PushDrawCallTask(_MIN_ CDyModelRenderer& rendererInstance)
{
  this->mDrawCallList.emplace_back(DyMakeNotNull(&rendererInstance));
}

void MDyRendering::RenderDrawCallQueue()
{
  const auto& setting = MDySetting::GetInstance();

  // Reset previous frame results of each framebuffers.
  this->pResetRenderingFramebufferInstances();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Draw

  const auto cameraCount = MDyWorld::GetInstance().GetFocusedCameraCount();
  for (TI32 i = 0; i < cameraCount; ++i)
  {
    const auto opCamera = MDyWorld::GetInstance().GetFocusedCameraValidReference(i);
    if (opCamera.has_value() == false) { continue; }
    const auto& validCameraRawPtr = *opCamera.value();
    const auto pixelizedViewportRect = validCameraRawPtr.GetPixelizedViewportRectangle();
    glViewport(pixelizedViewportRect[0], pixelizedViewportRect[1],
               pixelizedViewportRect[2], pixelizedViewportRect[3]
    );
    glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);

    for (const auto& drawInstance : this->mDrawCallList)
    {
      // General deferred rendering
      this->pRenderDeferredFrameBufferWith(*drawInstance, validCameraRawPtr);

#ifdef false
      if (this->mTempIsEnabledShadow)
      { // Basic shadow (directional light etc)
        glViewport(0, 0, 512, 512);
        this->pRenderShadowFrameBufferWith(drawInstance);
      }
#endif
    }
  }

  // Clear draw call list
  this->mDrawCallList.clear();

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
  TU32 depthBuffer = MDY_INITIALIZE_DEFUINT;
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

void MDyRendering::pRenderDeferredFrameBufferWith(_MIN_ const CDyModelRenderer& renderer, _MIN_ const CDyCamera& validCamera) noexcept
{
  const auto materialListCount  = renderer.GetMaterialListCount();
  const auto opSubmeshListCount = renderer.GetModelSubmeshCount();

  // Integrity test
  if (opSubmeshListCount.has_value() == false) { return; }

  TI32 iterationCount = MDY_INITIALIZE_DEFINT;
  if (materialListCount < opSubmeshListCount.value()) { iterationCount = materialListCount; }
  else                                                { iterationCount = opSubmeshListCount.value(); }

  for (TI32 i = 0; i < iterationCount; ++i)
  {
    auto& material      = const_cast<CDyMaterialResource&>(renderer.GetMaterialResourcePtr(i));
    auto shaderResource = material.GetShaderResource();
    if (shaderResource == nullptr)
    {
      MDY_LOG_CRITICAL("{} | Shader resource of {} is not binded, Can not render mesh.", "CDyMeshRenderer::Render", material.GetMaterialName());
      continue;
    }

    shaderResource->UseShader();

    const CDySubmeshResource& submesh = renderer.GetSubmeshResourcePtr(i);
    glBindVertexArray(submesh.GetVertexArrayId());

    const auto modelMatrix = glGetUniformLocation(shaderResource->GetShaderProgramId(), "modelMatrix");
    const auto viewMatrix = glGetUniformLocation(shaderResource->GetShaderProgramId(), "viewMatrix");
    const auto projMatirx = glGetUniformLocation(shaderResource->GetShaderProgramId(), "projectionMatrix");

    const auto& model = const_cast<FDyActor*>(renderer.GetBindedActor())->GetTransform()->GetTransform();

    glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &model[0].X);
    glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &validCamera.GetViewMatrix()[0].X);
    glUniformMatrix4fv(projMatirx, 1, GL_FALSE, &validCamera.GetProjectionMatrix()[0].X);

    // If skeleton animation is enabled, get bone transform and bind to shader.
#ifdef false
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
#endif

    // Bind textures of one material.
    const auto& textureResources        = material.GetBindedTextureResources();
    const auto  textureResourceListSize = static_cast<int32_t>(textureResources.size());
    for (int32_t j = 0; j < textureResourceListSize; ++j)
    {
      glUniform1i(glGetUniformLocation(shaderResource->GetShaderProgramId(), (std::string("uTexture") + std::to_string(j)).c_str()), j);

      const auto texturePointer = textureResources[j].mValidTexturePointer;
      glActiveTexture(GL_TEXTURE0 + j);
      switch (texturePointer->GetTextureType())
      {
      case EDyTextureStyleType::D1: glBindTexture(GL_TEXTURE_1D, texturePointer->GetTextureId()); break;
      case EDyTextureStyleType::D2: glBindTexture(GL_TEXTURE_2D, texturePointer->GetTextureId()); break;
      default: PHITOS_UNEXPECTED_BRANCH(); break;
      }
    }

    // Call function call drawing array or element. (not support instancing yet)
    if (submesh.IsEnabledIndices()) { glDrawElements(GL_TRIANGLES, submesh.GetIndicesCounts(), GL_UNSIGNED_INT, nullptr); }
    else                            { glDrawArrays(GL_TRIANGLES, 0, submesh.GetVertexCounts()); }

    // Unbind, unset, deactivate settings for this submesh and material.
    for (TI32 j = 0; j < textureResourceListSize; ++j)
    {
      glActiveTexture(GL_TEXTURE0 + j);
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Unbind present submesh vertex array object.
    glBindVertexArray(0);
    shaderResource->UnuseShader();
  }
}

void MDyRendering::pRenderShadowFrameBufferWith(_MIN_ const CDyModelRenderer& renderer) noexcept
{
#ifdef false
  this->mTempShadowObject->RenderScreen(renderer);
#endif
}

} /// ::dy namespace