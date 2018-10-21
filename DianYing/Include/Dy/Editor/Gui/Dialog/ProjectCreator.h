#ifndef GUARD_DY_EDITOR_GUI_DIALOG_PROJECTCREATOR_H
#define GUARD_DY_EDITOR_GUI_DIALOG_PROJECTCREATOR_H
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
#include <Dy/Editor/Descriptor/DialogDescriptor.h>
#include <Dy/Editor/Interface/IDyGuiHasChildren.h>

namespace dy::editor
{

class FDyProjectCreator final : public IDyGuiWinSingleton<FDyProjectCreator, PDyGuiDialogDescriptor>, public IDyGuiHasChildren
{
  MDY_GUISINGLETON_PROPERTIES(FDyProjectCreator);
  MDY_GUISINGLETON_DERIVED(FDyProjectCreator, PDyGuiDialogDescriptor);
public:
  /// Draw dialog.
  void DrawWindow(float dt) noexcept override final;

private:
  ///
  void pCreateNotSupportYetDialogMsg(const NotNull<bool*>& boolFlag, const std::string_view& msg);

  std::string         mDialogTitle              = "New Project";
  bool                mIsPoppedUp               = false;

  IDyGuiHasChildren*  mParentRawPtr             = nullptr;
  bool*               mParentBoolFlagPtr        = nullptr;
  bool                mErrorDialogVisible       = false;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_GUI_DIALOG_PROJECTCREATOR_H