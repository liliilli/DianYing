#ifndef GUARD_DY_CORE_RENDERING_BASICRENDERER_H
#define GUARD_DY_CORE_RENDERING_BASICRENDERER_H
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

#include <vector>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class   CDyCamera;
class   CDyModelRenderer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyBasicRenderer
/// @brief
///
class FDyBasicRenderer final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyBasicRenderer);
  FDyBasicRenderer();
  ~FDyBasicRenderer();

  /// @brief Rendering deferred contexts to default framebuffer.
  /// @param rendererList
  void RenderScreen(_MIN_ const std::vector<NotNull<CDyModelRenderer*>>& rendererList);

  /// @brief Clear properties of given framebuffer.
  void Clear();

private:
  /// @brief
  void pRenderScreen(_MIN_ const CDyModelRenderer& renderer, _MIN_ const CDyCamera& validCamera) noexcept;

  TDyIResourceBinderFrameBuffer mBinderFrameBuffer{"dyBtBasicRender"};

  friend class FDyDeferredRenderingMesh;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_BASICRENDERER_H