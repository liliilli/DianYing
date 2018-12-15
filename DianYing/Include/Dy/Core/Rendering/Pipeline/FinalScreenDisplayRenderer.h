#ifndef GUARD_DY_CORE_RENDERING_FINALDISPLAYRENDERER_H
#define GUARD_DY_CORE_RENDERING_FINALDISPLAYRENDERER_H
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
struct  PDyGlAttachmentInformation;
class   CDyShaderResource_Deprecated;
class   CDyModelResource_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyFinalScreenDisplayRenderer
/// @brief
///
class FDyFinalScreenDisplayRenderer final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(FDyFinalScreenDisplayRenderer);
  FDyFinalScreenDisplayRenderer();
  ~FDyFinalScreenDisplayRenderer();

  ///
  /// @brief Rendering deferred contexts to default framebuffer.
  ///
  void RenderScreen();

  ///
  /// @brief Clear properties of given framebuffer.
  ///
  void Clear();

private:
  /// Attachment information pointer
  PDyGlAttachmentInformation* mAttachmentPtr_Scene  = MDY_INITIALIZE_NULL;
  PDyGlAttachmentInformation* mAttachmentPtr_Ui     = MDY_INITIALIZE_NULL;

  /// Shader information pointer
  CDyShaderResource_Deprecated*          mShaderPtr            = MDY_INITIALIZE_NULL;
  CDyModelResource_Deprecated*           mScreenRenderTrianglePtr    = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_FINALDISPLAYRENDERER_H