#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Resource/Information/FDyModelSkeletonInformation.h>
#include <Dy/Meta/Information/MetaInfoModelSkeleton.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Library/HelperFilesystem.h>

namespace dy
{

FDyModelSkeletonInformation::FDyModelSkeletonInformation(_MIN_ const PDyModelSkelInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName}
{
  if (metaInfo.mIsCompressed == true)
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  }

  MDY_ASSERT_MSG_FORCE(IsFileExist(metaInfo.mExternalPath) == true, "Skeleton file is not exist.");
  const auto optJsonSkeleton = json::GetAtlasFromFile(metaInfo.mExternalPath);
  MDY_ASSERT_MSG_FORCE(optJsonSkeleton.has_value() == true, "Failed to load skeleton file.");

  const auto& jsonSkeleton = optJsonSkeleton.value();
  this->mSkeletonInfo = jsonSkeleton.get<decltype(mSkeletonInfo)>();
}

const std::string& FDyModelSkeletonInformation::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

TU32 FDyModelSkeletonInformation::GetNodeCount() const noexcept
{
  return static_cast<TU32>(this->mSkeletonInfo.mExportedSkeleton.size());
}

TU32 FDyModelSkeletonInformation::GetInputBoneCount() const noexcept
{
  return static_cast<TU32>(this->mSkeletonInfo.mBoneOffsetList.size());
}

const DSkeletonBone& FDyModelSkeletonInformation::GetRefSkeletonNode(_MIN_ TU32 iIndex) const noexcept
{
  MDY_ASSERT_MSG_FORCE(iIndex < this->GetNodeCount(), "Index must be within skeleton bone range.");
  return this->mSkeletonInfo.mExportedSkeleton[iIndex];
}

std::vector<TU32> FDyModelSkeletonInformation::GetChildrenNodeIdList(_MIN_ TI32 iIndex) const noexcept
{
  std::vector<TU32> childrenIdResult;
  for (TU32 i = 0, num = static_cast<TU32>(this->mSkeletonInfo.mExportedSkeleton.size()); i < num; ++i)
  {
    if (this->mSkeletonInfo.mExportedSkeleton[i].mParentSkeletonBoneIndex == iIndex) { childrenIdResult.emplace_back(i); }
  }

  return childrenIdResult;
}

const DMat4& FDyModelSkeletonInformation::GetRootInverseTransform() const noexcept
{
  return this->mSkeletonInfo.mSkeletonRootInverseTransform;
}

const DMat4& FDyModelSkeletonInformation::GetOffsetMatrixOfBone(_MIN_ TU32 iIndex) const noexcept
{
  return this->mSkeletonInfo.mBoneOffsetList[iIndex].mBoneOffsetMatrix;
}

const DSkeletonBone& FDyModelSkeletonInformation::GetRefSkeletonNodeFromBoneOffsetId(_MIN_ TU32 iIndex) const noexcept
{
  return this->mSkeletonInfo.mExportedSkeleton[this->mSkeletonInfo.mBoneOffsetList[iIndex].mIndexSkeletonNode];
}

const TI32 FDyModelSkeletonInformation::GetSkeletonNodeIdFromBoneOffsetId(TU32 iIndex) const noexcept
{
  return this->mSkeletonInfo.mBoneOffsetList[iIndex].mIndexSkeletonNode;
}

const std::vector<DBoneOffset>& FDyModelSkeletonInformation::GetOffsetBoneList() const noexcept
{
  return this->mSkeletonInfo.mBoneOffsetList;
}

} /// ::dy namespace