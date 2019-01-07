#include <precompiled.h>
#include <Dy/Editor/Gui/LogWindow.h>
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
#include <Dy/Editor/Gui/HelpLicenseWindow.h>
#include <imgui/imgui.h>
#include <Dy/Editor/Literals/LibraryLicenseString.h>

namespace dy::editor
{

EDySuccess FDyHelpLicenseWindow::pfInitialize(const PDyGuiComponentEmptyDescriptor& desc)
{
  return DY_SUCCESS;
}

EDySuccess FDyHelpLicenseWindow::pfRelease()
{
  return DY_SUCCESS;
}

void FDyHelpLicenseWindow::DrawWindow(float dt) noexcept
{
  ImGui::Begin("Lincense and third party library using", nullptr, ImGuiWindowFlags_NoCollapse);

  for (const auto& libraryInfo : dy::editor::kLibraryLicenseInfo)
  {
    ImGui::BeginGroup();
    ImGui::BeginChild(libraryInfo.GetLibraryName(), ImVec2(0, 100), true);
    ImGui::Text("Library"); ImGui::SameLine(); ImGui::Text(libraryInfo.GetLibraryName());
    ImGui::Text("Version"); ImGui::SameLine(); ImGui::Text(libraryInfo.GetVersionName());

    bool isHp = false;
    if (!libraryInfo.IsHomepageUrlEmpty())
    {
      if (ImGui::Button("Homepage"))
      {
      #if defined(_WIN32)
        ShellExecute(nullptr, nullptr, libraryInfo.GetHomepageUrl().c_str(), nullptr, nullptr, SW_SHOW);
      #elif defined(__linux__)
        // @todo EXECUTE WEB BROWSER WITH URL.
      #endif
      }
      isHp = true;
    }

    if (!libraryInfo.IsGithubUrlEmpty())
    {
      if (isHp) ImGui::SameLine();
      if (ImGui::Button("Github"))
      {
      #if defined(_WIN32)
        ShellExecute(nullptr, nullptr, libraryInfo.GetGithubUrl().c_str(), nullptr, nullptr, SW_SHOW);
      #elif defined(__linux__)
        // @todo EXECUTE WEB BROWSER WITH URL.
      #endif
      }
    }

    if (ImGui::Button("View license"))
    {
      //ImGui::TextWrapped(libraryInfo.GetLicenseStatement());
    };
    ImGui::EndChild();
    ImGui::EndGroup();
  }

  ImGui::End();
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
