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
#include <Dy/Helper/Type/Quaternion.h>

namespace
{

constexpr float defaultTimeDuration = 30.f;

} /// ::unnanemd namespace

namespace dy
{

CDyModelResource::~CDyModelResource()
{
  // Release all resource bind to this instance.
  if (this->__mLinkedModelInformationPtr) { this->__mLinkedModelInformationPtr->__pfResetModelResourceLink(); }
  this->mMeshResource.clear();
}

void CDyModelResource::UpdateBoneAnimationTransformList(float elapsedTime)
{
  // Get bone final transform list from informations's aiScene.
  auto& modelInformation = *this->__mLinkedModelInformationPtr;
  const auto& animation0 = modelInformation.mAnimationInformations[0];

  auto ticksPerSecond = animation0.mTickPerSecond;
  if (ticksPerSecond == 0.f)
  {
    ticksPerSecond = defaultTimeDuration;
  }

  // Calculate animation timepoint from animation start.
  const float animationTimePoint = std::fmod(elapsedTime * ticksPerSecond, static_cast<float>(animation0.mDuration));
  pReadNodeHierarchy(animationTimePoint, modelInformation, modelInformation.mRootBoneNode, modelInformation.mGlobalTransform);
}

const std::vector<DDyGeometryBoneInformation>&
CDyModelResource::GetModelAnimationTransformMatrixList() const noexcept
{
  return this->__mLinkedModelInformationPtr->mOverallModelBoneInformations;
}

void CDyModelResource::pReadNodeHierarchy(float animationElapsedTime, DDyModelInformation& modelInfo,
                                          const DMoeBoneNodeInformation& boneNode, const DDyMatrix4x4& parentTransform)
{
  using TNodeAnim = NotNull<const DMoeAnimationInformation::DAnimChannel*>;

  ///
  /// @brief
  /// @todo WRITE DOCUMENT
  ///
  static auto FindNodeAnimationChannel = [](const std::string& nodeAnimName, const DMoeAnimationInformation& pAnimation) -> std::optional<TNodeAnim>
  {
    for (TU32 i = 0; i < pAnimation.mAnimationChannels.size(); i++)
    {
      const auto& pNodeAnim = pAnimation.mAnimationChannels[i];
      if (nodeAnimName == pNodeAnim.mName) { return DyMakeNotNull(&pNodeAnim); }
    }
    /// else case
    return std::nullopt;
  };

  ///
  /// @brief
  /// @todo WRITE DOCUMENT
  ///
  static auto CalcInterpolatedScaling = [](float animElapsedTime, TNodeAnim nodeAnimChannel) -> DDyVector3
  {
    const auto scalingKeySize = static_cast<TI32>(nodeAnimChannel->mScalingKeys.size());

    PHITOS_ASSERT(scalingKeySize > 0, "nodeAnimChannel must have at least one scaling keys");
    if (scalingKeySize == 1) { return nodeAnimChannel->mScalingKeys[0]; }

    // Get key frame iterating and verifying running time.
    TI32 scalingIndex = MDY_INITIALIZE_DEFUINT;
    for (TI32 i = 0; i < scalingKeySize - 1; i++)
    {
      if (animElapsedTime < nodeAnimChannel->mScalingTime[i + 1])
      {
        scalingIndex = i;
        break;
      }
    }

    // Calculate factor for scale value linear interpolation between key and next key.
    PHITOS_ASSERT(scalingIndex + 1 < scalingKeySize, "scalingIndex out of bound.");
    const TU32 nextScalingIndex     = scalingIndex + 1;
    const auto deltaTimeFromFrames  = nodeAnimChannel->mScalingTime[nextScalingIndex] - nodeAnimChannel->mScalingTime[scalingIndex];

    const auto animationFactor      = (animElapsedTime - nodeAnimChannel->mScalingTime[scalingIndex]) / deltaTimeFromFrames;
    PHITOS_ASSERT(animationFactor >= 0.0f && animationFactor <= 1.0f, "Animation factor must be from 0.0f to 1.0f");

    // Get interpolated scaling vector (x, y, z)
    const aiVector3D& startScaling  = nodeAnimChannel->mScalingKeys[scalingIndex];
    const aiVector3D& endScaling    = nodeAnimChannel->mScalingKeys[nextScalingIndex];
    return startScaling + (endScaling - startScaling) * animationFactor;
  };

  ///
  /// @brief
  /// @todo WRITE DOCUMENT
  ///
  static auto CalcInterpolatedRotation = [](float animElapsedTime, TNodeAnim nodeAnimChannel) -> DDyQuaternion
  {
    const auto rotationKeySize = static_cast<TI32>(nodeAnimChannel->mRotationKeys.size());

    // We need at least two values to interpolate...
    PHITOS_ASSERT(rotationKeySize > 0, "nodeAnimChannel must have at least one rotation keys");
    if (rotationKeySize == 1) { return nodeAnimChannel->mRotationKeys[0]; }

    // Get key frame iterating and verifying running time.
    TI32 rotationIndex = MDY_INITIALIZE_DEFUINT;
    for (TI32 i = 0; i < rotationKeySize - 1; i++)
    {
      if (animElapsedTime < nodeAnimChannel->mRotationTime[i + 1])
      {
        rotationIndex = i;
        break;
      }
    }

    // Calculate factor for rotation quaternion slerp (maybe?) between key and next key.
    PHITOS_ASSERT(rotationIndex + 1 < rotationKeySize, "rotationIndex out of bound.");
    const TU32 nextRotationIndex    = rotationIndex + 1;
    const auto deltaTimeFromFrames  = nodeAnimChannel->mRotationTime[nextRotationIndex] - nodeAnimChannel->mRotationTime[rotationIndex];

    const auto animationFactor      = (animElapsedTime - nodeAnimChannel->mRotationTime[rotationIndex]) / deltaTimeFromFrames;
    PHITOS_ASSERT(animationFactor >= 0.0f && animationFactor <= 1.0f, "Animation factor must be from 0.0f to 1.0f");

    // Get interpolated rotation quaternion (v (xi + yj + zk), w)
    const auto& startQuaternion = nodeAnimChannel->mRotationKeys[rotationIndex];
    const auto& endQuaternion   = nodeAnimChannel->mRotationKeys[nextRotationIndex];

    return glm::slerp(startQuaternion.pGetQuaternion(), endQuaternion.pGetQuaternion(), animationFactor);
  };

  ///
  /// @brief
  /// @todo WRITE DOCUMENT
  ///
  static auto CalcInterpolatedPosition = [](float animElapsedTime, TNodeAnim nodeAnimChannel) -> DDyVector3
  {
    const auto positionKeySize = static_cast<TI32>(nodeAnimChannel->mPositionKeys.size());

    // We need at least two values to interpolate...
    PHITOS_ASSERT(positionKeySize > 0, "nodeAnimChannel must have at least one position keys");
    if (positionKeySize == 1) { return nodeAnimChannel->mPositionKeys[0]; }

    // Get key frame iterating and verifying running time.
    TI32 positionIndex = MDY_INITIALIZE_DEFUINT;
    for (TI32 i = 0; i < positionKeySize - 1; i++)
    {
      if (animElapsedTime < nodeAnimChannel->mPositionTime[i + 1])
      {
        positionIndex = i;
        break;
      }
    }

    // Calculate factor for position value linear interpolation between key and next key.
    PHITOS_ASSERT(positionIndex + 1 < positionKeySize, "positionIndex out of bound.");
    const TU32 nextPositionIndex    = positionIndex + 1;
    const auto deltaTimeFromFrames  = nodeAnimChannel->mPositionTime[nextPositionIndex] - nodeAnimChannel->mPositionTime[positionIndex];

    const auto animationFactor      = (animElapsedTime - nodeAnimChannel->mPositionTime[positionIndex]) / deltaTimeFromFrames;
    PHITOS_ASSERT(animationFactor >= 0.0f && animationFactor <= 1.0f, "Animation factor must be from 0.0f to 1.0f");

    // Get interpolated position (x, y, z)
    const aiVector3D& Start = nodeAnimChannel->mPositionKeys[positionIndex];
    const aiVector3D& End = nodeAnimChannel->mPositionKeys[nextPositionIndex];
    return Start + animationFactor * (End - Start);
  };

  //!
  //! Function Body
  //!

  // nodeName == boneName == animChannelName.
  const auto&  aiAnimation = modelInfo.mAnimationInformations[0]; // 하나밖에 없다고 가정함.
  DDyMatrix4x4 finalModel  = DDyMatrix4x4::IdentityMatrix();

  if (auto nodeAnimationChannel = FindNodeAnimationChannel(boneNode.mName, aiAnimation);
      nodeAnimationChannel.has_value())
  {
    // Interpolate scaling and generate scaling transformation matrix
    const DDyMatrix4x4  scalingMatrix      = DDyMatrix4x4::CreateWithScale(CalcInterpolatedScaling(animationElapsedTime, nodeAnimationChannel.value()));
    // Interpolate rotation and generate rotation transformation matrix
    const DDyMatrix4x4  rotationMatrix     = CalcInterpolatedRotation(animationElapsedTime, nodeAnimationChannel.value()).GetRotationMatrix4x4();
    // Interpolate translation and generate translation transformation matrix
    const DDyMatrix4x4  translationMatrix  = DDyMatrix4x4::CreateWithTranslation(CalcInterpolatedPosition(animationElapsedTime, nodeAnimationChannel.value()));

    // Combine the above transformations
    finalModel = parentTransform.Multiply(translationMatrix).Multiply(rotationMatrix).Multiply(scalingMatrix);

    if (const auto it = modelInfo.mBoneIdMap.find(boneNode.mName); it != modelInfo.mBoneIdMap.end())
    {
      const TU32 boneId               = it->second;
      const auto& boneOffsetMatrixRef = modelInfo.mOverallModelBoneInformations[boneId].mBoneOffsetMatrix;
      auto& finalTransformationRef    = modelInfo.mOverallModelBoneInformations[boneId].mFinalTransformation;
      finalTransformationRef          = finalModel.Multiply(boneOffsetMatrixRef);
    }
  }

  for (const auto& mChildrenNode : boneNode.mChildrenNodes)
  {
    pReadNodeHierarchy(animationElapsedTime, modelInfo, mChildrenNode, finalModel);
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
    if (submeshInformation.GetInformation().mIsEnabledSkeletalAnimation)
    {
      this->mIsEnabledModelSkeletalAnimation = true;
      MDY_LOG_DEBUG_D("{} | Model has bone so can be animated. | Model name : {}", "CDyModelResource::pInitializeModelResource", modelInformation.mModelName);
    }
  }
  return DY_SUCCESS;
}

} /// ::dy namespace