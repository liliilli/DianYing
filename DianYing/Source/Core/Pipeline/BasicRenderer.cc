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
#include <Dy/Core/Rendering/Pipeline/BasicRenderer.h>

#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Core/Resource/Internal/TextureEnums.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Management/Type/FramebufferInformation.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include "Dy/Core/Resource/Resource/FDyMeshResource.h"
#include "Dy/Core/Resource/Resource/FDyTextureResource.h"

//!
//! Temporary
//!

namespace dy
{

}

//!
//! Implementation
//!

namespace dy
{

FDyBasicRenderer::FDyBasicRenderer()
{
  auto& settingManager      = MDySetting::GetInstance();
  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();
  const auto overallSize = DDyVectorInt2{overallScreenWidth, overallScreenHeight};

  PDyGlFrameBufferInformation       framebufferInfo = {};
  PDyGlAttachmentInformation        attachmentInfo  = {};
  PDyGlAttachmentBinderInformation  binderInfo      = {};

  framebufferInfo.mFrameBufferName = sFrameBuffer_Deferred;
  framebufferInfo.mFrameBufferSize = overallSize;
  framebufferInfo.mIsUsingDefaultDepthBuffer = true;

  // Unlit texture buffer
  attachmentInfo.mAttachmentName = sAttachment_Unlit;
  attachmentInfo.mAttachmentSize = overallSize;
  attachmentInfo.mParameterList  = {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::Repeat},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::Repeat},
  };
  binderInfo.mAttachmentName = attachmentInfo.mAttachmentName;
  binderInfo.mAttachmentType = EDyGlAttachmentType::Color0;
  framebufferInfo.mAttachmentList.push_back(binderInfo);
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.SetAttachmentInformation(attachmentInfo));

  // Normal texture buffer
  attachmentInfo.mAttachmentName = sAttachment_Normal;
  attachmentInfo.mParameterList  = {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToEdge},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToEdge},
  };
  binderInfo.mAttachmentName = attachmentInfo.mAttachmentName;
  binderInfo.mAttachmentType = EDyGlAttachmentType::Color1;
  framebufferInfo.mAttachmentList.push_back(binderInfo);
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.SetAttachmentInformation(attachmentInfo));

  // Specular texture buffer
  attachmentInfo.mAttachmentName = sAttachment_Specular;
  attachmentInfo.mParameterList  = {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::Repeat},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::Repeat},
  };
  binderInfo.mAttachmentName = attachmentInfo.mAttachmentName;
  binderInfo.mAttachmentType = EDyGlAttachmentType::Color2;
  framebufferInfo.mAttachmentList.push_back(binderInfo);
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.SetAttachmentInformation(attachmentInfo));

  // Model position texture buffer
  attachmentInfo.mAttachmentName = sAttachment_ModelPosition;
  attachmentInfo.mParameterList  = {
    PDyGlTexParameterInformation\
    {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
    {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToEdge},
    {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToEdge},
  };
  binderInfo.mAttachmentName = attachmentInfo.mAttachmentName;
  binderInfo.mAttachmentType = EDyGlAttachmentType::Color3;
  framebufferInfo.mAttachmentList.push_back(binderInfo);
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.SetAttachmentInformation(attachmentInfo));

  // Create framebuffer.
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.InitializeNewFrameBuffer(framebufferInfo));
  MDY_LOG_INFO("{}::{} | Geometry buffer created.", "MDyRendering", "pCreateDeferredGeometryBuffers");

  this->mGivenFrameBufferPointer = framebufferManager.GetFrameBufferPointer(MSVSTR(sFrameBuffer_Deferred));
  MDY_ASSERT(this->mGivenFrameBufferPointer != nullptr, "Unexpected error.");

  // Create ubo information for "CameraBlock"
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  PDyUboConstructionDescriptor desc = {};
  desc.mBindingIndex      = 0;
  desc.mUboSpecifierName  = sUboCameraBlock;
  desc.mBufferDrawType    = EDyBufferDrawType::DynamicDraw;
  desc.mUboElementSize    = sizeof(DDyUboCameraBlock);
  desc.mUboArraySize      = 1;
  MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc));
}

