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
#include <Dy/Management/Rendering/RenderingManager.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>

#include <Dy/Component/CDyLightDirectional.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>
#include "Dy/Core/Resource/Resource/FDyModelResource.h"
#include "Dy/Management/WindowManager.h"
#include "Dy/Management/PhysicsManager.h"
#include "Dy/Component/CDyModelFilter.h"
#include <Dy/Builtin/Constant/GeneralValue.h>
#include <Dy/Management/InputManager.h>

#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/Internal/FDyCallStack.h>
#include <Dy/Element/Actor.h>
#include "Dy/Management/Internal/World/FDyWorldUIContainer.h"

#include <queue>
#include <Dy/Core/Rendering/Wrapper/FWrapperRenderPipeline.h>
#include <Dy/Core/Rendering/Wrapper/FWrapperRenderItem.h>
#include <Dy/Core/Rendering/Wrapper/FWrapperHandleRenderPipeline.h>

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

class MDyRendering::Impl final : IDyInitializeHelper<void>
{
public:
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
  CDyLightDirectional* GetPtrMainDirectionalLight() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalLight)(_MIN_ CDyLightDirectional& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalLight)(_MIN_ CDyLightDirectional& iRefLight);
    
  /// @brief Get ptr main directional shadow. If not exist, just return nullptr.
  CDyLightDirectional* GetPtrMainDirectionalShadow() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalShadow)(_MIN_ CDyLightDirectional& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalShadow)(_MIN_ CDyLightDirectional& iRefLight);

  /// @brief Get General (Default) ui projection matrix.
  const DDyMatrix4x4& GetGeneralUiProjectionMatrix() const noexcept;

  /// @brief Swap buffer.
  void SwapBuffers();

  /// @brief Enqueue static draw call to mesh with material.
  void EnqueueDrawMesh(
      _MIN_ DDyModelHandler::DActorInfo& iRefModelRenderer,
      _MIN_ const FDyMeshResource& iRefValidMesh, 
      _MIN_ const FDyMaterialResource& iRefValidMat);

  /// @brief Enqueue debug collider draw call.
  void EnqueueDebugDrawCollider(
      _MIN_ CDyPhysicsCollider& iRefCollider, 
      _MIN_ const DDyMatrix4x4& iTransformMatrix);
  
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

  CDyLightDirectional* mMainDirectionalLight   = nullptr;
  CDyLightDirectional* mMainDirectionalShadow  = nullptr;

  /// @brief The pointer handle list of activated point lights.
  TPointLightHandleList mActivatedPointLights = {};
  /// @brief The pointer handle list of activated spot lights.
  TSpotLightHandleList  mActivatedSpotLights = {};

  /// @brief Required skybox pointer for rendering on present frame.
  /// If rendered, skybox pointer will be nulled again.
  CDySkybox* mPtrRequiredSkybox = nullptr;

  /// @brief Activated directional light list.
  std::queue<TI32>  mDirLightAvailableList     = {};
  /// @brief Default UI projection matrix. (Orthogonal)
  DDyMatrix4x4      mUiGeneralProjectionMatrix = {};

#if defined(MDY_FLAG_IN_EDITOR)
  std::unique_ptr<FDyGrid>                    mGridEffect           = nullptr;
#endif /// MDY_FLAG_IN_EDITOR
};

bool MDyRendering::Impl::HasEntryRenderPipeline(const std::string& iEntryPipelineName)
{
  if (ContainsIf(this->mEntryRenderPipelines,
    [iEntryPipelineName](const auto& pipeline) 
    { return pipeline.GetName() == iEntryPipelineName; }) == false)
  {
    return false;
  }

  return true;
}

EDySuccess MDyRendering::Impl::ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated)
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

