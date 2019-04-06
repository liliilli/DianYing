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
#include <Dy/Management/Rendering/MRendering.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Dy/Management/MLog.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/MSetting.h>

#include <Dy/Component/CLightDirectional.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/Rendering/MUniformBufferObject.h>
#include <Dy/Component/CCamera.h>
#include <Dy/Component/CModelRenderer.h>
#include <Dy/Management/Helper/SProfilingHelper.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FResourceMaterial.h>
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>
#include "Dy/Core/Resource/Resource/FResourceModel.h"
#include "Dy/Management/MWindow.h"
#include "Dy/Management/MPhysics.h"
#include "Dy/Component/CModelFilter.h"
#include <Dy/Builtin/Constant/GeneralValue.h>
#include <Dy/Management/MInput.h>

#include <Dy/Helper/System/Pointer.h>
#include <Dy/Helper/Internal/FCallStack.h>
#include <Dy/Element/FActor.h>
#include "Dy/Management/Internal/World/MWorldUIContainers.h"

#include <queue>
#include <Dy/Core/Rendering/Wrapper/FWrapperRenderPipeline.h>
#include <Dy/Core/Rendering/Wrapper/FWrapperRenderItem.h>
#include <Dy/Core/Rendering/Wrapper/FWrapperHandleRenderPipeline.h>
#include <Dy/Helper/Library/HelperContainer.h>

//!
//! Forward declaration & Local translation unit function and data.
//!

namespace 
{

} /// ::unnamed namespace

//!
//! Implementation
//!


namespace dy
{

struct MRendering::Impl final : IInitializeHelper<void>
{
  DY_DEFAULT_PIMPL_CTOR(MRendering);
  EDySuccess Initialize() override final;
  void Release() override final;
  ~Impl();

  /// @brief PreRender update functin.
  void PreRender(TF32 dt);

  /// @brief Create render pipeline with specifier.
  /// If not found, just return DY_FAILURE.
  EDySuccess CreateRenderPipeline(const std::string& iPipelineSpecifier);
  /// @brief Create entry render pipeline handle into list.
  /// Created render pipeline will be rendered following order of child and local render item.
  void CreateHandleRenderPipeline(const PDyRenderPipelineInstanceMetaInfo& iEntryRenderPipeline);

  /// @brief Remove render pipeline with specifier.
  /// If not found in list, just return DY_FAILURE.
  EDySuccess RemoveRenderPipeline(const std::string& iPipelineSpecifier);

  /// @brief 
  /// @TODO LOGIC IS TEMPORARY.
  void SetupDrawModelTaskQueue();

  /// @brief Render level information.
  void RenderPipelines();
  
  /// @brief Get ptr main directional light. If not exist, just return nullptr.
  CLightDirectional* GetPtrMainDirectionalLight() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalLight)(_MIN_ CLightDirectional& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalLight)(_MIN_ CLightDirectional& iRefLight);
    
  /// @brief Get ptr main directional shadow. If not exist, just return nullptr.
  CLightDirectional* GetPtrMainDirectionalShadow() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalShadow)(_MIN_ CLightDirectional& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalShadow)(_MIN_ CLightDirectional& iRefLight);

  /// @brief Get General (Default) ui projection matrix.
  const DMat4& GetGeneralUiProjectionMatrix() const noexcept;

  /// @brief Swap buffer.
  void SwapBuffers();

  /// @brief Enqueue static draw call to mesh with material.
  void EnqueueDrawMesh(
      _MIN_ DDyModelHandler::DActorInfo& iRefModelRenderer,
      _MIN_ const FResourceMesh& iRefValidMesh, 
      _MIN_ const FResourceMaterial& iRefValidMat);

  /// @brief Enqueue debug collider draw call.
  void EnqueueDebugDrawCollider(
      _MIN_ CBasePhysicsCollider& iRefCollider, 
      _MIN_ const DMat4& iTransformMatrix);
  
  /// @brief Check Entry RenderPipeline is exist on rendering system.
  MDY_NODISCARD bool HasEntryRenderPipeline(const std::string& iEntryPipelineName);
  /// @brief Set activation of entry renderpipeline.
  /// If Activated, this pipeline will be rendered with arbitary order.
  /// If Deactivated, this pipeline will not be rendered but leave render resources valid.
  ///
  /// If not found, just do nothing.
  EDySuccess ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated);

  std::unordered_map<std::string, std::unique_ptr<FWrapperRenderPipeline>> 
    mRenderPipelines = {};

  std::unordered_map<std::string, std::unique_ptr<FWrapperRenderItem>> 
    mRenderItems = {};

  std::vector<FWrapperHandleRenderPipeline> mEntryRenderPipelines;

  std::vector<TMeshDrawCallItem> mOpaqueMeshDrawingList = {};
  std::vector<TMeshDrawCallItem> mTranslucentMeshDrawingList = {};
  std::vector<TDrawColliderItem> mDebugColliderDrawingList = {};
  std::vector<TUiDrawCallItem>   mUiObjectDrawingList = {};

  CLightDirectional* mMainDirectionalLight   = nullptr;
  CLightDirectional* mMainDirectionalShadow  = nullptr;

  /// @brief The pointer handle list of activated point lights.
  TPointLightHandleList mActivatedPointLights = {};
  /// @brief The pointer handle list of activated spot lights.
  TSpotLightHandleList  mActivatedSpotLights = {};

  /// @brief Required skybox pointer for rendering on present frame.
  /// If rendered, skybox pointer will be nulled again.
  CSkybox* mPtrRequiredSkybox = nullptr;

  /// @brief Activated directional light list.
  std::queue<TI32>  mDirLightAvailableList     = {};
  /// @brief Default UI projection matrix. (Orthogonal)
  DMat4      mUiGeneralProjectionMatrix = {};

