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

inline MPhysics::Impl::Impl()
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
  const auto& defaultSetting = this->GetDefaultSetting();
  //
  this->mDefaultMaterial = this->gPhysicx->createMaterial(
    defaultSetting.mCommonProperty.mDefaultStaticFriction,
    defaultSetting.mCommonProperty.mDefaultDynamicFriction,
    defaultSetting.mCommonProperty.mDefaultRestitution);
  //
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mDefaultMaterial), "PhysX Default material must be created.");

#ifdef false
  ///
  /// @function CreateStack
  /// @brief Test function for create stack actors which consist of shape and rigidbody (dynamic)
  ///
  static auto CreateStack = [&](const physx::PxTransform &t, physx::PxU32 size, physx::PxReal halfExtent)
  {
    using namespace physx;

    PxShape *shape = gPhysicx->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mDefaultMaterial);

    TI32 nmb = 0;
    for (PxU32 i = 0; i < size; i++)
    {
      for (PxU32 j = 0; j < size - i; j++)
      {
        const PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);

        PxRigidDynamic *body = gPhysicx->createRigidDynamic(t.transform(localTm));
        body->attachShape(*shape);

        // @TODO FOR DEBUG, REMOVE THIS AT PRODUCTION CODE
        sDebugActorName.push_back(fmt::format("{}_{}", "Stack", nmb));
        body->setName(sDebugActorName.rbegin()->c_str());

        //DySetupFiltering(DyMakeNotNull(body), EDyTempCollisionLayer::Stack, EDyTempCollisionLayer::Stack);

        PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
        gScene->addActor(*body);

        sRigidbodies.emplace_back(DyMakeNotNull(body));
        nmb += 1;
      }
    }
    shape->release();
  };

  ///
  /// @function CreateDynamic
  /// @brief Test function for creating spherical actor which consists of sphere shape and rigidbody
  /// by using helper function from PhysX SDK.
  ///
  static auto CreateDynamic = [&](const physx::PxTransform &t, const physx::PxGeometry &geometry,
    const physx::PxVec3 &velocity = physx::PxVec3(0)) -> physx::PxRigidDynamic*
  {
    using namespace physx;

    PxRigidDynamic *dynamic = PxCreateDynamic(*gPhysicx, t, geometry, *mDefaultMaterial, 10.0f);
    dynamic->setAngularDamping(0.5f);
    dynamic->setLinearVelocity(velocity);
    DySetupFiltering(DyMakeNotNull(dynamic), EDyTempCollisionLayer::Sphere, EDyTempCollisionLayer::Stack | EDyTempCollisionLayer::Floor);

    // @TODO FOR DEBUG, REMOVE THIS AT PRODUCTION CODE
    sDebugActorName.push_back(fmt::format("{}_0", "Sphere"));
    dynamic->setName(sDebugActorName.rbegin()->c_str());

    gScene->addActor(*dynamic);
    sRigidbodies.emplace_back(DyMakeNotNull(dynamic));

    return dynamic;
  };
  static physx::PxDefaultErrorCallback gDefaultErrorCallback;

  this->gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->defaultAllocatorCallback, gDefaultErrorCallback);
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNOTNULL(this->gFoundation), "PxCreateFoundation Failed!");

  this->gPhysicx = PxCreatePhysics(PX_PHYSICS_VERSION, *this->gFoundation, physx::PxTolerancesScale(), true, this->gPvd);
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNOTNULL(this->gPhysicx), "PxCreatePhysics Failed!");

  physx::PxSceneDesc sceneDescriptor{ this->gPhysicx->getTolerancesScale() };
  sceneDescriptor.filterShader = DyFilterShader;
  sceneDescriptor.simulationEventCallback = &this->mCallback;
  //sceneDescriptor.flags                    |= physx::PxSceneFlag::eREQUIRE_RW_LOCK;
  //sceneDesc.frictionType = PxFrictionType::eTWO_DIRECTIONAL;
  //sceneDesc.frictionType = PxFrictionType::eONE_DIRECTIONAL;
  //sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
  //sceneDesc.flags |= PxSceneFlag::eENABLE_AVERAGE_POINT;
  //sceneDesc.flags |= PxSceneFlag::eADAPTIVE_FORCE;
  sceneDescriptor.flags |= physx::PxSceneFlag::eENABLE_PCM;
  sceneDescriptor.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
  sceneDescriptor.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
  sceneDescriptor.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;
  sceneDescriptor.gpuMaxNumPartitions = 8;
  sceneDescriptor.gravity = physx::PxVec3{ 0, -9.81f, 0 };

  if (sceneDescriptor.cpuDispatcher == nullptr)
  {
    this->gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNOTNULL(this->gDispatcher), "PxDefaultCpuDispatcherCreate Failed!");
    sceneDescriptor.cpuDispatcher = this->gDispatcher;
  }
  if (sceneDescriptor.filterShader == nullptr)
  {
    sceneDescriptor.filterShader = physx::PxDefaultSimulationFilterShader;
  }

  this->gScene = this->gPhysicx->createScene(sceneDescriptor);
  physx::PxSceneWriteLock scopedLock(*this->gScene);

  physx::PxPvdSceneClient* pvdClient = this->gScene->getScenePvdClient();
  if (pvdClient)
  {
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
  }

  //mDefaultMaterial = gPhysicx->createMaterial(0.5f, 0.5f, 0.6f);
  //physx::PxRigidStatic *groundPlane = PxCreatePlane(*gPhysicx, physx::PxPlane(0, 1, 0, 0), *mDefaultMaterial);
  //DySetupFiltering(DyMakeNotNull(groundPlane), EDyTempCollisionLayer::Floor, EDyTempCollisionLayer::Sphere);

  // @TODO FOR DEBUG, REMOVE THIS AT PRODUCTION CODE
  sDebugActorName.push_back(fmt::format("{}_0", "Floor"));
  groundPlane->setName(sDebugActorName.rbegin()->c_str());

  gScene->addActor(*groundPlane);

  for (physx::PxU32 i = 0; i < 5; i++)
  {
    CreateStack(physx::PxTransform(physx::PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
  }
  CreateDynamic(physx::PxTransform(physx::PxVec3(0, 40, 100)), physx::PxSphereGeometry(10), physx::PxVec3(0, -50, -100));
#endif
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
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNULL(this->gScene), "PhysX scene is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNULL(this->gDispatcher), "PhysX cpu dispatcher is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNULL(this->gPhysicx), "PhysX physics is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNULL(this->gPvd), "PhysX pvd is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNULL(this->mCooking), "PhysX mesh cooker is not released before release Physics manager.");
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNULL(this->gFoundation), "PhysX foundation is not released before release Physics manager.");
}

