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
#include <Dy/Core/Rendering/Interface/IDyRenderer.h>

namespace dy
{

class FDyLevelCSMIntergration final : public IDyRenderer
{
public:
  FDyLevelCSMIntergration();
  ~FDyLevelCSMIntergration();

  /// @brief Check rendering phase is ready.
  MDY_NODISCARD bool IsReady() const noexcept override final;
  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen();
  /// @brief Clear properties of given framebuffer.
  void Clear() override final;

  /// @brief Try setup rendering, if failed, return DY_FAILURE.
  EDySuccess TryPushRenderingSetting() override final;
  /// @brief
  EDySuccess TryPopRenderingSetting() override final;

private:
  MDY_NODISCARD EDySuccess pSetupOpaqueCSMIntegration();
  MDY_NODISCARD EDySuccess pSetupTranslucentOITIntegration();

  TDyResourceBinderModel       mBinderTriangle   { "dyBtModelScrProjTri" };

  TDyResourceBinderFrameBuffer mBinderFrameBuffer  { "dyBtFbScrFin" };
  TDyResourceBinderShader      mBinderOpaqueShader { "dyBtGlslRenderCsmIntegration" };
  TDyResourceBinderAttachment  mBinderAttUnlit   { "dyBtUnlit" };
  TDyResourceBinderAttachment  mBinderAttNormal  { "dyBtNormal" };
  TDyResourceBinderAttachment  mBinderAttSpecular{ "dyBtSpecular" };
  TDyResourceBinderAttachment  mBinderAttPosition{ "dyBtModelPosition" };
  TDyResourceBinderAttachment  mBinderAttShadow  { "dyBtAtCSMLight" };
  TDyResourceBinderAttachment  mBinderAttZValue  { "dyBtDefZValue" };
  TDyResourceBinderAttachment  mBinderAttSSAO    { "dyBtAtSSAOOutputFinal" };
  TDyResourceBinderAttachment  mBinderAttSky     { "dyBtAtSkyOutput" };

  TDyResourceBinderFrameBuffer mBinderFbTranslucent { "dyBtFbIntgLevelTrans" };
  TDyResourceBinderShader      mBinderTransShader{ "dyBtShOITIntegration" };
  TDyResourceBinderAttachment  mBinderAttOpaque  { "dyBtAtOpaqueOutput" };
  TDyResourceBinderAttachment  mBinderAttOITColor{ "dyBtAtWBOITColor" };
  TDyResourceBinderAttachment  mBinderAttOITWeigh{ "dyBtAtWBOITWeight" };

  std::ptrdiff_t mAddrMainLight = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_LEVELCSMINTEGRATION_H