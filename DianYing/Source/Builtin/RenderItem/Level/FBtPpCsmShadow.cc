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

#include <Dy/Builtin/RenderItem/Level/FBtPpCsmShadow.h>
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
#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Management/SettingManager.h>

namespace dy
{

void FBtRenderItemCsmShadow::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("8dfca835-dee4-4c4a-bfd3-f3c23154116a", true);
}

FBtRenderItemCsmShadow::FBtRenderItemCsmShadow()
{
  FDyGLWrapper::QueryFloatVector(GL_MAX_VIEWPORT_DIMS, sViewportDims.data());
}

EDySuccess FBtRenderItemCsmShadow::OnPreRenderCheckCondition()
{
  // Check CSM Shadow feature is enabled.
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultShadow == false)
  {
    return DY_FAILURE;
  }

  // Whether camera is focused by main camera is true, by parent RenderPipeline
  // `FBtDefaultLevel`, we do not need to check more setting.
  auto* ptrLight = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (ptrLight == nullptr) 
  { 
    return DY_FAILURE; 
  }

  return this->mBinderFrameBuffer.IsResourceExist() == true
      && this->mDirLightShaderResource.IsResourceExist() == true ? DY_SUCCESS : DY_FAILURE;
}

void FBtRenderItemCsmShadow::OnFailedCheckCondition()
{
  if (this->mDirLightShaderResource.IsResourceExist() == false
  ||  this->mBinderFrameBuffer.IsResourceExist() == false) 
  { 
    return; 
  }

  this->mBinderFrameBuffer->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &one);
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FBtRenderItemCsmShadow::OnSetupRenderingSetting()
{
  // Update view frustum for shadow mapping.
  // Do not move the order of `PreRender` and checking assert statement.
  auto* ptrLight = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (this->mAddrMainDirectionalShadow != reinterpret_cast<ptrdiff_t>(ptrLight))
  {
    this->mAddrMainDirectionalShadow = reinterpret_cast<ptrdiff_t>(ptrLight);
    if (this->mAddrMainDirectionalShadow == 0) 
    {
      this->mViewMatrix = DDyMatrix4x4{};
    }
    else
    { 
      ptrLight->UpdateLightViewMatrix(); 
      this->mViewMatrix = ptrLight->GetLightViewMatrix(); 
    }
  }

  if (const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
      ptrLight != nullptr)
  {
    //
    ptrLight->UpdateSegmentFarPlanes(*ptrCamera);
    ptrLight->UpdateCSMFrustum(*ptrCamera);
    ptrLight->UpdateProjectionMatrix();
    ptrLight->UpdateLightProjectionAndViewports(
      *ptrCamera, 
      ptrLight->GetCSMFarPlanes(), 
      ptrLight->GetCSMNormalizedFarPlanes()
    );
    //
    this->mProjMatrix = ptrLight->GetProjectionMatrix();
  }

  MDY_ASSERT_MSG(
    this->mAddrMainDirectionalShadow == reinterpret_cast<ptrdiff_t>(ptrLight), 
    "CSM Renderer light handle is not matched to given light.");

  // Try update uniform value.
  using EUniform = EDyUniformVariableType;
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Integer>("uFrustumSegmentCount", static_cast<TI32>(kCSMSegment));
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Matrix4>("mProjMatrix", this->mProjMatrix);
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Matrix4>("mViewMatrix", this->mViewMatrix);

  // Set and push global internal setting.
  DDyGlGlobalStates status;
  using EValue = DDyGlGlobalStates::DCullfaceMode::EValue;
  status.mIsEnableDepthTest = true;
  status.mIsEnableCullface  = true;
  status.mCullfaceMode = EValue::Back;

  using DViewport = DDyGlGlobalStates::DViewport;
  DViewport viewport;

  /// @brief Setup indexed viewports of light shadow map segments for writing.
  const auto& lightViewports = ptrLight->GetCSMIndexedViewports();
  for (TU32 i = 0; i < kCSMSegment; ++i)
  {
    viewport.mViewportSettingList.emplace_back(i, lightViewports[i]);
  }
  status.mViewportSettingList = viewport;

  FDyGLWrapper::PushInternalGlobalState(status);

  this->mBinderFrameBuffer->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &one);
}

void FBtRenderItemCsmShadow::OnRender()
{
  // Cascade shadow mapping use different and mutliple viewport.
  // Render only opaque mesh list.
  auto& drawList = MDyRendering::GetInstance().GetOpaqueMeshQueueList();

  for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : drawList)
  { // Render
    this->RenderObject(
        *iPtrModel, 
        const_cast<FDyMeshResource&>(*iPtrValidMesh),
        const_cast<FDyMaterialResource&>(*iPtrValidMat)
    );
  }
}

void FBtRenderItemCsmShadow::RenderObject(
  DDyModelHandler::DActorInfo& iRefRenderer, 
  FDyMeshResource& iRefMesh,
  FDyMaterialResource& iRefMaterial)
{
  // Validation test
  const auto* ptrModelBinder = iRefRenderer.mPtrModelRenderer->GetModelResourceBinder();
  if (ptrModelBinder == nullptr) { return; }

  const auto& refModelMatrix = iRefRenderer.mPtrModelRenderer->GetBindedActor()->GetTransform()->GetTransform();
  this->mDirLightShaderResource->TryUpdateUniform<EDyUniformVariableType::Matrix4>("uModelMatrix", refModelMatrix);

  this->mDirLightShaderResource->UseShader();
  this->mDirLightShaderResource->TryUpdateUniformList();
  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. (not support instancing yet) TODO IMPLEMENT BATCHING SYSTEM.
  if (iRefMesh.IsEnabledIndices() == true)
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind present submesh vertex array object.
  FDyGLWrapper::UnbindVertexArrayObject();
  this->mDirLightShaderResource->DisuseShader();
}

void FBtRenderItemCsmShadow::OnReleaseRenderingSetting()
{
  this->mBinderFrameBuffer->UnbindFrameBuffer();
  FDyGLWrapper::PopInternalGlobalState();
}

void FBtRenderItemCsmShadow::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace
