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
#include <Dy/Component/Internal/Camera/DUboCameraBlock.h>
#include <Dy/Component/CTransform.h>

namespace dy
{

using TInstancingHash = std::pair<const FDyMeshResource*, const FDyMaterialResource*>;

}

namespace std
{

template<> struct hash<dy::TInstancingHash>
{
  using TArgumentType = dy::TInstancingHash;
  typedef std::size_t result_type;
  result_type operator()(const TArgumentType& s) const noexcept
  {
    result_type const h1 ( std::hash<std::string>{}(s.first->GetSpecifierName()) );
    result_type const h2 ( std::hash<std::string>{}(s.second->GetSpecifierName()) );
    return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
}
};

} /// ::std namespace

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

  const auto* ptrCamera = MWorld::GetInstance().GetPtrMainLevelCamera();
  if (MDY_CHECK_ISNULL(ptrCamera)) { return; }

  const auto& ptrValidCamera = *ptrCamera;
  auto& uboManager = MUniformBufferObject::GetInstance();
  {
    const auto flag = uboManager.UpdateUboContainer(
        (sUboCameraBlock),
        offsetof(DUboCameraBlock, mViewMatrix),
        sizeof(DUboCameraBlock::mViewMatrix),
        &ptrValidCamera.GetViewMatrix()[0].X);
    MDY_ASSERT_MSG(flag == DY_SUCCESS, "");
  }
  {
    const auto flag = uboManager.UpdateUboContainer(
        (sUboCameraBlock),
        offsetof(DUboCameraBlock, mProjMatrix),
        sizeof(DUboCameraBlock::mProjMatrix),
        &ptrValidCamera.GetProjectionMatrix()[0].X);
    MDY_ASSERT_MSG(flag == DY_SUCCESS, "");
  }
}

void FBtRenderItemOpaqueDefault::OnRender()
{
  auto& drawList = MRendering::GetInstance().GetOpaqueMeshQueueList();

  // Make instancing list.
  std::unordered_map<
    std::pair<const FDyMeshResource*, const FDyMaterialResource*>, 
    std::vector<MRendering::TMeshDrawCallItem*>> instancingList;

  for (auto& item : drawList)
  {
    auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] = item;
    if (iPtrValidMesh->IsSupportingInstancing() == true)
    {
      auto keyPair = std::pair(iPtrValidMesh.Get(), iPtrValidMat.Get());
      auto& list = instancingList[keyPair];
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
}

void FBtRenderItemOpaqueDefault::RenderObject(
  DDyModelHandler::DActorInfo& iRefRenderer, 
  FDyMeshResource& iRefMesh,
  FDyMaterialResource& iRefMaterial)
{
 // General deferred rendering
  auto& refActor = *iRefRenderer.mPtrModelRenderer->GetBindedActor();
  const auto& transform = refActor.GetTransform();
  const auto& refModelMatrix = transform->GetTransform();
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }

  using EUniform = EUniformVariableType;
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
  if (iRefMesh.IsEnabledIndices() == true)
  { XGLWrapper::Draw(EDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { XGLWrapper::Draw(EDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind, unset, deactivate settings for this submesh and material.
  // Unbind present submesh vertex array object.
  iRefMaterial.TryDetachTextureListFromShader();
  XGLWrapper::UnbindVertexArrayObject();
  shaderBinder->DisuseShader();
}

void FBtRenderItemOpaqueDefault::RenderStaticInstancingObjects(
  FDyMeshResource& iRefMesh,
  FDyMaterialResource& iRefMaterial,
  TU32 iCount)
{
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }
  
  using EUniform = EUniformVariableType;
  shaderBinder->TryUpdateUniform<EUniform::Float>("uBtDyActorId", TF32(0));
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
