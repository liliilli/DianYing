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
#include <Dy/Management/MPhysics.h>

#include <queue>
#include <PxPhysicsAPI.h>
#include <PxFoundation.h>

#include <Dy/Element/FActor.h>
#include <Dy/Helper/System/Pointer.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Component/CPhysicsRigidbody.h>
#include <Dy/Component/Internal/Physics/CBasePhysicsCollider.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Management/Type/Physics/DCollisionIssueItem.h>
#include <Dy/Management/Helper/FPhysXErrorCallback.h>

//!
//! Test
//!

namespace
{

static const auto dyFlagTrigger = physx::PxPairFlag::eTRIGGER_DEFAULT;
static const auto dyFlagHit     = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eCONTACT_DEFAULT;

[[nodiscard]] std::pair<TU8, TU8> 
GetFilterResult(_MIN_ const physx::PxFilterData& data0, _MIN_ const physx::PxFilterData& data1)
{
  const auto lhsId = data0.word0 & 0x000000FF;
  const auto rhsId = data1.word0 & 0x000000FF;
  const auto lhsShift = lhsId % 16;
  const auto rhsShift = rhsId % 16;

  TU8 lhsFlag = 0;
  if (lhsId < 16)       { lhsFlag = (data0.word1 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }
  else if (lhsId < 32)  { lhsFlag = (data0.word2 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }
  else if (lhsId < 48)  { lhsFlag = (data0.word3 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }

  TU8 rhsFlag = 0;
  if (rhsId < 16)       { rhsFlag = (data1.word1 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }
  else if (rhsId < 32)  { rhsFlag = (data1.word2 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }
  else if (rhsId < 48)  { rhsFlag = (data1.word3 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }

  return {lhsFlag, rhsFlag};
}

/// @function DyFilterShader
/// @brief PhysX Filter shader customized setting function.
physx::PxFilterFlags DyFilterShader(
  _MIN_ physx::PxFilterObjectAttributes attributes0, _MIN_ physx::PxFilterData filterData0,
  _MIN_ physx::PxFilterObjectAttributes attributes1, _MIN_ physx::PxFilterData filterData1,
  _MIN_ physx::PxPairFlags& pairFlags, _MIN_ const void* constantBlock, _MIN_ physx::PxU32 constantBlockSize)
{
  // If first 24bit (rigidbody specifier id) is same, just neglect a pair of collider.
  if ((filterData0.word0 & 0xFFFFFF00) == (filterData1.word0 & 0xFFFFFF00))
  {
    return physx::PxFilterFlag::eKILL;
  }

  // Get information from filterData0, filterData1.
  auto [lhsFlag, rhsFlag] = GetFilterResult(filterData0, filterData1);

  // 00 (collision) 01 (overlap) 10 (ignore)
  // -- 00 01 10
  // 00 Co Ov Ig
  // 01 Ov Ov Ig
  // 10 Ig Ig Ig

	// generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

  if (lhsFlag == rhsFlag)
  {
    if (lhsFlag == 0b00)      { pairFlags = dyFlagHit; }
    else if (lhsFlag == 0b01) { pairFlags = dyFlagTrigger; }
    else                      { return physx::PxFilterFlag::eKILL; }
  }
  else
  {
    if (lhsFlag < rhsFlag) { const auto temp = rhsFlag; rhsFlag = lhsFlag; lhsFlag = temp; }

    if (lhsFlag == 0b01)  { pairFlags = dyFlagTrigger; }
    else                  { return physx::PxFilterFlag::eKILL; }
  }

	return physx::PxFilterFlag::eDEFAULT;
}

} /// ::anonymous namespace

//!
//! Implementation
//!

namespace dy
{

struct MPhysics::Impl final : 
    public physx::PxDeletionListener, 
    public physx::PxSimulationEventCallback
{
public:
  DY_DECLARE_PIMPL_CTOR(MPhysics);
  virtual ~Impl();

  /// @brief Init PhysX All resource related to scene.
  void InitLevel();
  /// @brief Release PhysX All Resource related to scene.
  void ReleaseScene();

  auto& MDY_PRIVATE(GetRefInternalSdk)()
  {
    MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNOTNULL(this->gPhysicx), "Internal PhysX PxPhysics must be initailized.");
    return *this->gPhysicx;
  }

  auto& MDY_PRIVATE(GetRefScene)()
  {
    MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNOTNULL(this->gScene), "Internal PhysX PxScene must be initailized.");
    return *this->gScene;
  }

  /// @brief Register activated rigidbody instance.
  void MDY_PRIVATE(RegisterRigidbody)(_MIN_ CPhysicsRigidbody& iRefRigidbody)
  {
    this->mActivatedRigidbodyList.emplace_back(DyMakeNotNull(&iRefRigidbody));

    auto ptrRigidbodyComponent = this->mActivatedRigidbodyList.back();

    MDY_ASSERT_MSG_FORCE(this->gScene != nullptr, "Physics scene must be valid.");
    this->gScene->addActor(ptrRigidbodyComponent->__GetRefInternalRigidbody());
  }

  /// @brief Unregister deactivated rigidbody instance.
  void MDY_PRIVATE(UnregisterRigidbody)(_MIN_ CPhysicsRigidbody& iRefRigidbody)
  {
    const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActivatedRigidbodyList),
      [ptrSource = &iRefRigidbody](const auto& ptrTarget) { return ptrTarget.Get() == ptrSource; }
    );

    MDY_ASSERT_MSG_FORCE(it != this->mActivatedRigidbodyList.end(), "Unexpected error occurred.");

    this->gScene->removeActor(it->Get()->__GetRefInternalRigidbody());
    FaseErase(this->mActivatedRigidbodyList, it);
  }

  /// @brief Override function callback. When object of physx is remove, this function will be called.
	void onRelease(
      MDY_NOTUSED const physx::PxBase* observed, 
      MDY_NOTUSED void* userData, 
      MDY_NOTUSED physx::PxDeletionEventFlag::Enum deletionEvent) override final;

  /// @brief Get PhyX error callback function instance.
  static physx::PxErrorCallback& MDY_PRIVATE(GetPhysXErrorCallback)()
  {
    static physx::PxDefaultErrorCallback gDefaultErrorCallback;
    return gDefaultErrorCallback;
  }

  /// @brief
  MDY_NODISCARD FORCEINLINE physx::PxSimulationFilterShader 
  MDY_PRIVATE(GetSampleFilterShader)()
  {
    return physx::PxDefaultSimulationFilterShader;
  }

  FPhysXErrorCallback         gCallback;

  physx::PxPvd*                 gPvd        = nullptr;
  physx::PxFoundation*          gFoundation = nullptr;
  physx::PxPhysics*             gPhysicx    = nullptr; 
  physx::PxDefaultCpuDispatcher*gDispatcher = nullptr;
  physx::PxScene*               gScene      = nullptr;
  physx::PxMaterial*            mDefaultMaterial = nullptr;
  physx::PxCooking*             mCooking    = nullptr;

  physx::PxReal                 stackZ      = 10.0f;
  physx::PxDefaultAllocator     defaultAllocatorCallback;

  /// @brief Activated ptr-rigidbody list.
  std::vector<NotNull<CPhysicsRigidbody*>> mActivatedRigidbodyList {};
  
  /// @brief Call this function instead of `pEnqueueCollisionIssue`.
  /// Enqueue collision issue item to `mCollisionCallbackIssueQueue`.
  /// In enqueueing collision issue item, does not check there is any callback item given type,
  /// and not-nullity of given collider & actor variable.
  void pTryEnqueueCollisionIssue(
      ECollisionCallbackType iHitType,
      physx::PxPairFlags iInternalFlag, 
      CBasePhysicsCollider* i0, 
      CBasePhysicsCollider* i1, 
      const FHitResult& iHitResult);
 
  /// @brief Collision callback issue queue. This list must be cleared before other-script update phase.
  std::queue<DCollisionIssueItem> mCollisionCallbackIssueQueue{};

  // Implements PxSimulationEventCallback

	void onContact(const physx::PxContactPairHeader& pairHeader, 
                 const physx::PxContactPair* pairs, 
                 physx::PxU32 nbPairs) override final;
 
	void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override final {}
	void onWake(physx::PxActor** , physx::PxU32) override final {}
	void onSleep(physx::PxActor** , physx::PxU32) override final {}
	void onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) override final {}
  
  // Garbage Bin
	void onTrigger(MDY_NOTUSED physx::PxTriggerPair* pairs, MDY_NOTUSED physx::PxU32 count) override final 
  {  /* We do not use onTrigger on Dy. */ }
};

} /// ::dy namespace
#include <Dy/Management/Inline/MPhysicsImpl.inl>

namespace dy
{

EDySuccess MPhysics::pfInitialize()
{
  DY_INITIALIZE_PIMPL();
  return EDySuccess::DY_SUCCESS;
}
EDySuccess MPhysics::pfRelease()
{

  DY_RESET_PIMPL();
  return EDySuccess::DY_SUCCESS;
}

void MPhysics::Update(TF32 dt) 
{ 
  if (dt > 0)
  {
    DY_PIMPL->gScene->simulate(dt);
    DY_PIMPL->gScene->fetchResults(true);
    // We need to process callback & update transform information.
  }
}

void MPhysics::UpdateRenderObjectTransform(TF32 dt) 
{ 
 // Lock
  physx::PxSceneWriteLock scopedLock(*DY_PIMPL->gScene);

  // Iterate activated CDyPhysicsRigidbodies.
  for (auto& ptrRigidbodyComp : DY_PIMPL->mActivatedRigidbodyList)
  {
    auto& refRigidActor = ptrRigidbodyComp->MDY_PRIVATE(GetRefInternalRigidbody)();

    for (TU32 i = 0, numberShape = refRigidActor.getNbShapes(); i < numberShape; ++i)
    {
      physx::PxShape* shape = nullptr;
      MDY_NOTUSED const TU32 n = refRigidActor.getShapes(&shape, 1, i);
      MDY_ASSERT_MSG(n == 1, "Unexpected error occurred.");

      // Get transform and copy physics pose to graphics pose.
      const auto pxTransform = physx::PxShapeExt::getGlobalPose(*shape, refRigidActor);
      auto ptrTransformComp = ptrRigidbodyComp->GetBindedActor()->GetTransform();
      ptrTransformComp->MDY_PRIVATE(SetPxTransform)(pxTransform);

      // Set AABB (Axis-aligned bounding box) bound to each collider.
      const auto pxBound = physx::PxShapeExt::getWorldBounds(*shape, refRigidActor);
      auto* ptrCollider = static_cast<CBasePhysicsCollider*>(shape->userData);
      MDY_ASSERT_MSG(ptrCollider != nullptr, "Unexpected error occurred.");
      ptrCollider->UpdateBound(FBounds3D::CreateWith(pxBound));
    }
  }
}

void MPhysics::CallCallbackIssueOnce() 
{ 
  while (DY_PIMPL->mCollisionCallbackIssueQueue.empty() == false)
  {
    auto& refIssue = DY_PIMPL->mCollisionCallbackIssueQueue.front();
    //
    auto* ptrSelfRigidbody = refIssue.mPtrSelfActor->GetRigidbody();
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptrSelfRigidbody), "Unexpected error occurred.");

