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

  MDY_ASSERT_FORCE(DyFsIsFileExist(metaInfo.mExternalPath) == true, "Skeleton file is not exist.");
  const auto optJsonSkeleton = DyGetJsonAtlasFromFile(metaInfo.mExternalPath);
  MDY_ASSERT_FORCE(optJsonSkeleton.has_value() == true, "Failed to load skeleton file.");

  const auto& jsonSkeleton  = optJsonSkeleton.value();
  this->mSkeletonBoneList   = jsonSkeleton.get<decltype(mSkeletonBoneList)>();
}

const std::string& FDyModelSkeletonInformation::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

} /// ::dy namespace