#ifndef GUARD_DY_BUILTIN_RENDERITEM_FBTPPSsao_H
#define GUARD_DY_BUILTIN_RENDERITEM_FBTPPSsao_H
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

#include <Dy/Builtin/Abstract/ARenderItem.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Meta/Information/MetaInfoRenderItem.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>
#include <Dy/Management/Type/Render/DDyModelHandler.h>

namespace dy
{

/// @class FBtRenderItemSsao
/// @brief Instantiate post-processing Ssao (Screen-space ambient occlusion) default item
class FBtRenderItemSsao final : public ARenderItem
{
  MDY_REGISTER_RESOURCE_RENDER_ITEM(FBtRenderItemSsao, "dyBtPpSsao");
public:
  FBtRenderItemSsao();

  EDySuccess OnPreRenderCheckCondition() override final;
  bool AreResourcesValid();

  void OnFailedCheckCondition() override final;

  void OnSetupRenderingSetting() override final;

  void OnRender() override final;

  void OnReleaseRenderingSetting() override final;

  void OnPostRender() override final;

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

  std::vector<DVector3> mRayContainer;
  bool mIsRayInserted = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_RENDERITEM_FBTPPSsao_H