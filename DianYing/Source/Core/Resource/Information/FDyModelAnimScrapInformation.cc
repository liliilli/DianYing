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
#include <Dy/Core/Resource/Information/FDyModelAnimScrapInformation.h>
#include <Dy/Meta/Information/MetaInfoModelAnim.h>
#include <Dy/Helper/Library/HelperFilesystem.h>

namespace dy
{

FDyModelAnimScrapInformation::FDyModelAnimScrapInformation(_MIN_ const PDyModelAnimInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName}
{
  MDY_ASSERT_FORCE(DyFsIsFileExist(metaInfo.mExternalPath) == true, "Animation scrap file is not exist.");
  
  // Open file.
  std::FILE* fdFile = std::fopen(metaInfo.mExternalPath.c_str(), "rb");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(fdFile), "Unexpected error occurred.");
  
  // Open `mAnimationHeader` of given animation. 
  std::fread(&this->mAnimation.mAnimationHeader, sizeof(decltype(mAnimation)::mAnimationHeader), 1, fdFile);

  // Get animation node count, resize it.
  TU32 numAnimNode;
  std::fread(&numAnimNode, sizeof(TU32), 1, fdFile);
  this->mAnimation.mAnimationNodeList.resize(numAnimNode);

  // Open `mAnimationNodeList` of given animation.
  for (TU32 i = 0; i < numAnimNode; ++i)
  {
    // Read `mExportSkeleton` bone id.
    auto& refAnimNode = this->mAnimation.mAnimationNodeList[i];
    std::fread(&refAnimNode.mSkeletonBoneId, sizeof(TU32), 1, fdFile);

    // Get the number of position, rotation (xyzw), and scale channels and resize them.
    TU32 numPosition, numRotation, numScale;
    std::fread(&numPosition, sizeof(TU32), 1, fdFile);
    std::fread(&numRotation, sizeof(TU32), 1, fdFile);
    std::fread(&numScale, sizeof(TU32), 1, fdFile);
    refAnimNode.mPositionList.resize(numPosition);
    refAnimNode.mRotationList.resize(numRotation);
    refAnimNode.mScaleList.resize(numScale);

    // Read position list of refAnimNode.
    for (auto& refPos : refAnimNode.mPositionList)
    {
      std::fread(&refPos.mStartSecond, sizeof(refPos.mStartSecond), 1, fdFile);
      std::fread(&refPos.mTranslate[0], sizeof(TF32), 3, fdFile);
    }
    // Read rotation (x, y, z, w) quaternion list of refAnimNode.
    for (auto& refRot : refAnimNode.mRotationList)
    {
      std::fread(&refRot, sizeof(refRot), 1, fdFile);
    }
    // Read scaling list of refAnimNode.
    for (auto& refScl : refAnimNode.mScaleList)
    {
      std::fread(&refScl.mStartSecond, sizeof(refScl.mStartSecond), 1, fdFile);
      std::fread(&refScl.mScale[0], sizeof(float), 3, fdFile);
    }
  }
  std::fclose(fdFile);
}

const std::string& FDyModelAnimScrapInformation::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

} /// ::dy namespace