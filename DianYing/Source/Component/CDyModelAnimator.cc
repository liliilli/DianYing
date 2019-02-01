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
#include <Dy/Component/CDyModelAnimator.h>
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>
#include <Dy/Element/Actor.h>
#include <Dy/Component/CDyModelFilter.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Core/Resource/Information/FDyModelAnimScrapInformation.h>
#include <Dy/Core/Resource/Information/FDyModelSkeletonInformation.h>

namespace dy
{

CDyModelAnimator::CDyModelAnimator(_MIN_ FDyActor& actorReference) : ADyGeneralBaseComponent{actorReference}
{ }

EDySuccess CDyModelAnimator::Initialize(_MIN_ const PDyModelAnimatorComponentMetaInfo& descriptor)
{
  // Initialize skeleton & (temporary) animation scrap.
  this->mBinderAnimationScrap.TryRequireResource(descriptor.mDetails.mTempAnimationScrap);
  this->mBinderSkeleton.TryRequireResource(descriptor.mDetails.mSkeletonSpecifier);

  // Component activation check.
  if (descriptor.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CDyModelAnimator::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

void CDyModelAnimator::Update(_MIN_ TF32 dt)
{
  // If given animation is not valid, just return with doing nothing.
  if (this->mBinderSkeleton.IsResourceExist() == false
  ||  this->mBinderAnimationScrap.IsResourceExist() == false) { return; }

  // @TODO TEMPORARY CODE.
  switch (this->mStatus.mStatus)
  {
  case EDyAnimatorStatus::Stop: /* Do nothing */ break;
  case EDyAnimatorStatus::Init: 
  {
    this->mStatus.mPtrPresentAnimatorInfo = this->mBinderAnimationScrap.Get();
    this->mStatus.mElapsedTime = 0;
    this->mStatus.mScrapMode = EDyAnimationScrapMode::Loop;

    // Assert animation and bone count is same.
    this->mStatus.mFinalTransformList.resize(this->mBinderSkeleton->GetInputBoneCount());
    for (auto& transform : this->mStatus.mFinalTransformList)
    {
      transform = DDyMatrix4x4::IdentityMatrix();
    }
    this->mStatus.mStatus = EDyAnimatorStatus::Play; 
  } break;
  case EDyAnimatorStatus::Play: 
  {
    this->mStatus.mElapsedTime += dt * this->mStatus.mPtrPresentAnimatorInfo->GetRateScale();
    const bool isLooped = this->mStatus.mScrapMode == EDyAnimationScrapMode::Loop ? true : false;

    // Skeleton bone and transform, and bone animation channel is same. Find root animation node.
    const auto rootBoneIdList = this->mBinderSkeleton->GetChildrenNodeIdList(-1);
    for (const auto& idBone : rootBoneIdList)
    {
      this->TryUpdateFinalTransform(idBone, DDyMatrix4x4::IdentityMatrix(), isLooped);
    }
  } break;
  case EDyAnimatorStatus::Pause: break;
  default: ;
  }
}

void CDyModelAnimator::TryUpdateFinalTransform(_MIN_ TI32 idSkelNode, _MIN_ const DDyMatrix4x4& parentTransform, _MIN_ bool iIsLooped)
{
  const auto& animScrap = this->mStatus.mPtrPresentAnimatorInfo->GetAnimNodeList();
  const auto& refNode = this->mBinderSkeleton->GetRefSkeletonNode(idSkelNode);
  auto localTransform = refNode.mLocalTransform;

  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(animScrap), 
      [idSkelNode](const auto& iNode) { return iNode.mSkeletonNodeIndex == idSkelNode; });
  if (it != animScrap.end())
  {
    const auto animNodeId = std::distance(animScrap.begin(), it);

    // Get scaling vector. (vector x,y,z can be linearly interpolated)   
    const auto inpScl = this->mStatus.mPtrPresentAnimatorInfo->GetInterpolatedScaling(
        this->mStatus.mElapsedTime, animNodeId, iIsLooped);
    // Get scaled rotation matrix. (quaternion x,y,z,w must be process slerp as a 4x4 matrix.)
    const auto inpRot = this->mStatus.mPtrPresentAnimatorInfo->GetInterpolatedRotation(
        this->mStatus.mElapsedTime, animNodeId, iIsLooped);
    // Get position vector. (vector x,y,z can be linearly interpolated)
    const auto inpPos = this->mStatus.mPtrPresentAnimatorInfo->GetInterpolatedPosition(
        this->mStatus.mElapsedTime, animNodeId, iIsLooped);

    localTransform = DDyMatrix4x4::CreateWithTranslation(inpPos).Rotate(inpRot).Scale(inpScl);
  }

  // Calculate final transform without offset matrix.
  const DDyMatrix4x4 finalTransform = parentTransform.Multiply(localTransform);

  const auto& offsetBoneList = this->mBinderSkeleton->GetOffsetBoneList();
  const auto itBone = std::find_if(
      MDY_BIND_BEGIN_END(offsetBoneList), 
      [idSkelNode](const auto& iNode) { return iNode.mIndexSkeletonNode == idSkelNode; });
  if (itBone != offsetBoneList.end())
  {
    // Update transform. Set final transform (uniform)
    const auto id = std::distance(offsetBoneList.begin(), itBone);
    this->mStatus.mFinalTransformList[id] = 
        this->mBinderSkeleton->GetRootInverseTransform()
        .Multiply(finalTransform)
        .Multiply(itBone->mBoneOffsetMatrix);
  }

  const auto rootBoneIdList = this->mBinderSkeleton->GetChildrenNodeIdList(idSkelNode);
  for (const auto& idBone : rootBoneIdList)
  {
    this->TryUpdateFinalTransform(idBone, finalTransform, iIsLooped);
  }
}

std::string CDyModelAnimator::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

const std::vector<DDyMatrix4x4>& CDyModelAnimator::GetFinalTransformList() const noexcept
{
  return this->mStatus.mFinalTransformList;
}

void CDyModelAnimator::TryActivateInstance()
{
  // Get model filter component.
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CDyModelFilter>();
  if (opRenderer.has_value() == false) { return; }

  CDyModelFilter& refCompFilter = *opRenderer.value();
  if (refCompFilter.IsComponentActivated() == false) { return; }

  // Get model handle manager, and ask for whether model handler is already bound.
  auto& handleManager = FDyModelHandlerManager::GetInstance();
  const auto& modelSpecifier = refCompFilter.GetModelSpecifier();
  if (handleManager.IsBoundModelExist(modelSpecifier) == false)
  {
    // If not, let manager create handler and bound it,
    // and let actor bind animator to handler and register specifier as key.
    MDY_CALL_ASSERT_SUCCESS(handleManager.TryCreateHandler(modelSpecifier));
  }
  handleManager.BindToHandler(modelSpecifier, *this->GetBindedActor(), *this);

  // Bind to activated animator component list of manager.
  auto& worldManager = MDyWorld::GetInstance();
  worldManager.__BindActiveModelAnimator(*this);
}

void CDyModelAnimator::TryDeactivateInstance()
{
  // Unbind from activated animator component list of manager.
  auto& worldManager = MDyWorld::GetInstance();
  worldManager.__UnbindActiveModelAnimator(*this);

  // Get model filter component.
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CDyModelFilter>();
  if (opRenderer.has_value() == false) { return; }

  CDyModelFilter& refCompFilter = *opRenderer.value();
  if (refCompFilter.IsComponentActivated() == false) { return; }

  // GC renderer ptr to given handler.
  auto& handleManager       = FDyModelHandlerManager::GetInstance();
  const auto& modelSpecifier = refCompFilter.GetModelSpecifier();

  if (handleManager.IsBoundModelExist(modelSpecifier) == true)
  {
    // Unbind this component from handler instance.
    handleManager.UnbindToHandler(modelSpecifier, *this->GetBindedActor(), *this);

    // Check Actor instance need to be removed. 
    if (handleManager.IsActorInfoNeedToBeGc(modelSpecifier, *this->GetBindedActor()) == true)
    {
      MDY_CALL_ASSERT_SUCCESS(handleManager.TryRemoveBoundActor(modelSpecifier, *this->GetBindedActor()));

      // Because handler have been broken immutability, so check Bound model instance need to be gced also.
      if (handleManager.IsBoundModelNeedToGc(modelSpecifier) == true)
      {
        MDY_CALL_ASSERT_SUCCESS(handleManager.TryRemoveBoundModel(modelSpecifier));
      }
    }
  }
}

} /// ::dy namespace