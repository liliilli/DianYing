#ifndef GUARD_DY_MANAGEMENT_INTENRAL_WORLD_FDYWORLDUICONTAINER_H
#define GUARD_DY_MANAGEMENT_INTENRAL_WORLD_FDYWORLDUICONTAINER_H
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

#include <Dy/Element/Canvas/Widget.h>

namespace dy
{

///
/// @class FDyWorldUIContainer
/// @brief UI Container type.
///
class FDyWorldUIContainer final
{
public:
  /// @brief Try create debug ui layout to screen as highest priority. (10xx) \n
  /// If debug ui is already spawned, just return DY_FAILURE, or DY_SUCCESS.
  MDY_NODISCARD EDySuccess  TryCreateDebugUi();
  /// @brief Check debug Ui is now on exist.
  MDY_NODISCARD bool        IsDebugUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already removed, just return DY_FAILURE.
  MDY_NODISCARD EDySuccess  TryRemoveDebugUi();
  /// @brief Try draw debug ui if exist.
  /// @TODO THIS FUNCTION IS TEMPORARY.
  void TryRenderDebugUi();

  /// @brief Try create loading ui layout. \n
  /// If Loading UI Widget meta information is not exist, just return DY_FAILURE doing nothing.
  MDY_NODISCARD EDySuccess  TryCreateLoadingUi();
  /// @brief Check loading ui is now on exist.
  MDY_NODISCARD bool        IsLoadingUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already remove or not exist, return DY_FAILURE.
  MDY_NODISCARD EDySuccess  TryRemoveLoadingUi();
  /// @brief Try draw loading ui if exist.
  /// @TODO THIS FUNCTION IS TEMPORARY.
  void TryRenderLoadingUi();

private:
  std::unique_ptr<FDyUiWidget> mDebugUi   = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyUiWidget> mLoadingUi = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTENRAL_WORLD_FDYWORLDUICONTAINER_H