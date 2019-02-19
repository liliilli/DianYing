#ifndef GUARD_DY_CORE_RENDERING_UIBASICRENDERER_H
#define GUARD_DY_CORE_RENDERING_UIBASICRENDERER_H
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
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyCamera;
class CDyModelRenderer;
class FDyUiObject;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FDyUIBasicRenderer final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyUIBasicRenderer);
  FDyUIBasicRenderer();
  ~FDyUIBasicRenderer();

  /// @brief Rendering deferred contexts to framebuffer.
  void RenderScreen(_MIN_ std::vector<NotNull<FDyUiObject*>>& uiRenderList);

  /// @brief Clear properties of given framebuffer.
  void Clear();

private:
  TDyResourceBinderFrameBuffer mBinderFrameBuffer{ "dyBtFbUiBasic" };
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_UIBASICRENDERER_H