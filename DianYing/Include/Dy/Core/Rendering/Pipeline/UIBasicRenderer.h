#ifndef GUARD_DY_CORE_RENDERING_UIBASICRENDERER_H
#define GUARD_DY_CORE_RENDERING_UIBASICRENDERER_H
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

//!
//! Forward declaration
//!

namespace dy
{
class   CDyCamera;
class   CDyModelRenderer;
struct  PDyGlFrameBufferInformation;
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

  ///
  /// @brief Rendering deferred contexts to framebuffer.
  ///
  void RenderScreen();

  ///
  /// @brief Clear properties of given framebuffer.
  ///
  void Clear();

private:
  /// mDyBtFbUiBasic
  PDyGlFrameBufferInformation* mDyBtFbUiBasic = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_UIBASICRENDERER_H