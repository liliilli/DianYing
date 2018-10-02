#ifndef GUARD_DY_RENDERING_MANAGER_H
#define GUARD_DY_RENDERING_MANAGER_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Core/Component/Object/DeferredRenderingMesh.h>
#include <Dy/Core/Component/Object/PostEffectSsao.h>
#include <Dy/Core/Component/Object/Grid.h>
#include "Dy/Core/Rendering/BasicShadow.h"

namespace dy::editor
{
class FDyMainViewport;
}

namespace dy
{

///
/// @class MDyRendering
/// @brief Rendering maanger
///
class MDyRendering final : public ISingleton<MDyRendering>
{
  MDY_SINGLETON_PROPERTIES(MDyRendering);
  MDY_SINGLETON_DERIVED(MDyRendering);
public:
  ///
  /// @brief
  ///
  void PushDrawCallTask(CDyMeshRenderer& rendererInstance);

  ///
  /// @brief
  ///
  void RenderDrawCallQueue();

private:
  ///
  /// @brief Create geometry buffers (aka G-buffer) for deferred rendering.
  ///
  void pCreateDeferredGeometryBuffers() noexcept;

  ///
  /// @brief Release geometry buffers (aka G-buffer) for deferred rendering.
  ///
  void pReleaseGeometryBuffers() noexcept;

  ///
  /// @brief Reset all of rendering framebuffers related to rendering of scene for new frame rendering.
  ///
  void pResetRenderingFramebufferInstances() noexcept;

  ///
  void pRenderDeferredFrameBufferWith(const CDyMeshRenderer& renderer) noexcept;

  ///
  void pRenderShadowFrameBufferWith(const CDyMeshRenderer& renderer) noexcept;

  TU32                mDeferredFrameBufferId  = MDY_INITIALIZE_DEFUINT;
  std::array<TU32, 4> mAttachmentBuffers      = {};
  const TI32          mAttachmentBuffersCount = static_cast<TI32>(mAttachmentBuffers.size());

  std::unique_ptr<FDyDeferredRenderingMesh>   mFinalRenderingMesh   = nullptr;
  std::queue<CDyMeshRenderer*>                mDrawCallQueue        = {};

  bool                                        mTempIsEnabledSsao    = true;
  std::unique_ptr<FDyPostEffectSsao>          mTempSsaoObject       = nullptr;

  bool                                        mTempIsEnabledShadow  = true;
  std::unique_ptr<FDyBasicShadow>             mTempShadowObject     = nullptr;

#if defined(MDY_FLAG_IN_EDITOR)
  std::unique_ptr<FDyGrid>                    mGridEffect           = nullptr;
#endif /// MDY_FLAG_IN_EDITOR

  friend class FDyDeferredRenderingMesh;
  friend class editor::FDyMainViewport;
  friend class FDyPostEffectSsao;
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MANAGER_H