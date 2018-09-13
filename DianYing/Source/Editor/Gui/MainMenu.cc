#include <precompiled.h>
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
      if (ImGui::MenuItem("New Level"))
      {

      };
      if (ImGui::MenuItem("Open Level"))
      {

      };

      ImGui::Separator();
      if (ImGui::MenuItem("Save Current Level"))
      {

      };
      if (ImGui::MenuItem("Save Current Level As..."))
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

    if (ImGui::BeginMenu("Edit"))
    {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
      // Disabled item
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}

      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {}
      if (ImGui::MenuItem("Copy", "CTRL+C")) {}
      if (ImGui::MenuItem("Paste", "CTRL+V")) {}

      ImGui::EndMenu();
    }

#ifdef false
    if (ImGui::BeginMenu("View"))
    {
      ImGui::MenuItem("Cpu Usage", nullptr, &sViewCpuUsage);
      ImGui::MenuItem("Console", nullptr, &sViewLogWindow);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
      ImGui::MenuItem("Licenses", nullptr, &sHelpLicenseWindow);
      ImGui::MenuItem("About", nullptr, &sHelpAboutWindow);
      ImGui::EndMenu();
    }
#endif

    ImGui::EndMainMenuBar();
  }

  pRenderSubwindows(dt);
}

} /// ::dy::editor namespace