#ifndef GUARD_DY_GUI_MAIN_MENU_H
#define GUARD_DY_GUI_MAIN_MENU_H
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
#include <Dy/Editor/Interface/IDyGuiHasChildren.h>

namespace dy::editor
{

class FDyMainMenu final : public IDyGuiWinSingleton<FDyMainMenu, PDyGuiComponentEmptyDescriptor>, public IDyGuiHasChildren
{
  MDY_GUISINGLETON_PROPERTIES(FDyMainMenu);
  MDY_GUISINGLETON_DERIVED(FDyMainMenu, PDyGuiComponentEmptyDescriptor);
public:
  ///
  /// @brief
  ///
  void DrawWindow(float dt) noexcept override;

private:

};

} /// ::dy::editor namespace

#endif /// GUARD_DY_GUI_MAIN_MENU_H