EDySuccess MDyRendering::pfInitialize()
{ 
  this->mInternal = new (std::nothrow) Impl();
  this->mInternal->Initialize();
  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfRelease()
{
  delete this->mInternal; this->mInternal = nullptr;
  return DY_SUCCESS;
}

void MDyRendering::PreRender(TF32 iDt) 
{ 
  this->mInternal->PreRender(iDt); 
}

void MDyRendering::SetupDrawModelTaskQueue() 
{ 
  this->mInternal->SetupDrawModelTaskQueue(); 
}

void MDyRendering::EnqueueDrawMesh(
  DDyModelHandler::DActorInfo& iRefModelRenderer,
  const FDyMeshResource& iRefValidMesh, 
  const FDyMaterialResource& iRefValidMat)
{
  this->mInternal->EnqueueDrawMesh(iRefModelRenderer, iRefValidMesh, iRefValidMat);
}

void MDyRendering::EnqueueDebugDrawCollider(
  CDyPhysicsCollider& iRefCollider, 
  const DDyMatrix4x4& iTransformMatrix)
{
  this->mInternal->EnqueueDebugDrawCollider(iRefCollider, iTransformMatrix);
}

void MDyRendering::RenderPipelines() 
{ 
  this->mInternal->RenderPipelines(); 
}

void MDyRendering::MDY_PRIVATE(BindMainDirectionalLight)(CDyLightDirectional& iRefLight)
{
  return this->mInternal->MDY_PRIVATE(BindMainDirectionalLight)(iRefLight);
}
CDyLightDirectional* MDyRendering::GetPtrMainDirectionalLight() const noexcept
{
  return this->mInternal->GetPtrMainDirectionalLight();
}
EDySuccess MDyRendering::MDY_PRIVATE(UnbindMainDirectionalLight)(CDyLightDirectional& iRefLight)
{
  return this->mInternal->MDY_PRIVATE(UnbindMainDirectionalLight)(iRefLight);
}

void MDyRendering::MDY_PRIVATE(BindMainDirectionalShadow)(CDyLightDirectional& iRefLight)
{
  this->mInternal->MDY_PRIVATE(BindMainDirectionalShadow)(iRefLight);
}
CDyLightDirectional* MDyRendering::GetPtrMainDirectionalShadow() const noexcept
{
  return this->mInternal->GetPtrMainDirectionalShadow();
}
EDySuccess MDyRendering::MDY_PRIVATE(UnbindMainDirectionalShadow)(CDyLightDirectional& iRefLight)
{
  return this->mInternal->MDY_PRIVATE(UnbindMainDirectionalShadow)(iRefLight);
}

void MDyRendering::MDY_PRIVATE(BindPointLight)(CDyLightPoint& iRefLight)
{
  auto& handleList = this->mInternal->mActivatedPointLights;
  if (Contains(handleList, &iRefLight) == true)
  {
    MDY_UNEXPECTED_BRANCH();
    return;
  }

  handleList.emplace_back(&iRefLight);
}

EDySuccess MDyRendering::MDY_PRIVATE(UnbindPointLight)(CDyLightPoint& iRefLight)
{
  auto& handleList = this->mInternal->mActivatedPointLights;
  if (Contains(handleList, &iRefLight) == false)
  {
    DyPushLogCritical("Failed to unbind handle of point light.");
    return DY_FAILURE;
  }

  const auto it = std::find(MDY_BIND_BEGIN_END(handleList), &iRefLight);
  handleList.erase(it);
  return DY_SUCCESS;
}

MDyRendering::TPointLightHandleList&
MDyRendering::MDY_PRIVATE(GetActivatedPointLights)() noexcept
{
  return this->mInternal->mActivatedPointLights;
}

void MDyRendering::MDY_PRIVATE(BindSpotLight)(CDyLightSpot& iRefLight)
{
  auto& handleList = this->mInternal->mActivatedSpotLights;
  if (Contains(handleList, &iRefLight) == true)
  {
    MDY_UNEXPECTED_BRANCH();
    return;
  }

  handleList.emplace_back(&iRefLight);
}

EDySuccess MDyRendering::MDY_PRIVATE(UnbindSpotLight)(CDyLightSpot& iRefLight)
{
  auto& handleList = this->mInternal->mActivatedSpotLights;
  if (Contains(handleList, &iRefLight) == false)
  {
    DyPushLogCritical("Failed to unbind handle of spot light.");
    return DY_FAILURE;
  }

  const auto it = std::find(MDY_BIND_BEGIN_END(handleList), &iRefLight);
  handleList.erase(it);
  return DY_SUCCESS;
}

MDyRendering::TSpotLightHandleList& 
MDyRendering::MDY_PRIVATE(GetActivatedSpotLights)() noexcept
{
  return this->mInternal->mActivatedSpotLights;
}

const DDyMatrix4x4& MDyRendering::GetGeneralUiProjectionMatrix() const noexcept
{
  return this->mInternal->GetGeneralUiProjectionMatrix();
}

void MDyRendering::SwapBuffers() { this->mInternal->SwapBuffers(); }

std::vector<MDyRendering::TMeshDrawCallItem>& MDyRendering::GetOpaqueMeshQueueList()
{
  return this->mInternal->mOpaqueMeshDrawingList;
}

std::vector<MDyRendering::TMeshDrawCallItem>& MDyRendering::GetTranclucentOitMeshQueueList()
{
  return this->mInternal->mTranslucentMeshDrawingList;
}

std::vector<MDyRendering::TDrawColliderItem>& MDyRendering::GetColliderMeshQueueList()
{
  return this->mInternal->mDebugColliderDrawingList;
}

std::vector<MDyRendering::TUiDrawCallItem>& MDyRendering::GetUiObjectQueuelist()
{
  return this->mInternal->mUiObjectDrawingList;
}

bool MDyRendering::HasRenderItem(const std::string& iRenderItemName)
{
  return DyIsMapContains(this->mInternal->mRenderItems, iRenderItemName);
}

FWrapperRenderItem* MDyRendering::GetRenderItem(const std::string& iRenderItemName)
{
  if (this->HasRenderItem(iRenderItemName) == false) 
  { 
    return nullptr; 
  }

  return this->mInternal->mRenderItems.at(iRenderItemName).get();
}

bool MDyRendering::HasRenderPipeline(const std::string& iRenderPipelineName)
{
  return DyIsMapContains(this->mInternal->mRenderPipelines, iRenderPipelineName);
}

FWrapperRenderPipeline* MDyRendering::GetRenderPipeline(const std::string& iRenderPipelineName)
{
  if (this->HasRenderPipeline(iRenderPipelineName) == false)
  {
    return nullptr;
  }

  return this->mInternal->mRenderPipelines.at(iRenderPipelineName).get();
}

bool MDyRendering::HasEntryRenderPipeline(const std::string& iEntryPipelineName)
{
  return this->mInternal->HasEntryRenderPipeline(iEntryPipelineName);
}

EDySuccess MDyRendering::ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated)
{
  return this->mInternal->ActivateEntryRenderPipeline(iEntryPipelineName, iIsActivated);
}

} /// ::dy namespace
