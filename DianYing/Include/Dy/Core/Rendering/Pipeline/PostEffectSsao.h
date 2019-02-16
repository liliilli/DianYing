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

  EDySuccess TrySetupRendering() override final;

  void Clear() override final;

private:

  TDyLResourceBinderModel       mBinderTriangle     { "dyBtModelScrProjTri" };

  TDyLResourceBinderFrameBuffer mBinderFbSSAO       { "dyBtFbSSAO" };
  TDyLResourceBinderShader      mBinderShSSAO       { "dyBtShSSAO" };
  TDyLResourceBinderAttachment  mBinderAttWorldPos  { "dyBtModelPosition" };
  TDyLResourceBinderAttachment  mBinderAttWorldNorm { "dyBtNormal" };
  TDyLResourceBinderTexture     mBinderTexNoise     { "dyBtTexSSAONoiseMap" };

  TDyLResourceBinderFrameBuffer mBinderFbSSAOBlur   { "dyBtFbSSAOBlur" };
  TDyLResourceBinderShader      mBinderShSSAOBlur   { "dyBtPostEffectSsaoBlur" };
  TDyLResourceBinderAttachment  mBinderAttSSAOOpt   { "dyBtAtSSAOOutput" };

  std::vector<DDyVector3> mRayContainer;
  bool mIsRayInserted = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_OBJECT_POSTEFFECT_SSAO_H