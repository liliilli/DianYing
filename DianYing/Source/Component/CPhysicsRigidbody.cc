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
#include <Dy/Component/CPhysicsRigidbody.h>
#include <Dy/Element/FActor.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Component/Internal/Physics/CBasePhysicsCollider.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/MPhysics.h>
#include <Dy/Management/Type/Physics/DCollisionIssueItem.h>
#include <extensions/PxRigidActorExt.h>
#include <PxPhysicsAPI.h>
#include <Dy/Component/CTransform.h>

namespace dy
{

TU32 CPhysicsRigidbody::sRigidbodyIdCounter = 0;

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
CPhysicsRigidbody::CPhysicsRigidbody(FActor& actorReference) : AGeneralBaseComponent{actorReference}
{ }

EDySuccess CPhysicsRigidbody::SetRigidbodyType(EDyRigidbodyType type) noexcept
{
  // Check this component is activated.
  if (this->IsComponentActivated() == true) { return DY_FAILURE; }

  if (this->mType != type) { this->mType = type; }
  return DY_SUCCESS;
}

EDyRigidbodyType CPhysicsRigidbody::GetRigidbodyType() const noexcept
{
  return this->mType;
}

void CPhysicsRigidbody::SetGravity(bool iNewValue) noexcept
{
  if (this->mIsEnableGravity != iNewValue)
  {
    this->mIsEnableGravity = iNewValue;
    this->pUpdateSettingGravity(this->mIsEnableGravity);
  }
}

bool CPhysicsRigidbody::GetGravity() const noexcept
{
  return this->mIsEnableGravity;
}

EDySuccess CPhysicsRigidbody::Initialize(const PDyRigidbodyComponentMetaInfo& descriptor)
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
    const auto& settingManager = MSetting::GetInstance();
    const auto& physics = settingManager.GetPhysicsSetting();
    // If we can not find `lockPreset` lock preset, just let it be.
    if (Contains(physics.mLockPresetContainer, lockPreset) == false)
    {
      DyPushLogError("Failed to find lock preset item from setting, {}.", lockPreset);
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

void CPhysicsRigidbody::Release()
{
  // Component activation check.
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
  // Clear all collision callback.
  this->mCallbackContainer.onHit          .RemoveAll();
  this->mCallbackContainer.onOverlapBegin .RemoveAll();
  this->mCallbackContainer.onOverlapEnd   .RemoveAll();
}

void CPhysicsRigidbody::CallCollisionCallback(ECollisionCallbackType iType, DCollisionIssueItem& iItem)
{
  switch (iType)
  {
  case ECollisionCallbackType::OnHit: 
  { // OnHit.
    for (auto& item : this->mCallbackContainer.onHit.GetBoundCallbackList())
    {
      if (item.mIsCallable == false) { continue; }
      item.mCallbackFunction(iItem.mPtrSelfCollider, *iItem.mPtrSelfActor, iItem.mPtrOtherCollider, *iItem.mPtrOtherActor, iItem.mHitResult);      
    }
  } break;
  case ECollisionCallbackType::OnOverlapBegin: 
  { // OnOverlapBegin.
    for (auto& item : this->mCallbackContainer.onOverlapBegin.GetBoundCallbackList())
    {
      if (item.mIsCallable == false) { continue; }
      item.mCallbackFunction(iItem.mPtrSelfCollider, *iItem.mPtrSelfActor, iItem.mPtrOtherCollider, *iItem.mPtrOtherActor, iItem.mHitResult);      
    }
  } break;
  case ECollisionCallbackType::OnOverlapEnd: 
  { // OnOverlapEnd.
    for (auto& item : this->mCallbackContainer.onOverlapEnd.GetBoundCallbackList())
    {
      if (item.mIsCallable == false) { continue; }
      item.mCallbackFunction(iItem.mPtrSelfCollider, *iItem.mPtrSelfActor, iItem.mPtrOtherCollider, *iItem.mPtrOtherActor, iItem.mHitResult);      
    }
  } break;
  }
}

std::string CPhysicsRigidbody::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

#define MDY_PHYSX_WRITE_LOCK() \
  ::physx::PxSceneWriteLock lock(::dy::MPhysics::GetInstance().MDY_PRIVATE(GetRefScene)())

void CPhysicsRigidbody::TryActivateInstance()
{
  auto& bindedActor = *this->GetBindedActor();
  switch (this->mType)
  {
  case EDyRigidbodyType::Static: 
  { this->pActivateStaticActor();
  } break;
  case EDyRigidbodyType::Kinematic: 
  case EDyRigidbodyType::Dynamic: 
  { this->pActivateDynamicNKinematicActor();
  } break;
  }

  // Set properties, and assign rigidbody specifier id.
  this->mRigidbodySpecifierId = ++CPhysicsRigidbody::sRigidbodyIdCounter;
  // If `sRigidbodyIdCounter` hit maximum value, just reset to 0.
  if (CPhysicsRigidbody::sRigidbodyIdCounter == static_cast<TU32>(0xFFFFFF) + 1)
  {
    CPhysicsRigidbody::sRigidbodyIdCounter = 0;
  }

  auto ptrColliderList =  bindedActor.GetGeneralComponentList<CBasePhysicsCollider>();
  // Try insert collider and try populate resource.
  for (auto& ptrCollider : ptrColliderList)
  {
    if (ptrCollider->IsComponentActivated() == false) { continue; }
    if (ptrCollider->IsRegistered() == true) { continue; }
    this->RegisterCollider(*ptrCollider);
  }
  
  // Do something.
  MPhysics::GetInstance().MDY_PRIVATE(RegisterRigidbody)(*this);
}

void CPhysicsRigidbody::pActivateDynamicNKinematicActor()
{
  auto& bindedActor    = *this->GetBindedActor();
  // Update transform.
  MDY_NOUSE_RTVAL_EXPR(bindedActor.GetTransform()->GetTransform());
  const auto& worldPos = bindedActor.GetTransform()->GetFinalWorldPosition();
  const auto& worldRot = bindedActor.GetTransform()->GetRotationQuaternion();

  // Initialize internal resource.
  auto& refPhysics = MPhysics::GetInstance();
  auto& refSdk = refPhysics.MDY_PRIVATE(GetRefInternalSdk)();
  const auto& defaultSetting = refPhysics.GetDefaultSetting();

  {
    ::physx::PxSceneWriteLock lock(::dy::MPhysics::GetInstance().MDY_PRIVATE(GetRefScene)());
    MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNULL(this->mOwnerInternalActor), "Internal rigidbody actor must be null.");

    // Create RigidDynamic instance.
    this->mOwnerInternalActor = refSdk.createRigidDynamic(physx::PxTransform(worldPos, worldRot));
    this->mOwnerInternalActor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

    auto& dynamicActor = static_cast<physx::PxRigidDynamic&>(*this->mOwnerInternalActor);
    // Set damping.
    dynamicActor.setAngularDamping(defaultSetting.mCommonProperty.mDefaultAngularDamping);
    // Disable default kinematic mode, so mOwnerInternalActor has a dynamic mode.
    if (this->mType == EDyRigidbodyType::Dynamic)
    { dynamicActor.setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false); }
    else
    { dynamicActor.setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true); }
    // Set gravity.
    this->pUpdateSettingGravity(this->mIsEnableGravity);
  }
}

