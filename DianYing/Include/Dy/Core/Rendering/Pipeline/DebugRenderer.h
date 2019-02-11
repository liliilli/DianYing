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

/// @class FDyDebugRenderer
/// @brief Debug renderer.
class FDyDebugRenderer final : public IDyRenderer
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyDebugRenderer);
  FDyDebugRenderer() = default;
  ~FDyDebugRenderer() = default;

  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen(_MIN_ CDyPhysicsCollider& iRefCollider, _MIN_ const DDyMatrix4x4& iTransformMatrix);

  /// @brief Clear properties of given framebuffer.
  void Clear() override final;

  /// @brief
  bool IsReady() const noexcept override final;

  /// @brief
  EDySuccess TrySetupRendering() override final;

private:
  TDyIResourceBinderShader      mBinderShader     {"dyBtShDebugLine"};
  TDyIResourceBinderFrameBuffer mBinderFrameBuffer{"dyBtFbDebug"};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_PIPELINE_DebugRenderer_H