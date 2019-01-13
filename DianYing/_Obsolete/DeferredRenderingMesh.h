#ifndef GUARD_DY_RENDERING_MESH_H
#define GUARD_DY_RENDERING_MESH_H
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

//!
//! Forward declaration
//!

namespace dy
{
struct  DDyUboDirectionalLight;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FDyDeferredRenderingMesh final
{
public:
  FDyDeferredRenderingMesh();
  ~FDyDeferredRenderingMesh();

  /// @brief Rendering deferred contexts to default framebuffer.
  void RenderScreen();

  /// @brief Try setup rendering, if failed, return DY_FAILURE.
  MDY_NODISCARD EDySuccess TrySetupRendering();

  /// @brief Check rendering phase is ready.
  MDY_NODISCARD bool IsReady() const noexcept;

  /// @brief Clear properties of given framebuffer.
  void Clear();

private:
  void pInitializeShaderSetting();
  void pInitializeUboBuffers();

  TDyIResourceBinderShader      mBinderShader     { "dyBtShaderGlDeferred" };
  TDyIResourceBinderModel       mBinderTriangle   { "dyBtModelScrProjTri" };
  TDyIResourceBinderFrameBuffer mBinderFrameBuffer{ "dyBtFbScrFin" };
  TDyIResourceBinderAttachment  mBinderAttUnlit   { "dyBtUnlit" };
  TDyIResourceBinderAttachment  mBinderAttNormal  { "dyBtNormal" };
  TDyIResourceBinderAttachment  mBinderAttSpecular{ "dyBtSpecular" };
  TDyIResourceBinderAttachment  mBinderAttPosition{ "dyBtModelPosition" };
  TDyIResourceBinderAttachment  mBinderAttShadow  { "dyBtAtDirBscShadow" };

  std::ptrdiff_t mAddrMainLight = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MESH_H