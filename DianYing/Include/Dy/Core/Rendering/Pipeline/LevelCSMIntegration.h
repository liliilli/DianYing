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
  ///
  MDY_NODISCARD EDySuccess pSetupOpaqueCSMIntegration();
  ///
  MDY_NODISCARD EDySuccess pSetupTranslucentOITIntegration();

  ///
  void pRenderCSMIntegration();
  ///
  void pRenderOITIntegration();

  TDyLResourceBinderModel       mBinderTriangle   { "dyBtModelScrProjTri" };

  TDyLResourceBinderFrameBuffer mBinderFrameBuffer  { "dyBtFbScrFin" };
  TDyLResourceBinderShader      mBinderOpaqueShader { "dyBtGlslRenderCsmIntegration" };
  TDyLResourceBinderAttachment  mBinderAttUnlit   { "dyBtUnlit" };
  TDyLResourceBinderAttachment  mBinderAttNormal  { "dyBtNormal" };
  TDyLResourceBinderAttachment  mBinderAttSpecular{ "dyBtSpecular" };
  TDyLResourceBinderAttachment  mBinderAttPosition{ "dyBtModelPosition" };
  TDyLResourceBinderAttachment  mBinderAttShadow  { "dyBtAtCSMLight" };
  TDyLResourceBinderAttachment  mBinderAttZValue  { "dyBtDefZValue" };
  TDyLResourceBinderAttachment  mBinderAttSSAO    { "dyBtAtSSAOOutputFinal" };
  TDyLResourceBinderAttachment  mBinderAttSky     { "dyBtAtSkyOutput" };

  TDyLResourceBinderFrameBuffer mBinderFbTranslucent { "dyBtFbIntgLevelTrans" };
  TDyLResourceBinderShader      mBinderTransShader{ "dyBtShOITIntegration" };
  TDyLResourceBinderAttachment  mBinderAttOpaque  { "dyBtAtOpaqueOutput" };
  TDyLResourceBinderAttachment  mBinderAttOITColor{ "dyBtAtWBOITColor" };
  TDyLResourceBinderAttachment  mBinderAttOITWeigh{ "dyBtAtWBOITWeight" };

  std::ptrdiff_t mAddrMainLight = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCSMINTEGRATION_H