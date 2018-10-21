#ifndef GUARD_DY_RENDERING_MESH_H
#define GUARD_DY_RENDERING_MESH_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Core/Component/MeshRenderer.h>
#include <Dy/Core/Component/Resource/ShaderResource.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DDyUboDirectionalLight;
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
  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD EDySuccess pInitializeGeometries();

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD EDySuccess pInitializeShaderSetting();

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD EDySuccess pInitializeUboBuffers();

  inline static constexpr TI32 sDirectionalLightCount = 5;

  GLuint              mVao            = MDY_INITIALIZE_DEFUINT;
  GLuint              mVbo            = MDY_INITIALIZE_DEFUINT;
  GLuint              mDirLight       = MDY_INITIALIZE_DEFUINT;
  CDyShaderResource*  mShaderPtr      = MDY_INITIALIZE_NULL;

  std::queue<TI32>    mAvailableList  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MESH_H