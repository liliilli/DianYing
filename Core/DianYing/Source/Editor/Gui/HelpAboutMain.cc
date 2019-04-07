#include <precompiled.h>
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
#include <Dy/Editor/Gui/HelpAboutMain.h>
#include <imgui/imgui.h>

namespace dy::editor
{

EDySuccess FDyHelpAboutMain::pfInitialize(const PDyGuiAboutMainDescriptor& desc)
{
  this->mParentRawPtr       = desc.mParentRawPtr;
  this->mParentBoolFlagPtr  = desc.mParentBoolFlag;

  return EDySuccess::DY_SUCCESS;
}

EDySuccess FDyHelpAboutMain::pfRelease()
{
  return EDySuccess::DY_SUCCESS;
}

void FDyHelpAboutMain::DrawWindow(float dt) noexcept
{
  // About window
  if (ImGui::Begin("About DY(DianYing)", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
  {
    ImGui::Text(R"dy(DianYing, %s)dy", "v0.0.1");
    ImGui::Text(R"dy(Copyright 2018 Jongmin Yun (neuliliilli).)dy");
    ImGui::Text(R"dy(DianYing is licensed under the MIT license, see)dy");

    ImGui::SameLine();
    if (ImGui::Button("LICENSE"))
    {
      this->sHelpAboutLicenseWindow = !this->sHelpAboutLicenseWindow;
    }
    ImGui::SameLine();

    ImGui::Text(R"dy(for more information.)dy");
    ImGui::End();
  }

  // License window
  if (this->sHelpAboutLicenseWindow)
  {
    ImGui::Begin("About DianYing license...", &this->sHelpAboutLicenseWindow, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text(R"dy(MIT License
Copyright (c) 2018-2019 Jongmin Yun
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)dy");
    ImGui::End();
  }
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR