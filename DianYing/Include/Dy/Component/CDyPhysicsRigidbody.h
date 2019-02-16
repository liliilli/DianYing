#ifndef GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H
#define GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H
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

#include <PxRigidDynamic.h>
#include <bitset>

#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Component/Internal/Physics/FDyCollisionSignatureContainer.h>
#include <Dy/Component/Type/Physics/EDyCollisionCbType.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DDyCollisionIssueItem;
class CDyPhysicsCollider;
} /// ::dy namesapce

//!
//! Implementation
//!

namespace dy
{

/// @class CDyPhysicsRigidbody
/// @brief Rigidbody component that process physics simulation & manages collision callback with colliders of actor.
class CDyPhysicsRigidbody final : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDyRigidbodyComponentMetaInfo>
{
public:
  CDyPhysicsRigidbody(FDyActor& actorReference);
  virtual ~CDyPhysicsRigidbody() = default;
    
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDyPhysicsRigidbody);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyPhysicsRigidbody)
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyPhysicsRigidbody);

  /// @brief Set rigidbody type. But this component should be deactivated when calling this function,
  /// because internal physics system.
  EDySuccess SetRigidbodyType(_MIN_ EDyRigidbodyType type) noexcept;
  /// @brief get rigidbody type of this component.
  MDY_NODISCARD EDyRigidbodyType GetRigidbodyType() const noexcept;

  /// @brief Set gravity. If component is activated and type is Dynamic, reflect gravity setting.
  void SetGravity(_MIN_ bool iNewValue) noexcept;
  /// @brief Get gravity activation value.
  MDY_NODISCARD bool GetGravity() const noexcept;

  /// @brief Initialize component.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyRigidbodyComponentMetaInfo& descriptor) override;
  /// @brief Release component.
  void Release() override final;

  /// @brief Call collision callback.
  void CallCollisionCallback(_MIN_ EDyCollisionCbType iType, _MIN_ DDyCollisionIssueItem& iItem);
  /// @brief Update anyway.
  void Update(_MIN_ TF32 dt) override final {};

  /// @brief Get string information of this component.
  MDY_NODISCARD std::string ToString() override final;
  
  /// @brief Register activated, valid collider component.
  /// Registered collider will be attached to internal physics system.
  void RegisterCollider(_MIN_ CDyPhysicsCollider& iRefCollider);

  /// @brief Unregister deactivated, valid collider component.
  /// Unregistred collider will be detached from internal physics system.
  void UnregisterCollider(_MIN_ CDyPhysicsCollider& iRefCollider);

  /// @brief
  ///
  EDySuccess BindShapeToRigidbody(_MIN_ CDyPhysicsCollider& iRefShape);

  /// @brief
  ///
  EDySuccess UnbindShapeFromRigidbody(_MIN_ physx::PxShape& iRefShape);

  /// @brief Add collision callback. \n 
  /// If given type, there is already collision callback of iRawFunction, do nothing just return DY_FAILURE.
  template <EDyCollisionCbType TCbType, typename TScriptType>
  EDySuccess AddCollisionCallback(
      _MIN_ TScriptType& iRefScript, 
      _MIN_ typename TCollisionCbTypeSignature<TCbType>::Type::template RawType<TScriptType> iRawFunction)
  {
    static_assert(IsInheritancedFrom<TScriptType, ADyActorCppScript> == true, "TScriptType must be inheritenced from ADyActorCppScript.");
    if constexpr (TCbType == EDyCollisionCbType::OnHit)
    {
      // Try insert 
      auto* ptrItem = this->mCallbackContainer.onHit.AddCallback(iRefScript, iRawFunction);
      if (ptrItem == nullptr) { return DY_FAILURE; } 
      // Try bind item (unique-id specifically) into Script.
      iRefScript.MDY_PRIVATE_SPECIFIER(BindCollisionCbHandle)(*this, TCbType, (void*&)iRawFunction);
      return DY_SUCCESS;
    }
    else if constexpr (TCbType == EDyCollisionCbType::OnOverlapBegin) 
    {
      // Try insert 
      auto* ptrItem = this->mCallbackContainer.onOverlapBegin.AddCallback(iRefScript, iRawFunction);
      if (ptrItem == nullptr) { return DY_FAILURE; } 
      // Try bind item (unique-id specifically) into Script.
      iRefScript.MDY_PRIVATE_SPECIFIER(BindCollisionCbHandle)(*this, TCbType, (void*&)iRawFunction);
      return DY_SUCCESS;
    }
    else if constexpr (TCbType == EDyCollisionCbType::OnOverlapEnd)
    {
      // Try insert 
      auto* ptrItem = this->mCallbackContainer.onOverlapEnd.AddCallback(iRefScript, iRawFunction);
      if (ptrItem == nullptr) { return DY_FAILURE; } 
      // Try bind item (unique-id specifically) into Script.
      iRefScript.MDY_PRIVATE_SPECIFIER(BindCollisionCbHandle)(*this, TCbType, (void*&)iRawFunction);
      return DY_SUCCESS;
    }
  }
  /// @brief Remove collision callback. \n
  /// If not found matched collision callback item given type, just return DY_FAILURE.
  EDySuccess RemoveCollisionCallback(_MIN_ EDyCollisionCbType iType, _MIN_ const void* iId);

  /// @brief Get binded (registered) activated collider instance list.
  auto& GetBindedActivatedColliderList() noexcept { return this->mPtrColliderList; }

  /// @brief Get rigidbody internal specifier value.
  MDY_NODISCARD std::optional<TU32> 
  MDY_PRIVATE_SPECIFIER(GetRigidbodySpecifier)() const noexcept;

  /// @brief Get reference instance of rigidbody. When call this, `mOwnerInternalActor` must be valid.
  MDY_NODISCARD physx::PxRigidActor& 
  MDY_PRIVATE_SPECIFIER(GetRefInternalRigidbody)() noexcept;

