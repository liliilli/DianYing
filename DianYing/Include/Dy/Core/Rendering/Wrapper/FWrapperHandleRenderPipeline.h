#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_FWrapperHandleRenderPipeline_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_FWrapperHandleRenderPipeline_H
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

#include <Dy/Helper/Internal/FProxyHandler.h>
#include <Dy/Core/Rendering/Interface/IRenderPipeline.h>
#include <Dy/Helper/Type/DUuid.h>
#include <Dy/Helper/Internal/FProxyHandle.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyRenderPipelineInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation 
//!

namespace dy
{

/// @class FWrapperHandleRenderPipeline
/// @brief Actual render pipeline instance type that have handles of render pipeline and render item.
/// This instance manages render pipeline order recursively.
class FWrapperHandleRenderPipeline final
{
public:
  FWrapperHandleRenderPipeline(const PDyRenderPipelineInstanceMetaInfo& iMetaInfo);

  /// @brief Try render with constructured pipeline.
  void TryRender();

  /// @brief Get specifier name of this render pipeline.
  MDY_NODISCARD const std::string& GetName() const noexcept;

  /// @brief Get Uuid of this render pipeline.
  MDY_NODISCARD const DUuid& GetUuid() const noexcept;

  /// @brief Get activation flag of this entry render pipeline instance.
  MDY_NODISCARD bool IsActivated() const noexcept;

  /// @brief Set activation flag of this entry render pipeline instance.
  void Activate(bool iIsActivated) noexcept;

private:
  /// @brief
  FProxyHandle<IRenderPipeline> mHandleRenderPipeline;

  /// @brief
  std::vector<FWrapperHandleRenderPipeline> mHandleRenderPipelines;
  /// @brief
  std::vector<FProxyHandle<IRenderItem>>    mHandleRenderItems;

  /// @brief Specifier string of entry render pipeline.
  std::string mSpecifierName;
  /// @brief Uuid of entry render pipeline.
  DUuid       mUuid;
  /// @brief Activation flag.
  bool        mIsActivated = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_FWrapperHandleRenderPipeline_H
