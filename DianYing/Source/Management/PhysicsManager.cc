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
#include <Dy/Management/PhysicsManager.h>

#include <PxFoundation.h>

#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Component/CDyPhysicsRigidbody.h>
#include "Dy/Component/CDyPhysicsCollider.h"
#include "Dy/Management/Rendering/RenderingManager.h"

//!
//! Test
//!

namespace
{

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
  const auto lhsId = filterData0.word0 & 0x000000FF;
  const auto rhsId = filterData1.word0 & 0x000000FF;
  const auto lhsShift = lhsId % 16;
  const auto rhsShift = rhsId % 16;

  unsigned int lhsFlag = 0;
  if (lhsId < 16)       { lhsFlag = (filterData0.word1 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }
  else if (lhsId < 32)  { lhsFlag = (filterData0.word2 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }
  else if (lhsId < 48)  { lhsFlag = (filterData0.word3 & (0b11 << (rhsShift * 2))) >> rhsShift * 2; }

  unsigned int rhsFlag = 0;
  if (rhsId < 16)       { rhsFlag = (filterData1.word1 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }
  else if (rhsId < 32)  { rhsFlag = (filterData1.word2 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }
  else if (rhsId < 48)  { rhsFlag = (filterData1.word3 & (0b11 << (lhsShift * 2))) >> lhsShift * 2; }

  // 00 (collision) 01 (overlap) 10 (ignore)
  // -- 00 01 10
  // 00 Co Ov Ig
  // 01 Ov Ov Ig
  // 10 Ig Ig Ig

	// generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

  if (lhsFlag == rhsFlag)
  {
    if (lhsFlag == 0b00)      { pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT; }
    else if (lhsFlag == 0b01) { pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT; }
    else                      { return physx::PxFilterFlag::eKILL; }
  }
  else
  {
    if (lhsFlag < rhsFlag) { const auto temp = rhsFlag; rhsFlag = lhsFlag; lhsFlag = temp; }

    if (lhsFlag == 0b01)  { pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT; }
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

EDySuccess MDyPhysics::pfInitialize()
{
  MDY_ASSERT(MDY_CHECK_ISNULL(this->gFoundation), "Foundation is already exist.");
  MDY_ASSERT(MDY_CHECK_ISNULL(this->gPhysicx), "Physics is already exist.");
  MDY_ASSERT(MDY_CHECK_ISNULL(this->mCooking), "Cooking is already exist.");
  MDY_ASSERT(MDY_CHECK_ISNULL(this->mDefaultMaterial), "Default material is already exist.");

  this->gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->defaultAllocatorCallback, MDY_PRIVATE_SPECIFIER(GetPhysXErrorCallback)());
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gFoundation), "PhysX Foundation must be created successfully.");

  // Get scale from setting manager, but we use defualt value temporary. 
  physx::PxTolerancesScale temporalScale{};
  // MDySetting::GetInstance().GetGridScale();

  this->gPhysicx = PxCreatePhysics(PX_PHYSICS_VERSION, *this->gFoundation, temporalScale, false, this->gPvd);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gPhysicx), "PhysX Physics Instance must be created successfully.");

  if (PxInitExtensions(*this->gPhysicx, this->gPvd) == false) { MDY_UNEXPECTED_BRANCH(); }

  physx::PxCookingParams params(temporalScale);
	params.meshWeldTolerance = 0.001f;
	params.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eWELD_VERTICES);
	params.buildGPUData = true; //Enable GRB data being produced in cooking
  this->mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *this->gFoundation, params);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mCooking), "PhysX Cooking Instance must be created successfully.");

  this->gPhysicx->registerDeletionListener(*this, physx::PxDeletionEventFlag::eUSER_RELEASE);

  // Setup default material.
  const auto& defaultSetting = this->GetDefaultSetting();
  //
  this->mDefaultMaterial = this->gPhysicx->createMaterial(
      defaultSetting.mCommonProperty.mDefaultStaticFriction,
      defaultSetting.mCommonProperty.mDefaultDynamicFriction,
      defaultSetting.mCommonProperty.mDefaultRestitution);
  //
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mDefaultMaterial), "PhysX Default material must be created.");

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
  MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->gFoundation), "PxCreateFoundation Failed!");

  this->gPhysicx = PxCreatePhysics(PX_PHYSICS_VERSION, *this->gFoundation, physx::PxTolerancesScale(), true, this->gPvd);
  MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->gPhysicx), "PxCreatePhysics Failed!");

  physx::PxSceneDesc sceneDescriptor { this->gPhysicx->getTolerancesScale() };
  sceneDescriptor.filterShader              = DyFilterShader;
  sceneDescriptor.simulationEventCallback   = &this->mCallback;
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
    MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->gDispatcher), "PxDefaultCpuDispatcherCreate Failed!");
    sceneDescriptor.cpuDispatcher = this->gDispatcher;
  }
  if (sceneDescriptor.filterShader == nullptr)
  { sceneDescriptor.filterShader = physx::PxDefaultSimulationFilterShader; }

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
  return DY_SUCCESS;
}

