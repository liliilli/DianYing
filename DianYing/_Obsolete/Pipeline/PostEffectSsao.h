#ifndef GUARD_DY_COMPONENT_OBJECT_POSTEFFECT_SSAO_H
#define GUARD_DY_COMPONENT_OBJECT_POSTEFFECT_SSAO_H
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

class FDyPostEffectSsao final : public IDyRenderer
{
public:
  FDyPostEffectSsao();
  virtual ~FDyPostEffectSsao() = default;

  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen();

  bool IsReady() const noexcept override final;

  EDySuccess TryPushRenderingSetting() override final;

  void Clear() override final;

  EDySuccess TryPopRenderingSetting() override final { return DY_SUCCESS; }

private:

  TDyResourceBinderModel       mBinderTriangle     { "dyBtModelScrProjTri" };

  TDyResourceBinderFrameBuffer mBinderFbSSAO       { "dyBtFbSSAO" };
  TDyResourceBinderShader      mBinderShSSAO       { "dyBtShSSAO" };
  TDyResourceBinderAttachment  mBinderAttWorldPos  { "dyBtModelPosition" };
  TDyResourceBinderAttachment  mBinderAttWorldNorm { "dyBtNormal" };
  TDyResourceBinderTexture     mBinderTexNoise     { "dyBtTexSSAONoiseMap" };

  TDyResourceBinderFrameBuffer mBinderFbSSAOBlur   { "dyBtFbSSAOBlur" };
  TDyResourceBinderShader      mBinderShSSAOBlur   { "dyBtPostEffectSsaoBlur" };
  TDyResourceBinderAttachment  mBinderAttSSAOOpt   { "dyBtAtSSAOOutput" };

  std::vector<DDyVector3> mRayContainer;
  bool mIsRayInserted = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_OBJECT_POSTEFFECT_SSAO_H