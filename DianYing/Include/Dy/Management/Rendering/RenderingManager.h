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
class   CDyLightPoint;
class   CDyLightSpot;
class   FWrapperRenderItem;
class   FWrapperRenderPipeline;
struct  DDyUboDirectionalLight;
class   CDyCamera;
class   CDyModelRenderer;
class   CDyLightDirectional;
class   CDyPhysicsCollider;
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

/// @class MDyRendering
/// @brief Rendering maanger
class MDyRendering final : public IDySingleton<MDyRendering>
{
  MDY_SINGLETON_PROPERTIES(MDyRendering);
  MDY_SINGLETON_DERIVED(MDyRendering);
public:
  using TMeshDrawCallItem = std::tuple<
      NotNull<DDyModelHandler::DActorInfo*>,
      NotNull<const FDyMeshResource*>, 
      NotNull<const FDyMaterialResource*>
  >;

  using TDrawColliderItem = std::pair<NotNull<CDyPhysicsCollider*>, DMatrix4x4>; 
  using TUiDrawCallItem = NotNull<FDyUiObject*>;
  using TPointLightHandleList = std::vector<CDyLightPoint*>; 
  using TSpotLightHandleList  = std::vector<CDyLightSpot*>; 

  /// @brief PreRender update functin.
  void PreRender(TF32 dt);

  /// @brief 
  /// @TODO LOGIC IS TEMPORARY.
  void SetupDrawModelTaskQueue();

  /// @brief Render level information.
  void RenderPipelines();

  /// @brief Get ptr main directional light. If not exist, just return nullptr.
  MDY_NODISCARD CDyLightDirectional* GetPtrMainDirectionalLight() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalLight)(CDyLightDirectional& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalLight)(CDyLightDirectional& iRefLight);
    
  /// @brief Get ptr main directional shadow. If not exist, just return nullptr.
  MDY_NODISCARD CDyLightDirectional* GetPtrMainDirectionalShadow() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalShadow)(CDyLightDirectional& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalShadow)(CDyLightDirectional& iRefLight);

  /// @brief Private function, bind activated point light into candidate list.
  void        MDY_PRIVATE(BindPointLight)(CDyLightPoint& iRefLight);
  /// @brief Private function, unbind point light from list.
  EDySuccess  MDY_PRIVATE(UnbindPointLight)(CDyLightPoint& iRefLight);
  /// @brief Get activated point light components.
  TPointLightHandleList& MDY_PRIVATE(GetActivatedPointLights)() noexcept;

  /// @brief Private function, bind activated spot light into candidate list.
  void        MDY_PRIVATE(BindSpotLight)(CDyLightSpot& iRefLight);
  /// @brief Private function, unbind spot light from list.
  EDySuccess  MDY_PRIVATE(UnbindSpotLight)(CDyLightSpot& iRefLight);
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
      CDyPhysicsCollider& iRefCollider, 
      const DMatrix4x4& iTransformMatrix);

  class Impl; Impl* mInternal = nullptr;

  friend class MPhysics;
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MANAGER_H