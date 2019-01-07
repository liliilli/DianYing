#ifndef GUARD_DY_CORE_RENDERING_BASICSHADOW_H
#define GUARD_DY_CORE_RENDERING_BASICSHADOW_H
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

#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class   CDyModelRenderer;
class   CDyDirectionalLight;
} /// ::dy namespace

//!
//! Implemenetation
//!

namespace dy
{
///
/// @class FDyBasicShadow
/// @brief Basic shadow effect dkdk
///
class FDyBasicShadow final
{
public:
  FDyBasicShadow();
  ~FDyBasicShadow();

  ///
  /// @brief Rendering deferred contexts to default framebuffer.
  /// @param renderer
  ///
  void RenderScreen(const CDyModelRenderer& renderer);

  ///
  /// @brief Clear properties of given framebuffer.
  ///
  void Clear();

  ///
  /// @brief  Check directional light is being used.
  /// @return Flag for checking directional light shadow mapping is being used.
  ///
  MDY_NODISCARD bool IsAvailableDirectionalLightShadow() const noexcept;

  ///
  /// @brief
  /// @param  container
  /// @return
  ///
  MDY_NODISCARD EDySuccess UpdateDirectionalLightShadowToGpu(_MIN_ const CDyDirectionalLight& container);

  ///
  /// @brief
  /// @param  container
  /// @return
  ///
  MDY_NODISCARD EDySuccess UnbindDirectionalLightShadowToGpu(_MIN_ const CDyDirectionalLight& container);

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD TU32 GetDirectionalLightDepthTextureId() const noexcept;

  ///
  /// @brief
  /// @return
  ///
  FORCEINLINE MDY_NODISCARD TU32 GetShadowFrameBufferId() const noexcept
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
    return MDY_INITIALIZE_DEFUINT;
  }

private:
  /// Flag for checking directional light shadow mapping is being used.
  bool                mIsUsingShadowDirectionalLight = false;

  TDyIResourceBinderShader       mDirLightShaderResource { "dyBtShaderGlRenderBscShadow" };
  TDyIResourceBinderFrameBuffer  mBinderFrameBuffer      { "dyBtFbShadow" };
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_BASICSHADOW_H