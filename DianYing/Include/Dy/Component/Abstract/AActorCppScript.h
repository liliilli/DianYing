#ifndef GUARD_DY_COMPONENT_IDYCPPSCRIPT_H
#define GUARD_DY_COMPONENT_IDYCPPSCRIPT_H
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

#include <Dy/Component/Interface/IScriptable.h>
#include <Dy/Helper/System/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class ECollisionCallbackType;
class CDyPhysicsRigidbody;
class FDyActor;
class CActorScriptCpp;
class MGameTimer;
class FDyTimerHandle;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class AActorCppScript
/// @brief Cpp script for Actor.
MDY_ABSTRACT AActorCppScript : public IScriptable
{
public:
  /// @brief Get widget reference.
  MDY_NODISCARD FDyActor& GetActorReference();

  /// @brief Get Game-Timer manager.
  MDY_NODISCARD MGameTimer& GetGameTimerManager() noexcept;

  void MDY_PRIVATE(BindPtrTimerHandle)(FDyTimerHandle& iRefTimerHandler);
  void MDY_PRIVATE(DetachPtrTimerHandle)(FDyTimerHandle& iRefTimerHandler);
  void MDY_PRIVATE(AbortAllValidTimerHandler)();

  void MDY_PRIVATE(BindCollisionCbHandle)(CDyPhysicsRigidbody& iRefRigidbody, ECollisionCallbackType iType, const void* iUniqueId);
  EDySuccess MDY_PRIVATE(DetachCollisionCbHandle)(const void * iSpecificationId);
  void MDY_PRIVATE(AbortAllCollisionCallback)();

private:
  /// @brief Set reference to outside world.
  /// @param outsideReference Reference of WidgetScriptCpp instance.
  void pfSetOutsideReference(_MIN_ CActorScriptCpp& outsideReference) noexcept;

  /// @brief
  CActorScriptCpp* mOutside = nullptr;
  /// @brief
  std::vector<NotNull<FDyTimerHandle*>> mPtrTimerHandleList = {};

  using TCollisionCbHandle = std::tuple<CDyPhysicsRigidbody*, ECollisionCallbackType, const void*>;
  /// @brief Collision handle list. 
  std::vector<TCollisionCbHandle> mCollisionCbHandleList = {};

  friend class CActorScriptCpp;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_IDYCPPSCRIPT_H