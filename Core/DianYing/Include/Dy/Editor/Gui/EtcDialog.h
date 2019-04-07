#ifndef GUARD_DY_EDITOR_GUI_ETC_DIALOG_H
#define GUARD_DY_EDITOR_GUI_ETC_DIALOG_H
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

#include <Dy/Editor/Descriptor/DialogDescriptor.h>
#include <Dy/Editor/Interface/IDyGuiWindowSingletonCrtp.h>

namespace dy::editor
{

class FDyDialog final : public IDyGuiWinSingleton<FDyDialog, PDyGuiDialogDescriptor>
{
  MDY_GUISINGLETON_PROPERTIES(FDyDialog);
  MDY_GUISINGLETON_DERIVED(FDyDialog, PDyGuiDialogDescriptor);
public:
  /// Draw dialog.
  void DrawWindow(float dt) noexcept override final;

private:
  std::string mDialogTitle              = "Dialog";
  std::string mDialogTextBody           = "";

  bool        mIsPoppedUp               = false;
  bool*       mParentBoolFlagPtr        = nullptr;
  IDyGuiHasChildren* mParentRawPtr      = nullptr;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_GUI_ETC_DIALOG_H