    //
    ptrSelfRigidbody->CallCollisionCallback(refIssue.mType, refIssue);
    DY_PIMPL->mCollisionCallbackIssueQueue.pop();
  }
  // List will be cleard automatically.
}

void MPhysics::InitScene() 
{ 
  DY_PIMPL->InitLevel(); 
}

void MPhysics::ReleaseScene() 
{ 
  DY_PIMPL->ReleaseScene(); 
}

const DDySettingPhysics& MPhysics::GetDefaultSetting() const noexcept
{
  return MSetting::GetInstance().GetPhysicsSetting();
}

const physx::PxMaterial& MPhysics::GetDefaultPhysicsMaterial() const noexcept
{
  return *DY_PIMPL->mDefaultMaterial;
}

physx::PxPhysics& MPhysics::MDY_PRIVATE(GetRefInternalSdk)() 
{ 
  return DY_PIMPL->__GetRefInternalSdk(); 
}

physx::PxScene& MPhysics::MDY_PRIVATE(GetRefScene)() 
{ 
  return DY_PIMPL->__GetRefScene(); 
}

void MPhysics::UpdateInternalPxSceneParameter() 
{ 
  // If gScene is null, just return to outside and do nothing.
  if (DY_PIMPL->gScene == nullptr) { return; }

  static constexpr TF32 enable = 1.0f;
  static constexpr TF32 disable = 0.0f;
  using PxVP = physx::PxVisualizationParameter;

  // Set collision shape.
  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/structPxVisualizationParameter.html
  if (MSetting::GetInstance().IsRenderPhysicsCollisionShape() == true)
  {
    if (DY_PIMPL->gScene->getVisualizationParameter(PxVP::eCOLLISION_SHAPES) == disable)
    { 
      DY_PIMPL->gScene->setVisualizationParameter(PxVP::eCOLLISION_SHAPES, enable); 
    }
  }
  else
  {
    if (DY_PIMPL->gScene->getVisualizationParameter(PxVP::eCOLLISION_SHAPES) == enable)
    { 
      DY_PIMPL->gScene->setVisualizationParameter(PxVP::eCOLLISION_SHAPES, disable); 
    }
  }
}

