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
#include <Dy/Element/Actor.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include "Dy/Core/Resource/Resource/FDyMeshResource.h"
#include "Dy/Core/Resource/Resource/FDyFrameBufferResource.h"
#include <Dy/Component/CDyCamera.h>
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"
#include "Dy/Component/CDyModelAnimator.h"

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
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer((sUboCameraBlock)));
}

void FDyBasicRenderer::PreRender()
{
  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  if (MDY_CHECK_ISNULL(ptrCamera)) { return; }

  const auto& ptrValidCamera = *ptrCamera;
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  {
    const auto flag = uboManager.UpdateUboContainer(
        (sUboCameraBlock),
        offsetof(DDyUboCameraBlock, mViewMatrix),
        sizeof(DDyUboCameraBlock::mViewMatrix),
        &ptrValidCamera.GetViewMatrix()[0].X);
    MDY_ASSERT_MSG(flag == DY_SUCCESS, "");
  }
  {
    const auto flag = uboManager.UpdateUboContainer(
        (sUboCameraBlock),
        offsetof(DDyUboCameraBlock, mProjMatrix),
        sizeof(DDyUboCameraBlock::mProjMatrix),
        &ptrValidCamera.GetProjectionMatrix()[0].X);
    MDY_ASSERT_MSG(flag == DY_SUCCESS, "");
  }
}

void FDyBasicRenderer::RenderScreen(
    _MIN_ DDyModelHandler::DActorInfo& iRefRenderer,
    _MIN_ FDyMeshResource& iRefMesh, 
    _MIN_ FDyMaterialResource& iRefMaterial)
{ 
  if (this->IsReady() == false) { return; }

  // General deferred rendering
  auto& refActor = *iRefRenderer.mPtrModelRenderer->GetBindedActor();
  const auto& transform = refActor.GetTransform();
  const auto& refModelMatrix    = transform->GetTransform();
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }

  using EUniform = EDyUniformVariableType;
  shaderBinder->TryUpdateUniform<EUniform::Matrix4>("uModelMatrix", refModelMatrix);
  shaderBinder->TryUpdateUniform<EUniform::Float>("uBtDyActorId", TF32(refActor.GetId()));
  // If this model has animator, update uniform.
  if (iRefRenderer.mPtrModelAnimator != nullptr)
  {
    const auto& finalTransformList = iRefRenderer.mPtrModelAnimator->GetFinalTransformList();
    shaderBinder->TryUpdateUniform<EUniform::Matrix4Array>("mBoneTransform[0]", finalTransformList);
    shaderBinder->TryUpdateUniform<EUniform::Integer>("mNumBoneTransform", static_cast<TI32>(finalTransformList.size()));
  }

  shaderBinder->UseShader();
  shaderBinder->TryUpdateUniformList();
  iRefMaterial.TryUpdateTextureList();
  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. 
  // (not support instancing yet) TODO IMPLEMENT BATCHING SYSTEM.
  if (iRefMesh.IsEnabledIndices() == true)
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind, unset, deactivate settings for this submesh and material.
  // Unbind present submesh vertex array object.
  iRefMaterial.TryDetachTextureListFromShader();
  FDyGLWrapper::UnbindVertexArrayObject();
  shaderBinder->DisuseShader();
}

void FDyBasicRenderer::Clear()
{ 
  if (this->IsReady() == false) { return; }

  { MDY_GRAPHIC_SET_CRITICALSECITON();
    this->mBinderFrameBuffer->BindFrameBuffer();
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->mBinderFrameBuffer->UnbindFrameBuffer();
  }
}

bool FDyBasicRenderer::IsReady() const noexcept
{
  return this->mBinderFrameBuffer.IsResourceExist() == true;
}

EDySuccess FDyBasicRenderer::TryPushRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return DY_SUCCESS;
}

EDySuccess FDyBasicRenderer::TryPopRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->mBinderFrameBuffer->UnbindFrameBuffer();
  return DY_SUCCESS;
}

} /// ::dy namespace