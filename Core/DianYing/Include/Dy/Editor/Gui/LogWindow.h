#ifndef GUARD_DY_GUI_LOG_WINDOW_H
#define GUARD_DY_GUI_LOG_WINDOW_H
#include "Dy/Editor/Descriptor/EmptyDescriptor.h"
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
#if defined(MDY_FLAG_IN_EDITOR)

#include <Dy/Editor/Interface/IDyGuiWindowSingletonCrtp.h>
#include <imgui/imgui.h>

namespace dy::editor
{

///
/// @class FDyLogWindow
/// @brief Gui logging window.
///
class FDyLogWindow final : public IDyGuiWinSingleton<FDyLogWindow, PDyGuiComponentEmptyDescriptor>
{
  MDY_GUISINGLETON_PROPERTIES(FDyLogWindow);
  MDY_GUISINGLETON_DERIVED(FDyLogWindow, PDyGuiComponentEmptyDescriptor);
public:
  void DrawWindow(float dt) noexcept override final;

private:
  /// Clear log message window.
  void pClearLog() noexcept;

  /// Get log from spdlog sink stream and output to display.
  void pPushLog() noexcept;

  ImGuiTextBuffer     mBuf;
  ImGuiTextFilter     mFilter;
  ImVector<int>       mLineOffsets;        // Index to lines offset
  bool                mScrollToBottom;
};

} /// ::dy::editor namespace

#endif
#endif /// GUARD_DY_GUI_LOG_WINDOW_H