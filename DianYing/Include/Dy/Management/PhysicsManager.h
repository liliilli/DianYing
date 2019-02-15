#ifndef GUARD_DY_MANAGEMENT_PHYSICSMANAGER_H
#define GUARD_DY_MANAGEMENT_PHYSICSMANAGER_H
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

#include <PxPhysicsAPI.h>

#include <Dy/Element/Interface/IDyUpdatable.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Helper/PhysXErrorCallback.h>
#include <Dy/Management/Helper/PhysxSimulationCallback.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DDySettingPhysics;
class CDyPhysicsRigidbody;
} /// ::dy namespace

//!
//! Implemnetation
//!

namespace dy
{

///
/// @class MDyPhysics
/// @brief Manages physical simulation, raytracing, sweeping and overlapping etc.
///
class MDyPhysics final : public IDySingleton<MDyPhysics>, public IDyUpdatable, public physx::PxDeletionListener, public physx::PxSimulationEventCallback
{
  MDY_SINGLETON_PROPERTIES(MDyPhysics);
  MDY_SINGLETON_DERIVED(MDyPhysics);
public:
  /// Update physical simulation
  void Update(float dt) override final;

  /// @brief Init PhysX All resource related to scene.
  void InitScene();
  /// @brief Release PhysX All Resource related to scene.
  void ReleaseScene();

  /// @brief Get default setting instance.
  const DDySettingPhysics& GetDefaultSetting() const noexcept;

  /// @brief Get default physics material instance reference.
  const physx::PxMaterial& GetDefaultPhysicsMaterial() const noexcept;

  auto& MDY_PRIVATE_SPECIFIER(GetRefInternalSdk)()
  {
    MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->gPhysicx), "Internal PhysX PxPhysics must be initailized.");
    return *this->gPhysicx;
  }

  auto& MDY_PRIVATE_SPECIFIER(GetRefScene)()
  {
    MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->gScene), "Internal PhysX PxScene must be initailized.");
    return *this->gScene;
  }

  /// @brief Update internal PxScene parameter (PxVisualizationParameter)
  void UpdateInternalPxSceneParameter();

  /// @brief Try enqueue debug line.
  void TryEnqueueDebugDrawCall();

  /// @brief Register activated rigidbody instance.
  void MDY_PRIVATE_SPECIFIER(RegisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody);

  /// @brief Unregister deactivated rigidbody instance.
  void MDY_PRIVATE_SPECIFIER(UnregisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody);

private:
  /// @brief Override function callback. When object of physx is remove, this function will be called.
	void onRelease(
      _MIN_ MDY_NOTUSED const physx::PxBase* observed, 
      _MIN_ MDY_NOTUSED void* userData, 
      _MIN_ MDY_NOTUSED physx::PxDeletionEventFlag::Enum deletionEvent) override final;

  /// @brief Get PhyX error callback function instance.
  static physx::PxErrorCallback& MDY_PRIVATE_SPECIFIER(GetPhysXErrorCallback)();

  /// @brief
  MDY_NODISCARD FORCEINLINE physx::PxSimulationFilterShader 
  MDY_PRIVATE_SPECIFIER(GetSampleFilterShader)()
  {
    return physx::PxDefaultSimulationFilterShader;
  }

  FDyPhysXErrorCallback         gCallback;

  physx::PxPvd*                 gPvd        = MDY_INITIALIZE_NULL;
  physx::PxFoundation*          gFoundation = MDY_INITIALIZE_NULL;
  physx::PxPhysics*             gPhysicx    = MDY_INITIALIZE_NULL; 
  physx::PxDefaultCpuDispatcher*gDispatcher = MDY_INITIALIZE_NULL;
  physx::PxScene*               gScene      = MDY_INITIALIZE_NULL;
  physx::PxMaterial*            mDefaultMaterial   = MDY_INITIALIZE_NULL;
  physx::PxCooking*             mCooking    = MDY_INITIALIZE_NULL;

  physx::PxReal                 stackZ      = 10.0f;
  physx::PxDefaultAllocator     defaultAllocatorCallback;

  /// @brief Activated ptr-rigidbody list.
  std::vector<NotNull<CDyPhysicsRigidbody*>> mActivatedRigidbodyList {};

  // Implements PxSimulationEventCallback

	void onContact(_MIN_ const physx::PxContactPairHeader& pairHeader, _MIN_ const physx::PxContactPair* pairs, _MIN_ physx::PxU32 nbPairs) override final;
	void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override final {}
	void onWake(physx::PxActor** , physx::PxU32) override final {}
	void onSleep(physx::PxActor** , physx::PxU32) override final {}
	void onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) override final {}
  
  // Garbage Bin
	void onTrigger(MDY_NOTUSED physx::PxTriggerPair* pairs, MDY_NOTUSED physx::PxU32 count) override final {  /* We do not use onTrigger on Dy. */ }
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_PHYSICSMANAGER_H