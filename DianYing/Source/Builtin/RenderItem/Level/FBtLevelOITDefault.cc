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

#include <Dy/Builtin/RenderItem/Level/FBtLevelOITDefault.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/Rendering/MUniformBufferObject.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Component/CModelRenderer.h>
#include <Dy/Element/FActor.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Management/Helper/SProfilingHelper.h>
#include <Dy/Component/CTransform.h>

namespace dy
{

void FBtRenderLevelOitDefault::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("921d0018-78ac-453d-b082-6c0e2a7c35d2", true);
}

EDySuccess FBtRenderLevelOitDefault::OnPreRenderCheckCondition()
{
  auto& list = MRendering::GetInstance().GetTranclucentOitMeshQueueList();
  return list.empty() == false
      && this->mBinderFrameBuffer.IsResourceExist() == true 
      && this->mCompareZDepth.IsResourceExist() == true 
    ? DY_SUCCESS : DY_FAILURE;
}

void FBtRenderLevelOitDefault::OnFailedCheckCondition()
{
  if (this->mBinderFrameBuffer.IsResourceExist() == false) { return; }

  /* Do nothing */
  { 
    this->mBinderFrameBuffer->BindFrameBuffer();
    glClearBufferfv(GL_COLOR, 0, &DColorRGBA::Black.R);
    glClearBufferfv(GL_COLOR, 1, &DColorRGBA::White.R);
    this->mBinderFrameBuffer->UnbindFrameBuffer();
  }
}

void FBtRenderLevelOitDefault::OnSetupRenderingSetting()
{
  this->mBinderFrameBuffer->BindFrameBuffer();
  DGlGlobalStates status;
  using EMode   = DGlGlobalStates::DPolygonMode::EMode;
  using EValue  = DGlGlobalStates::DPolygonMode::EValue;
  using EEqut   = DGlGlobalStates::DBlendMode::EEqut;
  using EFunc   = DGlGlobalStates::DBlendMode::EFunc;
  using DBlendMode = DGlGlobalStates::DBlendMode;

  status.mIsEnableBlend = true;
  status.mIsEnableDepthTest = false;
  status.mIsEnableCullface  = false;

  DBlendMode blendingList{};
  blendingList.mBlendingSettingList.emplace_back(EEqut::SrcAddDst, EFunc::One, EFunc::One);
  blendingList.mBlendingSettingList.emplace_back(EEqut::SrcAddDst, EFunc::Zero, EFunc::OneMinusSrcColor);
  status.mBlendMode = blendingList;

  XGLWrapper::PushInternalGlobalState(status);

  glClearBufferfv(GL_COLOR, 0, &DColorRGBA::Black.R);
  glClearBufferfv(GL_COLOR, 1, &DColorRGBA::White.R);

  this->mBinderFrameBuffer->BindFrameBuffer();
}

void FBtRenderLevelOitDefault::OnRender()
{
  auto& drawList = MRendering::GetInstance().GetTranclucentOitMeshQueueList();

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
        *iPtrModel->mPtrModelRenderer,
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

    std::vector<DMat4> instancingMatrixes;
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
      itemList.size() * sizeof(DMat4), 
      instancingMatrixes.data(), 
      GL_DYNAMIC_DRAW);
    glBindVertexBuffer(1, instancingId, 0, sizeof(DMat4));

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

  SProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(drawList.size()));
  drawList.clear();
}

void FBtRenderLevelOitDefault::RenderObject(
  CModelRenderer& iRefRenderer, 
  FDyMeshResource& iRefMesh,
  FDyMaterialResource& iRefMaterial)
{
  // General deferred rendering
  auto ptrModelTransform = iRefRenderer.GetBindedActor()->GetTransform();
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }

  shaderBinder->TryUpdateUniform<EUniformVariableType::Matrix4>("uModelMatrix", ptrModelTransform->GetTransform());
  shaderBinder->TryUpdateUniform<EUniformVariableType::Float>("uAlphaOffset", 0.5f);
  shaderBinder->TryUpdateUniform<EUniformVariableType::Float>("uDepthScale",  0.1f);
  shaderBinder->TryInsertTextureRequisition(10, this->mCompareZDepth->GetSourceAttachmentId());

  shaderBinder->UseShader();
  shaderBinder->TryUpdateUniformList();
  iRefMaterial.TryUpdateTextureList();
  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. 
  if (iRefMesh.IsEnabledIndices() == true)
  { XGLWrapper::Draw(EDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { XGLWrapper::Draw(EDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind present submesh vertex array object.
  // Unbind, unset, deactivate settings for this submesh and material.
  XGLWrapper::UnbindVertexArrayObject();
  shaderBinder->DisuseShader();
}

void FBtRenderLevelOitDefault::RenderStaticInstancingObjects(
  FDyMeshResource& iRefMesh,
  FDyMaterialResource& iRefMaterial, 
  TU32 iCount)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }
  
  using EUniform = EUniformVariableType;
  shaderBinder->TryUpdateUniform<EUniformVariableType::Float>("uAlphaOffset", 0.5f);
  shaderBinder->TryUpdateUniform<EUniformVariableType::Float>("uDepthScale",  0.1f);
  shaderBinder->TryInsertTextureRequisition(10, this->mCompareZDepth->GetSourceAttachmentId());

  shaderBinder->UseShader();
  shaderBinder->TryUpdateUniformList();

  iRefMaterial.TryUpdateTextureList();
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

  // Unbind, unset, deactivate settings for this submesh and material.
  // Unbind present submesh vertex array object.
  //iRefMaterial.TryDetachTextureListFromShader();
  XGLWrapper::UnbindVertexArrayObject();
  shaderBinder->DisuseShader();
}

void FBtRenderLevelOitDefault::OnReleaseRenderingSetting()
{
  this->mBinderFrameBuffer->UnbindFrameBuffer();

  XGLWrapper::PopInternalGlobalState();
}

void FBtRenderLevelOitDefault::OnPostRender()
{

}

} /// ::dy namespace
