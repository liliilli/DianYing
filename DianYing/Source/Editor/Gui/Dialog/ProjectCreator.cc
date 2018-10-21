#include <precompiled.h>
#include "Dy/Core/Component/Internal/MaterialType.h"
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
/// @TODO IMPLEMENT MAKING PROJECT SOLUTION.
///

/// Header file
#include <Dy/Editor/Gui/Dialog/ProjectCreator.h>

#include <filesystem>

#include <imgui/imgui.h>
#include <Dy/Editor/Gui/EtcDialog.h>
#include <Dy/Editor/Gui/Helper/ComponentHelper.h>
#include <Dy/Editor/Helper/Win32Helper.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/Editor/GuiWindowFactory.h>

//!
//! Forward declaration and local variables
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sProjectPathTooltip, R"dy(
Specify the path for creating project of dy.
)dy");

MDY_SET_IMMUTABLE_STRING(sProjectNameTooltip, R"dy(
Specify the project name for creating project of dy.
)dy");

MDY_SET_IMMUTABLE_STRING(sErrorDirectoryPathEmpty, R"dy(
Directory path is not specified.
Press '...' button and specify valid directory path for making project.
)dy");

MDY_SET_IMMUTABLE_STRING(sErrorProjectNameEmpty, R"dy(
Project name is not speicified.
Press enter valid project name for making project.
)dy");

MDY_SET_IMMUTABLE_STRING(sErrorProjectNameFirstAlpha, R"dy(
First charcter of project name must be alphabet.
)dy");

MDY_SET_IMMUTABLE_STRING(sErrorProjectNameNotCorrect, R"dy(
Project name must be consisted of alphabet and digit number.
)dy");

///
/// @enum EDyIntegrityTestFlag
/// @brief Flag for integrity test
///
enum EDyIntegrityTestFlag
{
  PassIntegrityTest,
  FailedByDirectoryPathEmpty,
  FailedByProjectNameEmpty,
  FailedByProjectNameFirstAlpha,
  FailedByProjectNameNotCorrect,
};

///
/// @brief Check integrity test for making DY project solution.
/// @param directoryPath directory Path
/// @param projectName project Name to create .dsol and setting.
///
[[nodiscard]] EDyIntegrityTestFlag CheckIntegrity(const char* directoryPath, const char* projectName)
{
  if (std::strlen(directoryPath) == 0)  { return FailedByDirectoryPathEmpty; }
  if (const auto len = std::strlen(projectName); len == 0) { return FailedByProjectNameEmpty; }
  else
  { // Check first character is not digit, and all characters are alphabet.
    if (std::isdigit(projectName[0]))   { return FailedByProjectNameNotCorrect; }

    for (size_t i = 1; i < len; ++i)
    {
      if (!std::isalpha(projectName[i])) { return FailedByProjectNameNotCorrect; }
    }
  }

  return PassIntegrityTest;
}

