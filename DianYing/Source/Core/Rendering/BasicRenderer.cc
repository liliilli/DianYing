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
#include <Dy/Core/Rendering/BasicRenderer.h>

#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Core/Component/Resource/ShaderResource.h>
#include <Dy/Core/Component/Resource/SubmeshResource.h>
#include <Dy/Core/Component/Resource/MaterialResource.h>
#include <Dy/Core/Component/Resource/TextureResource.h>
#include <Dy/Core/Component/Internal/TextureEnums.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>

//!
//! Implementation
//!

namespace dy
{

FDyBasicRenderer::FDyBasicRenderer()
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
    MDY_UNEXPECTED_BRANCH();
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
    MDY_UNEXPECTED_BRANCH();
  }

  // Depth g-buffer
  TU32 depthBuffer = MDY_INITIALIZE_DEFUINT;
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, overallScreenWidth, overallScreenHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

  // Let framebuffer know that attachmentBuffer's id will be drawn at framebuffer.
  std::array<GLenum, 4> attachmentEnumList = {
      GL_COLOR_ATTACHMENT0, // gUnlit
      GL_COLOR_ATTACHMENT1, // gNormal
      GL_COLOR_ATTACHMENT2, // gSpecular
      GL_COLOR_ATTACHMENT3  // gPosition
  };
  glDrawBuffers(static_cast<TI32>(attachmentEnumList.size()), &attachmentEnumList[0]);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  MDY_LOG_INFO("{}::{} | Geometry buffer created.", "MDyRendering", "pCreateDeferredGeometryBuffers");
}

FDyBasicRenderer::~FDyBasicRenderer()
{
  glDeleteTextures(this->mAttachmentBuffersCount, &this->mAttachmentBuffers[0]);
  if (this->mDeferredFrameBufferId) { glDeleteFramebuffers(1, &this->mDeferredFrameBufferId); }

  MDY_LOG_INFO("{}::{} | Geometry buffer released.", "MDyRendering", "pCreateDeferredGeometryBuffers");
}

void FDyBasicRenderer::RenderScreen(_MIN_ const std::vector<NotNull<CDyModelRenderer*>>& rendererList)
{
  auto& worldManager      = MDyWorld::GetInstance();
  const auto cameraCount  = worldManager.GetFocusedCameraCount();

  // (1) Deferred rendering for opaque objects + shadowing
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);

  for (TI32 cameraId = 0; cameraId < cameraCount; ++cameraId)
  { // Get valid CDyCamera instance pointer address.
    const auto opCamera = worldManager.GetFocusedCameraValidReference(cameraId);
    if (opCamera.has_value() == false) { continue; }

    // Set viewport values to camera's properties.
    const auto& validCameraRawPtr     = *opCamera.value();
    const auto pixelizedViewportRect  = validCameraRawPtr.GetPixelizedViewportRectangle();
    glViewport(pixelizedViewportRect[0], pixelizedViewportRect[1],
               pixelizedViewportRect[2], pixelizedViewportRect[3]);

    for (const auto& drawInstance : rendererList)
    { // General deferred rendering
      this->pRenderScreen(*drawInstance, validCameraRawPtr);
    }
  }

  // Return to default frame buffer.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyBasicRenderer::pRenderScreen(const CDyModelRenderer& renderer, const CDyCamera& validCamera) noexcept
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
      default: MDY_UNEXPECTED_BRANCH(); break;
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

} /// ::dy namespace