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
#include <Dy/Editor/Gui/MainMenu.h>

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
///
class FDyEditorGuiWindowFactory final
{
public:
  MDY_STATIC_CLASS(FDyEditorGuiWindowFactory);

  template <typename TGuiComponentType, typename TGuiComponentDescriptor>
  [[nodiscard]] static std::unique_ptr<IDyGuiComponentBase>
  CreateGuiWindow(const TGuiComponentDescriptor& descriptor)
  {
    auto ptr = std::make_unique<TGuiComponentType>();
    if (ptr->Initialize(descriptor) == DY_FAILURE)
    {
      return nullptr;
    }
    else
    {
      /// @reference https://www.ficksworkshop.com/blog/how-to-static-cast-std-unique-ptr
      return std::unique_ptr<IDyGuiComponentBase>(static_cast<IDyGuiComponentBase*>(ptr.release()));
    }
  }

#ifdef false
  template <typename TGuiComponentType>
  [[nodiscard]] static EDySuccess RemoveGuiWindow(const IDyGuiHasChildren& parentWindow)
  {
    auto it = parentWindow.mGuiChildComponentMap(TGuiComponentType::pfGetHashKey());
    if (it == parentWindow.mGuiChildComponentMap.end())
    {
      return DY_FAILURE;
    }
    else
    {
      if (it->second->Release() == DY_FAILURE)
      {
        MDY_LOG_ERROR("{} | Failed to release gui window. | Name : {}", "FDyEditorGuiWindowFactory::RemoveGuiWindow", MDY_TO_STRING(TGuiComponentType));
        return DY_FAILURE;
      }

      return DY_SUCCESS;
    }
  }
#endif
};

///
/// @class MDyEditorGui
/// @brief Gui management class for gui editor.
///
class MDyEditorGui : public ISingleton<MDyEditorGui>, public IDyGuiComponentBase
{
  MDY_SINGLETON_PROPERTIES(MDyEditorGui);
  MDY_SINGLETON_DERIVED(MDyEditorGui);
public:
  void DrawWindow(float dt) noexcept override;

private:
  std::unique_ptr<FDyMainMenu> mMainMenu = nullptr;

};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_MANAGEMENT_EDITOR_GUI_MANAGER_H