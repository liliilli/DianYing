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
#include <Dy/Management/Type/Render/DDyGlGlobalStatus.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class   FWrapperRenderItem;
class   FWrapperRenderPipeline;
struct  DDyUboDirectionalLight;
class   CDyCamera;
class   CDyModelRenderer;
class   CDyDirectionalLight;
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

  using TDrawColliderItem = std::pair<NotNull<CDyPhysicsCollider*>, DDyMatrix4x4>; 
  using TUiDrawCallItem = NotNull<FDyUiObject*>;

  /// @brief PreRender update functin.
  void PreRender(_MIN_ TF32 dt);

  /// @brief 
  /// @TODO LOGIC IS TEMPORARY.
  void SetupDrawModelTaskQueue();

  /// @brief Render level information.
  void RenderPipelines();

  /// @brief Get ptr main directional light. If not exist, just return nullptr.
  MDY_NODISCARD CDyDirectionalLight* GetPtrMainDirectionalLight() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalLight)(CDyDirectionalLight& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalLight)(CDyDirectionalLight& iRefLight);
    
  /// @brief Get ptr main directional shadow. If not exist, just return nullptr.
  MDY_NODISCARD CDyDirectionalLight* GetPtrMainDirectionalShadow() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalShadow)(CDyDirectionalLight& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalShadow)(CDyDirectionalLight& iRefLight);

  /// @brief Get General (Default) ui projection matrix.
  const DDyMatrix4x4& GetGeneralUiProjectionMatrix() const noexcept;
  /// @brief Insert GL global status.
  void InsertInternalGlobalStatus(const DDyGlGlobalStatus& iNewStatus); 
  /// @brief Pop GL global status.
  void PopInternalGlobalStatus();

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
      const DDyMatrix4x4& iTransformMatrix);

  class Impl; Impl* mInternal = nullptr;

  friend class MDyPhysics;
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MANAGER_H