#ifndef GUARD_DY_BUILTIN_RENDERITEM_FBTLEVELINTEGRATION_H
#define GUARD_DY_BUILTIN_RENDERITEM_FBTLEVELINTEGRATION_H
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

namespace dy
{

/// @class FBtRenderItemLevelIntegeration
/// @brief Instantiate level integration default item
class FBtRenderItemLevelIntegeration final : public ARenderItem
{
  MDY_REGISTER_RESOURCE_RENDER_ITEM(FBtRenderItemLevelIntegeration, "dyBtLevelIntegration");
public:
  EDySuccess OnPreRenderCheckCondition() override final;

  void OnFailedCheckCondition() override final {};

  void OnSetupRenderingSetting() override final;
  void pSetupOpaqueCSMIntegration();
  void pSetupTranslucentOITIntegration();

  void OnRender() override final;

  void OnReleaseRenderingSetting() override final;

  void OnPostRender() override final {};

private:
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
  TDyResourceBinderAttachment  mBinderAttEmissive{ "dyBtEmissiveResult" };

  TDyResourceBinderFrameBuffer mBinderFbTranslucent { "dyBtFbIntgLevelTrans" };
  TDyResourceBinderShader      mBinderTransShader{ "dyBtShOITIntegration" };
  TDyResourceBinderAttachment  mBinderAttOpaque  { "dyBtAtOpaqueOutput" };
  TDyResourceBinderAttachment  mBinderAttOITColor{ "dyBtAtWBOITColor" };
  TDyResourceBinderAttachment  mBinderAttOITWeigh{ "dyBtAtWBOITWeight" };

  std::ptrdiff_t mAddrMainLight = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_RENDERITEM_FBTLEVELINTEGRATION_H