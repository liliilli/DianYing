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
};

void to_json(nlohmann::json& oJson, const PDyRenderPipelineInstanceMetaInfo& iMeta);
void from_json(const nlohmann::json& iJson, PDyRenderPipelineInstanceMetaInfo& oMeta);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_PMetaInfoRenderPipeline_H