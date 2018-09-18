#ifndef GUARD_DY_MANAGEMENT_EDITOR_GUI_MANAGER_H
#define GUARD_DY_MANAGEMENT_EDITOR_GUI_MANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Editor/Gui/MainMenu.h>

//!
//! Forward declaration
//!

namespace dy::editor
{
class MDyEditorGui;
}

//!
//! Implementation
//!

namespace dy::editor
{

///
/// @class MDyEditorGui
/// @brief Gui management class for gui editor.
///
class MDyEditorGui : public ISingleton<MDyEditorGui>, public IDyGuiComponentBase
{
  MDY_SINGLETON_PROPERTIES(MDyEditorGui);
  MDY_SINGLETON_DERIVED(MDyEditorGui);
public:

  void Update(float dt) noexcept override final;

  void DrawWindow(float dt) noexcept override final;

private:
  ///
  /// @brief
  ///
  void __pfInsertDeleteComponent(IDyGuiComponentBase* validInstance) noexcept
  {
    this->__mDeleteCandidateList.emplace_back(validInstance);
  }
  std::vector<IDyGuiComponentBase*> __mDeleteCandidateList = {};

  std::unique_ptr<FDyMainMenu>  mMainMenu = nullptr;

  friend class FDyEditorGuiWindowFactory;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_MANAGEMENT_EDITOR_GUI_MANAGER_H