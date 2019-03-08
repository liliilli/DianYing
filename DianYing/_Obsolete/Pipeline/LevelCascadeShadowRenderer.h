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
#include <Dy/Management/Type/Render/DDyModelHandler.h>
#include <Dy/Core/Rendering/Interface/IDyRenderer.h>

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
class FDyLevelCascadeShadowRenderer final : public IDyRenderer
{
public:
  FDyLevelCascadeShadowRenderer();
  ~FDyLevelCascadeShadowRenderer() = default;

  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen(
      DDyModelHandler::DActorInfo& iRefRenderer,
      FDyMeshResource& iRefMesh, 
      FDyMaterialResource& iRefMaterial);

  /// @brief Clear properties of given framebuffer.
  void Clear() override final;

  bool IsReady() const noexcept override final;

  /// @brief Try setup rendering, if failed, return DY_FAILURE.
  EDySuccess TryPushRenderingSetting() override final;
  EDySuccess TryPopRenderingSetting() override final;

private:
  /// @brief Update 
  void PreRender();

  TDyResourceBinderShader      mDirLightShaderResource { "dyBtGlslRenderCsmLightNormal" };
  TDyResourceBinderFrameBuffer mBinderFrameBuffer      { "dyBtFbCSM" };

  static std::array<TF32, 2>   sViewportDims;
  std::ptrdiff_t               mAddrMainDirectionalShadow = 0;

  DDyMatrix4x4 mViewMatrix;
  DDyMatrix4x4 mProjMatrix;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCASCADESHADOWRENDERER_H