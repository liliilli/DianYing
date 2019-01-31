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

  this->mRateScale = metaInfo.mDetails.mRateScale;
}

const std::string& FDyModelAnimScrapInformation::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

DDyVector3 FDyModelAnimScrapInformation::GetInterpolatedScaling(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const
{
  const auto& refBone = this->mAnimation.mAnimationNodeList[iBoneIndex];
  
  // If `isLooped` is true, we need to mod elasped time to new value within range.
  if (iIsLooped == true)
  {
    iElapsedTime = std::fmod(iElapsedTime, this->mAnimation.mAnimationHeader.mDurationSecond);
  }
  
  // Get start point.
  TU32 idTargetScale = 0;
  const TU32 num = refBone.mScaleList.size();
  // If number is only one, just return with it.
  if (num == 1)
  {
    return refBone.mScaleList.front().mScale;
  }
  
  for (TU32 i = 0; i < num; ++i)
  {
    auto& refScale = refBone.mScaleList[i];
    if (refScale.mStartSecond <= iElapsedTime) { idTargetScale = i; }
  }

  // If idTargetScale is end of scale list.
  // [0  ][1  ][2  ].....[N  ]__[0  ]
  // |<-dt(0) :: v = (dt - 0)/(1 - 0); result = S0 * (1 - v) + S1 * v;
  //                       |<-dt(N) When Looped
  //                       L v = (dt - N) / (elapsed_time - N); result = SN * (1 - v) + S0 * v;
  //                     |<-dt(N) When not looped, just return SN
  if (const TU32 endId = num - 1; idTargetScale == endId)
  {
    if (iIsLooped == false) { return refBone.mScaleList[endId].mScale; }
    else
    {
      auto& refFirstBone = refBone.mScaleList.front();
      auto& refLastBone  = refBone.mScaleList.back();
      const auto v = 
          (iElapsedTime - refLastBone.mStartSecond) 
        / (this->mAnimation.mAnimationHeader.mDurationSecond - refLastBone.mStartSecond);

      return math::Lerp(refLastBone.mScale, refFirstBone.mScale, v);
    }
  }
  else
  {
    auto& refLhs = refBone.mScaleList[idTargetScale];
    auto& refRhs = refBone.mScaleList[idTargetScale + 1];
    const auto v = 
        (iElapsedTime - refLhs.mStartSecond) 
      / (refRhs.mStartSecond - refLhs.mStartSecond);

    return math::Lerp(refLhs.mScale, refLhs.mScale, v);
  }
}

DDyQuaternion FDyModelAnimScrapInformation::GetInterpolatedRotation(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const
{
  const auto& refBone = this->mAnimation.mAnimationNodeList[iBoneIndex];
  
  // If `isLooped` is true, we need to mod elasped time to new value within range.
  if (iIsLooped == true)
  {
    iElapsedTime = std::fmod(iElapsedTime, this->mAnimation.mAnimationHeader.mDurationSecond);
  }
  
  // Get start point.
  TU32 idTargetScale = 0;
  const TU32 num = refBone.mRotationList.size();
  // If number is only one, just return with it.
  if (num == 1)
  {
    const auto& refRot = refBone.mRotationList.front();
    return DDyQuaternion{refRot.mX, refRot.mY, refRot.mZ, refRot.mW};
  }

  for (TU32 i = 0; i < num; ++i)
  {
    auto& refScale = refBone.mRotationList[i];
    if (refScale.mStartSecond <= iElapsedTime) { idTargetScale = i; }
  }

  // Do interpolation.
  // If idTargetScale is end of rotation list.
  if (const TU32 endId = num - 1; idTargetScale == endId)
  {
    if (iIsLooped == false) { return refBone.mScaleList[endId].mScale; }
    else
    {
      auto& refFirstBone = refBone.mRotationList.front();
      auto& refLastBone  = refBone.mRotationList.back();
      const auto v = 
          (iElapsedTime - refLastBone.mStartSecond) 
        / (this->mAnimation.mAnimationHeader.mDurationSecond - refLastBone.mStartSecond);

      // Get s-interpolated quaternion.
      const DDyQuaternion left  = {refLastBone.mX, refLastBone.mY, refLastBone.mZ, refLastBone.mW};
      const DDyQuaternion right = {refFirstBone.mX, refFirstBone.mY, refFirstBone.mZ, refFirstBone.mW};
      return math::Slerp(left, right, v);
    }
  }
  else
  {
    auto& refLhs = refBone.mRotationList[idTargetScale];
    auto& refRhs = refBone.mRotationList[idTargetScale + 1];
    const auto v = 
        (iElapsedTime - refLhs.mStartSecond) 
      / (refRhs.mStartSecond - refLhs.mStartSecond);

    // Get s-interpolated quaternion.
    const DDyQuaternion left  = {refLhs.mX, refLhs.mY, refLhs.mZ, refLhs.mW};
    const DDyQuaternion right = {refRhs.mX, refRhs.mY, refRhs.mZ, refRhs.mW};
    return math::Slerp(left, right, v);
  }
}

DDyVector3 FDyModelAnimScrapInformation::GetInterpolatedPosition(_MIN_ TF32 iElapsedTime, _MIN_ TU32 iBoneIndex, _MIN_ bool iIsLooped) const
{
  const auto& refBone = this->mAnimation.mAnimationNodeList[iBoneIndex];
  
  // If `isLooped` is true, we need to mod elasped time to new value within range.
  if (iIsLooped == true)
  {
    iElapsedTime = std::fmod(iElapsedTime, this->mAnimation.mAnimationHeader.mDurationSecond);
  }
  
  // Get start point.
  TU32 idTargetScale = 0;
  const TU32 num = refBone.mPositionList.size();
  // If number is only one, just return with it.
  if (num == 1) { return refBone.mPositionList.front().mTranslate; }

  for (TU32 i = 0; i < num; ++i)
  {
    auto& refScale = refBone.mPositionList[i];
    if (refScale.mStartSecond <= iElapsedTime) { idTargetScale = i; }
  }

  // If idTargetScale is end of scale list.
  // [0  ][1  ][2  ].....[N  ]__[0  ]
  // |<-dt(0) :: v = (dt - 0)/(1 - 0); result = S0 * (1 - v) + S1 * v;
  //                       |<-dt(N) When Looped
  //                       L v = (dt - N) / (elapsed_time - N); result = SN * (1 - v) + S0 * v;
  //                     |<-dt(N) When not looped, just return SN
  if (const TU32 endId = num - 1; idTargetScale == endId)
  {
    if (iIsLooped == false) { return refBone.mPositionList[endId].mTranslate; }
    else
    {
      auto& refFirstBone = refBone.mPositionList.front();
      auto& refLastBone  = refBone.mPositionList.back();
      const auto v = 
          (iElapsedTime - refLastBone.mStartSecond) 
        / (this->mAnimation.mAnimationHeader.mDurationSecond - refLastBone.mStartSecond);

      return math::Lerp(refLastBone.mTranslate, refFirstBone.mTranslate, v);
    }
  }
  else
  {
    auto& refLhs = refBone.mPositionList[idTargetScale];
    auto& refRhs = refBone.mPositionList[idTargetScale + 1];
    const auto v = 
        (iElapsedTime - refLhs.mStartSecond) 
      / (refRhs.mStartSecond - refLhs.mStartSecond);

    return math::Lerp(refLhs.mTranslate, refLhs.mTranslate, v);
  }
}

const decltype(DDyAnimationSequence::mAnimationNodeList)& 
FDyModelAnimScrapInformation::GetAnimNodeList() const noexcept
{
  return this->mAnimation.mAnimationNodeList;
}

TU32 FDyModelAnimScrapInformation::GetSkeletonBoneId(_MIN_ TU32 iId) const noexcept
{
  return this->mAnimation.mAnimationNodeList[iId].mSkeletonBoneId;
}

TF32 FDyModelAnimScrapInformation::GetRateScale() const noexcept
{
  return this->mRateScale;
}

} /// ::dy namespace