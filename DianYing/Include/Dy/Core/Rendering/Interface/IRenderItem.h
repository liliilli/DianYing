#ifndef GUARD_DY_CORE_RENDERING_INTERFACE_IRENDERITEM_H
#define GUARD_DY_CORE_RENDERING_INTERFACE_IRENDERITEM_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

namespace dy
{

/// @interface IRenderItem
/// @brief Render Item interface.
///
/// Rendering procedure will be held like below. \n
/// OnPreRenderCheckCondition() = True => OnSetup => OnRender => OnRelease => OnPost => Next item. \n
/// L False ====> OnFailed => ... => Next item.
class IRenderItem
{
public:
  IRenderItem() = default;
  IRenderItem(const IRenderItem&) = delete;
  IRenderItem& operator=(const IRenderItem&) = delete;
  IRenderItem(IRenderItem&&) noexcept = default;
  IRenderItem& operator=(IRenderItem&&) noexcept = default;

  virtual ~IRenderItem() = 0;

  /// @brief Check pre-render condition, this item could be able to be rendered.
  /// Returned value must be satisfy true when condition is satisfied.
  [[nodiscard]] virtual EDySuccess OnPreRenderCheckCondition() = 0;

  /// @brief If `OnPreRenderCheckCondition` is returned with DY_FAILURE,
  /// This function will be executed.
  virtual void OnFailedCheckCondition() = 0;

  /// @brief Called prior to call `OnRender`.
  /// Recommend to wrtie this function to setup rendering setting.
  virtual void OnSetupRenderingSetting() = 0;

  /// @brief Render function.
  virtual void OnRender() = 0;

  /// @brief Called afterward calling `OnRender`.
  /// Recommend to write this function to release rendering setting.
  virtual void OnReleaseRenderingSetting() = 0;

  /// @brief If rendered, this function also called as final function of each item.
  /// If Condition was false, this function is not called.
  virtual void OnPostRender() = 0;
};

inline IRenderItem::~IRenderItem() = default;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_INTERFACE_IRENDERITEM_H
