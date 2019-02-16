#ifndef GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H
#define GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H
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

#include <PxFiltering.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include "Dy/Helper/Type/Area3D.h"

//!
//! Forward declaration
//!

namespace physx
{
class PxShape;
} /// ::physx namespace

namespace dy
{
class CDyPhysicsRigidbody;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyPhysicsCollider
/// @brief Collider component of physics simuation.
///
class CDyPhysicsCollider : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDyColliderComponentMetaInfo>
{
public:
  CDyPhysicsCollider(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference) {};
  virtual ~CDyPhysicsCollider() = default;
      
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDyPhysicsCollider);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyPhysicsCollider)
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyPhysicsCollider);

  /// @brief Initialize component.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc) override;

  /// @brief Update anyway.
  void Update(_MIN_ TF32 dt) override final {};
  /// @brief Update collider mesh information.
  virtual void UpdateColliderMesh() = 0;
  /// @brief Update AABB bound.
  void UpdateBound(_MIN_ const DDyArea3D& iArea);
  /// @brief Get const reference instance of collider mesh.
  MDY_NODISCARD const std::vector<DDyVector3>& GetColliderMesh() const noexcept;

  /// @brief Check notify hit event.
  MDY_NODISCARD bool IsNotifyHitEvent() const noexcept; 
  /// @brief Check notify overlap event.
  MDY_NODISCARD bool IsNotifyOverlapEvent() const noexcept;
  /// @brief Check this collider is registered into CDyPhysicsRigidbody component.
  MDY_NODISCARD bool IsRegistered() const noexcept;
  /// @brief Check collider need to update internal mesh information.
  MDY_NODISCARD bool IsNeedToUpdateColliderMesh() const noexcept;
  /// @brief Get collider type of this component.
  MDY_NODISCARD EDyColliderType GetColliderType() const noexcept;
  /// @brief Get bound of collider.
  MDY_NODISCARD const DDyArea3D& GetBound() const noexcept;

  /// @brief Set register flag.
  void MDY_PRIVATE_SPECIFIER(SetRegisterFlag)(_MIN_ bool iFlag) noexcept;

  /// @brief Initialize internal (PhysX) resource.
  virtual void InitializeInternalResource(_MINOUT_ CDyPhysicsRigidbody& iRefRigidbody) = 0; 
  /// @brief Release internal (PhysX) resource.
  void ReleaseInternalResource(_MINOUT_ CDyPhysicsRigidbody& iRefRigidbody);

  /// @brief Get internal shape pointer. Returned value does not guarantee not nullity.
  MDY_NODISCARD physx::PxShape* MDY_PRIVATE_SPECIFIER(GetPtrInternalShape)() const noexcept;
  /// @brief Internal filter data.
  physx::PxFilterData mInternalFilterData{};

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  /// @brief If true, this component notify hit event (callback) when target hit.
  bool mNotifyHitEvent = false;
  /// @brief If true, this component notify overlap event when target overlapped.
  bool mNotifyOverlapEvent = false;
  /// @brief Flag for registration. \n
  /// When activated & Activate rigidActor component is exist on Actor, this flag must be true.
  /// Otherwise, this flag must be false.
  bool mIsRegistered = false; 

  /// @brief Collision filter preset specifier.
  std::string mFilterPresetSpecifier = MDY_INITIALIZE_EMPTYSTR;

protected:
  /// @brief Make filter data. This function does not have side effects.
  MDY_NODISCARD physx::PxFilterData CreateFilterDataValue(
      _MIN_ const CDyPhysicsRigidbody& iRigidbody, 
      _MIN_ const std::string& iLayerName,
      _MIN_ std::vector<EDyCollisionFilter>& iFilterData);

  /// Collider type of this component.
  EDyColliderType mColliderType = EDyColliderType::NoneError;
  /// @brief Internal shape pointer instance. 
  physx::PxShape* mPtrInternalShape = nullptr;

  /// @brief Collision filter values.
  std::vector<EDyCollisionFilter> mFilterValues{};
  /// @brief Collision spcifier value.
  std::string mCollisionTagName = MDY_INITIALIZE_EMPTYSTR;
  /// @brief Collision mesh information need to be updated.
  bool mIsCollisionMeshDirty = true;
  /// @brief Collision mesh information.
  std::vector<DDyVector3> mColliderMeshInformation; 
  /// @brief AABB bound information.
  DDyArea3D mAABBBound;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H