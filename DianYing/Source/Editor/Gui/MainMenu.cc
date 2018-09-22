#include <precompiled.h>
#include "Dy/Editor/Descriptor/DialogDescriptor.h"
#include "Dy/Editor/Gui/EtcDialog.h"
#include "Dy/Editor/Gui/HelpAboutMain.h"
#include "Dy/Editor/Gui/HelpLicenseWindow.h"
#include "Dy/Editor/Gui/ViewViewportMain.h"
#include "Dy/Editor/Gui/LogWindow.h"
#include "Dy/Editor/Gui/MainSetting.h"
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

/// Header file
#include <Dy/Editor/Gui/MainMenu.h>

#include <imgui/imgui.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/Editor/GuiWindowFactory.h>

namespace dy::editor
{

EDySuccess FDyMainMenu::pfInitialize(const PDyGuiComponentEmptyDescriptor& desc)
{
  return DY_SUCCESS;
}

EDySuccess FDyMainMenu::pfRelease()
{
  return DY_SUCCESS;
}

void FDyMainMenu::DrawWindow(float dt) noexcept
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New Level", nullptr, &this->mMenuItemNewLevelFlag, true))
      {
        this->pCreateNotSupportYetDialogMsg(&this->mMenuItemNewLevelFlag);
      }
      if (ImGui::MenuItem("Open Level", nullptr, &this->mMenuItemOpenLevelFlag, true))
      {
        this->pCreateNotSupportYetDialogMsg(&this->mMenuItemOpenLevelFlag);
      }

      ImGui::Separator();
      if (ImGui::MenuItem("Save Current Level", nullptr, false, false))
      {

      };
      if (ImGui::MenuItem("Save Current Level As...", nullptr, false, false))
      {

      };

      ImGui::Separator();
      if (ImGui::MenuItem("Exit Editor"))
      {
        const auto ptr = MDyWindow::GetInstance().GetGlfwWindowContext();
        glfwSetKeyCallback(ptr, nullptr);
        glfwSetCursorPosCallback(ptr, nullptr);
        glfwDestroyWindow(ptr);
      };
      ImGui::EndMenu();
    }

    // Edit
    // L Undo
    // L Redo
    // -------
    // L Cut
    // L Copy
    // L Paste
    // -------
    // L Project Configuration
    if (ImGui::BeginMenu("Edit"))
    {
      if (ImGui::MenuItem("Undo", "CTRL+Z", false, false))
      {

      }
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
      {

      }

      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X", false, false))
      {

      }
      if (ImGui::MenuItem("Copy", "CTRL+C", false, false))
      {

      }
      if (ImGui::MenuItem("Paste", "CTRL+V", false, false))
      {

      }

      ImGui::Separator();
      if (ImGui::MenuItem("Project configuration", nullptr, &this->mMenuItemViewProjectConfiguration))
      {
        if (this->mMenuItemViewProjectConfiguration)
        {
          if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyMainSetting>(PDyGuiComponentEmptyDescriptor{});
              ptr)
          {
            auto [it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
            if (!result) { PHITOS_UNEXPECTED_BRANCH(); }
          }
        }
        else { this->mSubWindows.erase(FDyMainSetting::__mHashVal); }
      }

      ImGui::EndMenu();
    }

    // View
    // L Cpu Usage
    // L Console
    // L Viewport
    // L Log View
    if (ImGui::BeginMenu("View"))
    {
      if (ImGui::MenuItem("Cpu Usage", nullptr, &this->mMenuItemViewCpuUsage, false))
      {

      }
      if (ImGui::MenuItem("Viewport", nullptr, &this->mMenuItemViewViewport))
      {
        if (this->mMenuItemViewViewport)
        {
          if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyMainViewport>(PDyGuiComponentEmptyDescriptor{});
              ptr)
          {
            auto [it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
            if (!result) { PHITOS_UNEXPECTED_BRANCH(); }
          }
        }
        else { this->mSubWindows.erase(FDyMainViewport::__mHashVal); }
      }
      if (ImGui::MenuItem("Log View", nullptr, &this->mMenuItemViewLogWindow))
      {
        if (this->mMenuItemViewLogWindow)
        {
          if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyLogWindow>(PDyGuiComponentEmptyDescriptor{});
              ptr)
          {
            auto [it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
            if (!result) { PHITOS_UNEXPECTED_BRANCH(); }
          }
        }
        else { this->mSubWindows.erase(FDyLogWindow::__mHashVal); }
      }
      if (ImGui::MenuItem("World Outliner", nullptr, false, false))
      {

      }
      if (ImGui::MenuItem("Detail View", nullptr, false, false))
      {

      }
      ImGui::EndMenu();
    }

    // Help
    // L License
    // L About
    if (ImGui::BeginMenu("Help"))
    {
      // License
      if (ImGui::MenuItem("Library Licenses", nullptr, &this->mMenuItemHelpLicenseWindow, true))
      {
        if (this->mMenuItemHelpLicenseWindow)
        {
          if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyHelpLicenseWindow>(PDyGuiComponentEmptyDescriptor{});
              ptr)
          {
            auto [it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
            if (!result) { PHITOS_UNEXPECTED_BRANCH(); }
          }
        }
        else { this->mSubWindows.erase(FDyHelpLicenseWindow::__mHashVal); }
      }
      // About
      if (ImGui::MenuItem("About", nullptr, &this->mMenuItemHelpAboutWindow, true))
      {
        if (this->mMenuItemHelpAboutWindow)
        {
          const PDyGuiAboutMainDescriptor desc(DyMakeNotNull(&this->mMenuItemHelpAboutWindow), DyMakeNotNull(this));
          if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyHelpAboutMain>(desc); ptr)
          {
            auto [it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
            if (!result) { PHITOS_UNEXPECTED_BRANCH(); }
          }
        }
        else { this->mSubWindows.erase(FDyHelpAboutMain::__mHashVal); }
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  pRenderSubwindows(dt);
}

void FDyMainMenu::pCreateNotSupportYetDialogMsg(bool* boolFlag)
{
  PDyGuiDialogDescriptor desc;
  desc.mDialogTitle = "Warning!";
  desc.mDialogTextBody = "This feature is not supported yet.";
  desc.mParentRawPtr = this;
  desc.mParentBoolFlag = boolFlag;

  if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyDialog>(desc); ptr)
  {
    auto[it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
    if (!result) { PHITOS_UNEXPECTED_BRANCH(); }
  }
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR