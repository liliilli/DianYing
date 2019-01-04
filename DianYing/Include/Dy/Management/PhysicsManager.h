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
class MDyPhysics final : public IDySingleton<MDyPhysics>, public IDyUpdatable
{
  MDY_SINGLETON_PROPERTIES(MDyPhysics);
  MDY_SINGLETON_DERIVED(MDyPhysics);
public:
  /// Update physical simulation
  void Update(float dt) override final;

private:
  FDyPhysXErrorCallback         gCallback;

  physx::PxDefaultAllocator     gAllocator;
  physx::PxPvd*                 gPvd;
  physx::PxFoundation*          gFoundation;
  physx::PxPhysics*             gPhysicx;
  physx::PxCpuDispatcher*       gDispatcher;
  physx::PxScene*               gScene;
  physx::PxMaterial*            gMaterial;

  physx::PxReal                 stackZ = 10.0f;

  // Callback helper class instance.
  FDyPxSimulationEventCallback  mCallback = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_PHYSICSMANAGER_H