FDyBasicRenderer::~FDyBasicRenderer()
{
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer(MSVSTR(sUboCameraBlock)));

  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.RemoveFrameBuffer(MSVSTR(sFrameBuffer_Deferred)));
  this->mGivenFrameBufferPointer = nullptr;

  MDY_LOG_INFO("{}::{} | Geometry buffer released.", "MDyRendering", "pCreateDeferredGeometryBuffers");
}

void FDyBasicRenderer::RenderScreen(_MIN_ const std::vector<NotNull<CDyModelRenderer*>>& rendererList)
{ // Integrity test
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mGivenFrameBufferPointer), "Unexpected error.");

  // Deferred rendering for opaque objects
  glBindFramebuffer(GL_FRAMEBUFFER, this->mGivenFrameBufferPointer->GetFramebufferId());

  auto& worldManager     = MDyWorld::GetInstance();
  auto& uboManager       = MDyUniformBufferObject::GetInstance();
  const auto cameraCount = worldManager.GetFocusedCameraCount();
  for (TI32 cameraId = 0; cameraId < cameraCount; ++cameraId)
  { // Get valid CDyCamera instance pointer address.
    const auto opCamera = worldManager.GetFocusedCameraValidReference(cameraId);
    if (opCamera.has_value() == false) { continue; }

    const auto& validCameraRawPtr = *opCamera.value();
    {
      const auto flag = uboManager.UpdateUboContainer(
          MSVSTR(sUboCameraBlock),
          offsetof(DDyUboCameraBlock, mViewMatrix),
          sizeof(DDyUboCameraBlock::mViewMatrix),
          &validCameraRawPtr.GetViewMatrix()[0].X);
      MDY_ASSERT(flag == DY_SUCCESS, "");
    }
    {
      const auto flag = uboManager.UpdateUboContainer(
          MSVSTR(sUboCameraBlock),
          offsetof(DDyUboCameraBlock, mProjMatrix),
          sizeof(DDyUboCameraBlock::mProjMatrix),
          &validCameraRawPtr.GetProjectionMatrix()[0].X);
      MDY_ASSERT(flag == DY_SUCCESS, "");
    }

    // Set viewport values to camera's properties.
    const auto viewportRect       = validCameraRawPtr.GetPixelizedViewportRectangle();
    glViewport(viewportRect[0], viewportRect[1], viewportRect[2], viewportRect[3]);

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
    const auto& material = renderer.GetMaterialResourcePtr(i);
    auto* ptrShader      = material.GetShaderResourcePtr();
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptrShader), "Shader resource is not binded yet.");

    ptrShader->UseShader();

    const auto& submesh = renderer.GetSubmeshResourcePtr(i);
    glBindVertexArray(submesh.GetVertexArrayId());

    const auto modelMatrix = glGetUniformLocation(ptrShader->GetShaderProgramId(), "modelMatrix");
    const auto& model = const_cast<FDyActor*>(renderer.GetBindedActor())->GetTransform()->GetTransform();
    glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &model[0].X);

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
    const auto& textureResources = material.GetBindedTextureResourcePtrList();
    const auto  textureResourceListSize = static_cast<int32_t>(textureResources.size());
    for (int32_t j = 0; j < textureResourceListSize; ++j)
    {
      glUniform1i(glGetUniformLocation(ptrShader->GetShaderProgramId(), (std::string("uTexture") + std::to_string(j)).c_str()), j);

      const auto* texturePointer = textureResources[j];
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
    ptrShader->DisuseShader();
  }
}

void FDyBasicRenderer::Clear()
{ // Integrity test
  if (MDY_CHECK_ISNULL(this->mGivenFrameBufferPointer)) { return; }

  // Reset overall deferred framebuffer setting
  glBindFramebuffer(GL_FRAMEBUFFER, this->mGivenFrameBufferPointer->GetFramebufferId());
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


} /// ::dy namespace