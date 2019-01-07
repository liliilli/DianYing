#ifndef GUARD_DY_EDITOR_INTERFACE_GUI_HAS_CHILDREN_H
#define GUARD_DY_EDITOR_INTERFACE_GUI_HAS_CHILDREN_H
#include "IDyGuiComponentBase.h"
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

#include <unordered_map>
#include <Dy/Helper/GlobalType.h>

namespace dy::editor
{

using TCrc32HashValue = TU32;

///
/// @class IDyGuiHasChildren
/// @brief
///
class IDyGuiHasChildren
{
  std::unordered_map<TCrc32HashValue, std::unique_ptr<IDyGuiComponentBase>>& GetChildrenMap() noexcept
  {
    return this->mSubWindows;
  }

protected:
  ///
  /// @brief Render and polling clicking or keyboard input to subwindows.
  ///
  void pRenderSubwindows(float dt) noexcept
  {
    for (auto it = this->mSubWindows.begin(); it != this->mSubWindows.end(); ++it)
    {
      if (it->second)
      {
        it->second->DrawWindow(dt);
      }
      else
      {
        it = this->mSubWindows.erase(it);
        if (this->mSubWindows.empty()) break;
      }
    }
  }

  std::unordered_map<TCrc32HashValue, std::unique_ptr<IDyGuiComponentBase>> mSubWindows;

  friend class FDyEditorGuiWindowFactory;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_EDITOR_INTERFACE_GUI_HAS_CHILDREN_H