EDySuccess MDyPhysics::pfRelease()
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
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gScene), "PhysX scene is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gDispatcher), "PhysX cpu dispatcher is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gPhysicx), "PhysX physics is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gPvd), "PhysX pvd is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->mCooking), "PhysX mesh cooker is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gFoundation), "PhysX foundation is not released before release Physics manager.");

  return DY_SUCCESS;
}

void MDyPhysics::Update(_MIN_ TF32 dt)
{
  if (this->mIsInitialized == false) { return; }

  if (dt > 0)
  {
    this->gScene->simulate(dt);
    this->gScene->fetchResults(true);

    // We need to process callback & update transform information.

  }
}

void MDyPhysics::InitScene()
{
  physx::PxSceneDesc tempSceneDesc{this->gPhysicx->getTolerancesScale()};
  tempSceneDesc.gravity = physx::PxVec3{0.0f, -9.81f, 0.0f};

  if (MDY_CHECK_ISNULL(tempSceneDesc.cpuDispatcher))
  {
    this->gDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gDispatcher), "PhysX Cpu dispatcher must be created validly.");
    tempSceneDesc.cpuDispatcher = this->gDispatcher;
  }

  if (MDY_CHECK_ISNULL(tempSceneDesc.filterShader))
  {
    tempSceneDesc.filterShader = this->MDY_PRIVATE_SPECIFIER(GetSampleFilterShader)();
  }

  // Set scene descriptor flags
  tempSceneDesc.flags |=  physx::PxSceneFlag::eENABLE_PCM 
                      |   physx::PxSceneFlag::eENABLE_STABILIZATION
                      |   physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS
                      |   physx::PxSceneFlag::eREQUIRE_RW_LOCK;

  tempSceneDesc.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;
	tempSceneDesc.gpuMaxNumPartitions = 8;
  tempSceneDesc.filterShader = DyFilterShader;

  this->gScene = this->gPhysicx->createScene(tempSceneDesc);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gScene), "PhysX Scene must be created successfully.");

  // Set Scene setting (atomically)
  {
    physx::PxSceneWriteLock scopedLock(*this->gScene);
    auto flags = this->gScene->getFlags();
    (void)flags;

    using PxVP = physx::PxVisualizationParameter;
    this->gScene->setVisualizationParameter(PxVP::eSCALE, 1.0f);
    this->gScene->setVisualizationParameter(PxVP::eACTOR_AXES, 1.0f);

    physx::PxPvdSceneClient* pvdClient = this->gScene->getScenePvdClient();
    if (MDY_CHECK_ISNOTNULL(pvdClient))
    {
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
  }
}

void MDyPhysics::ReleaseScene()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gScene),       "PhysX Scene must be valid.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gDispatcher),  "PhysX Dispatcher must be valid.");

  {
    physx::PxSceneWriteLock scopedLock(*this->gScene);
    // Do something that is relevent to gScene but not direct.
  }

  this->gScene->release();
  this->gScene = nullptr;

  this->gDispatcher->release();
  this->gDispatcher = nullptr;
}

const DDySettingPhysics& MDyPhysics::GetDefaultSetting() const noexcept
{
  return MDySetting::GetInstance().GetPhysicsSetting();
}

const physx::PxMaterial& MDyPhysics::GetDefaultPhysicsMaterial() const noexcept
{
  return *this->mDefaultMaterial;
}

void MDyPhysics::UpdateInternalPxSceneParameter()
{
  // If gScene is null, just return to outside and do nothing.
  if (MDY_CHECK_ISNULL(this->gScene)) { return; }

  static constexpr TF32 enable  = 1.0f;
  static constexpr TF32 disable = 0.0f;
  using PxVP = physx::PxVisualizationParameter;

  // Set collision shape.
  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/apireference/files/structPxVisualizationParameter.html
  if (MDySetting::GetInstance().IsRenderPhysicsCollisionShape() == true)
  {
    if (this->gScene->getVisualizationParameter(PxVP::eCOLLISION_SHAPES) == disable)
    {
      this->gScene->setVisualizationParameter(PxVP::eCOLLISION_SHAPES, enable);
    }
  }
  else
  {
    if (this->gScene->getVisualizationParameter(PxVP::eCOLLISION_SHAPES) == enable)
    {
      this->gScene->setVisualizationParameter(PxVP::eCOLLISION_SHAPES, disable);
    }
  }
}

