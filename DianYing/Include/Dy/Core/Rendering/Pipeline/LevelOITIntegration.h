#ifndef GUARD_DY_CORE_RENDERING_PIPELINE_LEVELOITINTEGRATION_H
#define GUARD_DY_CORE_RENDERING_PIPELINE_LEVELOITINTEGRATION_H
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

namespace dy
{

class FDyLevelOITIntegration final : public IDyRenderer
{
public:
  FDyLevelOITIntegration() = default;
  virtual ~FDyLevelOITIntegration() = default;

  /// @brief Check rendering phase is ready.
  MDY_NODISCARD bool IsReady() const noexcept override final;
  /// @brief Try setup rendering, if failed, return DY_FAILURE.
  MDY_NODISCARD EDySuccess TrySetupRendering() override final;
  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen();
  /// @brief Clear properties of given framebuffer.
  void Clear() override final;

private:
  TDyIResourceBinderFrameBuffer mBinderFrameBuffer{ "dyBtFbWBOIT" };
  TDyIResourceBinderShader      mBinderShader     { "dyBtShOITIntegration" };
  TDyIResourceBinderMesh        mBinderTriangle   { "dyBtMsScrOITTriangle" };
  TDyIResourceBinderAttachment  mBinderAttOITColor{ "dyBtAtWBOITColor" };
  TDyIResourceBinderAttachment  mBinderAttOITWeigh{ "dyBtAtWBOITWeight" };
  TDyIResourceBinderAttachment  mBinderAttOpaqScre{ "dyBtAtScrFin_Output" };
};


} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_LEVELOITINTEGRATION_H