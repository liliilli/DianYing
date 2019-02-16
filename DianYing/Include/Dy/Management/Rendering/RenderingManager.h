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
#include <Dy/Core/Resource/Object/Grid.h>
#include <Dy/Core/Rendering/Pipeline/BasicRenderer.h>
#include <Dy/Core/Rendering/Pipeline/FinalScreenDisplayRenderer.h>
#include <Dy/Core/Rendering/Pipeline/PostEffectSsao.h>
#include <Dy/Core/Rendering/Pipeline/UIBasicRenderer.h>
#include <Dy/Core/Rendering/Pipeline/LevelCascadeShadowRenderer.h>
#include <Dy/Core/Rendering/Pipeline/LevelCSMIntegration.h>
#include <Dy/Core/Rendering/Pipeline/LevelOITRenderer.h>
#include <Dy/Management/Type/Render/DDyModelHandler.h>
#include <Dy/Core/Rendering/Pipeline/DebugRenderer.h>
#include <Dy/Core/Rendering/Pipeline/PostEffectSky.h>
#include <Dy/Helper/Pointer.h>

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
  std::unique_ptr<FDyDebugRenderer>               mDebugRenderer        = MDY_INITIALIZE_NULL;

  using TMeshDrawCallItem = std::tuple<
      NotNull<DDyModelHandler::DActorInfo*>,
      NotNull<const FDyMeshResource*>, 
      NotNull<const FDyMaterialResource*>
  >;

  using TDrawColliderItem = std::pair<NotNull<CDyPhysicsCollider*>, DDyMatrix4x4>; 

  std::vector<TMeshDrawCallItem> mOpaqueMeshDrawingList = {};
  std::vector<TMeshDrawCallItem> mTranslucentMeshDrawingList = {};
  std::vector<TDrawColliderItem> mDebugColliderDrawingList = {};

  CDyDirectionalLight* mMainDirectionalLight   = nullptr;
  CDyDirectionalLight* mMainDirectionalShadow  = nullptr;

  /// @brief Required skybox pointer for rendering on present frame.
  /// If rendered, skybox pointer will be nulled again.
  CDySkybox* mPtrRequiredSkybox = nullptr;

  std::queue<TI32>    mDirLightAvailableList  = {};

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