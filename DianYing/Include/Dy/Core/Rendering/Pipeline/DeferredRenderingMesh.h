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

#include <queue>
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

  ///
  /// @brief Rendering deferred contexts to default framebuffer.
  ///
  void RenderScreen();

  ///
  /// @brief Clear properties of given framebuffer.
  ///
  void Clear();

  ///
  /// @brief  Issue available directional light index if not available, just return nullopt;
  /// @return Directional light index value or nullopt (no value).
  ///
  MDY_NODISCARD std::optional<TI32> GetAvailableDirectionalLightIndex() noexcept;

  ///
  /// @brief  Update FUCKFUCKFUCKFCUFKCUKFUCK
  /// @param  index
  /// @param  container
  /// @return
  ///
  MDY_NODISCARD EDySuccess UpdateDirectionalLightValueToGpu(
      _MIN_ const TI32 index,
      _MIN_ const DDyUboDirectionalLight& container);

  ///
  /// @brief
  /// @param  index
  /// @return
  ///
  MDY_NODISCARD EDySuccess UnbindDirectionalLight(_MIN_ const TI32 index);

private:
  void pInitializeShaderSetting();
  void pInitializeUboBuffers();

  inline static constexpr TI32 sDirectionalLightCount = 5;

  TDyIResourceBinderShader      mBinderShader     { "dyBtShaderGlDeferred" };
  TDyIResourceBinderModel       mBinderTriangle   { "dyBtModelScrProjTri" };
  TDyIResourceBinderFrameBuffer mBinderFrameBuffer{ "dyBtFbScrFin" };
  TDyIResourceBinderAttachment  mBinderAttUnlit   { "dyBtUnlit" };
  TDyIResourceBinderAttachment  mBinderAttNormal  { "dyBtNormal" };
  TDyIResourceBinderAttachment  mBinderAttSpecular{ "dyBtSpecular" };
  TDyIResourceBinderAttachment  mBinderAttPosition{ "dyBtModelPosition" };
  TDyIResourceBinderAttachment  mBinderAttShadow  { "dyBtAtDirBscShadow" };

  std::queue<TI32>    mDirLightAvailableList  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MESH_H