inline void MPhysics::Impl::UpdateRenderObjectTransform(TF32 iDt)
{
  // Lock
  physx::PxSceneWriteLock scopedLock(*this->gScene);

  // Iterate activated CDyPhysicsRigidbodies.
  for (auto& ptrRigidbodyComp : this->mActivatedRigidbodyList)
  {
    auto& refRigidActor = ptrRigidbodyComp->MDY_PRIVATE(GetRefInternalRigidbody)();
    const auto numberShape = refRigidActor.getNbShapes();
    for (TU32 i = 0; i < numberShape; ++i)
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
      ptrCollider->UpdateBound(pxBound);
    }
  }
}

inline void MPhysics::Impl::CallCallbackIssueOnce()
{
  while (this->mCollisionCallbackIssueQueue.empty() == false)
  {
    auto& refIssue = this->mCollisionCallbackIssueQueue.front();
    //
    auto* ptrSelfRigidbody = refIssue.mPtrSelfActor->GetRigidbody();
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptrSelfRigidbody), "Unexpected error occurred.");

    //
    ptrSelfRigidbody->CallCollisionCallback(refIssue.mType, refIssue);
    this->mCollisionCallbackIssueQueue.pop();
  }
  // List will be cleard automatically.
}

inline void MPhysics::Impl::Update(TF32 iDt)
{
  if (iDt > 0)
  {
    this->gScene->simulate(iDt);
    this->gScene->fetchResults(true);

    // We need to process callback & update transform information.
  }
}

inline void MPhysics::Impl::InitScene()
{
  physx::PxSceneDesc tempSceneDesc{ this->gPhysicx->getTolerancesScale() };
  tempSceneDesc.gravity = physx::PxVec3{ 0.0f, -9.81f, 0.0f };

  if (MDY_CHECK_ISNULL(tempSceneDesc.cpuDispatcher))
  {
    this->gDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->gDispatcher), "PhysX Cpu dispatcher must be created validly.");
    tempSceneDesc.cpuDispatcher = this->gDispatcher;
  }

  if (MDY_CHECK_ISNULL(tempSceneDesc.filterShader))
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
    if (MDY_CHECK_ISNOTNULL(pvdClient))
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

inline void MPhysics::Impl::UpdateInternalPxSceneParameter()
{
  // If gScene is null, just return to outside and do nothing.
  if (MDY_CHECK_ISNULL(this->gScene)) { return; }

  static constexpr TF32 enable = 1.0f;
  static constexpr TF32 disable = 0.0f;
  using PxVP = physx::PxVisualizationParameter;

  // Set collision shape.
  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/structPxVisualizationParameter.html
  if (MSetting::GetInstance().IsRenderPhysicsCollisionShape() == true)
  {
    if (this->gScene->getVisualizationParameter(PxVP::eCOLLISION_SHAPES) == disable)
    { this->gScene->setVisualizationParameter(PxVP::eCOLLISION_SHAPES, enable); }
  }
  else
  {
    if (this->gScene->getVisualizationParameter(PxVP::eCOLLISION_SHAPES) == enable)
    { this->gScene->setVisualizationParameter(PxVP::eCOLLISION_SHAPES, disable); }
  }
}

inline void MPhysics::Impl::TryEnqueueDebugDrawCall()
{
  // If gScene is null, just return to outside and do nothing.
  if (MDY_CHECK_ISNULL(this->gScene)) { return; }

  auto& renderingManager = MRendering::GetInstance();

  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/DebugVisualization.html#debugvisualization
  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/BestPractices.html
  for (auto& ptrRigidbody : this->mActivatedRigidbodyList)
  {
    auto& internalRigidbody = ptrRigidbody->MDY_PRIVATE(GetRefInternalRigidbody)();
    const auto globalTransform = internalRigidbody.getGlobalPose();
    const auto& colliderList = ptrRigidbody->GetBindedActivatedColliderList();

    for (auto& ptrCollider : colliderList)
    {
      auto* ptrPxShape = ptrCollider->MDY_PRIVATE(GetPtrInternalShape)();
      const auto localPose = ptrPxShape->getLocalPose();
      const auto transformMatrix = DMatrix4x4(globalTransform * localPose);
      // Enqueue draw list.
      // Iterate `Collider` and insert queue with transform + mesh.
      renderingManager.EnqueueDebugDrawCollider(*ptrCollider, transformMatrix);
    }
  }
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
      result.mContactPosition = internalBuffer.position;

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