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

#include <Dy/Element/Interface/IDyUpdatable.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DDySettingPhysics;
class CDyPhysicsRigidbody;
} /// ::dy namespace

namespace physx
{
class PxMaterial;
class PxPhysics;
class PxScene;
}

//!
//! Implemnetation
//!

namespace dy
{

/// @class MPhysics
/// @brief Manages physical simulation, raytracing, sweeping and overlapping etc.
class MPhysics final : public IDySingleton<MPhysics>, public IDyUpdatable
{
  MDY_SINGLETON_PROPERTIES(MPhysics);
  MDY_SINGLETON_DERIVED(MPhysics);
public:
  /// @brief Update render object transform.
  void UpdateRenderObjectTransform(TF32 dt);
  /// @brief Update internal PxScene parameter (PxVisualizationParameter)
  void UpdateInternalPxSceneParameter();
  /// @brief Call callback function when not empty and satisfies given condition.
  /// After calling all function, callback list will be cleared.
  void CallCallbackIssueOnce();
  /// @brief Update physical simulation
  void Update(TF32 dt) override final;

  /// @brief Init PhysX All resource related to scene.
  void InitScene();
  /// @brief Release PhysX All Resource related to scene.
  void ReleaseScene();

  /// @brief Get default setting instance.
  const DDySettingPhysics& GetDefaultSetting() const noexcept;
  /// @brief Get default physics material instance reference.
  const physx::PxMaterial& GetDefaultPhysicsMaterial() const noexcept;

  physx::PxPhysics& MDY_PRIVATE(GetRefInternalSdk)();
  physx::PxScene&   MDY_PRIVATE(GetRefScene)();

  /// @brief Try enqueue debug line.
  void TryEnqueueDebugDrawCall();

  /// @brief Register activated rigidbody instance.
  void MDY_PRIVATE(RegisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody);
  /// @brief Unregister deactivated rigidbody instance.
  void MDY_PRIVATE(UnregisterRigidbody)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody);

private:
  class Impl; Impl* mPimpl = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_PHYSICSMANAGER_H