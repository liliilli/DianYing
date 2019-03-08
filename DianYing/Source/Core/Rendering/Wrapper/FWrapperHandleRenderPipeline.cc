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

#include <Dy/Core/Rendering/Wrapper/FWrapperHandleRenderPipeline.h>
#include <Dy/Meta/Information/MetaInfoRenderPipeline.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Core/Rendering/Wrapper/FWrapperRenderItem.h>
#include <Dy/Core/Rendering/Wrapper/FWrapperRenderPipeline.h>
#include <Dy/Management/IO/MetaInfoManager.h>

namespace dy
{

FWrapperHandleRenderPipeline::FWrapperHandleRenderPipeline(const PDyRenderPipelineInstanceMetaInfo& iMetaInfo)
  : mSpecifierName{iMetaInfo.mSpecifierName},
    mUuid{iMetaInfo.mUuid}
{
  auto& managerRender = MDyRendering::GetInstance();

  auto* ptrRenderPipeline = managerRender.GetRenderPipeline(iMetaInfo.mSpecifierName);
  ptrRenderPipeline->AttachHandle(this->mHandleRenderPipeline);

  this->mHandleRenderItems.resize(iMetaInfo.mLocalRenderItemNames.size());
  for (size_t i = 0, size = iMetaInfo.mLocalRenderItemNames.size(); i < size; ++i)
  {
    MDY_ASSERT_FORCE(managerRender.HasRenderItem(iMetaInfo.mLocalRenderItemNames[i]) == true);
    auto* ptrRenderItem = managerRender.GetRenderItem(iMetaInfo.mLocalRenderItemNames[i]);
    MDY_ASSERT_FORCE(ptrRenderItem != nullptr);

    ptrRenderItem->AttachHandle(this->mHandleRenderItems[i]);
  }

  this->mHandleRenderPipelines.reserve(iMetaInfo.mChildPipelineNames.size());
  const auto& managerMetaInfo = MDyMetaInfo::GetInstance();
  for (const auto& childRenderPipelineName : iMetaInfo.mChildPipelineNames)
  {
    auto& metaRenderPipeline = managerMetaInfo.GetRenderPipeline(childRenderPipelineName);
    this->mHandleRenderPipelines.emplace_back(metaRenderPipeline);
  }
}

bool FWrapperHandleRenderPipeline::IsActivated() const noexcept
{
  return this->mIsActivated;
}

void FWrapperHandleRenderPipeline::Activate(bool iIsActivated) noexcept
{
  this->mIsActivated = iIsActivated;
}

void FWrapperHandleRenderPipeline::TryRender()
{
  if (this->mHandleRenderPipeline->OnPreRenderCheckCondition() == DY_SUCCESS)
  {
    this->mHandleRenderPipeline->OnSetupRenderingSetting();

    // Iterate child pipelines.
    for (auto& childRenderPipeline : this->mHandleRenderPipelines)
    {
      childRenderPipeline.TryRender();
    }

    // And render local render item.
    for (auto& localRenderItem : this->mHandleRenderItems)
    {
      if (localRenderItem->OnPreRenderCheckCondition() == DY_SUCCESS)
      {
        localRenderItem->OnSetupRenderingSetting();
        localRenderItem->OnRender();
        localRenderItem->OnReleaseRenderingSetting();
        localRenderItem->OnPostRender();
      }
      else
      {
        localRenderItem->OnFailedCheckCondition();
      }
    }

    this->mHandleRenderPipeline->OnReleaseRenderingSetting();
    this->mHandleRenderPipeline->OnPostRender();
  }
  else
  {
    this->mHandleRenderPipeline->OnFailedCheckCondition();
  }
}

const std::string& FWrapperHandleRenderPipeline::GetName() const noexcept
{
  return this->mSpecifierName;
}

const DUuid& FWrapperHandleRenderPipeline::GetUuid() const noexcept
{
  return this->mUuid;
}

} /// ::dy namespace