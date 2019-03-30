#ifndef GUARD_DY_BUILTIN_RENDERITEM_FBTDEBUGPICKING_H
#define GUARD_DY_BUILTIN_RENDERITEM_FBTDEBUGPICKING_H
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
#include <Dy/Core/Resource/Type/TResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class CModelRenderer;
class CBasePhysicsCollider;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FBtRenderDebugPicking
/// @brief Instantiate default debug picking render item
class FBtRenderDebugPicking final : public ARenderItem
{
  MDY_REGISTER_RESOURCE_RENDER_ITEM(
    FBtRenderDebugPicking, 
    "dyBtDebugPicking");
public:
  EDySuccess OnPreRenderCheckCondition() override final;
  bool AreResourcesValid();

  void OnFailedCheckCondition() override final {};

  void OnSetupRenderingSetting() override final;

  void OnRender() override final;
  void RenderObject(CBasePhysicsCollider& iRefCollider, const DMat4& iTransformMatrix);

  void OnReleaseRenderingSetting() override final;

  void OnPostRender() override final {};

private:
  TDyResourceBinderFrameBuffer mBinderFrameBuffer{"dyBtFbDebug"};
  TDyResourceBinderShader      mBinderShader     {"dyBtShDebugLine"};
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_RENDERITEM_FBTDEBUGPICKING_H