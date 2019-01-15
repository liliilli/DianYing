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

void FDyBasicRenderer::PreRender()
{
  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  if (MDY_CHECK_ISNULL(ptrCamera)) { return; }

  const auto& ptrValidCamera = *ptrCamera;
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  {
    const auto flag = uboManager.UpdateUboContainer(
        MSVSTR(sUboCameraBlock),
        offsetof(DDyUboCameraBlock, mViewMatrix),
        sizeof(DDyUboCameraBlock::mViewMatrix),
        &ptrValidCamera.GetViewMatrix()[0].X);
    MDY_ASSERT(flag == DY_SUCCESS, "");
  }
  {
    const auto flag = uboManager.UpdateUboContainer(
        MSVSTR(sUboCameraBlock),
        offsetof(DDyUboCameraBlock, mProjMatrix),
        sizeof(DDyUboCameraBlock::mProjMatrix),
        &ptrValidCamera.GetProjectionMatrix()[0].X);
    MDY_ASSERT(flag == DY_SUCCESS, "");
  }

  glDisable(GL_CULL_FACE);

}

void FDyBasicRenderer::RenderScreen(
    _MIN_ CDyModelRenderer& iRefRenderer,
    _MIN_ FDyMeshResource& iRefMesh, 
    _MIN_ FDyMaterialResource& iRefMaterial)
{ 
  if (this->mBinderFrameBuffer.IsResourceExist() == false) { return; }
  this->mBinderFrameBuffer->BindFrameBuffer();
  glBlendFunci(1, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Get valid Main CDyCamera instance pointer address.
  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  if (MDY_CHECK_ISNULL(ptrCamera)) { return; }

  // General deferred rendering
  const auto& refModelMatrix = iRefRenderer.GetBindedActor()->GetTransform()->GetTransform();
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }

  shaderBinder->UseShader();
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Matrix4>("modelMatrix", refModelMatrix);
  shaderBinder.TryUpdateUniformList();
  iRefMaterial.TryUpdateTextureList();

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

  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. (not support instancing yet) TODO IMPLEMENT BATCHING SYSTEM.
  if (iRefMesh.IsEnabledIndices() == true)
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind present submesh vertex array object.
  FDyGLWrapper::UnbindVertexArrayObject();

  // Unbind, unset, deactivate settings for this submesh and material.
  iRefMaterial.TryDetachTextureListFromShader();
  shaderBinder->DisuseShader();

  this->mBinderFrameBuffer->UnbindFrameBuffer();
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