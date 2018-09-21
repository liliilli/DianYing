#ifndef GUARD_DY_EDITOR_GUI_HELPLINCENSEWINDOW_H
#define GUARD_DY_EDITOR_GUI_HELPLINCENSEWINDOW_H
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

class FDyHelpLicenseWindow final : public IDyGuiWinSingleton<FDyHelpLicenseWindow, PDyGuiComponentEmptyDescriptor>
{
  MDY_GUISINGLETON_PROPERTIES(FDyHelpLicenseWindow);
  MDY_GUISINGLETON_DERIVED(FDyHelpLicenseWindow, PDyGuiComponentEmptyDescriptor);
public:
  /// Draw dialog.
  void DrawWindow(float dt) noexcept override final;

private:
  bool                sHelpAboutLicenseWindow = false;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_GUI_HELPLINCENSEWINDOW_H