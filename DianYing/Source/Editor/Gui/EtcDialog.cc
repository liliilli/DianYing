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
/// @TODO IMPLEMENT WRAPPING TEXT
///

/// Header file
#include <Dy/Editor/Gui/EtcDialog.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Dy/Editor/Gui/Helper/ComponentHelper.h>
#include <Dy/Management/Editor/GuiWindowFactory.h>

namespace dy::editor
{

EDySuccess FDyDialog::pfInitialize(const PDyGuiDialogDescriptor& desc)
{
  this->mDialogTitle        = desc.mDialogTitle;
  this->mDialogTextBody     = desc.mDialogTextBody;
  this->mParentRawPtr       = desc.mParentRawPtr;
  this->mParentBoolFlagPtr  = desc.mParentBoolFlag;

  return DY_SUCCESS;
}

EDySuccess FDyDialog::pfRelease()
{
  if (this->mParentBoolFlagPtr)
  {
    *(this->mParentBoolFlagPtr) = false;
  }

  MDY_LOG_INFO("Released");
  return DY_SUCCESS;
}

void FDyDialog::DrawWindow(float dt) noexcept
{
  // Draw dialog
  ImGui::PushID(this->mDialogTitle.c_str());

  if (!this->mIsPoppedUp)
  {
    ImGui::OpenPopup(this->mDialogTitle.c_str());
    this->mIsPoppedUp = true;
  }

  if (ImGui::BeginPopupModal(this->mDialogTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("%s", this->mDialogTextBody.c_str());
    if (ImGui::Button("OK"))
    {
      MDY_LOG_INFO("{} | Pressed OK button.", "FDyDialog::DrawWindow");
      ImGui::CloseCurrentPopup();
      // @todo DELETE THIS!
      if (this->mParentRawPtr && FDyEditorGuiWindowFactory::RemoveGuiComponent<FDyDialog>(*(this->mParentRawPtr)))
      {

      }
    };
    ImGui::EndPopup();
  }

  ImGui::PopID();
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR