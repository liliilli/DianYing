#ifndef GUARD_DY_CORE_RENDERING_PIPELINE_LEVELOITRENDERER_H
#define GUARD_DY_CORE_RENDERING_PIPELINE_LEVELOITRENDERER_H
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

#include <Dy/Core/Rendering/Interface/IDyRenderer.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyModelRenderer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FDyLevelOITRenderer final : public IDyRenderer
{
public:
  FDyLevelOITRenderer() = default;
  virtual ~FDyLevelOITRenderer() = default;

  /// @brief Check rendering phase is ready.
  MDY_NODISCARD bool IsReady() const noexcept override final;

  /// @brief Try setup rendering, if failed, return DY_FAILURE.
  MDY_NODISCARD EDySuccess TryPushRenderingSetting() override final;

  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen(
    _MIN_ CDyModelRenderer& iRefRenderer,
    _MIN_ FDyMeshResource& iRefMesh, 
    _MIN_ FDyMaterialResource& iRefMaterial);

  EDySuccess TryPopRenderingSetting() override;

  /// @brief Clear properties of given framebuffer.
  void Clear() override final;

private:
  TDyResourceBinderFrameBuffer mBinderFrameBuffer{ "dyBtFbWBOIT" };
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_LEVELOITRENDERER_H