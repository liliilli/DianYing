#ifndef GUARD_DY_MANAGEMENT_PHYSICSMANAGER_H
#define GUARD_DY_MANAGEMENT_PHYSICSMANAGER_H
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

#include <PxPhysicsAPI.h>

#include <Dy/Element/Interface/IDyUpdatable.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Helper/PhysXErrorCallback.h>
#include <Dy/Management/Helper/PhysxSimulationCallback.h>

namespace dy
{

///
/// @class MDyPhysics
/// @brief Manages physical simulation, raytracing, sweeping and overlapping etc.
///
class MDyPhysics final : public IDySingleton<MDyPhysics>, public IDyUpdatable, public physx::PxDeletionListener
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
  physx::PxMaterial*            gMaterial   = MDY_INITIALIZE_NULL;
  physx::PxCooking*             mCooking    = MDY_INITIALIZE_NULL;

  physx::PxReal                 stackZ      = 10.0f;
  physx::PxDefaultAllocator     defaultAllocatorCallback;

  // Callback helper class instance.
  FDyPxSimulationEventCallback  mCallback = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_PHYSICSMANAGER_H