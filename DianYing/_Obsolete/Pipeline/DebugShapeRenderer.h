#ifndef GUARD_DY_CORE_RENDERING_PIPELINE_DebugRenderer_H
#define GUARD_DY_CORE_RENDERING_PIPELINE_DebugRenderer_H
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

//!
//! Forward declaration
//!

namespace dy
{
class CDyPhysicsCollider;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FDyDebugShapeRenderer
/// @brief Debug renderer.
class FDyDebugShapeRenderer final : public IDyRenderer
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyDebugShapeRenderer);
  FDyDebugShapeRenderer() = default;
  ~FDyDebugShapeRenderer() = default;

  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen(_MIN_ CDyPhysicsCollider& iRefCollider, _MIN_ const DDyMatrix4x4& iTransformMatrix);

  /// @brief Clear properties of given framebuffer.
  void Clear() override final;

  /// @brief
  bool IsReady() const noexcept override final;

  EDySuccess TryPushRenderingSetting() override final;
  EDySuccess TryPopRenderingSetting() override final;

private:
  TDyResourceBinderShader      mBinderShader     {"dyBtShDebugLine"};
  TDyResourceBinderFrameBuffer mBinderFrameBuffer{"dyBtFbDebug"};
  TDyResourceBinderMesh        mBinderSphere     {"dyBtMsDebugSphere"};
  TDyResourceBinderMesh        mBinderBox        {"dyBtMsDebugBox"};
  TDyResourceBinderMesh        mBinderCapsule    {"dyBtMsDebugCapsule"};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_DebugRenderer_H