void MDyPhysics::TryEnqueueDebugDrawCall()
{
  // If gScene is null, just return to outside and do nothing.
  if (MDY_CHECK_ISNULL(this->gScene)) { return; }

  auto& renderingManager = MDyRendering::GetInstance();

  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/DebugVisualization.html#debugvisualization
  // https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/BestPractices.html
  for (auto& ptrRigidbody : this->mActivatedRigidbodyList)
  {
    auto& internalRigidbody     = ptrRigidbody->MDY_PRIVATE_SPECIFIER(GetRefInternalRigidbody)();
    const auto globalTransform  = internalRigidbody.getGlobalPose();
    const auto& colliderList    = ptrRigidbody->GetBindedActivatedColliderList();

    for (auto& ptrCollider : colliderList)
    {
      auto* ptrPxShape      = ptrCollider->MDY_PRIVATE_SPECIFIER(GetPtrInternalShape)();
      const auto localPose  = ptrPxShape->getLocalPose();
      const auto transformMatrix = DDyMatrix4x4(globalTransform * localPose);
      // Enqueue draw list.
      // Iterate `Collider` and insert queue with transform + mesh.
      renderingManager.EnqueueDebugDrawCollider(*ptrCollider, transformMatrix);
    }
  }
}

void MDyPhysics::MDY_PRIVATE_SPECIFIER(RegisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody)
{
  this->mActivatedRigidbodyList.emplace_back(DyMakeNotNull(&iRefRigidbody));
  
  auto ptrRigidbodyComponent = this->mActivatedRigidbodyList.back();

  MDY_ASSERT_FORCE(this->gScene != nullptr, "Physics scene must be valid.");
  this->gScene->addActor(ptrRigidbodyComponent->__GetRefInternalRigidbody());
}

void MDyPhysics::MDY_PRIVATE_SPECIFIER(UnregisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody)
{
  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActivatedRigidbodyList), 
      [ptrSource = &iRefRigidbody](const auto& ptrTarget) { return ptrTarget.Get() == ptrSource; }
  );
  
  MDY_ASSERT_FORCE(it != this->mActivatedRigidbodyList.end(), "Unexpected error occurred.");

  this->gScene->removeActor(it->Get()->__GetRefInternalRigidbody());
  DyFastErase(this->mActivatedRigidbodyList, it);
}

void MDyPhysics::onRelease(
    _MIN_ MDY_NOTUSED const physx::PxBase* observed, 
    _MIN_ MDY_NOTUSED void* userData, 
    _MIN_ MDY_NOTUSED physx::PxDeletionEventFlag::Enum deletionEvent)
{
#ifdef false
  if(observed->is<physx::PxRigidActor>())
	{
		const physx::PxRigidActor* actor = static_cast<const physx::PxRigidActor*>(observed);

		removeRenderActorsFromPhysicsActor(actor);

		std::vector<physx::PxRigidActor*>::iterator actorIter = std::find(mPhysicsActors.begin(), mPhysicsActors.end(), actor);
		if(actorIter != mPhysicsActors.end()) { mPhysicsActors.erase(actorIter); }
	}
#endif
}

physx::PxErrorCallback& MDyPhysics::MDY_PRIVATE_SPECIFIER(GetPhysXErrorCallback)()
{
	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	return gDefaultErrorCallback;
}

void MDyPhysics::onContact(
    _MIN_ const physx::PxContactPairHeader& pairHeader, 
    _MIN_ const physx::PxContactPair* pairs,
    _MIN_ physx::PxU32 nbPairs)
{
	for(unsigned i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair& cp = pairs[i];

		if(cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
#ifdef false
			if((pairHeader.actors[0] == mSubmarineActor) || (pairHeader.actors[1] == mSubmarineActor))
			{
				PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];			
				Seamine* mine =  reinterpret_cast<Seamine*>(otherActor->userData);
				// insert only once
				if(std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
					mMinesToExplode.push_back(mine);

				break;
			}
#endif
		}
	}
}

void MDyPhysics::onTrigger(_MIN_ physx::PxTriggerPair* pairs, _MIN_ physx::PxU32 count)
{
	for(unsigned i = 0; i < count; i++)
	{
		// ignore pairs when shapes have been deleted
		if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

#ifdef false
		if((pairs[i].otherActor == mSubmarineActor) && (pairs[i].triggerActor == gTreasureActor))
		{
			gTreasureFound = true;
		}
#endif
	}
}

} /// ::dy namespace