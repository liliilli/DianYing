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

#include <queue>

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/Render/DDyModelHandler.h>
#include <Dy/Management/Type/Render/DDyGlGlobalStatus.h>
#include <Dy/Core/Rendering/Pipeline/BasicRenderer.h>
#include <Dy/Core/Rendering/Pipeline/FinalScreenDisplayRenderer.h>
#include <Dy/Core/Rendering/Pipeline/PostEffectSsao.h>
#include <Dy/Core/Rendering/Pipeline/UIBasicRenderer.h>
#include <Dy/Core/Rendering/Pipeline/LevelCascadeShadowRenderer.h>
#include <Dy/Core/Rendering/Pipeline/LevelCSMIntegration.h>
#include <Dy/Core/Rendering/Pipeline/LevelOITRenderer.h>
#include <Dy/Core/Rendering/Pipeline/DebugShapeRenderer.h>
#include <Dy/Core/Rendering/Pipeline/PostEffectSky.h>
#include <Dy/Core/Rendering/Pipeline/DebugAABBRenderer.h>
#include <Dy/Core/Rendering/Pipeline/Debug/PickingRenderer.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/Internal/FDyCallStack.h>

//!
//! Forward declaration
//!

namespace dy
{
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

///
/// @class MDyRendering
/// @brief Rendering maanger
///
class MDyRendering final : public IDySingleton<MDyRendering>
{
  MDY_SINGLETON_PROPERTIES(MDyRendering);
  MDY_SINGLETON_DERIVED(MDyRendering);
public:
  /// @brief PreRender update functin.
  void PreRender(_MIN_ TF32 dt);

  /// @brief 
  /// @TODO LOGIC IS TEMPORARY.
  void SetupDrawModelTaskQueue();

  /// @brief Render level information.
  void RenderLevelInformation();
  /// @brief Render level debug information. This function must be called in render phase.
  /// @reference https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/DebugVisualization.html#debugvisualization
  void RenderDebugInformation();
  /// @brief Render UI information.
  void RenderUIInformation();
  /// @brief Integrate Level information + Debug Information + UI Information.
  void Integrate();

  /// @brief Render only loading widget.
  void MDY_PRIVATE(RenderLoading());
  
  /// @brief Get ptr main directional light. If not exist, just return nullptr.
  MDY_NODISCARD CDyDirectionalLight* GetPtrMainDirectionalLight() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight);
    
  /// @brief Get ptr main directional shadow. If not exist, just return nullptr.
  MDY_NODISCARD CDyDirectionalLight* GetPtrMainDirectionalShadow() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight);

  /// @brief Get General (Default) ui projection matrix.
  const DDyMatrix4x4& GetGeneralUiProjectionMatrix() const noexcept;
  /// @brief Insert GL global status.
  void InsertInternalGlobalStatus(_MIN_ const DDyGlGlobalStatus& iNewStatus); 
  /// @brief Pop GL global status.
  void PopInternalGlobalStatus();

private:
  /// @brief Enqueue static draw call to mesh with material.
  void EnqueueDrawMesh(
      _MIN_ DDyModelHandler::DActorInfo& iRefModelRenderer,
      _MIN_ const FDyMeshResource& iRefValidMesh, 
      _MIN_ const FDyMaterialResource& iRefValidMat);

  /// @brief Enqueue debug collider draw call.
  void EnqueueDebugDrawCollider(
      _MIN_ CDyPhysicsCollider& iRefCollider, 
      _MIN_ const DDyMatrix4x4& iTransformMatrix);
  
  ///
  /// @brief Reset all of rendering framebuffers related to rendering of scene for new frame rendering.
  ///
  void pClearRenderingFramebufferInstances() noexcept;

  ///
  std::unique_ptr<FDyBasicRenderer>               mBasicOpaqueRenderer  = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyLevelCascadeShadowRenderer>  mCSMRenderer          = MDY_INITIALIZE_NULL; 
  std::unique_ptr<FDyLevelCSMIntergration>        mLevelFinalRenderer   = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyLevelOITRenderer>            mTranslucentOIT       = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyPostEffectSsao>              mSSAOPostEffect       = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyPostEffectSky>               mSkyPostEffect        = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyUIBasicRenderer>             mUiBasicRenderer      = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyFinalScreenDisplayRenderer>  mFinalDisplayRenderer = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyDebugShapeRenderer>          mDebugShapeRenderer   = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyDebugAABBRenderer>           mDebugAABBRenderer    = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyDebugPickingRenderer>        mDebugPickingRenderer = MDY_INITIALIZE_NULL;

  using TMeshDrawCallItem = std::tuple<
      NotNull<DDyModelHandler::DActorInfo*>,
      NotNull<const FDyMeshResource*>, 
      NotNull<const FDyMaterialResource*>
  >;

  using TDrawColliderItem = std::pair<NotNull<CDyPhysicsCollider*>, DDyMatrix4x4>; 
  using TUiDrawCallItem = NotNull<FDyUiObject*>;

  std::vector<TMeshDrawCallItem> mOpaqueMeshDrawingList = {};
  std::vector<TMeshDrawCallItem> mTranslucentMeshDrawingList = {};
  std::vector<TDrawColliderItem> mDebugColliderDrawingList = {};
  std::vector<TUiDrawCallItem>   mUiObjectDrawingList = {};

  CDyDirectionalLight* mMainDirectionalLight   = nullptr;
  CDyDirectionalLight* mMainDirectionalShadow  = nullptr;

  /// @brief Required skybox pointer for rendering on present frame.
  /// If rendered, skybox pointer will be nulled again.
  CDySkybox* mPtrRequiredSkybox = nullptr;

  /// @brief Activated directional light list.
  std::queue<TI32>  mDirLightAvailableList      = {};
  /// @brief Default UI projection matrix. (Orthogonal)
  DDyMatrix4x4      mUiGeneralProjectionMatrix  = {};

  /// @brief Global status stack for management. \n
  /// This container will be push & popped automatically by following rendering pipeline.
  /// This container must not be empty before termination of Dy application.
  FDyCallStack<DDyGlGlobalStatus> mInternalGlobalStatusStack;
  /// ▽ Actual state machine change logic will be operated in these stack.
  FDyCallStack<bool> mInternal_FeatBlendStack;
  FDyCallStack<bool> mInternal_FeatCullfaceStack;
  FDyCallStack<bool> mInternal_FeatDepthTestStack;
  FDyCallStack<bool> mInternal_FeatScissorTestStack;
  FDyCallStack<DDyGlGlobalStatus::DPolygonMode>   mInternal_PolygonModeStack;
  FDyCallStack<DDyGlGlobalStatus::DBlendMode>     mInternal_BlendModeStack;
  FDyCallStack<DDyGlGlobalStatus::DCullfaceMode>  mInternal_CullfaceModeStack;
  FDyCallStack<DDyGlGlobalStatus::DViewport>      mInternal_ViewportStack;

#if defined(MDY_FLAG_IN_EDITOR)
  std::unique_ptr<FDyGrid>                    mGridEffect           = nullptr;
#endif /// MDY_FLAG_IN_EDITOR

  friend class CDyDirectionalLight;
  friend class FDyDeferredRenderingMesh;
  friend class FDyPostEffectSsao;
  friend class editor::FDyMainViewport;

  friend class MDyPhysics;
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MANAGER_H