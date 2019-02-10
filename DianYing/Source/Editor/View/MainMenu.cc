#include <precompiled.h>
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

/// Header file
#include <Dy/Editor/View/MainMenu.h>

#include <imgui/imgui.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/SettingManager.h>

namespace dy::editor
{

void FDyEditor_MainMenu::Draw(_MIN_ MDY_NOTUSED TF32 dt) noexcept
{
  ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
  ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);

  if (ImGui::Begin("Dy Debug Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
  {
#ifdef false
    // If check button, activate (floating) check render flag.
    if (ImGui::Button("Check Render Flag"))
    {

    }
#endif
    {
      const auto modelRenderingModeFlag = MDySetting::GetInstance().GetRenderingMode();
      const TI32 originValue  = static_cast<std::underlying_type_t<decltype(modelRenderingModeFlag)>>(modelRenderingModeFlag);
      TI32 underlyingValue    = originValue;
      // If value is changed, process then body.
      if (ImGui::Combo("Model Rendering Mode", &underlyingValue, "Normal\0Wireframe\0\0"))
      {
        if (originValue != underlyingValue)
        {
          // Do logging process
          const auto newFlag = static_cast<EDyModelRenderingMode>(underlyingValue);
          switch (newFlag)
          {
          case EDyModelRenderingMode::FillNormal: 
          { MDY_LOG_INFO("Changed model rendering mode flag to {}.", "Normal");
          } break;
          case EDyModelRenderingMode::WireFrame: 
          { MDY_LOG_INFO("Changed model rendering mode flag to {}.", "Wireframe");
          } break;
          }

          // Change value.
          MDySetting::GetInstance().SetRenderingMode(newFlag);
        }
      }
    }

    // Next item..
  }
  ImGui::End();
}

#ifdef  false
void FDyEditor_MainMenu::DrawWindow(_MIN_ TF32 dt) noexcept
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New Project", nullptr, &this->mMenuItemNewProjectFlag))
      {
        this->pCreateCreateProjectDalog(DyMakeNotNull(&this->mMenuItemNewProjectFlag));
      }
      if (ImGui::MenuItem("Open Project", nullptr, &this->mNotSupport))
      {
        this->pCreateNotSupportYetDialogMsg(&this->mNotSupport);
      }
      if (ImGui::Selectable("Save project", false))
      {

      }

      ImGui::Separator();

      auto& editorSettingManager = MDyEditorSetting::GetInstance();
      const bool isProjectLoaded = editorSettingManager.GetmIsLoadedProject();

      if (ImGui::MenuItem("New Level", nullptr, &this->mMenuItemNewLevelFlag, isProjectLoaded))
      {
        this->pCreateNotSupportYetDialogMsg(&this->mMenuItemNewLevelFlag);
      }
      if (ImGui::MenuItem("Open Level", nullptr, &this->mMenuItemOpenLevelFlag, isProjectLoaded))
      {
        this->pCreateNotSupportYetDialogMsg(&this->mMenuItemOpenLevelFlag);
      }
      if (ImGui::MenuItem("Save Current Level", nullptr, &this->mNotSupport, isProjectLoaded))
      {
        this->pCreateNotSupportYetDialogMsg(&this->mNotSupport);
      };
      if (ImGui::MenuItem("Save Current Level As...", nullptr, &this->mNotSupport, isProjectLoaded))
      {
        this->pCreateNotSupportYetDialogMsg(&this->mNotSupport);
      };

      ImGui::Separator();
      if (ImGui::MenuItem("Exit Editor"))
      {
        const auto ptr = MDyWindow::GetInstance().GetGLMainWindow();
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
            if (!result) { MDY_UNEXPECTED_BRANCH(); }
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
            if (!result) { MDY_UNEXPECTED_BRANCH(); }
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
            if (!result) { MDY_UNEXPECTED_BRANCH(); }
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
            if (!result) { MDY_UNEXPECTED_BRANCH(); }
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
            if (!result) { MDY_UNEXPECTED_BRANCH(); }
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

void FDyEditor_MainMenu::pCreateNotSupportYetDialogMsg(bool* boolFlag)
{
  PDyGuiDialogDescriptor desc;
  desc.mDialogTitle = "Warning!";
  desc.mDialogTextBody = "This feature is not supported yet.";
  desc.mParentRawPtr = this;
  desc.mParentBoolFlag = boolFlag;

  if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyDialog>(desc); ptr)
  {
    auto[it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
    if (!result) { MDY_UNEXPECTED_BRANCH(); }
  }
}

void FDyEditor_MainMenu::pCreateCreateProjectDalog(NotNull<bool*> boolFlag)
{
  PDyGuiDialogDescriptor desc;
  desc.mDialogTitle     = "";
  desc.mDialogTextBody  = "";
  desc.mParentRawPtr    = this;
  desc.mParentBoolFlag  = boolFlag;

  if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyProjectCreator>(desc); ptr)
  {
    auto[it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
    if (!result) { MDY_UNEXPECTED_BRANCH(); }
  }
}
#endif

} /// ::dy::editor namespace
