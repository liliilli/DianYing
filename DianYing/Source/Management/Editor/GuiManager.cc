#include <precompiled.h>
#include "Dy/Management/Editor/GuiSetting.h"
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
#include <Dy/Management/Editor/GuiManager.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/Editor/GuiWindowFactory.h>

namespace dy::editor
{

EDySuccess MDyEditorGui::pfInitialize()
{
  MDY_LOG_INFO_D("MDyEditorGui manager initialized.");
  MDY_CALL_ASSERT_SUCCESS(MDyEditorSetting::Initialize());

  auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyMainMenu>(PDyGuiComponentEmptyDescriptor{});
  MDY_ASSERT(static_cast<bool>(ptr), "Failed to create MainMenu!");
  this->mMainMenu = std::unique_ptr<FDyMainMenu>(static_cast<FDyMainMenu*>(ptr.release()));

  return DY_SUCCESS;
}

EDySuccess MDyEditorGui::pfRelease()
{
  MDY_LOG_INFO_D("MDyEditorGui manager released.");
  MDY_CALL_ASSERT_SUCCESS(MDyEditorSetting::Release());
  return DY_SUCCESS;
}

void MDyEditorGui::Update(float dt) noexcept
{
  if (!this->__mDeleteCandidateList.empty())
  {
    for (auto it = this->__mDeleteCandidateList.begin(); it != this->__mDeleteCandidateList.end(); ++it)
    {
      delete *it;
    }
    this->__mDeleteCandidateList.clear();
  }
}

void MDyEditorGui::DrawWindow(float dt) noexcept
{
  if (mMainMenu)
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    this->mMainMenu->DrawWindow(dt);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR