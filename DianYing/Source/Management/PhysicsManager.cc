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
#include <Dy/Management/PhysicsManager.h>
#include <Dy/Helper/Pointer.h>
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

  // FunctionBody ∨

  gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gCallback);
  gPhysicx = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);

  physx::PxSceneDesc sceneDescriptor { gPhysicx->getTolerancesScale() };
  sceneDescriptor.filterShader              = DyFilterShader;
  sceneDescriptor.simulationEventCallback   = &this->mCallback;
  //sceneDescriptor.flags                    |= physx::PxSceneFlag::eREQUIRE_RW_LOCK;
  sceneDescriptor.gravity                   = physx::PxVec3{ 0, -9.81f, 0 };

  gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
  sceneDescriptor.cpuDispatcher = gDispatcher;
  sceneDescriptor.filterShader = physx::PxDefaultSimulationFilterShader;

  gScene = gPhysicx->createScene(sceneDescriptor);
  physx::PxSceneWriteLock scopedLock(*gScene);

  physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
  if (pvdClient)
  {
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
  }

  gMaterial = gPhysicx->createMaterial(0.5f, 0.5f, 0.6f);
  physx::PxRigidStatic *groundPlane = PxCreatePlane(*gPhysicx, physx::PxPlane(0, 1, 0, 0), *gMaterial);
  DySetupFiltering(DyMakeNotNull(groundPlane), EDyTempCollisionLayer::Floor, EDyTempCollisionLayer::Sphere);

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

  return DY_SUCCESS;
}

EDySuccess MDyPhysics::pfRelease()
{
#ifdef false
  //physx::PxSceneWriteLock scopedLock(*gScene);

  // PhysX release
  this->gScene->release();
  this->gPhysicx->release();
  this->gFoundation->release();
#endif

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

} /// ::dy namespace