#if defined(MDY_FLAG_IN_EDITOR)
  std::unique_ptr<FDyGrid>                    mGridEffect           = nullptr;
#endif /// MDY_FLAG_IN_EDITOR
};

bool MRendering::Impl::HasEntryRenderPipeline(const std::string& iEntryPipelineName)
{
  if (ContainsIf(this->mEntryRenderPipelines,
    [iEntryPipelineName](const auto& pipeline) 
    { return pipeline.GetName() == iEntryPipelineName; }) == false)
  {
    return false;
  }

  return true;
}

EDySuccess MRendering::Impl::ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated)
{
  if (this->HasEntryRenderPipeline(iEntryPipelineName) == false)
  {
    DyPushLogDebugError("Failed to find entry render pipeline, {}.", iEntryPipelineName);
    return DY_FAILURE;
  }

  auto it = std::find_if(
    MDY_BIND_BEGIN_END(this->mEntryRenderPipelines),
    [iEntryPipelineName](const auto& pipeline) 
    { return pipeline.GetName() == iEntryPipelineName; });
  it->Activate(iIsActivated);
  return DY_SUCCESS;
}

} /// ::dy namespace
#include <Dy/Management/Inline/MRenderingImpl.inl>

//!
//! Proxy
//!

namespace dy
{

EDySuccess MRendering::pfInitialize()
{ 
  DY_INITIALIZE_PIMPL();
  DY_PIMPL->Initialize();
  return DY_SUCCESS;
}

EDySuccess MRendering::pfRelease()
{
  DY_RESET_PIMPL();
  return DY_SUCCESS;
}

void MRendering::PreRender(TF32 iDt) 
{ 
  DY_PIMPL->PreRender(iDt); 
}

void MRendering::SetupDrawModelTaskQueue() 
{ 
  DY_PIMPL->SetupDrawModelTaskQueue(); 
}

void MRendering::EnqueueDrawMesh(
  DDyModelHandler::DActorInfo& iRefModelRenderer,
  const FResourceMesh& iRefValidMesh, 
  const FResourceMaterial& iRefValidMat)
{
  DY_PIMPL->EnqueueDrawMesh(iRefModelRenderer, iRefValidMesh, iRefValidMat);
}

void MRendering::EnqueueDebugDrawCollider(
  CBasePhysicsCollider& iRefCollider, 
  const DMat4& iTransformMatrix)
{
  DY_PIMPL->EnqueueDebugDrawCollider(iRefCollider, iTransformMatrix);
}

void MRendering::RenderPipelines() 
{ 
  DY_PIMPL->RenderPipelines(); 
}

void MRendering::MDY_PRIVATE(BindMainDirectionalLight)(CLightDirectional& iRefLight)
{
  return DY_PIMPL->MDY_PRIVATE(BindMainDirectionalLight)(iRefLight);
}
CLightDirectional* MRendering::GetPtrMainDirectionalLight() const noexcept
{
  return DY_PIMPL->GetPtrMainDirectionalLight();
}
EDySuccess MRendering::MDY_PRIVATE(UnbindMainDirectionalLight)(CLightDirectional& iRefLight)
{
  return DY_PIMPL->MDY_PRIVATE(UnbindMainDirectionalLight)(iRefLight);
}

void MRendering::MDY_PRIVATE(BindMainDirectionalShadow)(CLightDirectional& iRefLight)
{
  DY_PIMPL->MDY_PRIVATE(BindMainDirectionalShadow)(iRefLight);
}
CLightDirectional* MRendering::GetPtrMainDirectionalShadow() const noexcept
{
  return DY_PIMPL->GetPtrMainDirectionalShadow();
}
EDySuccess MRendering::MDY_PRIVATE(UnbindMainDirectionalShadow)(CLightDirectional& iRefLight)
{
  return DY_PIMPL->MDY_PRIVATE(UnbindMainDirectionalShadow)(iRefLight);
}

void MRendering::MDY_PRIVATE(BindPointLight)(CLightPoint& iRefLight)
{
  auto& handleList = DY_PIMPL->mActivatedPointLights;
  if (Contains(handleList, &iRefLight) == true)
  {
    MDY_UNEXPECTED_BRANCH();
    return;
  }

  handleList.emplace_back(&iRefLight);
}

EDySuccess MRendering::MDY_PRIVATE(UnbindPointLight)(CLightPoint& iRefLight)
{
  auto& handleList = DY_PIMPL->mActivatedPointLights;
  if (Contains(handleList, &iRefLight) == false)
  {
    DyPushLogCritical("Failed to unbind handle of point light.");
    return DY_FAILURE;
  }

  const auto it = std::find(MDY_BIND_BEGIN_END(handleList), &iRefLight);
  handleList.erase(it);
  return DY_SUCCESS;
}

MRendering::TPointLightHandleList&
MRendering::MDY_PRIVATE(GetActivatedPointLights)() noexcept
{
  return DY_PIMPL->mActivatedPointLights;
}

void MRendering::MDY_PRIVATE(BindSpotLight)(CLightSpot& iRefLight)
{
  auto& handleList = DY_PIMPL->mActivatedSpotLights;
  if (Contains(handleList, &iRefLight) == true)
  {
    MDY_UNEXPECTED_BRANCH();
    return;
  }

  handleList.emplace_back(&iRefLight);
}

EDySuccess MRendering::MDY_PRIVATE(UnbindSpotLight)(CLightSpot& iRefLight)
{
  auto& handleList = DY_PIMPL->mActivatedSpotLights;
  if (Contains(handleList, &iRefLight) == false)
  {
    DyPushLogCritical("Failed to unbind handle of spot light.");
    return DY_FAILURE;
  }

  const auto it = std::find(MDY_BIND_BEGIN_END(handleList), &iRefLight);
  handleList.erase(it);
  return DY_SUCCESS;
}

MRendering::TSpotLightHandleList& 
MRendering::MDY_PRIVATE(GetActivatedSpotLights)() noexcept
{
  return DY_PIMPL->mActivatedSpotLights;
}

const DMat4& MRendering::GetGeneralUiProjectionMatrix() const noexcept
{
  return DY_PIMPL->GetGeneralUiProjectionMatrix();
}

void MRendering::SwapBuffers() { DY_PIMPL->SwapBuffers(); }

std::vector<MRendering::TMeshDrawCallItem>& MRendering::GetOpaqueMeshQueueList()
{
  return DY_PIMPL->mOpaqueMeshDrawingList;
}

std::vector<MRendering::TMeshDrawCallItem>& MRendering::GetTranclucentOitMeshQueueList()
{
  return DY_PIMPL->mTranslucentMeshDrawingList;
}

std::vector<MRendering::TDrawColliderItem>& MRendering::GetColliderMeshQueueList()
{
  return DY_PIMPL->mDebugColliderDrawingList;
}

std::vector<MRendering::TUiDrawCallItem>& MRendering::GetUiObjectQueuelist()
{
  return DY_PIMPL->mUiObjectDrawingList;
}

bool MRendering::HasRenderItem(const std::string& iRenderItemName)
{
  return Contains(DY_PIMPL->mRenderItems, iRenderItemName);
}

FWrapperRenderItem* MRendering::GetRenderItem(const std::string& iRenderItemName)
{
  if (this->HasRenderItem(iRenderItemName) == false) 
  { 
    return nullptr; 
  }

  return DY_PIMPL->mRenderItems.at(iRenderItemName).get();
}

bool MRendering::HasRenderPipeline(const std::string& iRenderPipelineName)
{
  return Contains(DY_PIMPL->mRenderPipelines, iRenderPipelineName);
}

FWrapperRenderPipeline* MRendering::GetRenderPipeline(const std::string& iRenderPipelineName)
{
  if (this->HasRenderPipeline(iRenderPipelineName) == false)
  {
    return nullptr;
  }

  return DY_PIMPL->mRenderPipelines.at(iRenderPipelineName).get();
}

bool MRendering::HasEntryRenderPipeline(const std::string& iEntryPipelineName)
{
  return DY_PIMPL->HasEntryRenderPipeline(iEntryPipelineName);
}

EDySuccess MRendering::ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated)
{
  return DY_PIMPL->ActivateEntryRenderPipeline(iEntryPipelineName, iIsActivated);
}

DY_DEFINE_PIMPL(MRendering);
DY_DEFINE_DEFAULT_DESTRUCTOR(MRendering);

} /// ::dy namespace
