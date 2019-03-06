#ifndef GUARD_DY_CORE_RENDERING_INTERFACE_IRENDERPIPELINE_H
#define GUARD_DY_CORE_RENDERING_INTERFACE_IRENDERPIPELINE_H
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

#include <vector>
#include <Dy/Core/Rendering/Interface/IRenderItem.h>

namespace dy
{

/// @interface IRenderPipeline
/// @brief One Render Pipeline interface.
///
/// Rendering procedure will be held like below. \n
/// OnPreRenderCheckCondition() = True => OnSetup => Process rendering => OnRelease => OnPost => Next \n
/// L False ====> OnFailed => ... => Next item.
class IRenderPipeline
{
public:
  IRenderPipeline() = default;
  IRenderPipeline(const IRenderPipeline&) = delete;
  IRenderPipeline& operator=(const IRenderPipeline&) = delete;
  IRenderPipeline(IRenderPipeline&&) noexcept = default;
  IRenderPipeline& operator=(IRenderPipeline&&) noexcept = default;

  virtual ~IRenderPipeline() = 0;

  /// @brief Check pre-render condition, this item could be able to be rendered.
  /// Returned value must be satisfy true when condition is satisfied.
  [[nodiscard]] virtual EDySuccess OnPreRenderCheckCondition() = 0;

  /// @brief If `OnPreRenderCheckCondition` is returned with DY_FAILURE,
  /// This function will be executed.
  virtual void OnFailedCheckCondition() = 0;

  /// @brief Called prior to processing stored pipeline instance and render items.
  /// Recommend to wrtie this function to setup rendering setting.
  virtual void OnSetupRenderingSetting() = 0;

  /// @brief Called afterward processing stored pipeline instance and render items.
  /// Recommend to write this function to release rendering setting.
  virtual void OnReleaseRenderingSetting() = 0;

  /// @brief If rendered, this function also called as final function of each item.
  /// If Condition was false, this function is not called.
  virtual void OnPostRender() = 0;

private:
  std::vector<IRenderPipeline>  mSubRenderPipelines = {};
  std::vector<IRenderItem>      mLocalRenderItems   = {};
};

inline IRenderPipeline::~IRenderPipeline() = default;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_INTERFACE_IRENDERPIPELINE_H
