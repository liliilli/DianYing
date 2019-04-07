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

#include <filesystem>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Dy/Management/MWindow.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Core/Reflection/RGlobalInstanceManager.h>
#include <Dy/Builtin/GlobalInstance/FDyBtGiDebugStatus.h>
#include <Dy/Helper/MCS/Functions.h>
#include <Dy/Management/MLog.h>

namespace dy::editor
{

FDyEditor_MainMenu::FDyEditor_MainMenu()
{
  this->mPtrGlobalInstance = MDY_GET_GLOBALINSTANCE(FDyBtGiDebugStatus);
}
  
// https://qiita.com/Ushio/items/446d78c881334919e156
// https://qiita.com/izumin5210/items/26eaed69eea2c4318fcd
// https://qiita.com/ousttrue/items/ae7c8d5715adffc5b1fa
void FDyEditor_MainMenu::Draw(_MIN_ MDY_NOTUSED TF32 dt) noexcept
{
  ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
  ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);

  if (ImGui::Begin("Dy Debug Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar))
  {
    // Menu
    if (ImGui::BeginMenuBar())
    {
      // 
      if (ImGui::BeginMenu("Menu") == true)
      {
        // Edit mode 
        if (this->mPtrGlobalInstance->mEditMode == false)
        {
          if (ImGui::MenuItem("Edit mode", nullptr, false) == true)
          {
            DyPushLogCritical("Enter Edit mode...");

          }
        }
        else
        {
          if (ImGui::MenuItem("Return to play mode", nullptr, false) == true)
          {
            DyPushLogCritical("Return to play mode...");
          }
        }
        ImGui::Separator();

        // We need to push item flag to display modal dialog window.
        // https://github.com/ocornut/imgui/issues/2200
        // https://github.com/ocornut/imgui/issues/249
        ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
        if (ImGui::MenuItem("Export metafiles..") == true) 
        { 
          ImGui::OpenPopup("Export?"); 
        }
        ImGui::PopItemFlag();
        // Pop-up.
        if (ImGui::BeginPopupModal("Export?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
          ImGui::Text(u8"Exporting project setting files...\n\n");
          ImGui::Separator();

          // Export files with override option.
          mcs::Compress(MSetting::GetInstance().__GetEntrySettingFile(), true);
          // Create overaly dialogue window.

          // End
          ImGui::CloseCurrentPopup();
          ImGui::EndPopup();
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("View") == true)
      {
        static bool logValue = false;
        if (ImGui::MenuItem("Log", nullptr, &logValue) == true) { this->mPtrGlobalInstance->mLogFlag = logValue; }
        ImGui::EndMenu();
      }

      ImGui::EndMenuBar();
    }

#ifdef false
    ImGui::Text(u8"Hello world!\n"
      u8"アストラギウス銀河を二分するギルガメスとバララントは、\n"
      u8"もはや開戦の理由など誰も知らない戦争を100年も続けていた。\n"
      u8"その“百年戦争”の末期、ギルガメス軍の一兵士キリコ・キュービィーは、\n"
      u8"味方の基地を強襲するという不可解な作戦に参加させられる。\n"
      u8"その作戦でキリコは軍の最高機密「素体」を目にしたため軍から追われる身となり、\n"
      u8"町から町へ、星から星へと逃走の旅を続ける。\n"
      u8"その逃走と戦いの中で、やがて陰謀の闇を突きとめ、"
      u8"自分の出生に関わるさらなる謎の核心へとせまっていく。");
#endif

    auto& settingManager = MSetting::GetInstance();
    
    {
      const auto modelRenderingModeFlag = settingManager.GetRenderingMode();
      const TI32 originValue  = static_cast<std::underlying_type_t<decltype(modelRenderingModeFlag)>>(modelRenderingModeFlag);
      TI32 underlyingValue    = originValue;
      // If value is changed, process then body.
      if (ImGui::Combo("Model Rendering Mode", &underlyingValue, "Normal\0Wireframe\0\0"))
      {
        if (originValue != underlyingValue)
        {
          // Do logging process
          const auto newFlag = static_cast<ERenderingModelMode>(underlyingValue);
          switch (newFlag)
          {
          case ERenderingModelMode::FillNormal: 
          { DyPushLogInfo("Changed model rendering mode flag to {}.", "Normal");
          } break;
          case ERenderingModelMode::WireFrame: 
          { DyPushLogInfo("Changed model rendering mode flag to {}.", "Wireframe");
          } break;
          }

          // Change value.
          MSetting::GetInstance().SetRenderingMode(newFlag);
        }
      }
    }
    {
      const auto flag = settingManager.IsRenderPhysicsCollisionShape();
      auto mIsRenderCollisionShape = flag;

      if (ImGui::Checkbox("Enable render collision shape", &mIsRenderCollisionShape))
      {
        settingManager.SetRenderingPhysicsCollisionShape(mIsRenderCollisionShape);
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
        const auto ptr = MWindow::GetInstance().GetGLMainWindow();
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
