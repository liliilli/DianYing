#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Core/Component/Resource/ModelResource.h>

#include <assimp/scene.h>

#include <Dy/Core/Component/Information/ModelInformation.h>
#include <Dy/Core/Component/Resource/SubmeshResource.h>
#include <Dy/Management/LoggingManager.h>
#include "Dy/Helper/Type/Quaternion.h"

namespace dy
{

CDyModelResource::~CDyModelResource()
{
  // Release all resource bind to this instance.
  if (this->__mLinkedModelInformationPtr) { this->__mLinkedModelInformationPtr->__pfResetModelResourceLink(); }

  this->mMeshResource.clear();
}

const std::vector<std::unique_ptr<CDySubmeshResource>>& CDyModelResource::GetSubmeshResources() const noexcept
{
  return this->mMeshResource;
}

void CDyModelResource::GetBoneTransformLists(float runningTime, std::vector<DDyMatrix4x4>& transforms)
{
  DDyMatrix4x4 identityMatrix = DDyMatrix4x4::IdentityMatrix();
  const auto& modelScene = *this->__mLinkedModelInformationPtr->mInternalModelGeometryResource;
  const auto* animation0 = modelScene.mAnimations[0];

  float ticksPerSecond = static_cast<float>(animation0->mTicksPerSecond);
  if (ticksPerSecond == 0.f)
  {
    constexpr float defaultTimeDuration = 30.f;
    ticksPerSecond = defaultTimeDuration;
  }
  const float timeInTicks = runningTime * ticksPerSecond;
  const float animationTime = std::fmod(timeInTicks, static_cast<float>(animation0->mDuration));

  pReadNodeHierarchy(animationTime, *modelScene.mRootNode, *this->__mLinkedModelInformationPtr, identityMatrix);

  transforms.clear();
  transforms.reserve(this->__mLinkedModelInformationPtr->mModelBoneTotalCount);

  for (TI32 i = 0; i < this->__mLinkedModelInformationPtr->mModelBoneTotalCount; ++i)
  {
    transforms.emplace_back(this->__mLinkedModelInformationPtr->mOverallModelBoneInformations[i].mFinalTransformation);
  }
}

void CDyModelResource::SetBoneTransformLists(const std::vector<DDyMatrix4x4>& transforms)
{
  const auto transformSize = static_cast<TI32>(transforms.size());
  this->mOverallModelAnimationMatrix.clear();
  this->mOverallModelAnimationMatrix.insert(this->mOverallModelAnimationMatrix.begin(), MDY_BIND_BEGIN_END(transforms));
}

void CDyModelResource::pReadNodeHierarchy(float runningTime, const aiNode& nodeCursor, DDyModelInformation& modelInfo, const DDyMatrix4x4& parentTransform)
{
  ///
  /// @brief
  /// @todo WRITE DOCUMENT
  ///
  static auto FindNodeAnim = [](const std::string& findNodeName, const aiAnimation* pAnimation) -> const aiNodeAnim*
  {
    for (TU32 i = 0; i < pAnimation->mNumChannels; i++)
    {
      const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
      if (findNodeName == pNodeAnim->mNodeName.C_Str())
      {
        return pNodeAnim;
      }
    }
    return nullptr;
  };

  ///
  /// @brief
  /// @todo WRITE DOCUMENT
  ///
  static auto CalcInterpolatedScaling = [](float runningTime, const aiNodeAnim* nodeAnimation) -> DDyVector3
  {
    PHITOS_ASSERT(nodeAnimation->mNumScalingKeys > 0, "nodeAnimation must have at least one scaling keys");
    if (nodeAnimation->mNumScalingKeys == 1) { return nodeAnimation->mScalingKeys[0].mValue; }

    // Get key frame iterating and verifying running time.
    TU32 scalingIndex = MDY_NOT_INITIALIZED_0;
    for (TU32 i = 0; i < nodeAnimation->mNumScalingKeys - 1; i++)
    {
      if (runningTime < static_cast<float>(nodeAnimation->mScalingKeys[i + 1].mTime))
      {
        scalingIndex = i;
        break;
      }
    }

    // Calculate factor for scale value linear interpolation between key and next key.
    PHITOS_ASSERT(scalingIndex + 1 < nodeAnimation->mNumScalingKeys, "scalingIndex out of bound.");
    const TU32 nextScalingIndex = scalingIndex + 1;
    const auto deltaTime = static_cast<float>(nodeAnimation->mScalingKeys[nextScalingIndex].mTime - nodeAnimation->mScalingKeys[scalingIndex].mTime);
    const auto factor    = (runningTime - static_cast<float>(nodeAnimation->mScalingKeys[scalingIndex].mTime)) / deltaTime;
    PHITOS_ASSERT(factor >= 0.0f && factor <= 1.0f, "Animation factor must be from 0.0f to 1.0f");

    // Get interpolated scaling vector (x, y, z)
    const aiVector3D& startScaling  = nodeAnimation->mScalingKeys[scalingIndex].mValue;
    const aiVector3D& endScaling    = nodeAnimation->mScalingKeys[nextScalingIndex].mValue;
    return startScaling + (endScaling - startScaling) * factor;
  };

  ///
  /// @brief
  /// @todo WRITE DOCUMENT
  ///
  static auto CalcInterpolatedRotation = [](float runningTime, const aiNodeAnim* nodeAnimation) -> DDyQuaternion
  {
    // We need at least two values to interpolate...
    PHITOS_ASSERT(nodeAnimation->mNumRotationKeys > 0, "nodeAnimation must have at least one rotation keys");
    if (nodeAnimation->mNumRotationKeys == 1) { return nodeAnimation->mRotationKeys[0].mValue; }

    // Get key frame iterating and verifying running time.
    TU32 rotationIndex = MDY_NOT_INITIALIZED_0;
    for (TU32 i = 0; i < nodeAnimation->mNumRotationKeys - 1; i++)
    {
      if (runningTime < static_cast<float>(nodeAnimation->mRotationKeys[i + 1].mTime))
      {
        rotationIndex = i;
        break;
      }
    }

    // Calculate factor for rotation quaternion slerp (maybe?) between key and next key.
    PHITOS_ASSERT(rotationIndex + 1 < nodeAnimation->mNumRotationKeys, "rotationIndex out of bound.");
    const TU32 nextRotationIndex = rotationIndex + 1;
    const auto dt     = static_cast<float>(nodeAnimation->mRotationKeys[nextRotationIndex].mTime - nodeAnimation->mRotationKeys[rotationIndex].mTime);
    const auto factor = (runningTime - static_cast<float>(nodeAnimation->mRotationKeys[rotationIndex].mTime)) / dt;
    PHITOS_ASSERT(factor >= 0.0f && factor <= 1.0f, "Animation factor must be from 0.0f to 1.0f");

    // Get interpolated rotation quaternion (v (xi + yj + zk), w)
    const aiQuaternion& startQuaternion = nodeAnimation->mRotationKeys[rotationIndex].mValue;
    const aiQuaternion& endQuaternion   = nodeAnimation->mRotationKeys[nextRotationIndex].mValue;

    aiQuaternion outQuaternion;
    aiQuaternion::Interpolate(outQuaternion, startQuaternion, endQuaternion, factor);
    outQuaternion = outQuaternion.Normalize();
    return outQuaternion;
  };

  ///
  /// @brief
  /// @todo WRITE DOCUMENT
  ///
  static auto CalcInterpolatedPosition = [](float runningTime, const aiNodeAnim* nodeAnimation) -> DDyVector3
  {
    // We need at least two values to interpolate...
    PHITOS_ASSERT(nodeAnimation->mNumPositionKeys > 0, "nodeAnimation must have at least one position keys");
    if (nodeAnimation->mNumPositionKeys == 1) { return nodeAnimation->mPositionKeys[0].mValue; }

    // Get key frame iterating and verifying running time.
    TU32 positionIndex = MDY_NOT_INITIALIZED_0;
    for (TU32 i = 0; i < nodeAnimation->mNumRotationKeys - 1; i++)
    {
      if (runningTime < static_cast<float>(nodeAnimation->mPositionKeys[i + 1].mTime))
      {
        positionIndex = i;
        break;
      }
    }

    // Calculate factor for position value linear interpolation between key and next key.
    PHITOS_ASSERT(positionIndex + 1 < nodeAnimation->mNumPositionKeys, "positionIndex out of bound.");
    const TU32 nextPositionIndex = positionIndex + 1;
    const auto dt = static_cast<float>(nodeAnimation->mPositionKeys[nextPositionIndex].mTime - nodeAnimation->mPositionKeys[positionIndex].mTime);
    const auto factor = (runningTime - static_cast<float>(nodeAnimation->mPositionKeys[positionIndex].mTime)) / dt;
    PHITOS_ASSERT(factor >= 0.0f && factor <= 1.0f, "Animation factor must be from 0.0f to 1.0f");

    // Get interpolated position (x, y, z)
    const aiVector3D& Start = nodeAnimation->mPositionKeys[positionIndex].mValue;
    const aiVector3D& End = nodeAnimation->mPositionKeys[nextPositionIndex].mValue;
    return Start + factor * (End - Start);
  };

  //
  const std::string   nodeName  = nodeCursor.mName.C_Str();
  const aiAnimation*  animation = modelInfo.pfGetModelGeometryResource()->mAnimations[0];
  DDyMatrix4x4        nodeTransformation{nodeCursor.mTransformation};

  if (const aiNodeAnim* nodeAnimation = FindNodeAnim(nodeName, animation); nodeAnimation)
  {
    // Interpolate scaling and generate scaling transformation matrix
    const DDyVector3 scalingVector          = CalcInterpolatedScaling(runningTime, nodeAnimation);
    const DDyMatrix4x4 scalingMatrix        = DDyMatrix4x4::CreateWithScale(scalingVector);

    // Interpolate rotation and generate rotation transformation matrix
    const DDyQuaternion rotationQuaternion  = CalcInterpolatedRotation(runningTime, nodeAnimation);
    const DDyMatrix4x4  rotationMatrix      = rotationQuaternion.GetRotationMatrix4x4();

    // Interpolate translation and generate translation transformation matrix
    const DDyVector3    translation         = CalcInterpolatedPosition(runningTime, nodeAnimation);
    const DDyMatrix4x4  translationMatrix   = DDyMatrix4x4::CreateWithTranslation(translation);

    // Combine the above transformations
    nodeTransformation = scalingMatrix.Multiply(rotationMatrix.Multiply(translationMatrix));
  }
  const DDyMatrix4x4 globalTransformationMatrix = parentTransform.Multiply(nodeTransformation);

  if (const auto it = modelInfo.mBoneStringBoneIdMap.find(nodeName); it != modelInfo.mBoneStringBoneIdMap.end())
  {
    const TU32 boneId = it->second;
    auto& finalTransformationRef = modelInfo.mOverallModelBoneInformations[boneId].mFinalTransformation;
    auto& boneOffsetMatrixRef    = modelInfo.mOverallModelBoneInformations[boneId].mBoneOffsetMatrix;
    finalTransformationRef       = modelInfo.mGlobalInverseTransform.Multiply(globalTransformationMatrix.Multiply(boneOffsetMatrixRef));
    //finalTransformationRef       = globalTransformationMatrix.Multiply(boneOffsetMatrixRef);
  }

  for (TU32 i = 0; i < nodeCursor.mNumChildren; i++)
  {
    pReadNodeHierarchy(runningTime, *nodeCursor.mChildren[i], modelInfo, globalTransformationMatrix);
  }
}

EDySuccess CDyModelResource::pInitializeModelResource(const DDyModelInformation& modelInformation)
{
  const auto& submeshInformations = modelInformation.mSubmeshInformations;
  for (const auto& submeshInformation : submeshInformations)
  {
    std::unique_ptr<CDySubmeshResource> meshResource = std::make_unique<CDySubmeshResource>();
    if (meshResource->pfInitializeSubmeshResource(submeshInformation) == DY_FAILURE)
    {
      MDY_LOG_ERROR("{} | Failed to create submesh resource. | Model name : {}",
                    "CDyModelResource::pInitializeModelResource", modelInformation.mModelName);
      return DY_FAILURE;
    }

    this->mMeshResource.emplace_back(std::move(meshResource));
    if (!this->mIsEnabledModelSkeletalAnimation && submeshInformation.GetInformation().mIsEnabledSkeletalAnimation)
    {
      this->mIsEnabledModelSkeletalAnimation = true;
      MDY_LOG_DEBUG_D("{} | Model has bone so can be animated. | Model name : {}",
                      "CDyModelResource::pInitializeModelResource", modelInformation.mModelName);
    }
  }
  return DY_SUCCESS;
}

} /// ::dy namespace