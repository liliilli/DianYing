#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
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
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include "Dy/Core/Resource/Resource/FDyMeshResource.h"
#include "Dy/Core/Resource/Resource/FDyTextureResource.h"
#include "Dy/Core/Resource/Resource/FDyFrameBufferResource.h"
#include <Dy/Component/CDyCamera.h>
#include "Dy/Management/Helper/SDyProfilingHelper.h"
#include "Dy/Core/Resource/Resource/FDyModelResource.h"
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"

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
}

void FDyBasicRenderer::RenderScreen(_MIN_ const std::vector<NotNull<CDyModelRenderer*>>& rendererList)
{ // Validation test
  if (this->mBinderFrameBuffer.IsResourceExist() == false) { return; }
  glBindFramebuffer(GL_FRAMEBUFFER, this->mBinderFrameBuffer->GetFrameBufferId());

  auto& worldManager     = MDyWorld::GetInstance();
  auto& uboManager       = MDyUniformBufferObject::GetInstance();
  const auto cameraCount = worldManager.GetFocusedCameraCount();

  // Request draw calls (without SW occlusion culling)
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
    const auto viewportRect = validCameraRawPtr.GetPixelizedViewportRectangle();
    glViewport(viewportRect[0], viewportRect[1], viewportRect[2], viewportRect[3]);

    for (auto& drawInstance : rendererList)
    { // General deferred rendering
      auto& refRenderer    = *drawInstance;
      const auto& worldPos = refRenderer.GetBindedActor()->GetTransform()->GetFinalWorldPosition();
      // Check frustum.
      if (validCameraRawPtr.CheckIsPointInFrustum(worldPos) == false) { continue; }

      const auto& refModelMatrix  = refRenderer.GetBindedActor()->GetTransform()->GetTransform();
      this->pRenderScreen(refRenderer, refModelMatrix, validCameraRawPtr);
      SDyProfilingHelper::AddScreenRenderedActorCount(1);
    }
  }

  // Return to default frame buffer.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyBasicRenderer::pRenderScreen(
    _MIN_ const CDyModelRenderer& renderer, 
    _MIN_ const DDyMatrix4x4& iModelMatrix, 
    _MIN_ const CDyCamera& validCamera) noexcept
{
  // Validation test
  const auto* ptrModelBinder      = renderer.GetModelResourceBinder();
  if (MDY_CHECK_ISNULL(ptrModelBinder)) { return; }

  const auto& meshList            = ptrModelBinder->Get()->GetMeshResourceList();
  const auto  opSubmeshListCount  = renderer.GetModelSubmeshCount();
  if (opSubmeshListCount.has_value() == false) { return; }

  for (TI32 i = 0; i < opSubmeshListCount.value(); ++i)
  {
    auto& material = const_cast<std::decay_t<decltype(renderer.GetMaterialResourcePtr(i))>&>(renderer.GetMaterialResourcePtr(i));
    auto& shaderBinder = material.GetShaderResourceBinder();
    if (shaderBinder.IsResourceExist() == false) { continue; }

    shaderBinder->UseShader();
    shaderBinder.TryUpdateUniform<EDyUniformVariableType::Matrix4>("modelMatrix", iModelMatrix);
    shaderBinder.TryUpdateUniformList();
    material.TryUpdateTextureList();

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

    const auto& ptrMesh = *meshList[i];
    FDyGLWrapper::BindVertexArrayObject(ptrMesh->GetVertexArrayId());

    // Call function call drawing array or element. (not support instancing yet) TODO IMPLEMENT BATCHING SYSTEM.
    if (ptrMesh->IsEnabledIndices() == true)
    { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, ptrMesh->GetIndicesCounts()); }
    else
    { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, ptrMesh->GetVertexCounts()); }

    // Unbind present submesh vertex array object.
    FDyGLWrapper::UnbindVertexArrayObject();

    // Unbind, unset, deactivate settings for this submesh and material.
    material.TryDetachTextureListFromShader();
    shaderBinder->DisuseShader();
  }
}

void FDyBasicRenderer::Clear()
{ // Integrity test
  if (this->mBinderFrameBuffer.IsResourceExist() == false) { return; }
  glBindFramebuffer(GL_FRAMEBUFFER, this->mBinderFrameBuffer->GetFrameBufferId());

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


} /// ::dy namespace