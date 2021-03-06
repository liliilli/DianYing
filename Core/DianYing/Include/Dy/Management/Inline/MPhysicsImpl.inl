#ifndef GUARD_DY_MANAGEMENT_PHYSICSMANAGER_IMPL_INL
#define GUARD_DY_MANAGEMENT_PHYSICSMANAGER_IMPL_INL
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

#include <Dy/Component/CTransform.h>

namespace dy
{

inline MPhysics::Impl::Impl(MPhysics& parent) : mImplParent(parent)
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(this->gFoundation), "Foundation is already exist.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(this->gPhysicx), "Physics is already exist.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(this->mCooking), "Cooking is already exist.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNULL(this->mDefaultMaterial), "Default material is already exist.");

  this->gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->defaultAllocatorCallback, MDY_PRIVATE(GetPhysXErrorCallback)());
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->gFoundation), "PhysX Foundation must be created successfully.");

  // Get scale from setting manager, but we use defualt value temporary. 
  physx::PxTolerancesScale temporalScale{};
  // MSetting::GetInstance().GetGridScale();

  this->gPhysicx = PxCreatePhysics(PX_PHYSICS_VERSION, *this->gFoundation, temporalScale, false, this->gPvd);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->gPhysicx), "PhysX Physics Instance must be created successfully.");

  if (PxInitExtensions(*this->gPhysicx, this->gPvd) == false) { MDY_UNEXPECTED_BRANCH(); }

  physx::PxCookingParams params(temporalScale);
  params.meshWeldTolerance = 0.001f;
  params.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eWELD_VERTICES);
  params.buildGPUData = true; //Enable GRB data being produced in cooking
  this->mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *this->gFoundation, params);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mCooking), "PhysX Cooking Instance must be created successfully.");

  this->gPhysicx->registerDeletionListener(*this, physx::PxDeletionEventFlag::eUSER_RELEASE);

  // Setup default material.
  const auto& defaultSetting = this->mImplParent.GetDefaultSetting();
  //
  this->mDefaultMaterial = this->gPhysicx->createMaterial(
    defaultSetting.mCommonProperty.mDefaultStaticFriction,
    defaultSetting.mCommonProperty.mDefaultDynamicFriction,
    defaultSetting.mCommonProperty.mDefaultRestitution);
  //
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mDefaultMaterial), "PhysX Default material must be created.");

  // Do nothing because when initialize level, all resource will be populated.
}

inline MPhysics::Impl::~Impl()
{
  this->mDefaultMaterial->release();
  this->mDefaultMaterial = nullptr;

  this->mCooking->release();
  this->mCooking = nullptr;

  PxCloseExtensions();

  this->gPhysicx->release();
  this->gPhysicx = nullptr;

  if (MDY_CHECK_ISNOTNULL(this->gPvd))
  { // Optional. (visual debugger)
    this->gPvd->release();
    this->gPvd = nullptr;
  }

  this->gFoundation->release();
  this->gFoundation = nullptr;

  // This function just check all resource is released.
  MDY_ASSERT_MSG_FORCE(this->gScene == nullptr, 
    "PhysX scene is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(this->gDispatcher == nullptr, 
    "PhysX cpu dispatcher is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(this->gPhysicx == nullptr, 
    "PhysX physics is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(this->gPvd == nullptr, 
    "PhysX pvd is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(this->mCooking == nullptr, 
    "PhysX mesh cooker is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(this->gFoundation == nullptr, 
    "PhysX foundation is not released before release Physics manager.");
}

inline void MPhysics::Impl::InitLevel()
{
  // Set gravity of level.
  const auto& physicsSetting = MSetting::GetInstance().GetPhysicsSetting();
  physx::PxSceneDesc tempSceneDesc{ this->gPhysicx->getTolerancesScale() };
  tempSceneDesc.gravity = physx::PxVec3{ 0.0f, -physicsSetting.mCommonProperty.mGravity, 0.0f };

  if (tempSceneDesc.cpuDispatcher == nullptr)
  {
    this->gDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->gDispatcher), "PhysX Cpu dispatcher must be created validly.");
    tempSceneDesc.cpuDispatcher = this->gDispatcher;
  }

  if (tempSceneDesc.filterShader == nullptr)
  { tempSceneDesc.filterShader = this->MDY_PRIVATE(GetSampleFilterShader)(); } 

  // Set scene descriptor flags
  tempSceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM
    | physx::PxSceneFlag::eENABLE_STABILIZATION
    | physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS
    | physx::PxSceneFlag::eREQUIRE_RW_LOCK;

  tempSceneDesc.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;
  tempSceneDesc.gpuMaxNumPartitions = 8;
  tempSceneDesc.filterShader = DyFilterShader;

  this->gScene = this->gPhysicx->createScene(tempSceneDesc);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->gScene), "PhysX Scene must be created successfully.");

  // Set Scene setting (atomically)
  {
    physx::PxSceneWriteLock scopedLock(*this->gScene);
    auto flags = this->gScene->getFlags();
    (void)flags;

    using PxVP = physx::PxVisualizationParameter;
    this->gScene->setVisualizationParameter(PxVP::eSCALE, 1.0f);
    this->gScene->setVisualizationParameter(PxVP::eACTOR_AXES, 1.0f);
    this->gScene->setSimulationEventCallback(this);

    physx::PxPvdSceneClient* pvdClient = this->gScene->getScenePvdClient();
    if (pvdClient != nullptr)
    {
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
  }
}

inline void MPhysics::Impl::ReleaseScene()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->gScene), "PhysX Scene must be valid.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->gDispatcher), "PhysX Dispatcher must be valid.");

  {
    physx::PxSceneWriteLock scopedLock(*this->gScene);
    // Do something that is relevent to gScene but not direct.
  }

  this->gScene->release();
  this->gScene = nullptr;

  this->gDispatcher->release();
  this->gDispatcher = nullptr;
}

