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
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Element/Actor.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Type/EDyDrawType.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>

namespace dy
{

void FBtRenderLevelOitDefault::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("921d0018-78ac-453d-b082-6c0e2a7c35d2", true);
}

EDySuccess FBtRenderLevelOitDefault::OnPreRenderCheckCondition()
{
  auto& list = MDyRendering::GetInstance().GetTranclucentOitMeshQueueList();
  return list.empty() == false
      && this->mBinderFrameBuffer.IsResourceExist() == true ? DY_SUCCESS : DY_FAILURE;
}

void FBtRenderLevelOitDefault::OnFailedCheckCondition()
{
  /* Do nothing */
}

void FBtRenderLevelOitDefault::OnSetupRenderingSetting()
{
  this->mBinderFrameBuffer->BindFrameBuffer();
  DDyGlGlobalStates status;
  using EMode   = DDyGlGlobalStates::DPolygonMode::EMode;
  using EValue  = DDyGlGlobalStates::DPolygonMode::EValue;
  using EEqut   = DDyGlGlobalStates::DBlendMode::EEqut;
  using EFunc   = DDyGlGlobalStates::DBlendMode::EFunc;
  using DBlendMode = DDyGlGlobalStates::DBlendMode;

  status.mIsEnableBlend = true;
  status.mIsEnableDepthTest = false;
  status.mIsEnableCullface  = false;

  DBlendMode blendingList{};
  blendingList.mBlendingSettingList.emplace_back(EEqut::SrcAddDst, EFunc::One, EFunc::One);
  blendingList.mBlendingSettingList.emplace_back(EEqut::SrcAddDst, EFunc::Zero, EFunc::OneMinusSrcColor);
  status.mBlendMode = blendingList;

  FDyGLWrapper::PushInternalGlobalState(status);

  glClearBufferfv(GL_COLOR, 0, &DColorRGBA::Black.R);
  glClearBufferfv(GL_COLOR, 1, &DColorRGBA::White.R);

  this->mBinderFrameBuffer->BindFrameBuffer();
}

void FBtRenderLevelOitDefault::OnRender()
{
  auto& drawList = MDyRendering::GetInstance().GetTranclucentOitMeshQueueList();
  for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : drawList)
  { // Render
    this->RenderObject(
      *iPtrModel->mPtrModelRenderer,
      const_cast<FDyMeshResource&>(*iPtrValidMesh),
      const_cast<FDyMaterialResource&>(*iPtrValidMat)
    );
  }

  SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(drawList.size()));
  drawList.clear();
}

void FBtRenderLevelOitDefault::RenderObject(
  CDyModelRenderer& iRefRenderer, 
  FDyMeshResource& iRefMesh,
  FDyMaterialResource& iRefMaterial)
{
  // General deferred rendering
  auto ptrModelTransform = iRefRenderer.GetBindedActor()->GetTransform();
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }

  shaderBinder->TryUpdateUniform<EDyUniformVariableType::Matrix4>(
    "uModelMatrix", 
    ptrModelTransform->GetTransform());
  shaderBinder->TryUpdateUniform<EDyUniformVariableType::Matrix4>(
    "uRotationMatrix", 
    ptrModelTransform->GetRotationMatrix());
  shaderBinder->TryUpdateUniform<EDyUniformVariableType::Float>("uAlphaOffset", 0.75f);
  shaderBinder->TryUpdateUniform<EDyUniformVariableType::Float>("uDepthScale",  0.1f);

  shaderBinder->UseShader();
  shaderBinder->TryUpdateUniformList();
  iRefMaterial.TryUpdateTextureList();
  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. 
  if (iRefMesh.IsEnabledIndices() == true)
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind present submesh vertex array object.
  // Unbind, unset, deactivate settings for this submesh and material.
  FDyGLWrapper::UnbindVertexArrayObject();
  shaderBinder->DisuseShader();
}

void FBtRenderLevelOitDefault::OnReleaseRenderingSetting()
{
  this->mBinderFrameBuffer->UnbindFrameBuffer();

  FDyGLWrapper::PopInternalGlobalState();
}

void FBtRenderLevelOitDefault::OnPostRender()
{

}

} /// ::dy namespace
