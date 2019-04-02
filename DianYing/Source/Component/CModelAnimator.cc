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
#include <Dy/Component/CModelAnimator.h>
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>
#include <Dy/Element/FActor.h>
#include <Dy/Component/CModelFilter.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/Resource/Information/FInformationModelAnimScrap.h>
#include <Dy/Core/Resource/Information/FInformationModelSkeleton.h>

namespace dy
{

CModelAnimator::CModelAnimator(FActor& actorReference) : AGeneralBaseComponent{actorReference}
{ }

EDySuccess CModelAnimator::Initialize(const PDyModelAnimatorComponentMetaInfo& descriptor)
{
  // Initialize skeleton & (temporary) animation scrap.
  this->mBinderAnimationScrap.TryRequireResource(descriptor.mDetails.mTempAnimationScrap);
  this->mBinderSkeleton.TryRequireResource(descriptor.mDetails.mSkeletonSpecifier);

  // Component activation check.
  if (descriptor.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CModelAnimator::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

void CModelAnimator::Update(TF32 dt)
{
  // If given animation is not valid, just return with doing nothing.
  if (this->mBinderSkeleton.IsResourceExist() == false
  ||  this->mBinderAnimationScrap.IsResourceExist() == false) { return; }

  // @TODO TEMPORARY CODE.
  switch (this->mStatus.mStatus)
  {
  case EAnimatorStatus::Stop: /* Do nothing */ break;
  case EAnimatorStatus::Init: 
  {
    this->mStatus.mPtrPresentAnimatorInfo = this->mBinderAnimationScrap.Get();
    this->mStatus.mElapsedTime = 0;
    this->mStatus.mScrapMode = EAnimationScrapMode::Loop;

    // Assert animation and bone count is same.
    this->mStatus.mFinalTransformList.resize(this->mBinderSkeleton->GetInputBoneCount());
    for (auto& transform : this->mStatus.mFinalTransformList)
    {
      transform = DMat4::Identity();
    }
    this->mStatus.mStatus = EAnimatorStatus::Play; 
  } break;
  case EAnimatorStatus::Play: 
  {
    this->mStatus.mElapsedTime += dt * this->mStatus.mPtrPresentAnimatorInfo->GetRateScale();
    const bool isLooped = this->mStatus.mScrapMode == EAnimationScrapMode::Loop ? true : false;

    // Skeleton bone and transform, and bone animation channel is same. Find root animation node.
    const auto rootBoneIdList = this->mBinderSkeleton->GetChildrenNodeIdList(-1);
    for (const auto& idBone : rootBoneIdList)
    {
      this->TryUpdateFinalTransform(idBone, DMat4::Identity(), isLooped);
    }
  } break;
  case EAnimatorStatus::Pause: break;
  default: ;
  }
}

void CModelAnimator::TryUpdateFinalTransform(TI32 idSkelNode, const DMat4& parentTransform, bool iIsLooped)
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

    localTransform = FMat4::Scale(FMat4::Rotate(FMat4::CreateWithTranslation(inpPos), inpRot), inpScl);
  }

  // Calculate final transform without offset matrix.
  const DMat4 finalTransform = parentTransform * localTransform;

  const auto& offsetBoneList = this->mBinderSkeleton->GetOffsetBoneList();
  const auto itBone = std::find_if(
      MDY_BIND_BEGIN_END(offsetBoneList), 
      [idSkelNode](const auto& iNode) { return iNode.mIndexSkeletonNode == idSkelNode; });
  if (itBone != offsetBoneList.end())
  {
    // Update transform. Set final transform (uniform)
    const auto id = std::distance(offsetBoneList.begin(), itBone);
    this->mStatus.mFinalTransformList[id] = 
      ((this->mBinderSkeleton->GetRootInverseTransform()
    * finalTransform)
    * itBone->mBoneOffsetMatrix);
  }

  const auto rootBoneIdList = this->mBinderSkeleton->GetChildrenNodeIdList(idSkelNode);
  for (const auto& idBone : rootBoneIdList)
  {
    this->TryUpdateFinalTransform(idBone, finalTransform, iIsLooped);
  }
}

std::string CModelAnimator::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

const std::vector<DMat4>& CModelAnimator::GetFinalTransformList() const noexcept
{
  return this->mStatus.mFinalTransformList;
}

void CModelAnimator::TryActivateInstance()
{
  // Get model filter component.
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CModelFilter>();
  if (opRenderer.has_value() == false) { return; }

  CModelFilter& refCompFilter = *opRenderer.value();
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
  auto& worldManager = MWorld::GetInstance();
  worldManager.__BindActiveModelAnimator(*this);
}

void CModelAnimator::TryDeactivateInstance()
{
  // Unbind from activated animator component list of manager.
  auto& worldManager = MWorld::GetInstance();
  worldManager.__UnbindActiveModelAnimator(*this);

  // Get model filter component.
  auto opRenderer = this->GetBindedActor()->GetGeneralComponent<CModelFilter>();
  if (opRenderer.has_value() == false) { return; }

  CModelFilter& refCompFilter = *opRenderer.value();
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