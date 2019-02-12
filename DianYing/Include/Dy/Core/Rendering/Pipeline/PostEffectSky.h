#ifndef GUARD_DY_CORE_RENDERING_PIPELINE_PostEffectSky_H
#define GUARD_DY_CORE_RENDERING_PIPELINE_PostEffectSky_H
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
#include <Dy/Core/Rendering/Interface/IDyRenderer.h>

namespace dy
{

/// @class FDyPostEffectSky
/// @brief Posteffect for rendering sky. This pipeline compare skybox uvw & uv, 
/// and unlit attachment texture for discarding redundant sky pixel.
class FDyPostEffectSky final : public IDyRenderer
{
public:
  FDyPostEffectSky() = default;
  virtual ~FDyPostEffectSky() = default;

  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen();

  bool IsReady() const noexcept override final;

  EDySuccess TrySetupRendering() override final;

  void Clear() override final;

private:
  // Output
  TDyIResourceBinderFrameBuffer mBinderFbSkyRend    { "dyBtFbSkybox" };
  // Shader
  TDyIResourceBinderShader      mBinderShdSkybox    { "dyBtShGlSkyboxDefault" };
  // Mesh for skybox
  TDyIResourceBinderMesh        mBinderMeshSkybox   { "dyBtMsBox" };
  // uTexture1 (Source)
  TDyIResourceBinderAttachment  mBinderAttUnlit     { "dyBtUnlit" };
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_PostEffectSky_H