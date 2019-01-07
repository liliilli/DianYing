#ifndef GUARD_DY_EDITOR_GUI_VIEW_WORLDOUTLINER_H
#define GUARD_DY_EDITOR_GUI_VIEW_WORLDOUTLINER_H
#if defined(MDY_FLAG_IN_EDITOR)
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

#include <Dy/Editor/Interface/IDyGuiWindowSingletonCrtp.h>
#include <Dy/Editor/Descriptor/EmptyDescriptor.h>

namespace dy::editor
{

///
/// @class FDyViewWorldOutliner
/// @brief World outliner which display objects and world components of this world.
///
class FDyViewWorldOutliner final : public IDyGuiWinSingleton<FDyViewWorldOutliner, PDyGuiComponentEmptyDescriptor>
{
  MDY_GUISINGLETON_PROPERTIES(FDyViewWorldOutliner);
  MDY_GUISINGLETON_DERIVED(FDyViewWorldOutliner, PDyGuiComponentEmptyDescriptor);
public:
  void Update(float dt) noexcept override final;

  void DrawWindow(float dt) noexcept override final;

private:


};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_GUI_VIEW_WORLDOUTLINER_H