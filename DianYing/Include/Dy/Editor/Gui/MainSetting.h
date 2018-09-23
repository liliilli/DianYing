#ifndef GUARD_DY_EDITOR_GUI_MAINSETTING_H
#define GUARD_DY_EDITOR_GUI_MAINSETTING_H
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

///
/// @class FDyMainSetting
/// @brief Main setting window.
///
class FDyMainSetting final : public IDyGuiWinSingleton<FDyMainSetting, PDyGuiComponentEmptyDescriptor>
{
  MDY_GUISINGLETON_PROPERTIES(FDyMainSetting);
  MDY_GUISINGLETON_DERIVED(FDyMainSetting, PDyGuiComponentEmptyDescriptor);
public:

  /// Draw window
  void DrawWindow(float dt) noexcept override final;

private:
  bool mEnabledMenuDescription = true;
  char mProjectName[64] = {0,};
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_GUI_MAINSETTING_H