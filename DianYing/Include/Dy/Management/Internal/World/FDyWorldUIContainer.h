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
class FDyWorldUIContainer final : public FDyNameGenerator
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

  /// @brief Create UI widget.
  /// If 

  /// @brief Get activated UI widget list.
  MDY_NODISCARD std::vector<NotNull<FDyUiWidget*>>& GetActivatedUiWidgetList() noexcept; 

private:
  std::unique_ptr<FDyUiWidget> mDebugUi   = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyUiWidget> mLoadingUi = MDY_INITIALIZE_NULL;

  /// @brief Ui Widget for general UI map.
  TUiWidgetMap              mGeneralUiWidgetMap {};
  /// @brief Activated UI widget ptr list.
  std::vector<NotNull<FDyUiWidget*>> mPtrActivatedGeneralUiWidgetList {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTENRAL_WORLD_FDYWORLDUICONTAINER_H