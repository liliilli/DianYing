#ifndef GUARD_DY_MANAGEMENT_INTENRAL_WORLD_FDYWORLDUICONTAINER_H
#define GUARD_DY_MANAGEMENT_INTENRAL_WORLD_FDYWORLDUICONTAINER_H
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

#include <Dy/Element/Canvas/Widget.h>
#include <Dy/Helper/Internal/FDyNameGenerator.h>

namespace dy
{

///
/// @class FDyWorldUIContainer
/// @brief UI Container type.
///
class FDyWorldUIContainer final 
{
public:
  using TUiWidgetMap = std::unordered_map<std::string, std::unique_ptr<FDyUiWidget>>;

  /// @brief Try create debug ui layout to screen as highest priority. (10xx) \n
  /// If debug ui is already spawned, just return DY_FAILURE, or DY_SUCCESS.
  MDY_NODISCARD EDySuccess  TryCreateDebugUi();
  /// @brief Try get debug UI. If not exist, just return nullptr.
  MDY_NODISCARD FDyUiWidget* GetPtrDebugUi() const noexcept;
  /// @brief Check debug Ui is now on exist.
  MDY_NODISCARD bool        IsDebugUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already removed, just return DY_FAILURE.
  MDY_NODISCARD EDySuccess  TryRemoveDebugUi();

  /// @brief Try create loading ui layout. \n
  /// If Loading UI Widget meta information is not exist, just return DY_FAILURE doing nothing.
  MDY_NODISCARD EDySuccess  TryCreateLoadingUi();
  /// @brief Try get loading UI. If not exist, just return nullptr.
  MDY_NODISCARD FDyUiWidget* GetPtrLoadingUi() const noexcept;
  /// @brief Check loading ui is now on exist.
  MDY_NODISCARD bool        IsLoadingUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already remove or not exist, return DY_FAILURE.
  MDY_NODISCARD EDySuccess  TryRemoveLoadingUi();

  /// @brief IsUiObjectExist
  MDY_NODISCARD bool IsUiObjectExist(_MIN_ const std::string& iUiObjectName) const noexcept;
  
  /// @brief Create UI widget.
  MDY_NODISCARD DDyUiBinder CreateUiObject(
    _MIN_ const std::string& iUiName, 
    _MIN_ const PDyMetaWidgetRootDescriptor& iRoot,
    _MIN_ TU32 ZOrder);
  /// @brief Get Ui Object as a binder.
  MDY_NODISCARD std::optional<DDyUiBinder> GetUiObject(_MIN_ const std::string& iUiName);
  /// @brief Remove UI Widget.
  MDY_NODISCARD EDySuccess RemoveUiObject(_MIN_ const std::string& iUiName);
  /// @brief Clear & Remove Ui general object list.
  void ClearGeneralUiObjectList();

  /// @brief Get activated UI widget list.
  MDY_NODISCARD std::vector<NotNull<FDyUiWidget*>>& GetActivatedUiWidgetList() noexcept; 

  /// @brief Bind (Enroll) active Ui object (widget) into internal container.
  /// This function must be called in `FDyUiWidget`.
  void BindActiveUiObject(_MIN_ FDyUiWidget& iRefWidget);
  /// @brief Unbind deactivated ui object component. This function must be called in `FDyUiWidget`.
  MDY_NODISCARD EDySuccess UnbindActiveUiObject(_MIN_ FDyUiWidget& iRefWidget);

private:
  std::unique_ptr<FDyUiWidget> mDebugUi   = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyUiWidget> mLoadingUi = MDY_INITIALIZE_NULL;

  /// @brief Ui Widget for general UI map.
  TUiWidgetMap mGeneralUiWidgetMap {};
  /// @brief Activated UI widget ptr list.
  std::vector<NotNull<FDyUiWidget*>> mPtrActivatedGeneralUiWidgetList {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTENRAL_WORLD_FDYWORLDUICONTAINER_H