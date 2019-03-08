#ifndef GUARD_DY_BUILTIN_RENDERITEM_FBTPPCSMSHADOW_H
#define GUARD_DY_BUILTIN_RENDERITEM_FBTPPCSMSHADOW_H
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

/// @class FBtRenderItemCsmShadow
/// @brief Instantiate post-processing csm shadow default item
class FBtRenderItemCsmShadow final : public ARenderItem
{
  MDY_REGISTER_RESOURCE_RENDER_ITEM(FBtRenderItemCsmShadow, "dyBtPpCsmShadow");
public:
  FBtRenderItemCsmShadow();

  EDySuccess OnPreRenderCheckCondition() override final;

  void OnFailedCheckCondition() override final;

  void OnSetupRenderingSetting() override final;

  void OnRender() override final;
  void RenderObject(
    DDyModelHandler::DActorInfo& iRefRenderer,
    FDyMeshResource& iRefMesh, 
    FDyMaterialResource& iRefMaterial);

  void OnReleaseRenderingSetting() override final;

  void OnPostRender() override final;

private:
  TDyResourceBinderShader       mDirLightShaderResource { "dyBtGlslRenderCsmLightNormal" };
  TDyResourceBinderFrameBuffer  mBinderFrameBuffer      { "dyBtFbCSM" };

  std::array<TF32, 2>           sViewportDims;
  std::ptrdiff_t                mAddrMainDirectionalShadow = 0;

  DDyMatrix4x4 mViewMatrix;
  DDyMatrix4x4 mProjMatrix;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_RENDERITEM_FBTPPCSMSHADOW_H