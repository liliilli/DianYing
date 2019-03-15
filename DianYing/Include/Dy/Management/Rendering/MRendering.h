#ifndef GUARD_DY_RENDERING_MANAGER_H
#define GUARD_DY_RENDERING_MANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/Render/DDyModelHandler.h>
#include <Dy/Management/Type/Render/DDyGlGlobalStates.h>
#include <Dy/Helper/System/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class   CLightPoint;
class   CLightSpot;
class   FWrapperRenderItem;
class   FWrapperRenderPipeline;
struct  DUboDirectionalLight;
class   CCamera;
class   CModelRenderer;
class   CLightDirectional;
class   CBasePhysicsCollider;
class   CDySkybox;
class   FDyUiObject;
} /// ::dy namespace

namespace dy::editor
{
class   FDyMainViewport;
} /// ::dy::editor namespace

//!
//! Implementation
//!

namespace dy
{

/// @class MRendering
/// @brief Rendering maanger
class MRendering final : public ISingleton<MRendering>
{
  MDY_SINGLETON_PROPERTIES(MRendering);
  MDY_SINGLETON_DERIVED(MRendering);
public:
  using TMeshDrawCallItem = std::tuple<
      NotNull<DDyModelHandler::DActorInfo*>,
      NotNull<const FDyMeshResource*>, 
      NotNull<const FDyMaterialResource*>
  >;

  using TDrawColliderItem = std::pair<NotNull<CBasePhysicsCollider*>, DMatrix4x4>; 
  using TUiDrawCallItem = NotNull<FDyUiObject*>;
  using TPointLightHandleList = std::vector<CLightPoint*>; 
  using TSpotLightHandleList  = std::vector<CLightSpot*>; 

  /// @brief PreRender update functin.
  void PreRender(TF32 dt);

  /// @brief 
  /// @TODO LOGIC IS TEMPORARY.
  void SetupDrawModelTaskQueue();

  /// @brief Render level information.
  void RenderPipelines();

  /// @brief Get ptr main directional light. If not exist, just return nullptr.
  MDY_NODISCARD CLightDirectional* GetPtrMainDirectionalLight() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalLight)(CLightDirectional& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalLight)(CLightDirectional& iRefLight);
    
  /// @brief Get ptr main directional shadow. If not exist, just return nullptr.
  MDY_NODISCARD CLightDirectional* GetPtrMainDirectionalShadow() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalShadow)(CLightDirectional& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalShadow)(CLightDirectional& iRefLight);

  /// @brief Private function, bind activated point light into candidate list.
  void        MDY_PRIVATE(BindPointLight)(CLightPoint& iRefLight);
  /// @brief Private function, unbind point light from list.
  EDySuccess  MDY_PRIVATE(UnbindPointLight)(CLightPoint& iRefLight);
  /// @brief Get activated point light components.
  TPointLightHandleList& MDY_PRIVATE(GetActivatedPointLights)() noexcept;

  /// @brief Private function, bind activated spot light into candidate list.
  void        MDY_PRIVATE(BindSpotLight)(CLightSpot& iRefLight);
  /// @brief Private function, unbind spot light from list.
  EDySuccess  MDY_PRIVATE(UnbindSpotLight)(CLightSpot& iRefLight);
  /// @brief Get activated spot light components.
  TSpotLightHandleList& MDY_PRIVATE(GetActivatedSpotLights)() noexcept;

  /// @brief Get General (Default) ui projection matrix.
  const DMatrix4x4& GetGeneralUiProjectionMatrix() const noexcept;
  /// @brief Swap buffer.
  void SwapBuffers();

  /// @todo TEMPORARY API
  std::vector<TMeshDrawCallItem>& GetOpaqueMeshQueueList();
  /// @todo TEMPORARY API
  std::vector<TMeshDrawCallItem>& GetTranclucentOitMeshQueueList(); 
  /// @todo TEMPORARY API
  std::vector<TDrawColliderItem>& GetColliderMeshQueueList(); 
  /// @todo TEMPORARY API
  std::vector<TUiDrawCallItem>& GetUiObjectQueuelist();

  /// @brief Check RenderItem is exist on rendering system.
  MDY_NODISCARD bool HasRenderItem(const std::string& iRenderItemName);
  /// @brief Get handle instance of RenderItem into handle.
  MDY_NODISCARD FWrapperRenderItem* GetRenderItem(const std::string& iRenderItemName);
  /// @brief Check RenderPipeline is exist on rendering system.
  MDY_NODISCARD bool HasRenderPipeline(const std::string& iRenderPipelineName);
  /// @brief Get handle instance of RenderPipeline into handle.
  MDY_NODISCARD FWrapperRenderPipeline* GetRenderPipeline(const std::string& iRenderPipelineName);
  /// @brief Check Entry RenderPipeline is exist on rendering system.
  MDY_NODISCARD bool HasEntryRenderPipeline(const std::string& iEntryPipelineName);
  /// @brief Set activation of entry renderpipeline.
  /// If Activated, this pipeline will be rendered with arbitary order.
  /// If Deactivated, this pipeline will not be rendered but leave render resources valid.
  ///
  /// If not found, just do nothing.
  EDySuccess ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated);

private:
  /// @brief Enqueue static draw call to mesh with material.
  void EnqueueDrawMesh(
      DDyModelHandler::DActorInfo& iRefModelRenderer,
      const FDyMeshResource& iRefValidMesh, 
      const FDyMaterialResource& iRefValidMat);

  /// @brief Enqueue debug collider draw call.
  void EnqueueDebugDrawCollider(
      CBasePhysicsCollider& iRefCollider, 
      const DMatrix4x4& iTransformMatrix);

  class Impl; Impl* mInternal = nullptr;

  friend class MPhysics;
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MANAGER_H