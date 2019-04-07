#ifndef GUARD_DY_GUI_MAIN_VIEWPORT_H
#define GUARD_DY_GUI_MAIN_VIEWPORT_H
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
/// @class FDyMainViewport
/// @brief Main viewport which displays game scene in editor.
///
class FDyMainViewport final : public IDyGuiWinSingleton<FDyMainViewport, PDyGuiComponentEmptyDescriptor>
{
  MDY_GUISINGLETON_PROPERTIES(FDyMainViewport);
  MDY_GUISINGLETON_DERIVED(FDyMainViewport, PDyGuiComponentEmptyDescriptor);
public:
  void DrawWindow(float dt) noexcept override final;

private:
  bool mIsEnabledShowGrid = false;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_GUI_MAIN_VIEWPORT_H