void CPhysicsRigidbody::pActivateStaticActor()
{
  auto& bindedActor = *this->GetBindedActor();
  // Update transform.
  MDY_NOUSE_RTVAL_EXPR(bindedActor.GetTransform()->GetTransform());
  const auto& worldPos = bindedActor.GetTransform()->GetFinalWorldPosition();
  const auto& worldRot = bindedActor.GetTransform()->GetRotationQuaternion();

  // Initialize internal resource.
  auto& refPhysics = MPhysics::GetInstance();
  auto& refSdk = refPhysics.MDY_PRIVATE(GetRefInternalSdk)();
  const auto& defaultSetting = refPhysics.GetDefaultSetting();

  {
    MDY_PHYSX_WRITE_LOCK();
    MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNULL(this->mOwnerInternalActor), "Internal rigidbody actor must be null.");

    // Create RigidDynamic instance.
    this->mOwnerInternalActor = refSdk.createRigidStatic(physx::PxTransform(worldPos, worldRot));
    this->mOwnerInternalActor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);
  }
}

void CPhysicsRigidbody::RegisterCollider(CBasePhysicsCollider& iRefCollider)
{
  // Try find duplication.
  const auto isExist = std::any_of(
      MDY_BIND_BEGIN_END(this->mPtrColliderList), 
      [iPtrCollider = &iRefCollider](const auto& ptrCollider) { return ptrCollider == iPtrCollider; });
  if (isExist == true)
  {
    DyPushLogWarning("Collider component may be duplicated.");
    return;
  }

  // Insert collider instance.
  this->mPtrColliderList.emplace_back(DyMakeNotNull(&iRefCollider));
  // If inserted, call resource population function from collider.
  iRefCollider.MDY_PRIVATE(SetRegisterFlag)(true);
  iRefCollider.InitializeInternalResource(*this);
}

void CPhysicsRigidbody::TryDeactivateInstance()
{
  // Do something.
  MPhysics::GetInstance().MDY_PRIVATE(UnregisterRigidbody)(*this);

  // And remove collider registration.
  while (this->mPtrColliderList.empty() == false)
  {
    // Release internal resource & unregister.
    auto& ptrCollider = this->mPtrColliderList.back();
    ptrCollider->ReleaseInternalResource(*this);
    ptrCollider->MDY_PRIVATE(SetRegisterFlag)(false);

    // We need to remove from last element because of fast erasion.
    this->mPtrColliderList.erase(this->mPtrColliderList.end() - 1);
  }

  // Reset properties.
  this->mRigidbodySpecifierId = 0;

  {
    ::physx::PxSceneWriteLock lock(::dy::MPhysics::GetInstance().MDY_PRIVATE(GetRefScene)());
    MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNOTNULL(this->mOwnerInternalActor), "Internal rigidbody actor must be valid.");
    MDY_ASSERT_MSG_FORCE(this->mOwnerInternalActor->isReleasable() == true, "Internal rigidbody actor is not releasable.");

    this->mOwnerInternalActor->release();
    this->mOwnerInternalActor = nullptr;
  }
}

void CPhysicsRigidbody::UnregisterCollider(CBasePhysicsCollider& iRefCollider)
{
  // Try find instance.
  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mPtrColliderList),
      [iPtrCollider = &iRefCollider](const auto& ptrCollider) { return ptrCollider == iPtrCollider; });
  MDY_ASSERT_MSG_FORCE(it != this->mPtrColliderList.end(), "Unexpected error occurred.");

  // If found, release internal resource from collider.
  it->Get()->ReleaseInternalResource(*this);
  it->Get()->MDY_PRIVATE(SetRegisterFlag)(false);
  // And remove.
  FaseErase(this->mPtrColliderList, it);
}

EDySuccess CPhysicsRigidbody::BindShapeToRigidbody(CBasePhysicsCollider& iRefShape)
{
  if (this->IsComponentActivated() == false)      { return DY_FAILURE; }
  if (MDY_CHECK_ISNULL(this->mOwnerInternalActor)) { return DY_FAILURE; }

  const auto& mat = MPhysics::GetInstance().GetDefaultPhysicsMaterial();

  const auto geometryHolder = iRefShape.__GetPtrInternalShape()->getGeometry();
  // Create new shape (with default materia) and set pose.
  auto* newShape = physx::PxRigidActorExt::createExclusiveShape(*this->mOwnerInternalActor,  geometryHolder.any(), mat);
  MDY_ASSERT_MSG_FORCE(newShape != nullptr, "Unexpected error occurred.");
  // CAUTION(!) settin filter data must be hald afterward createExclusiveShape.
  newShape->setSimulationFilterData(iRefShape.mInternalFilterData);
  // Set user data for handling collision callback, and etc.
  newShape->userData = &iRefShape;

  return DY_SUCCESS;
}

EDySuccess CPhysicsRigidbody::UnbindShapeFromRigidbody(physx::PxShape& iRefShape)
{
  if (this->IsComponentActivated() == false)      { return DY_FAILURE; }
  if (MDY_CHECK_ISNULL(this->mOwnerInternalActor)) { return DY_FAILURE; }

  this->mOwnerInternalActor->detachShape(iRefShape);
  return DY_SUCCESS;
}

EDySuccess CPhysicsRigidbody::RemoveCollisionCallback(ECollisionCallbackType iType, const void* iId)
{
  switch (iType)
  {
  case ECollisionCallbackType::OnHit:           return this->mCallbackContainer.onHit.RemoveCallback(iId);
  case ECollisionCallbackType::OnOverlapBegin:  return this->mCallbackContainer.onOverlapBegin.RemoveCallback(iId);
  case ECollisionCallbackType::OnOverlapEnd:    return this->mCallbackContainer.onOverlapEnd.RemoveCallback(iId); 
  }
  return DY_FAILURE;
}

std::optional<TU32> CPhysicsRigidbody::MDY_PRIVATE(GetRigidbodySpecifier)() const noexcept
{
  // If deactivated, just return null value.
  if (MDY_CHECK_ISNULL(this->mOwnerInternalActor)) { return std::nullopt; }

  // If activated, return as bitset type.
  return this->mRigidbodySpecifierId;
}

physx::PxRigidActor& CPhysicsRigidbody::MDY_PRIVATE(GetRefInternalRigidbody)() noexcept
{
  return *this->mOwnerInternalActor;
}

void CPhysicsRigidbody::pUpdateSettingGravity(const bool& iNewValue)
{
  if (this->mOwnerInternalActor == nullptr)      { return; }
  if (this->mType != EDyRigidbodyType::Dynamic) { return; }

  this->mOwnerInternalActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !iNewValue);
}

} /// ::dy namespace
