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

#include <Dy/Component/Interface/IDyScriptable.h>
#include <Dy/Helper/System/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyCollisionCbType;
class CDyPhysicsRigidbody;
class FDyActor;
class CDyActorScriptCpp;
class MGameTimer;
class FDyTimerHandle;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class ADyActorCppScript
/// @brief Cpp script for Actor.
///
MDY_ABSTRACT ADyActorCppScript : public IDyScriptable
{
public:
  /// @brief Get widget reference.
  MDY_NODISCARD FDyActor& GetActorReference();

  /// @brief Get Game-Timer manager.
  MDY_NODISCARD MGameTimer& GetGameTimerManager() noexcept;

  void MDY_PRIVATE(BindPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler);
  void MDY_PRIVATE(DetachPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler);
  void MDY_PRIVATE(AbortAllValidTimerHandler)();

  void MDY_PRIVATE(BindCollisionCbHandle)(_MIN_ CDyPhysicsRigidbody& iRefRigidbody, _MIN_ EDyCollisionCbType iType, _MIN_ const void* iUniqueId);
  EDySuccess MDY_PRIVATE(DetachCollisionCbHandle)(_MIN_ const void * iSpecificationId);
  void MDY_PRIVATE(AbortAllCollisionCallback)();

private:
  /// @brief Set reference to outside world.
  /// @param outsideReference Reference of WidgetScriptCpp instance.
  void pfSetOutsideReference(_MIN_ CDyActorScriptCpp& outsideReference) noexcept;

  /// @brief
  CDyActorScriptCpp* mOutside = nullptr;
  /// @brief
  std::vector<NotNull<FDyTimerHandle*>> mPtrTimerHandleList = {};

  using TCollisionCbHandle = std::tuple<CDyPhysicsRigidbody*, EDyCollisionCbType, const void*>;
  /// @brief Collision handle list. 
  std::vector<TCollisionCbHandle> mCollisionCbHandleList = {};

  friend class CDyActorScriptCpp;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_IDYCPPSCRIPT_H