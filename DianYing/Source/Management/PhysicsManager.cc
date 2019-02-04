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
#include <Dy/Management/LoggingManager.h>

//!
//! Test
//!

namespace
{

std::vector<dy::NotNull<physx::PxRigidBody*>> sRigidbodies = {};

///
/// @function DyFilterShader
/// @brief PhysX Filter shader customized setting function.
///
physx::PxFilterFlags DyFilterShader(
  physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
  physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
  physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	// let triggers through
	if(physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
  }
	return physx::PxFilterFlag::eDEFAULT;
}

///
/// @enum EDyTempCollisionLayer
///
enum EDyTempCollisionLayer
{
  Stack  = (1 << 0),
  Sphere = (1 << 1),
  Floor  = (1 << 2),
};

///
/// @brief Setup filering to PxActor to apply.
/// @param actor
/// @param filterGroup
/// @param filterMask
///
void DySetupFiltering(const dy::NotNull<physx::PxRigidActor*>& actor, physx::PxU32 filterGroup, physx::PxU32 filterMask)
{
  physx::PxFilterData filterData;
	filterData.word0  = filterGroup;  // word0 = own ID
	filterData.word1  = filterMask;	  // word1 = ID mask to filter pairs that trigger a contact callback;

	const physx::PxU32 numShapes = actor->getNbShapes();
  std::vector<physx::PxShape*> shapes(numShapes);
  actor->getShapes(&shapes[0], numShapes);

	for(physx::PxU32 i = 0; i < numShapes; i++)
	{
	  physx::PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
}

std::vector<std::string> sDebugActorName;

}

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyPhysics::pfInitialize()
{
#ifdef false
  ///
  /// @function CreateStack
  /// @brief Test function for create stack actors which consist of shape and rigidbody (dynamic)
  ///
  static auto CreateStack = [&](const physx::PxTransform &t, physx::PxU32 size, physx::PxReal halfExtent)
  {
    using namespace physx;

    PxShape *shape = gPhysicx->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);

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

    PxRigidDynamic *dynamic = PxCreateDynamic(*gPhysicx, t, geometry, *gMaterial, 10.0f);
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

  //gMaterial = gPhysicx->createMaterial(0.5f, 0.5f, 0.6f);
  //physx::PxRigidStatic *groundPlane = PxCreatePlane(*gPhysicx, physx::PxPlane(0, 1, 0, 0), *gMaterial);
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
  // This function just check all resource is released.
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gScene), "PhysX scene is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gDispatcher), "PhysX cpu dispatcher is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gPhysicx), "PhysX physics is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gPvd), "PhysX pvd is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->mCooking), "PhysX mesh cooker is not released before release Physics manager.");
  MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->gFoundation), "PhysX foundation is not released before release Physics manager.");

  return DY_SUCCESS;
}

void MDyPhysics::Update(float dt)
{
#ifdef false
  // PhysX step physics
  if (dt > 0)
  {
    gScene->simulate(dt);
    gScene->fetchResults(true);
  }

#endif

  // Print information
#ifdef false
  for (const auto& rigidbody : sRigidbodies)
  {
    const auto& position = rigidbody->getGlobalPose().p;
    const auto& quat     = rigidbody->getGlobalPose().q;

    MDY_LOG_CRITICAL("Rigidbody Position : ({}, {}, {}) Quaternion : ({}, {}, {}, {})",
                     position.x, position.y, position.z,
                     quat.x, quat.y, quat.z, quat.w);
  }
#endif
}

void MDyPhysics::InitScene()
{
  MDY_ASSERT(MDY_CHECK_ISNULL(this->gFoundation), "Foundation is already exist.");
  MDY_ASSERT(MDY_CHECK_ISNULL(this->gPhysicx), "Physics is already exist.");
  MDY_ASSERT(MDY_CHECK_ISNULL(this->mCooking), "Cooking is already exist.");
  MDY_ASSERT(MDY_CHECK_ISNULL(this->gMaterial), "Default material is already exist.");

  this->gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->defaultAllocatorCallback, MDY_PRIVATE_SPECIFIER(GetPhysXErrorCallback)());
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gFoundation), "PhysX Foundation must be created successfully.");

  // Get scale from setting manager, but we use defualt value temporary. 
  physx::PxTolerancesScale temporalScale;
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
  this->gMaterial = this->gPhysicx->createMaterial(0.5f, 0.5, 1.0f);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gMaterial), "PhysX Default material must be created.");

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

  this->gScene = this->gPhysicx->createScene(tempSceneDesc);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gScene), "PhysX Scene must be created successfully.");

  // Set Scene setting (atomically)
  {
    physx::PxSceneWriteLock scopedLock(*this->gScene);
    auto flags = this->gScene->getFlags();
    (void)flags;

    this->gScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 0.0f);
    this->gScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES,	1.0f);

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
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gFoundation),  "PhysX Foundation must be valid.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gPhysicx),     "PhysX Physicx must be valid.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gScene),       "PhysX Scene must be valid.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gDispatcher),  "PhysX Dispatcher must be valid.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mCooking),     "PhysX Cooking must be valid.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->gMaterial),    "PhysX Default material must be valid.");

  {
    physx::PxSceneWriteLock scopedLock(*this->gScene);
    // Do something that is relevent to gScene but not direct.
  }

  this->gScene->release();
  this->gScene = nullptr;

  this->gDispatcher->release();
  this->gDispatcher = nullptr;
  
  this->gMaterial->release();
  this->gMaterial = nullptr;
    
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

} /// ::dy namespace