inline void MPhysics::Impl::onRelease(
    const physx::PxBase* observed, 
    void* userData,
    physx::PxDeletionEventFlag::Enum deletionEvent)
{
#ifdef false
  if (observed->is<physx::PxRigidActor>())
  {
    const physx::PxRigidActor* actor = static_cast<const physx::PxRigidActor*>(observed);

    removeRenderActorsFromPhysicsActor(actor);

    std::vector<physx::PxRigidActor*>::iterator actorIter =
      std::find(mPhysicsActors.begin(), mPhysicsActors.end(), actor);
    if (actorIter != mPhysicsActors.end()) { mPhysicsActors.erase(actorIter); }
  }
#endif
}

inline void MPhysics::Impl::pTryEnqueueCollisionIssue(
    ECollisionCallbackType iHitType, 
    physx::PxPairFlags iInternalFlag,
    CBasePhysicsCollider* i0, 
    CBasePhysicsCollider* i1, 
    const FHitResult& iHitResult)
{
  // Make Collision issue item to be accessed when calling collision callback function.
  DCollisionIssueItem item;
  item.mType = iHitType;
  item.mHitResult = iHitResult;

  // Check flags.
  bool (CBasePhysicsCollider::*ConditionFunction)() const = nullptr;
  switch (iHitType)
  {
  case ECollisionCallbackType::OnHit:
    ConditionFunction = &CBasePhysicsCollider::IsNotifyHitEvent; break;
  case ECollisionCallbackType::OnOverlapBegin:
  case ECollisionCallbackType::OnOverlapEnd:
    ConditionFunction = &CBasePhysicsCollider::IsNotifyOverlapEvent; break;
  }

  if ((i0->*ConditionFunction)() == true)
  {
    item.mPtrSelfCollider = i0; item.mPtrSelfActor = i0->GetBindedActor();
    item.mPtrOtherCollider = i1; item.mPtrOtherActor = i1->GetBindedActor();
    this->mCollisionCallbackIssueQueue.push(item);
  }
  if ((i1->*ConditionFunction)() == true)
  {
    item.mPtrSelfCollider = i1; item.mPtrSelfActor = i1->GetBindedActor();
    item.mPtrOtherCollider = i0; item.mPtrOtherActor = i0->GetBindedActor();
    this->mCollisionCallbackIssueQueue.push(item);
  }
}

inline void MPhysics::Impl::onContact(
    const physx::PxContactPairHeader& pairHeader,
    const physx::PxContactPair* pairs, 
    physx::PxU32 nbPairs)
{
  // Check actors are not destroyed
  if (pairHeader.flags & (physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0
                        | physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
  {
    DyPushLogError("onContact(): Actors have been deleted!");
    return;
  }

  for (unsigned i = 0; i < nbPairs; i++)
  {
    const physx::PxContactPair& cp = pairs[i];

    // Get contact information of a pair of shape.
    physx::PxContactPairPoint contactPointBuffer[1];
    cp.extractContacts(contactPointBuffer, 1);
    auto& internalBuffer = contactPointBuffer[0];

    if (cp.flags.isSet(physx::PxContactPairFlag::eREMOVED_SHAPE_0) == false
      && cp.flags.isSet(physx::PxContactPairFlag::eREMOVED_SHAPE_1) == false)
    {
      const auto* shape0 = cp.shapes[0]; MDY_ASSERT_MSG_FORCE(shape0 != nullptr, "Test failed.");
      const auto* shape1 = cp.shapes[1]; MDY_ASSERT_MSG_FORCE(shape1 != nullptr, "Test failed.");

      // Get collider components.
      auto* ptrCollider0 = static_cast<CBasePhysicsCollider*>(shape0->userData);
      MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptrCollider0), "Unexpected error occurred.");
      auto* ptrCollider1 = static_cast<CBasePhysicsCollider*>(shape1->userData);
      MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptrCollider1), "Unexpected error occurred.");

      const auto filterFlag0 = shape0->getSimulationFilterData();
      const auto filterFlag1 = shape1->getSimulationFilterData();

      // Make FHitResult
      FHitResult result;
      result.mContactPosition = FVec3::CreateVec3(internalBuffer.position);

      // We do not process when `ignored` status because `ignored` status will be killed by physx internal system logic.
      using ECbType = ECollisionCallbackType;
      auto[lhsFlag, rhsFlag] = GetFilterResult(filterFlag0, filterFlag1);
      if (lhsFlag == rhsFlag)
      {
        if (lhsFlag == 0b00)
        { // When hit..
          this->pTryEnqueueCollisionIssue(ECbType::OnHit, cp.events, ptrCollider0, ptrCollider1, result);
        }
        else if (lhsFlag == 0b01)
        { // When trigger.
          if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
          { this->pTryEnqueueCollisionIssue(ECbType::OnOverlapBegin, cp.events, ptrCollider0, ptrCollider1, result); }
          else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
          { this->pTryEnqueueCollisionIssue(ECbType::OnOverlapEnd, cp.events, ptrCollider0, ptrCollider1, result); }
        }
      }
      else
      { // When table point is on fivot-point. [1x1, 2x2, 3x3...]
        if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        { this->pTryEnqueueCollisionIssue(ECbType::OnOverlapBegin, cp.events, ptrCollider0, ptrCollider1, result); }
        else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        { this->pTryEnqueueCollisionIssue(ECbType::OnOverlapEnd, cp.events, ptrCollider0, ptrCollider1, result); }
      }
    }
  }
}

}

#endif /// GUARD_DY_MANAGEMENT_PHYSICSMANAGER_IMPL_INL_PHYSICSMANAGER_IMPL_INL