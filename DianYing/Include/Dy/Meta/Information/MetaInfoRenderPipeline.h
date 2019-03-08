#ifndef GUARD_DY_META_INFORMATION_PMetaInfoRenderPipeline_H
#define GUARD_DY_META_INFORMATION_PMetaInfoRenderPipeline_H
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

#include <unordered_set>
#include <nlohmann/json_fwd.hpp>

#include <Dy/Core/Rendering/Interface/IRenderPipeline.h>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <Dy/Helper/Type/DUuid.h>

namespace dy
{

/// @struct PDyRenderPipelineInstanceMetaInfo
/// @brief Meta information for constructing Rendering pipeline.
struct PDyRenderPipelineInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  /// @brief Name of render pipeline.
  std::string mSpecifierName;
  /// @brief Uuid specifier. This value must be specified and not empty value.
  DUuid       mUuid{true};
  /// @brief Child render-pipeline name. Order will be determined by item order of list.
  std::vector<std::string> mChildPipelineNames = {};
  /// @brief Local render item name. Order will be determined by item order of list.
  std::vector<std::string> mLocalRenderItemNames = {};

  using TPipelineFunction = std::unique_ptr<IRenderPipeline>(*)();
  /// @brief do not touch, this will be handled automatically in Dy System.
  TPipelineFunction mBtInstantiationFunction = nullptr;

  /// @brief Check given render pipeline meta information is valid item
  /// to all local & child render pipeline, by searching metainfo manager.
  ///
  /// This function must not be called prior to initialization of metainfo manager.
  /// and release of manager.
  [[nodiscard]] static bool 
  HasValidChildItems(const PDyRenderPipelineInstanceMetaInfo& iRenderPipeline);

  /// @brief Get all children pipeline names recursively.
  /// If given child pipeline name is not valid on Dy system, just return no value.
  [[nodiscard]] static std::optional<std::unordered_set<std::string>> 
  GetAllChildPipelineNames(const PDyRenderPipelineInstanceMetaInfo& iRenderPipeline);

  /// @brief Get all children and local render item names recursively.
  /// If given child render pipeline and render item not valid on Dy system, 
  /// just return not value.
  [[nodiscard]] static std::optional<std::unordered_set<std::string>>
  GetAllRenderItemNames(const PDyRenderPipelineInstanceMetaInfo& iRenderPipeline);
};

void to_json(nlohmann::json& oJson, const PDyRenderPipelineInstanceMetaInfo& iMeta);
void from_json(const nlohmann::json& iJson, PDyRenderPipelineInstanceMetaInfo& oMeta);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_PMetaInfoRenderPipeline_H