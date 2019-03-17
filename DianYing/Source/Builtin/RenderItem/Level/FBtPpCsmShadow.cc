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
#include <Dy/Management/MWorld.h>
#include <Dy/Management/Rendering/MUniformBufferObject.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Component/CCamera.h>
#include <Dy/Component/CModelRenderer.h>
#include <Dy/Element/FActor.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Component/CModelAnimator.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Management/Helper/SProfilingHelper.h>
#include <Dy/Component/CLightDirectional.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Component/CTransform.h>

namespace dy
{

void FBtRenderItemCsmShadow::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("8dfca835-dee4-4c4a-bfd3-f3c23154116a", true);
}

FBtRenderItemCsmShadow::FBtRenderItemCsmShadow()
{
  XGLWrapper::QueryFloatVector(GL_MAX_VIEWPORT_DIMS, sViewportDims.data());
}

EDySuccess FBtRenderItemCsmShadow::OnPreRenderCheckCondition()
{
  // Check CSM Shadow feature is enabled.
  const auto& information = MSetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultShadow == false)
  {
    return DY_FAILURE;
  }

  // Whether camera is focused by main camera is true, by parent RenderPipeline
  // `FBtDefaultLevel`, we do not need to check more setting.
  auto* ptrLight = MRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (ptrLight == nullptr) 
  { 
    return DY_FAILURE; 
  }

  return this->mBinderFrameBuffer.IsResourceExist() == true
      && this->mDirLightShaderResource.IsResourceExist() == true 
      && this->mInstancingShaderResource.IsResourceExist() == true
      ? DY_SUCCESS : DY_FAILURE;
}

void FBtRenderItemCsmShadow::OnFailedCheckCondition()
{
  if (this->mDirLightShaderResource.IsResourceExist() == false
  ||  this->mBinderFrameBuffer.IsResourceExist() == false
  ||  this->mInstancingShaderResource.IsResourceExist() == false) 
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
  auto* ptrLight = MRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (this->mAddrMainDirectionalShadow != reinterpret_cast<ptrdiff_t>(ptrLight))
  {
    this->mAddrMainDirectionalShadow = reinterpret_cast<ptrdiff_t>(ptrLight);
    if (this->mAddrMainDirectionalShadow == 0) 
    {
      this->mViewMatrix = DMatrix4x4{};
    }
    else
    { 
      ptrLight->UpdateLightViewMatrix(); 
      this->mViewMatrix = ptrLight->GetLightViewMatrix(); 
    }
  }

  if (const auto* ptrCamera = MWorld::GetInstance().GetPtrMainLevelCamera();
      ptrLight != nullptr)
  {
    //
    ptrLight->UpdateSegmentFarPlanes(*ptrCamera);
    ptrLight->UpdateCSMFrustum(*ptrCamera);
    ptrLight->UpdateProjectionMatrix();
    ptrLight->UpdateLightProjectionAndViewports(
      *ptrCamera, 
      ptrLight->GetCSMFarPlanes(), 
      ptrLight->GetUboShadowInfo().mNormalizedFarPlanes
    );
    //
    this->mProjMatrix = ptrLight->GetProjectionMatrix();
  }

  MDY_ASSERT_MSG(
    this->mAddrMainDirectionalShadow == reinterpret_cast<ptrdiff_t>(ptrLight), 
    "CSM Renderer light handle is not matched to given light.");

  // Try update uniform value.
  using EUniform = EUniformVariableType;
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Integer>("uFrustumSegmentCount", static_cast<TI32>(kCSMSegment));
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Matrix4>("mProjMatrix", this->mProjMatrix);
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Matrix4>("mViewMatrix", this->mViewMatrix);

  this->mInstancingShaderResource->TryUpdateUniform<EUniform::Integer>("uFrustumSegmentCount", static_cast<TI32>(kCSMSegment));
  this->mInstancingShaderResource->TryUpdateUniform<EUniform::Matrix4>("mProjMatrix", this->mProjMatrix);
  this->mInstancingShaderResource->TryUpdateUniform<EUniform::Matrix4>("mViewMatrix", this->mViewMatrix);

  // Set and push global internal setting.
  DGlGlobalStates status;
  using EValue = DGlGlobalStates::DCullfaceMode::EValue;
  status.mIsEnableDepthTest = true;
  status.mIsEnableCullface  = true;
  status.mCullfaceMode = EValue::Back;

  using DViewport = DGlGlobalStates::DViewport;
  DViewport viewport;

  /// @brief Setup indexed viewports of light shadow map segments for writing.
  const auto& lightViewports = ptrLight->GetCSMIndexedViewports();
  for (TU32 i = 0; i < kCSMSegment; ++i)
  {
    viewport.mViewportSettingList.emplace_back(i, lightViewports[i]);
  }
  status.mViewportSettingList = viewport;

  XGLWrapper::PushInternalGlobalState(status);

  this->mBinderFrameBuffer->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &one);
}