///
/// @brief Check directory is exist and .dsol (dy project solution) file is exists.
/// @param directoryPath directory Path
/// @param projectName project Name to create .dsol and setting.
///
[[nodiscard]] EDySuccess CheckNotDuplicatedSolutionOnDirectory(const char* directoryPath, const char* projectName)
{
  const auto solutionPathLen = std::strlen(directoryPath) + std::strlen(projectName) + 16;
  TC8* solutionPath = new TC8[solutionPathLen];
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
  sprintf_s(solutionPath, solutionPathLen, "%s/%s.dsol", directoryPath, projectName);
#else
  MDY_NOT_IMPLEMENTED_ASSERT();
#endif

  if (std::filesystem::exists(directoryPath) && std::filesystem::exists(solutionPath))
  {
    delete[] solutionPath;
    return DY_FAILURE;
  }
  else
  {
    delete[] solutionPath;
    return DY_SUCCESS;
  }
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::editor
{

EDySuccess FDyProjectCreator::pfInitialize(const PDyGuiDialogDescriptor& desc)
{
  this->mParentRawPtr       = desc.mParentRawPtr;
  this->mParentBoolFlagPtr  = desc.mParentBoolFlag;

  return DY_SUCCESS;
}

EDySuccess FDyProjectCreator::pfRelease()
{
  if (this->mParentBoolFlagPtr) { *(this->mParentBoolFlagPtr) = false; }

  MDY_LOG_INFO("Released");
  return DY_SUCCESS;
}

void FDyProjectCreator::DrawWindow(float dt) noexcept
{
  ImGui::PushID(this->mDialogTitle.c_str());

  if (!this->mIsPoppedUp)
  {
    ImGui::OpenPopup(this->mDialogTitle.c_str());
    this->mIsPoppedUp = true;
  }

  if (ImGui::BeginPopupModal(this->mDialogTitle.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
  {
    ImGui::Text("Select a location for your project to be stored");
    ImGui::Separator();

    static char directoryPath[256]  = "";
    static char projectName[64]     = "";

    if (ImGui::Button("...", ImVec2(0, 0)))
    {
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
      // Get directory path and reflect directory (valid) string to directoryPath;
      auto unicodeDirectoryPath = DyWinGetDirectoryPathFromDialog();
      if (!unicodeDirectoryPath.empty())
      { // IMGUI DOES NOT SUPPORT WCHAR_T BUT CHAR WITH UTF8 BY USING INTERNAL LOGIC.
        std::wcstombs(directoryPath, unicodeDirectoryPath.c_str(), IM_ARRAYSIZE(directoryPath));
      }
#else
      MDY_NOT_IMPLEMENTED_ASSERT();
#endif
    };

    ImGui::InputText("Project path folder", directoryPath, IM_ARRAYSIZE(directoryPath), ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    DyShowHelpTooltipMarker(sProjectPathTooltip);

    ImGui::InputText("Project name", projectName, IM_ARRAYSIZE(projectName));
    ImGui::SameLine();
    DyShowHelpTooltipMarker(sProjectNameTooltip);

    if (ImGui::Button("Create Project"))
    {
      MDY_LOG_INFO("{} | Pressed OK button.", "FDyProjectCreator::DrawWindow");

      // Check path and project name are not vacant.
      switch (const auto flag = CheckIntegrity(directoryPath, projectName); flag)
      {
      case FailedByDirectoryPathEmpty:
        MDY_LOG_ERROR(sErrorDirectoryPathEmpty.data());
#ifdef false
        pCreateNotSupportYetDialogMsg(DyMakeNotNull(&this->mErrorDialogVisible), sErrorDirectoryPathEmpty);
#endif
        break;
      case FailedByProjectNameEmpty:
        MDY_LOG_ERROR(sErrorProjectNameEmpty.data());
#ifdef false
        pCreateNotSupportYetDialogMsg(DyMakeNotNull(&this->mErrorDialogVisible), sErrorProjectNameEmpty);
#endif
        break;
      case FailedByProjectNameFirstAlpha:
        MDY_LOG_ERROR(sErrorProjectNameFirstAlpha.data());
#ifdef false
        pCreateNotSupportYetDialogMsg(DyMakeNotNull(&this->mErrorDialogVisible), sErrorProjectNameFirstAlpha);
#endif
        break;
      case FailedByProjectNameNotCorrect:
        MDY_LOG_ERROR(sErrorProjectNameNotCorrect.data());
#ifdef false
        pCreateNotSupportYetDialogMsg(DyMakeNotNull(&this->mErrorDialogVisible), sErrorProjectNameNotCorrect);
#endif
        break;
      case PassIntegrityTest:
        // Check if there is already dy project checking .dsol file.
        // If not exist anything but just vacant, create .dsol file with multithreading.
        // representing loading message.
        if (CheckNotDuplicatedSolutionOnDirectory(directoryPath, projectName) == DY_FAILURE)
        {
          // Output error dialogue.
        }
        else
        {
          // Execute make solution.
          ImGui::CloseCurrentPopup();
          std::memset(directoryPath, 0, IM_ARRAYSIZE(directoryPath));
          std::memset(projectName, 0, IM_ARRAYSIZE(projectName));

          if (this->mParentRawPtr &&
              FDyEditorGuiWindowFactory::RemoveGuiComponent<FDyProjectCreator>(*(this->mParentRawPtr)))
          { }
        }
        break;
      }
    }
    ImGui::EndPopup();
  }
  ImGui::PopID();
}

void FDyProjectCreator::pCreateNotSupportYetDialogMsg(const NotNull<bool*>& boolFlag, const std::string_view& msg)
{
  this->mErrorDialogVisible = true;

  PDyGuiDialogDescriptor desc;
  desc.mDialogTitle     = "Project Creation Failed";
  desc.mDialogTextBody  = msg;
  desc.mParentRawPtr    = this;
  desc.mParentBoolFlag  = boolFlag;

  if (auto [hashVal, ptr] = FDyEditorGuiWindowFactory::CreateGuiComponent<FDyDialog>(desc); ptr)
  {
    auto[it, result] = this->mSubWindows.try_emplace(hashVal, std::move(ptr));
    if (!result) { MDY_UNEXPECTED_BRANCH(); }
  }
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR