#ifndef GUARD_DY_BUILTIN_RENDERITEM_FBTPPSKYBOX_H
#define GUARD_DY_BUILTIN_RENDERITEM_FBTPPSKYBOX_H
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
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Meta/Information/MetaInfoRenderItem.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

namespace dy
{

/// @class FBtRenderItemSkybox
/// @brief Instantiate post-processing Skybox (using environment cube map) default item
class FBtRenderItemSkybox final : public ARenderItem
{
  MDY_REGISTER_RESOURCE_RENDER_ITEM(FBtRenderItemSkybox, "dyBtPpSkybox");
public:
  EDySuccess OnPreRenderCheckCondition() override final;
  bool AreResourcesValid();

  void OnFailedCheckCondition() override final;

  void OnSetupRenderingSetting() override final;

  void OnRender() override final;

  void OnReleaseRenderingSetting() override final;

  void OnPostRender() override final;

private:
  // Output
  TDyResourceBinderFrameBuffer mBinderFbSkyRend    { "dyBtFbSkybox" };
  // Shader
  TDyResourceBinderShader      mBinderShdSkybox    { "dyBtShGlSkyboxDefault" };
  // Mesh for skybox
  TDyResourceBinderMesh        mBinderMeshSkybox   { "dyBtMsBox" };
  // uTexture1 (Source)
  TDyResourceBinderAttachment  mBinderAttUnlit     { "dyBtUnlit" };
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_RENDERITEM_FBTPPSKYBOX_H