void FBtRenderItemCsmShadow::OnRender()
{
  // Cascade shadow mapping use different and mutliple viewport.
  // Render only opaque mesh list.
  auto& drawList = MRendering::GetInstance().GetOpaqueMeshQueueList();

  // Make instancing list.
  std::unordered_map<const FDyMeshResource*, std::vector<MRendering::TMeshDrawCallItem*>> instancingList;

  for (auto& item : drawList)
  {
    auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] = item;
    if (iPtrValidMesh->IsSupportingInstancing() == true)
    {
      auto& list = instancingList[iPtrValidMesh.Get()];
      list.emplace_back(&item);
    }
    else
    {
      // Render without instancing.
      this->RenderObject(
        *iPtrModel,
        const_cast<FDyMeshResource&>(*iPtrValidMesh),
        const_cast<FDyMaterialResource&>(*iPtrValidMat)
      );
    }
  }

  for (auto& [_, itemList] : instancingList)
  {
    if (itemList.empty() == true) { continue; }
    auto& [__, iMainValidMesh, iMainValidMaterial] = *itemList.front();
    const auto instancingId = *iMainValidMesh->GetInstancingBufferId();

    std::vector<DMatrix4x4> instancingMatrixes;
    instancingMatrixes.reserve(itemList.size());

    for (auto& item : itemList)
    {
      auto& ptrModel = std::get<0>(*item);
      instancingMatrixes.emplace_back(
        ptrModel->mPtrModelRenderer->GetBindedActor()->GetTransform()->GetTransform()
      );
    }

    // We need to construct vertex binding and attribute binding connection 
    // whenever instancing buffer is renewed. (maybe)
    auto& ptr = const_cast<FDyMeshResource&>(*iMainValidMesh);
    ptr.BindVertexArray();
    glBindVertexArray(iMainValidMesh->GetVertexArrayId());
    glBindBuffer(GL_ARRAY_BUFFER, instancingId);
    glBufferData(GL_ARRAY_BUFFER, 
      itemList.size() * sizeof(DMatrix4x4), 
      instancingMatrixes.data(), 
      GL_DYNAMIC_DRAW);
    glBindVertexBuffer(1, instancingId, 0, sizeof(DMatrix4x4));

    glEnableVertexAttribArray(10);
    glEnableVertexAttribArray(11);
    glEnableVertexAttribArray(12);
    glEnableVertexAttribArray(13);

    glVertexAttribFormat(10, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribFormat(11, 4, GL_FLOAT, GL_FALSE, 16);
    glVertexAttribFormat(12, 4, GL_FLOAT, GL_FALSE, 32);
    glVertexAttribFormat(13, 4, GL_FLOAT, GL_FALSE, 48);

    glVertexAttribBinding(10, 1);
    glVertexAttribBinding(11, 1);
    glVertexAttribBinding(12, 1);
    glVertexAttribBinding(13, 1);

    glVertexBindingDivisor(1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->RenderStaticInstancingObjects(
      const_cast<FDyMeshResource&>(*iMainValidMesh),
      const_cast<FDyMaterialResource&>(*iMainValidMaterial), itemList.size());
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
  this->mDirLightShaderResource->TryUpdateUniform<EUniformVariableType::Matrix4>("uModelMatrix", refModelMatrix);

  this->mDirLightShaderResource->UseShader();
  this->mDirLightShaderResource->TryUpdateUniformList();
  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. (not support instancing yet) TODO IMPLEMENT BATCHING SYSTEM.
  if (iRefMesh.IsEnabledIndices() == true)
  { XGLWrapper::Draw(EDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { XGLWrapper::Draw(EDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind present submesh vertex array object.
  XGLWrapper::UnbindVertexArrayObject();
  this->mDirLightShaderResource->DisuseShader();
}

void FBtRenderItemCsmShadow::RenderStaticInstancingObjects(
  FDyMeshResource& iRefMesh,
  FDyMaterialResource& iRefMaterial, 
  TU32 iCount)
{
  this->mInstancingShaderResource->UseShader();
  this->mInstancingShaderResource->TryUpdateUniformList();
  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. 
  if (iRefMesh.IsEnabledIndices() == true)
  { 
    XGLWrapper::DrawInstanced(EDrawType::Triangle, true, iRefMesh.GetIndicesCounts(), iCount); 
  }
  else
  { 
    XGLWrapper::DrawInstanced(EDrawType::Triangle, false, iRefMesh.GetVertexCounts(), iCount); 
  }

  // Unbind present submesh vertex array object.
  XGLWrapper::UnbindVertexArrayObject();
  this->mInstancingShaderResource->DisuseShader(); 
}

void FBtRenderItemCsmShadow::OnReleaseRenderingSetting()
{
  this->mBinderFrameBuffer->UnbindFrameBuffer();
  XGLWrapper::PopInternalGlobalState();
}

void FBtRenderItemCsmShadow::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace
