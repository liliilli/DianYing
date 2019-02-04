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
#include <Dy/Management/SettingManager.h>
#include <Dy/Helper/ContainerHelper.h>

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
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void CDyPhysicsRigidbody::TryDeactivateInstance()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace
