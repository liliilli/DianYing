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

namespace dy
{

EDySuccess MDyPhysics::pfInitialize()
{
  ///
  /// @function CreateStack
  /// @brief Test function for create stack actors which consist of shape and rigidbody (dynamic)
  ///
  static auto CreateStack = [&](const physx::PxTransform &t, physx::PxU32 size, physx::PxReal halfExtent)
  {
    using namespace physx;

    PxShape *shape = gPhysicx->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
    for (PxU32 i = 0; i < size; i++)
    {
      for (PxU32 j = 0; j < size - i; j++)
      {
        const PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);

        PxRigidDynamic *body = gPhysicx->createRigidDynamic(t.transform(localTm));
        body->attachShape(*shape);

        PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
        gScene->addActor(*body);
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
    gScene->addActor(*dynamic);

    return dynamic;
  };

  gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gCallback);
  gPhysicx = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);

  physx::PxSceneDesc sceneDescriptor{ gPhysicx->getTolerancesScale() };
  sceneDescriptor.gravity = physx::PxVec3{ 0, -9.81f, 0 };

  gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
  sceneDescriptor.cpuDispatcher = gDispatcher;
  sceneDescriptor.filterShader = physx::PxDefaultSimulationFilterShader;

  gScene = gPhysicx->createScene(sceneDescriptor);

  physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
  if (pvdClient)
  {
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
  }

  gMaterial = gPhysicx->createMaterial(0.5f, 0.5f, 0.6f);
  physx::PxRigidStatic *groundPlane = PxCreatePlane(*gPhysicx, physx::PxPlane(0, 1, 0, 0), *gMaterial);

  gScene->addActor(*groundPlane);

  for (physx::PxU32 i = 0; i < 5; i++)
  {
    CreateStack(physx::PxTransform(physx::PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
  }
  CreateDynamic(physx::PxTransform(physx::PxVec3(0, 40, 100)), physx::PxSphereGeometry(10), physx::PxVec3(0, -50, -100));

  return DY_SUCCESS;
}

EDySuccess MDyPhysics::pfRelease()
{
  // PhysX release
  this->gScene->release();
  this->gPhysicx->release();
  this->gFoundation->release();

  return DY_SUCCESS;
}

void MDyPhysics::Update(float dt)
{
  // PhysX step physics
  gScene->simulate(dt);
  gScene->fetchResults(true);
}

} /// ::dy namespace