void MPhysics::TryEnqueueDebugDrawCall() 
{ 
 // If gScene is null, just return to outside and do nothing.
  if (DY_PIMPL->gScene == nullptr) { return; }

  auto& renderingManager = MRendering::GetInstance();

  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/DebugVisualization.html#debugvisualization
  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/BestPractices.html
  for (auto& ptrRigidbody : DY_PIMPL->mActivatedRigidbodyList)
  {
    auto& internalRigidbody = ptrRigidbody->MDY_PRIVATE(GetRefInternalRigidbody)();
    const auto globalTransform = internalRigidbody.getGlobalPose();
    const auto& colliderList = ptrRigidbody->GetBindedActivatedColliderList();

    for (auto& ptrCollider : colliderList)
    {
      auto* ptrPxShape = ptrCollider->MDY_PRIVATE(GetPtrInternalShape)();
      const auto localPose = ptrPxShape->getLocalPose();
      const auto transformMatrix = FMat4::CreateMat4(globalTransform * localPose);
      // Enqueue draw list.
      // Iterate `Collider` and insert queue with transform + mesh.
      renderingManager.EnqueueDebugDrawCollider(*ptrCollider, transformMatrix);
    }
  }
}

void MPhysics::MDY_PRIVATE(RegisterRigidbody)(_MIN_ CPhysicsRigidbody& iRefRigidbody)
{
  DY_PIMPL->MDY_PRIVATE(RegisterRigidbody)(iRefRigidbody);
}

void MPhysics::MDY_PRIVATE(UnregisterRigidbody)(_MIN_ CPhysicsRigidbody& iRefRigidbody)
{
  DY_PIMPL->MDY_PRIVATE(UnregisterRigidbody)(iRefRigidbody);
}

DY_DEFINE_PIMPL(MPhysics);
DY_DEFINE_DEFAULT_DESTRUCTOR(MPhysics);

} /// ::dy namespace