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
#include <Dy/Core/Rendering/Pipeline/DeferredRenderingMesh.h>
#include <Dy/Core/Rendering/Pipeline/FinalScreenDisplayRenderer.h>
#include <Dy/Core/Rendering/Pipeline/PostEffectSsao.h>
#include <Dy/Core/Rendering/Pipeline/UIBasicRenderer.h>
#include <Dy/Core/Rendering/Pipeline/LevelCascadeShadowRenderer.h>

//!
//! Forward declaration
//!

namespace dy
{
struct  DDyUboDirectionalLight;
class   CDyCamera;
class   CDyModelRenderer;
class   CDyDirectionalLight;
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
  ///
  /// @brief
  /// @param rendererInstance
  /// @TODO SCRIPT THIS!
  ///
  void PushDrawCallTask(_MIN_ CDyModelRenderer& rendererInstance);

  ///
  /// @brief
  /// @TODO SCRIPT THIS!
  ///
  void RenderDrawCallQueue();

  /// @brief Render only loading widget.
  void MDY_PRIVATE_SPECIFIER(RenderLoading());

private:
  ///
  /// @brief Reset all of rendering framebuffers related to rendering of scene for new frame rendering.
  ///
  void pClearRenderingFramebufferInstances() noexcept;

  ///
  /// @brief  Issue available directional light index. If not available, just no value.
  /// @param  Instance
  /// @return
  ///
  MDY_NODISCARD std::optional<TI32> pGetAvailableDirectionalLightIndex(_MIN_ const CDyDirectionalLight&);

  ///
  /// @brief  Unbind valid directional light component that which is being binded to system.
  /// @param  component Binded directional light component which has valid index.
  /// @return If successfully unbinded, return true or false.
  ///
  MDY_NODISCARD EDySuccess pUnbindDirectionalLight(_MIN_ const CDyDirectionalLight& component);

  ///
  /// @brief  Update UBO directional light container value to gpu memory for lighting.
  /// @param  index     Valid index value which got by using pGetAvaiableDirectionalLightIndex().
  /// @param  container Uniform buffer object C++ container instance.
  /// @return If process is succeeded, return true but false when index is oob or container value is not valid.
  ///
  MDY_NODISCARD EDySuccess pUpdateDirectionalLightValueToGpu(
      _MIN_ const TI32 index,
      _MIN_ const DDyUboDirectionalLight& container);

  ///
  /// @brief  Check whether or not directional light shadow is available to instance.
  /// @param  Instance for type trailing
  /// @return If available, just return true or false.
  ///
  MDY_NODISCARD bool pfIsAvailableDirectionalLightShadow(_MIN_ const CDyDirectionalLight&);

  ///
  /// @brief
  /// @param
  /// @return
  ///
  MDY_NODISCARD EDySuccess pfUpdateDirectionalLightShadowToGpu(_MIN_ const CDyDirectionalLight& component);

  ///
  /// @brief
  /// @param
  /// @return
  ///
  MDY_NODISCARD EDySuccess pfUnbindDirectionalLightShadowToGpu(_MIN_ const CDyDirectionalLight& component);

  //!
  //! Members
  //!

  ///
  std::unique_ptr<FDyBasicRenderer>               mBasicOpaqueRenderer  = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyLevelCascadeShadowRenderer>  mCSMRenderer          = MDY_INITIALIZE_NULL; 
  std::unique_ptr<FDyPostEffectSsao>              mTempSsaoObject       = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyDeferredRenderingMesh>       mSceneFinalRenderer   = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyFinalScreenDisplayRenderer>  mFinalDisplayRenderer = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyUIBasicRenderer>             mUiBasicRenderer      = MDY_INITIALIZE_NULL;

  /// Use basic renderer
  std::vector<NotNull<CDyModelRenderer*>>         mOpaqueDrawCallList   = {};

#if defined(MDY_FLAG_IN_EDITOR)
  std::unique_ptr<FDyGrid>                    mGridEffect           = nullptr;
#endif /// MDY_FLAG_IN_EDITOR

  friend class CDyDirectionalLight;
  friend class FDyDeferredRenderingMesh;
  friend class FDyPostEffectSsao;
  friend class editor::FDyMainViewport;
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MANAGER_H