#include <precompiled.h>
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

/// Header file
#include <Dy/Component/CDyPhysicsRigidbody.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Component/CDyPhysicsCollider.h>
#include <Dy/Management/PhysicsManager.h>

namespace dy
{

TU32 CDyPhysicsRigidbody::sRigidbodyIdCounter = 0;

/*
 * {
     "Type": "PhysicsRigidbody",
     "Activated": true,
     "Details": {
       "IsSimulatePhysics": false, "IsEnableGravity": false,
       "MassInKg": 1.0, "LinearDamping": 1.0, "AngularDamping": 1.0,
       "LockPos": { "X": false, "Y": false, "Z": false },
       "LockRot": { "X": false, "Y": false, "Z": false },
       "LockPreset": ""
     }
   }
 */
CDyPhysicsRigidbody::CDyPhysicsRigidbody(FDyActor& actorReference) : ADyGeneralBaseComponent{actorReference}
{ }

EDySuccess CDyPhysicsRigidbody::SetRigidbodyType(_MIN_ EDyRigidbodyType type) noexcept
{
  // Check this component is activated.
  if (this->IsComponentActivated() == true) { return DY_FAILURE; }

  if (this->mType != type) { this->mType = type; }
  return DY_SUCCESS;
}

EDyRigidbodyType CDyPhysicsRigidbody::GetRigidbodyType() const noexcept
{
  return this->mType;
}

void CDyPhysicsRigidbody::SetGravity(_MIN_ bool iNewValue) noexcept
{
  if (this->mIsEnableGravity != iNewValue)
  {
    this->mIsEnableGravity = iNewValue;
    this->pUpdateSettingGravity(this->mIsEnableGravity);
  }
}

bool CDyPhysicsRigidbody::GetGravity() const noexcept
{
  return this->mIsEnableGravity;
}

EDySuccess CDyPhysicsRigidbody::Initialize(_MIN_ const PDyRigidbodyComponentMetaInfo& descriptor)
{
  // Copy all values.
  this->mIsSimulatePhysics  = descriptor.mDetails.mIsSimulatePhysics;
  this->mIsEnableGravity    = descriptor.mDetails.mIsEnableGravity;
  this->mAngularDamping     = descriptor.mDetails.mAngularDamping;
  this->mLinearDamping      = descriptor.mDetails.mLinearDamping;
  this->mMassInKg           = descriptor.mDetails.mMassInKg;
  this->mLockPosition       = descriptor.mDetails.mLockPosition;
  this->mLockRotation       = descriptor.mDetails.mLockRotation;
  this->mType               = descriptor.mDetails.mType;

  // If lockPreset is not empty so get lock axis values from information..
  const auto& lockPreset = descriptor.mDetails.mLockPreset;
  if (lockPreset.empty() == false)
  {
    const auto& settingManager = MDySetting::GetInstance();
    const auto& physics = settingManager.GetPhysicsSetting();
    // If we can not find `lockPreset` lock preset, just let it be.
    if (DyIsMapContains(physics.mLockPresetContainer, lockPreset) == false)
    {
      MDY_LOG_ERROR("Failed to find lock preset item from setting, {}.", lockPreset);
    }
    else
    { // Otherwise, override values.
      const auto& value = physics.mLockPresetContainer.at(lockPreset);
      this->mLockPosition = value.mPosition;
      this->mLockRotation = value.mRotation;
    }
  }

  // Component activation check.
  if (descriptor.mInitiallyActivated == true) 
  { 
    this->Activate(); 
  }

  return DY_SUCCESS;
}

void CDyPhysicsRigidbody::Release()
{
  // Component activation check.
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

std::string CDyPhysicsRigidbody::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

#define MDY_PHYSX_WRITE_LOCK() \
  ::physx::PxSceneWriteLock lock(::dy::MDyPhysics::GetInstance().MDY_PRIVATE_SPECIFIER(GetRefScene)())

void CDyPhysicsRigidbody::TryActivateInstance()
{
  auto& bindedActor    = *this->GetBindedActor();
  // Update transform.
  MDY_NOUSE_RTVAL_EXPR(bindedActor.GetTransform()->GetTransform());
  const auto& worldPos = bindedActor.GetTransform()->GetFinalWorldPosition();
  const auto& worldRot = bindedActor.GetTransform()->GetRotationQuaternion();

  // Initialize internal resource.
  auto& refPhysics = MDyPhysics::GetInstance();
  auto& refSdk = refPhysics.MDY_PRIVATE_SPECIFIER(GetRefInternalSdk)();

  {
    MDY_PHYSX_WRITE_LOCK();
    MDY_ASSERT_FORCE(MDY_CHECK_ISNULL(this->mOwnerDynamicActor), "Internal rigidbody actor must be null.");

    // Create RigidDynamic instance.
    this->mOwnerDynamicActor = refSdk.createRigidDynamic(physx::PxTransform(worldPos, worldRot));
    this->mOwnerDynamicActor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

    const auto& defaultSetting = refPhysics.GetDefaultSetting();
    this->mOwnerDynamicActor->setAngularDamping(defaultSetting.mCommonProperty.mDefaultAngularDamping);
    // Disable default kinematic mode, so mOwnerDynamicActor has a dynamic mode.
    this->mOwnerDynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
    this->pUpdateSettingGravity(this->mIsEnableGravity);
  }

  // Set properties, and assign rigidbody specifier id.
  this->mRigidbodySpecifierId = ++CDyPhysicsRigidbody::sRigidbodyIdCounter;
  // If `sRigidbodyIdCounter` hit maximum value, just reset to 0.
  if (CDyPhysicsRigidbody::sRigidbodyIdCounter == static_cast<TU32>(0xFFFFFF) + 1)
  {
    CDyPhysicsRigidbody::sRigidbodyIdCounter = 0;
  }

  auto ptrColliderList =  bindedActor.GetGeneralComponentList<CDyPhysicsCollider>();
  // Try insert collider and try populate resource.
  for (auto& ptrCollider : ptrColliderList)
  {
    if (ptrCollider->IsComponentActivated() == false) { continue; }
    if (ptrCollider->IsRegistered() == true) { continue; }
    this->RegisterCollider(*ptrCollider);
  }
  
  // Do something.
  MDyPhysics::GetInstance().MDY_PRIVATE_SPECIFIER(RegisterRigidbody)(*this);
}

void CDyPhysicsRigidbody::RegisterCollider(_MIN_ CDyPhysicsCollider& iRefCollider)
{
  // Try find duplication.
  const auto isExist = std::any_of(
      MDY_BIND_BEGIN_END(this->mPtrColliderList), 
      [iPtrCollider = &iRefCollider](const auto& ptrCollider) { return ptrCollider == iPtrCollider; });
  if (isExist == true)
  {
    MDY_LOG_WARNING("Collider component may be duplicated.");
    return;
  }

  // Insert collider instance.
  this->mPtrColliderList.emplace_back(DyMakeNotNull(&iRefCollider));
  // If inserted, call resource population function from collider.
  iRefCollider.MDY_PRIVATE_SPECIFIER(SetRegisterFlag)(true);
  iRefCollider.InitializeInternalResource(*this);
}

void CDyPhysicsRigidbody::TryDeactivateInstance()
{
  // Do something.
  MDyPhysics::GetInstance().MDY_PRIVATE_SPECIFIER(UnregisterRigidbody)(*this);

  // And remove collider registration.
  while (this->mPtrColliderList.empty() == false)
  {
    // Release internal resource & unregister.
    auto& ptrCollider = this->mPtrColliderList.back();
    ptrCollider->ReleaseInternalResource(*this);
    ptrCollider->MDY_PRIVATE_SPECIFIER(SetRegisterFlag)(false);

    // We need to remove from last element because of fast erasion.
    this->mPtrColliderList.erase(this->mPtrColliderList.end() - 1);
  }

  // Reset properties.
  this->mRigidbodySpecifierId = 0;

  {
    MDY_PHYSX_WRITE_LOCK();
    MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->mOwnerDynamicActor), "Internal rigidbody actor must be valid.");
    MDY_ASSERT_FORCE(this->mOwnerDynamicActor->isReleasable() == true, "Internal rigidbody actor is not releasable.");

    this->mOwnerDynamicActor->release();
    this->mOwnerDynamicActor = nullptr;
  }
}

