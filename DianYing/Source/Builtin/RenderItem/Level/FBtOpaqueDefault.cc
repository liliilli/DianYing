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

#include <Dy/Builtin/RenderItem/Level/FBtOpaqueDefault.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Element/Actor.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Type/EDyDrawType.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Component/CDyModelAnimator.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>

namespace dy
{

void FBtRenderItemOpaqueDefault::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("aaa116de-6210-46fb-a551-a50554a08d95", true);
}

EDySuccess FBtRenderItemOpaqueDefault::OnPreRenderCheckCondition()
{
  // Whether camera is focused by main camera is true, by parent RenderPipeline
  // `FBtDefaultLevel`, we do not need to check more setting.
  return this->mBinderFrameBuffer.IsResourceExist() == true ? DY_SUCCESS : DY_FAILURE;
}

void FBtRenderItemOpaqueDefault::OnFailedCheckCondition()
{
  /* Do nothing */
}

void FBtRenderItemOpaqueDefault::OnSetupRenderingSetting()
{
  this->mBinderFrameBuffer->BindFrameBuffer();
  this->mBinderFrameBuffer->PushGlobalStates();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up UBO

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

void FBtRenderItemOpaqueDefault::OnRender()
{
  auto& drawList = MDyRendering::GetInstance().GetOpaqueMeshQueueList();
  for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : drawList)
  { // Render
    this->RenderObject(
      *iPtrModel,
      const_cast<FDyMeshResource&>(*iPtrValidMesh),
      const_cast<FDyMaterialResource&>(*iPtrValidMat)
    );
  }
  SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(drawList.size()));
}

void FBtRenderItemOpaqueDefault::RenderObject(
  DDyModelHandler::DActorInfo& iRefRenderer, 
  FDyMeshResource& iRefMesh,
  FDyMaterialResource& iRefMaterial)
{
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
    shaderBinder->TryUpdateUniform<EUniform::Matrix4Array>(
      "mBoneTransform[0]", 
      finalTransformList);
    shaderBinder->TryUpdateUniform<EUniform::Integer>(
      "mNumBoneTransform", 
      static_cast<TI32>(finalTransformList.size()));
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

void FBtRenderItemOpaqueDefault::OnReleaseRenderingSetting()
{
  this->mBinderFrameBuffer->PopGlobalStates();
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FBtRenderItemOpaqueDefault::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace
