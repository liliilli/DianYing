#ifndef GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCASCADESHADOWRENDERER_H
#define GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCASCADESHADOWRENDERER_H
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

#include <Dy/Core/Resource/Type/TDyResourceBinder.h>
#include <Dy/Builtin/Constant/CSM.h>
#include <Dy/Helper/Type/Area2D.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyCamera;
class CDyModelRenderer;
}

//!
//! Implementation
//!

namespace dy
{

/// @class FDyLevelCascadeShadowRenderer
/// @brief Cascade shadow mapping effect renderer.
class FDyLevelCascadeShadowRenderer final
{
public:
  FDyLevelCascadeShadowRenderer();
  ~FDyLevelCascadeShadowRenderer() = default;

  /// @brief Rendering deferred contexts to default framebuffer.
  /// @param renderer
  void RenderScreen(_MIN_ CDyModelRenderer& renderer);

  /// @TODO TEMPORARY
  /// @brief Update 
  void PreRender();

  /// @brief Setup indexed viewports of light shadow map segments for writing.
  void SetupViewport();

  /// @brief Clear properties of given framebuffer.
  void Clear();

  /// @brief Try setup rendering, if failed, return DY_FAILURE.
  MDY_NODISCARD EDySuccess TrySetupRendering();

private:
  /// @brief Update segments far planes.
  void UpdateSegmentFarPlanes(_MIN_ const CDyCamera& iPtrCamera);

  TDyIResourceBinderShader        mDirLightShaderResource { "dyBtGlslRenderCsmLightNormal" };
  TDyIResourceBinderFrameBuffer   mBinderFrameBuffer      { "dyBtFbCSM" };

  static std::array<TF32, 2>      sViewportDims;
  std::ptrdiff_t                  mAddrMainDirectionalShadow = 0;

  std::array<TF32, kCSMSegment>   mFarPlanes;
  std::array<TF32, kCSMSegment>   mNormalizedFarPlanes;
  DDyMatrix4x4                    mOldProjectionMatrix; 

  //std::array<DDyArea2D, kCSMSegment>    mLightViewports;
  //std::array<DDyMatrix4x4, kCSMSegment> mLightSegmentVPSBMatrices;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCASCADESHADOWRENDERER_H