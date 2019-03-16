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

/// Header file
#include <Dy/Core/Resource/Information/FDyModelInformation.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Builtin/Constant/GeneralValue.h>

namespace dy
{

FDyModelInformation::FDyModelInformation(_MIN_ const PDyModelInstanceMetaInfo& metaInfo) 
  : mSpecifierName{metaInfo.mSpecifierName},
    mModelTransform{metaInfo.mTransform}
{
  for (const auto& [meshSpecifier, materialSpecifier, isInstanced] : metaInfo.mMeshList)
  {
    mPairInstancingflag.emplace_back(isInstanced);
    SafeUniquePtrEmplaceBack(this->mMeshInformations, meshSpecifier);

    if (isInstanced == true)
    {
      SafeUniquePtrEmplaceBack(this->mMaterialInformations, materialSpecifier + kInstancingPostfix);
    }
    else
    {
      SafeUniquePtrEmplaceBack(this->mMaterialInformations, materialSpecifier);
    }
  }

  if (metaInfo.mSkeleton.mIsUsingSkeleton == true)
  {
    this->mBinderSkeleton.TryRequireResource(metaInfo.mSkeleton.mSkeletonSpecifier);
  }
}

const std::vector<bool>& FDyModelInformation::GetInstancingFlags() const noexcept
{
  return this->mPairInstancingflag;
}

} /// ::dy namespace