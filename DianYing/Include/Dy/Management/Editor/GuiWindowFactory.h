#ifndef GUARD_DY_MANAGEMENT_EDITOR_GUI_WINDOW_FACTORY_H
#define GUARD_DY_MANAGEMENT_EDITOR_GUI_WINDOW_FACTORY_H
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

#include <Dy/Management/LoggingManager.h>
#include <Dy/Editor/Interface/IDyGuiComponentBase.h>
#include <Dy/Editor/Interface/IDyGuiHasChildren.h>

#define MDY_STATIC_CLASS(__MAClassType__) \
  __MAClassType__() = delete; \
  ~__MAClassType__() = delete; \
  __MAClassType__(const __MAClassType__&) = delete; \
  __MAClassType__(__MAClassType__&&) = delete; \
  __MAClassType__& operator=(const __MAClassType__&) = delete; \
  __MAClassType__& operator=(__MAClassType__&&) = delete

namespace dy::editor
{

///
/// @class FDyEditorGuiWindowFactory
/// @brief Factory class for create and delete safely window instance from class which inherits IDyGuiHasChildren.
///
class FDyEditorGuiWindowFactory final
{
public:
  MDY_STATIC_CLASS(FDyEditorGuiWindowFactory);

  ///
  /// @brief Create gui component and return with hash value.
  ///
  template <typename TGuiComponentType, typename TGuiComponentDescriptor>
  [[nodiscard]] static std::pair<TU32, std::unique_ptr<IDyGuiComponentBase>>
  CreateGuiComponent(const TGuiComponentDescriptor& descriptor)
  {
    auto ptr = std::make_unique<TGuiComponentType>();
    if (ptr->Initialize(descriptor) == DY_FAILURE)
    {
      return {0, nullptr};
    }
    else
    {
      /// @reference https://www.ficksworkshop.com/blog/how-to-static-cast-std-unique-ptr
      return {
          ptr->__mHashVal,
          std::unique_ptr<IDyGuiComponentBase>(static_cast<IDyGuiComponentBase*>(ptr.release()))
      };
    }
  }

  ///
  /// @brief Delete gui component from parentWindow safely.
  ///
  template <typename TGuiComponentType>
  [[nodiscard]] static EDySuccess RemoveGuiComponent(IDyGuiHasChildren& parentWindow)
  {
    auto it = parentWindow.mSubWindows.find(TGuiComponentType::__mHashVal);
    if (it == parentWindow.mSubWindows.end())
    {
      MDY_LOG_ERROR("{} | Failed to find gui window instance. | Name : {}", "FDyEditorGuiWindowFactory::RemoveGuiComponent", MDY_TO_STRING(TGuiComponentType));
      return DY_FAILURE;
    }
    else
    {
      if (static_cast<TGuiComponentType*>(it->second.get())->Release() == DY_FAILURE)
      {
        MDY_LOG_ERROR("{} | Failed to release gui window. | Name : {}", "FDyEditorGuiWindowFactory::RemoveGuiComponent", MDY_TO_STRING(TGuiComponentType));
        return DY_FAILURE;
      }

      pInsertDeleteComponentToGuiManager(it->second.release());
      MDY_LOG_INFO("{} | Remove gui component window. | Name : {}", "FDyEditorGuiWindowFactory::RemoveGuiComponent", MDY_TO_STRING(TGuiComponentType));
      return DY_SUCCESS;
    }
  }

private:
  /// Insert will be deleted gui component to gui manager trash can.
  static void pInsertDeleteComponentToGuiManager(IDyGuiComponentBase* rawPtrInstance) noexcept;
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_MANAGEMENT_EDITOR_GUI_WINDOW_FACTORY_H