private:
  /// @brief Update gravity setting when actor is dynamic and valid.
  void pUpdateSettingGravity(_MIN_ const bool& iNewValue);

  /// Check this rigidbody (and collider) simulate physics. 
  /// If false, all collider do kinematic.
  bool mIsSimulatePhysics = false;
  /// Enable gravity or not.
  bool mIsEnableGravity = false;
  /// Mass of sum of collider.
  DDyClamp<TF32, 0, 100'000>  mMassInKg = 0.001f;
  /// Linear damping of rigidbody
  DDyClamp<TF32, 0, 10'000>   mLinearDamping = 1.0f;
  /// Angular damping of rigidbody
  DDyClamp<TF32, 0, 10'000>   mAngularDamping = 1.0f; 
  /// Lock position axis.
  DLockPreset::D3DAxis mLockPosition{};
  /// Lock rotation axis.
  DLockPreset::D3DAxis mLockRotation{};
  /// Type
  /// If `static`, does not move and fixed.
  /// If `kinematic`..
  EDyRigidbodyType mType = EDyRigidbodyType::Static;

  void TryActivateInstance() override final;
  void pActivateDynamicNKinematicActor();
  void pActivateStaticActor();
  void TryDeactivateInstance() override final;

  /// @brief Manages collider list.
  std::vector<NotNull<CDyPhysicsCollider*>> mPtrColliderList{};
  /// @brief Internal actor.
  physx::PxRigidActor* mOwnerInternalActor = nullptr;
  
  /// @brief Callback container for collision handling.
  FDyCollisionSignatureContainer mCallbackContainer;

  /// @brief 24bit rigidbody specifier unique-id value.
  TU32 mRigidbodySpecifierId : 24;

  /// @brief Counter for rigidbody-id assignment.
  static TU32 sRigidbodyIdCounter;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H