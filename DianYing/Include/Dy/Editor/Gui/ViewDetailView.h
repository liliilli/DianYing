#ifndef GUARD_DY_EDITOR_GUI_VIEW_DETAILVIEW_H
#define GUARD_DY_EDITOR_GUI_VIEW_DETAILVIEW_H
#if defined(MDY_FLAG_IN_EDITOR)
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

#include <Dy/Editor/Interface/IDyGuiWindowSingletonCrtp.h>
#include <Dy/Editor/Descriptor/EmptyDescriptor.h>

namespace dy::editor
{

class FDyViewDetailView final : public IDyGuiWinSingleton<FDyViewDetailView, PDyGuiComponentEmptyDescriptor>
{
  MDY_GUISINGLETON_PROPERTIES(FDyViewDetailView);
  MDY_GUISINGLETON_DERIVED(FDyViewDetailView, PDyGuiComponentEmptyDescriptor);
public:
  void Update(float dt) noexcept override final;

  void DrawWindow(float dt) noexcept override final;

private:

};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_GUI_VIEW_DETAILVIEW_H