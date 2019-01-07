#ifndef GUARD_DY_GUI_MAIN_MENU_H
#define GUARD_DY_GUI_MAIN_MENU_H
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
#include <Dy/Editor/Interface/IDyGuiHasChildren.h>
#include <Dy/Helper/Pointer.h>

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
  void DrawWindow(float dt) noexcept override final;

private:
  /// @brief Create dialogue modal message.
  void pCreateNotSupportYetDialogMsg(bool* boolFlag);
  /// @brief Create dialogue
  void pCreateCreateProjectDalog(NotNull<bool*> boolFlag);

  bool mMenuItemNewProjectFlag    = false;

  bool mMenuItemNewLevelFlag      = false;
  bool mMenuItemOpenLevelFlag     = false;
  bool mMenuItemViewCpuUsage      = false;
  bool mMenuItemViewLogWindow     = false;
  bool mMenuItemHelpLicenseWindow = false;
  bool mMenuItemHelpAboutWindow   = false;
  bool mMenuItemViewViewport      = false;

  bool mMenuItemViewProjectConfiguration = false;

  bool mNotSupport                = false;
};

} /// ::dy::editor namespace

#endif /// GUARD_DY_GUI_MAIN_MENU_H