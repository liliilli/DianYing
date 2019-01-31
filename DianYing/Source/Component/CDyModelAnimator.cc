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
    this->mStatus.mFinalTransformList.resize(this->mBinderSkeleton->GetBoneCount());
    this->mStatus.mStatus = EDyAnimatorStatus::Play; 
  } break;
  case EDyAnimatorStatus::Play: 
  {
    this->mStatus.mElapsedTime += dt * this->mStatus.mPtrPresentAnimatorInfo->GetRateScale();
    const bool isLooped = this->mStatus.mScrapMode == EDyAnimationScrapMode::Loop ? true : false;

    // Skeleton bone and transform, and bone animation channel is same.
    const DDyMatrix4x4 rootMatrix = this->mBinderSkeleton->GetRootInverseTransform(); 
    // Find root animation node.
    const auto rootBoneIdList = this->mBinderSkeleton->GetChildrenBoneIdList(-1);
    for (const auto& idBone : rootBoneIdList)
    {
      for (TU32 i = 0, num = static_cast<TU32>(this->mBinderAnimationScrap->GetAnimNodeList().size()); i < num; ++i)
      {
        // Check matching bone id of anim node and bone id.
        const auto boneId = this->mStatus.mPtrPresentAnimatorInfo->GetSkeletonBoneId(i);
        if (boneId != idBone) { continue; }

        // If matched, update transform.
        this->TryUpdateFinalTransform(i, rootMatrix, isLooped);
      }
    }
  } break;
  case EDyAnimatorStatus::Pause: break;
  default: ;
  }
}

void CDyModelAnimator::TryUpdateFinalTransform(_MIN_ TU32 idAnimNode, _MIN_ const DDyMatrix4x4& parentTransform, _MIN_ bool iIsLooped)
{
  // Get scaling vector. (vector x,y,z can be linearly interpolated)   
  const auto inpScl = this->mStatus.mPtrPresentAnimatorInfo->GetInterpolatedScaling(
      this->mStatus.mElapsedTime, idAnimNode, iIsLooped);
  // Get scaled rotation matrix. (quaternion x,y,z,w must be process slerp as a 4x4 matrix.)
  const auto inpRot = this->mStatus.mPtrPresentAnimatorInfo->GetInterpolatedRotation(
      this->mStatus.mElapsedTime, idAnimNode, iIsLooped);
  // Get position vector. (vector x,y,z can be linearly interpolated)
  const auto inpPos = this->mStatus.mPtrPresentAnimatorInfo->GetInterpolatedPosition(
      this->mStatus.mElapsedTime, idAnimNode, iIsLooped);

  // Calculate final transform without offset matrix.
  const DDyMatrix4x4 localTransform = DDyMatrix4x4::CreateWithScale(inpScl).Rotate(inpRot).Translate(inpPos);
  const DDyMatrix4x4 finalTransform = parentTransform.Multiply(localTransform);
  // Set final transform (uniform)
  const auto boneId   = this->mStatus.mPtrPresentAnimatorInfo->GetSkeletonBoneId(idAnimNode);
  const auto& refBone = this->mBinderSkeleton->GetRefBone(boneId);
  this->mStatus.mFinalTransformList[boneId] = finalTransform.Multiply(refBone.mOffsetMatrix);

  // Loop child bone node.
  const auto parentBoneIdList = this->mBinderSkeleton->GetChildrenBoneIdList(boneId);
  for (const auto& idChildBone : parentBoneIdList)
  {
    for (TU32 i = 0, num = static_cast<TU32>(this->mBinderAnimationScrap->GetAnimNodeList().size()); i < num; ++i)
    {
      // Check matching bone id of anim node and bone id.
      const auto idGivenChild = this->mStatus.mPtrPresentAnimatorInfo->GetSkeletonBoneId(i);
      if (idGivenChild != idChildBone) { continue; }

      // If matched, update transform.
      this->TryUpdateFinalTransform(i, finalTransform, iIsLooped);
    }
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