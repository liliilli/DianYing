#ifndef GUARD_DY_EDITOR_GUI_HELPABOUTMAIN_H
#define GUARD_DY_EDITOR_GUI_HELPABOUTMAIN_H
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
#include <Dy/Editor/Descriptor/AboutMainDescriptor.h>

//!
//! Forward declaration
//!

namespace dy::editor
{
class IDyGuiHasChildren;
} /// ::dy::editor namespace

//!
//! Implementation
//!

namespace dy::editor
{

class FDyHelpAboutMain final : public IDyGuiWinSingleton<FDyHelpAboutMain, PDyGuiAboutMainDescriptor>
{
  MDY_GUISINGLETON_PROPERTIES(FDyHelpAboutMain);
  MDY_GUISINGLETON_DERIVED(FDyHelpAboutMain, PDyGuiAboutMainDescriptor);
public:
  /// Draw dialog.
  void DrawWindow(float dt) noexcept override final;

private:
  bool                sHelpAboutLicenseWindow = false;

  bool*               mParentBoolFlagPtr  = nullptr;
  IDyGuiHasChildren*  mParentRawPtr       = nullptr;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_GUI_HELPABOUTMAIN_H