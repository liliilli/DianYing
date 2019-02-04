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

namespace dy
{

CDyPhysicsRigidbody::CDyPhysicsRigidbody(FDyActor& actorReference) : ADyGeneralBaseComponent{actorReference}
{ }

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
  if (descriptor.mInitiallyActivated == true) { this->Activate(); }

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

void CDyPhysicsRigidbody::TryActivateInstance()
{
  auto& bindedActor    = *this->GetBindedActor();
  auto ptrColliderList =  bindedActor.GetGeneralComponentList<CDyPhysicsCollider>();

  // Try insert collider and try populate resource.
  for (auto& ptrCollider : this->mPtrColliderList)
  {
    if (ptrCollider->IsRegistered() == true) { continue; }
    this->RegisterCollider(*ptrCollider);
  }
  
  // Do something.
}

void CDyPhysicsRigidbody::TryDeactivateInstance()
{
  // Do something.
  

  // And remove collider registration.
  while (this->mPtrColliderList.empty() == false)
  {
    // Release internal resource & unregister.
    auto& ptrCollider = this->mPtrColliderList.back();
    ptrCollider->ReleaseInternalResource();
    ptrCollider->MDY_PRIVATE_SPECIFIER(SetRegisterFlag)(false);

    // We need to remove from last element because of fast erasion.
    this->mPtrColliderList.erase(this->mPtrColliderList.end() - 1);
  }
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
  iRefCollider.InitializeInternalResource();
}

void CDyPhysicsRigidbody::UnregisterCollider(_MIN_ CDyPhysicsCollider& iRefCollider)
{
  // Try find instance.
  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mPtrColliderList),
      [iPtrCollider = &iRefCollider](const auto& ptrCollider) { return ptrCollider == iPtrCollider; });
  MDY_ASSERT_FORCE(it != this->mPtrColliderList.end(), "Unexpected error occurred.");

  // If found, release internal resource from collider.
  it->Get()->ReleaseInternalResource();
  it->Get()->MDY_PRIVATE_SPECIFIER(SetRegisterFlag)(false);
  // And remove.
  DyFastErase(this->mPtrColliderList, it);
}

} /// ::dy namespace
