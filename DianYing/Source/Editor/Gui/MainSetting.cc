#include <precompiled.h>
#include "Dy/Management/LoggingManager.h"
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

/// Header file
#include <Dy/Editor/Gui/MainSetting.h>
#include <imgui/imgui.h>

namespace dy::editor
{

EDySuccess FDyMainSetting::pfInitialize(const PDyGuiComponentEmptyDescriptor& desc)
{
  return DY_SUCCESS;
}

EDySuccess FDyMainSetting::pfRelease()
{
  return DY_SUCCESS;
}

void FDyMainSetting::DrawWindow(float dt) noexcept
{
  ImGui::SetNextWindowSize(ImVec2(800, 480), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Editor Preference", nullptr, ImGuiWindowFlags_NoCollapse))
  {
    {
      ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
      ImGui::BeginChild("SetLeftSideBar", ImVec2(200, 0), true);

      if (ImGui::Selectable("Description"))
      {
        if (!this->mEnabledMenuDescription) { this->mEnabledMenuDescription = true; }
      };
      if (ImGui::Selectable("Map & Mode"))
      {

      };
      if (ImGui::Selectable("Gameplay"))
      {

      };
      if (ImGui::Selectable("Audio"))
      {

      };
      if (ImGui::Selectable("Input"))
      {

      };

      ImGui::EndChild();
      ImGui::PopStyleVar();
    }

    ImGui::SameLine();

    {
      ImGui::BeginChild("SetRightDetail", ImVec2(0, 0));

      if (this->mEnabledMenuDescription)
      {
        ImGui::PushID("About");
        if (ImGui::CollapsingHeader("About", "About", false, true))
        {
          {
            ImGui::Text("Project Name"); ImGui::SameLine(); ImGui::PushItemWidth(200);
            if (ImGui::InputText("ProjectName", this->mProjectName, 64, ImGuiInputTextFlags_EnterReturnsTrue))
            {
              DyPushLogCritical("{}!", this->mProjectName);
            };
            ImGui::PopItemWidth();
          }

          {
            ImGui::Text("Description");
          }

          {
            ImGui::Text("UUID");
          }

          {
            ImGui::Text("Version");
          }
        }
      }
      ImGui::PopID();

      ImGui::EndChild();
    }

    ImGui::End();
  }
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR