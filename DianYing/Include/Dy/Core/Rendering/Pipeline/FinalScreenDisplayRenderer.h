#ifndef GUARD_DY_CORE_RENDERING_FINALDISPLAYRENDERER_H
#define GUARD_DY_CORE_RENDERING_FINALDISPLAYRENDERER_H
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
  TDyResourceBinderShader      mBinderShader     {"dyBtShaderGlScreenOut"};
  TDyResourceBinderModel       mBinderTriangle   {"dyBtModelScrProjTri"};
  TDyResourceBinderAttachment  mBinderAttSceneFinal{ "dyBtAtScrFin_Output" };
  TDyResourceBinderAttachment  mBinderAttUIFinal   { "dyBtAtUiBasic_Output" };
  TDyResourceBinderAttachment  mBinderAttDbgFinal  { "dyBtAtDebug" };
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_FINALDISPLAYRENDERER_H