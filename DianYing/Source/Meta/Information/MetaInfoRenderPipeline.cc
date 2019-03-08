#include <precompiled.h>
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

#include <Dy/Meta/Information/MetaInfoRenderPipeline.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

bool PDyRenderPipelineInstanceMetaInfo::HasValidChildItems(
  const PDyRenderPipelineInstanceMetaInfo& iRenderPipeline)
{
  const auto& managerMeta = MDyMetaInfo::GetInstance();
  for (const auto& childPipelineName : iRenderPipeline.mChildPipelineNames)
  {
    if (managerMeta.IsRenderPipelineExist(childPipelineName) == false)
    {
      DyPushLogError(
        "Failed to create render pipeline, {}. Child pipeline {} meta information is not exist.",
        iRenderPipeline.mSpecifierName, childPipelineName);
      return false;
    }

    // Recursion...
    auto& childRenderPipeline = managerMeta.GetRenderPipeline(childPipelineName);
    if (HasValidChildItems(childRenderPipeline) == false)
    {
      DyPushLogError("Failed to create render pipeline, {}.", iRenderPipeline.mSpecifierName);
      return false;
    }
  }

  for (const auto& localRenderItemName : iRenderPipeline.mLocalRenderItemNames)
  {
    if (managerMeta.IsRenderItemExist(localRenderItemName) == false)
    {
      DyPushLogError(
        "Failed to create render pipeline, {}. Local render item {} meta information is not exist.",
        iRenderPipeline.mSpecifierName, localRenderItemName);
      return false;
    }
  }

  return true;
}

std::optional<std::unordered_set<std::string>> 
PDyRenderPipelineInstanceMetaInfo::GetAllChildPipelineNames(
  const PDyRenderPipelineInstanceMetaInfo& iRenderPipeline)
{
  std::unordered_set<std::string> resultNameList;

  const auto& managerMeta = MDyMetaInfo::GetInstance();
  for (const auto& childPipelineName : iRenderPipeline.mChildPipelineNames)
  {
    // If not exist, just return nullopt as failure.
    if (managerMeta.IsRenderPipelineExist(childPipelineName) == false) { return std::nullopt; }

    // If exist, emplace_back with pipeline name.
    resultNameList.emplace(childPipelineName);
    // And add new children render pipeline recursively.
    auto result = GetAllChildPipelineNames(managerMeta.GetRenderPipeline(childPipelineName));
    if (result.has_value() == false) { return std::nullopt; }

    // If result is valid, (all children render pipeline are valid on Dy system)
    // insert it to resultNameList.
    resultNameList.insert(MDY_BIND_BEGIN_END(result.value()));
  }

  return resultNameList;
}

std::optional<std::unordered_set<std::string>> 
PDyRenderPipelineInstanceMetaInfo::GetAllRenderItemNames(
  const PDyRenderPipelineInstanceMetaInfo& iRenderPipeline)
{
  std::unordered_set<std::string> resultNameList;
  const auto& managerMeta = MDyMetaInfo::GetInstance();

  for (const auto& renderItemName : iRenderPipeline.mLocalRenderItemNames)
  {
    // If not exist, just return nullopt as failure.
    if (managerMeta.IsRenderItemExist(renderItemName) == false) { return std::nullopt; }

    resultNameList.emplace(renderItemName);
  }

  // Iterate to child render pipeline if exist.
  for (const auto& childPipelineName : iRenderPipeline.mChildPipelineNames)
  {
    // If not exist, just return nullopt as failure.
    if (managerMeta.IsRenderPipelineExist(childPipelineName) == false) { return std::nullopt; }

    // And add new child local render item recursively.
    auto result = GetAllRenderItemNames(managerMeta.GetRenderPipeline(childPipelineName));
    if (result.has_value() == false) { return std::nullopt; }

    // If result is valid, (all children render item are valid on Dy system)
    // insert it to resultNameList.
    resultNameList.insert(MDY_BIND_BEGIN_END(result.value()));
  }

  return resultNameList;
}

void to_json(nlohmann::json& oJson, const PDyRenderPipelineInstanceMetaInfo& iMeta)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& iJson, PDyRenderPipelineInstanceMetaInfo& oMeta)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace