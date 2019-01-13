#ifndef GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCSMINTEGRATION_H
#define GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCSMINTEGRATION_H
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

namespace dy
{

class FDyLevelCSMIntergration final
{
public:
  FDyLevelCSMIntergration();
  ~FDyLevelCSMIntergration();

  /// @brief Check rendering phase is ready.
  MDY_NODISCARD bool IsReady() const noexcept;
  /// @brief Try setup rendering, if failed, return DY_FAILURE.
  MDY_NODISCARD EDySuccess TrySetupRendering();
  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen();
  /// @brief Clear properties of given framebuffer.
  void Clear();

private:
  void pInitializeShaderSetting();

  TDyIResourceBinderFrameBuffer mBinderFrameBuffer{ "dyBtFbScrFin" };
  TDyIResourceBinderShader      mBinderShader     { "dyBtGlslRenderCsmIntegration" };
  TDyIResourceBinderModel       mBinderTriangle   { "dyBtModelScrProjTri" };
  TDyIResourceBinderAttachment  mBinderAttUnlit   { "dyBtUnlit" };
  TDyIResourceBinderAttachment  mBinderAttNormal  { "dyBtNormal" };
  TDyIResourceBinderAttachment  mBinderAttSpecular{ "dyBtSpecular" };
  TDyIResourceBinderAttachment  mBinderAttPosition{ "dyBtModelPosition" };
  TDyIResourceBinderAttachment  mBinderAttShadow  { "dyBtAtCSMLight" };
  TDyIResourceBinderAttachment  mBinderAttZValue  { "dyBtDefZValue" };

  std::ptrdiff_t mAddrMainLight = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCSMINTEGRATION_H