void CDyPhysicsRigidbody::UnregisterCollider(_MIN_ CDyPhysicsCollider& iRefCollider)
{
  // Try find instance.
  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mPtrColliderList),
      [iPtrCollider = &iRefCollider](const auto& ptrCollider) { return ptrCollider == iPtrCollider; });
  MDY_ASSERT_FORCE(it != this->mPtrColliderList.end(), "Unexpected error occurred.");

  // If found, release internal resource from collider.
  it->Get()->ReleaseInternalResource(*this);
  it->Get()->MDY_PRIVATE_SPECIFIER(SetRegisterFlag)(false);
  // And remove.
  DyFastErase(this->mPtrColliderList, it);
}

EDySuccess CDyPhysicsRigidbody::BindShapeToRigidbody(_MIN_ physx::PxShape& iRefShape)
{
  if (this->IsComponentActivated() == false)      { return DY_FAILURE; }
  if (MDY_CHECK_ISNULL(this->mOwnerDynamicActor)) { return DY_FAILURE; }

  const auto& mat = MDyPhysics::GetInstance().GetDefaultPhysicsMaterial();

  const auto geometryType   = iRefShape.getGeometryType();
  const auto geometryHolder = iRefShape.getGeometry();

  // Create new shape (with default materia) and set pose.
  auto* newShape = physx::PxRigidActorExt::createExclusiveShape(*this->mOwnerDynamicActor,  geometryHolder.any(), mat);
  MDY_ASSERT_FORCE(newShape != nullptr, "Unexpected error occurred.");

  return DY_SUCCESS;
}

EDySuccess CDyPhysicsRigidbody::UnbindShapeFromRigidbody(_MIN_ physx::PxShape& iRefShape)
{
  if (this->IsComponentActivated() == false)      { return DY_FAILURE; }
  if (MDY_CHECK_ISNULL(this->mOwnerDynamicActor)) { return DY_FAILURE; }

  this->mOwnerDynamicActor->detachShape(iRefShape);
  return DY_SUCCESS;
}

std::optional<TU32> CDyPhysicsRigidbody::MDY_PRIVATE_SPECIFIER(GetRigidbodySpecifier)() const noexcept
{
  // If deactivated, just return null value.
  if (MDY_CHECK_ISNULL(this->mOwnerDynamicActor)) { return std::nullopt; }

  // If activated, return as bitset type.
  return this->mRigidbodySpecifierId;
}

physx::PxRigidDynamic& CDyPhysicsRigidbody::MDY_PRIVATE_SPECIFIER(GetRefInternalRigidbody)() noexcept
{
  return *this->mOwnerDynamicActor;
}

void CDyPhysicsRigidbody::pUpdateSettingGravity(_MIN_ const bool& iNewValue)
{
  if (this->mOwnerDynamicActor == nullptr)      { return; }
  if (this->mType != EDyRigidbodyType::Dynamic) { return; }

  this->mOwnerDynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !iNewValue);
}

} /// ::dy namespace
