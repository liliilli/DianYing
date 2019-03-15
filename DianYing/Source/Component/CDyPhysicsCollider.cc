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
#include <Dy/Component/Internal/Physics/CBasePhysicsCollider.h>
#include <Dy/Element/FActor.h>
#include <Dy/Management/MPhysics.h>
#include <Dy/Helper/Library/HelperContainer.h>

namespace dy
{

EDySuccess CBasePhysicsCollider::Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc)
{
  this->mNotifyHitEvent     = desc.mDetails.mNotifyHitEvent;
  this->mNotifyOverlapEvent = desc.mDetails.mNotifyOverlapEvent;
  this->mColliderType       = desc.mDetails.mColliderType;

  this->mFilterPresetSpecifier = desc.mDetails.mCollisionFilterPresetSpecifier;
  // If filter preset specifier is not empty (get values from setting)
  if (this->mFilterPresetSpecifier.empty() == false)
  {
    const auto& setting = MPhysics::GetInstance().GetDefaultSetting();
    // Try to getting collision filter preset.
    if (Contains(setting.mFilterPresetContainer, this->mFilterPresetSpecifier) == false)
    { DyPushLogError("Failed to get collision filter preset values, {}", this->mFilterPresetSpecifier); }
    else
    {
      const auto& refValue = setting.mFilterPresetContainer.at(this->mFilterPresetSpecifier);
      this->mFilterValues = refValue;
    }
  }
  else
  {
    // Value. Recommend use preset, than customized values.
    this->mFilterValues = desc.mDetails.mCollisionFilter;
  }

  this->mCollisionTagName = desc.mDetails.mCollisionLayerName;
  const auto& collisionLayerList = MPhysics::GetInstance().GetDefaultSetting();

  // Get collision layer name but if empty, just get first name.
  if (this->mCollisionTagName.empty() == true)
  {
    this->mCollisionTagName = collisionLayerList.mCollisionTag.front();
  }
  else
  {
    // If we got non-empty string as collision tag (layer), check this is valid.
    const auto it = std::find(MDY_BIND_BEGIN_END(collisionLayerList.mCollisionTag), this->mCollisionTagName);
    MDY_ASSERT_MSG_FORCE(it != collisionLayerList.mCollisionTag.end(), "Not valid collision tag name.");
  }

  return DY_SUCCESS;
}

void CBasePhysicsCollider::UpdateBound(_MIN_ const DArea3D& iArea)
{
  this->mAABBBound = iArea;
}

const std::vector<DVector3>& CBasePhysicsCollider::GetColliderMesh() const noexcept
{
  return this->mColliderMeshInformation;
}

bool CBasePhysicsCollider::IsNotifyHitEvent() const noexcept
{
  return this->mNotifyHitEvent;
}

physx::PxShape* CBasePhysicsCollider::__GetPtrInternalShape() const noexcept
{
  return this->mPtrInternalShape;
}

bool CBasePhysicsCollider::IsNotifyOverlapEvent() const noexcept
{
  return this->mNotifyOverlapEvent;
}

bool CBasePhysicsCollider::IsRegistered() const noexcept
{
  return this->mIsRegistered;
}

bool CBasePhysicsCollider::IsNeedToUpdateColliderMesh() const noexcept
{
  return this->mIsCollisionMeshDirty;
}

EDyColliderType CBasePhysicsCollider::GetColliderType() const noexcept
{
  return this->mColliderType;
}

const DArea3D& CBasePhysicsCollider::GetBound() const noexcept
{
  return this->mAABBBound;
}

void CBasePhysicsCollider::MDY_PRIVATE(SetRegisterFlag)(_MIN_ bool iFlag) noexcept
{
  this->mIsRegistered = iFlag;
}

void CBasePhysicsCollider::ReleaseInternalResource(CPhysicsRigidbody& iRefRigidbody)
{
  iRefRigidbody.UnbindShapeFromRigidbody(*this->mPtrInternalShape);

  this->mPtrInternalShape->release();
  this->mPtrInternalShape = nullptr; 
}

void CBasePhysicsCollider::TryActivateInstance()
{
  auto& bindedActor   = *this->GetBindedActor();
  auto* ptrRigidbody  =  bindedActor.GetRigidbody();
  if (MDY_CHECK_ISNULL(ptrRigidbody) || ptrRigidbody->IsComponentActivated() == false)
  {
    // Look forward to catch signals from rigidbody... (Activated & Created)
    return;
  }

  ptrRigidbody->RegisterCollider(*this);
}

void CBasePhysicsCollider::TryDeactivateInstance()
{
  auto& bindedActor   = *this->GetBindedActor();
  auto* ptrRigidbody  =  bindedActor.GetRigidbody();
  if (MDY_CHECK_ISNULL(ptrRigidbody) || ptrRigidbody->IsComponentActivated() == false)
  {
    // Look forward to catch signals from rigidbody... (Activated & Created)
    return;
  }

  ptrRigidbody->UnregisterCollider(*this);
}

physx::PxFilterData CBasePhysicsCollider::CreateFilterDataValue(
    _MIN_ const CPhysicsRigidbody& iRigidbody,
    _MIN_ const std::string& iLayerName, 
    _MIN_ std::vector<EDyCollisionFilter>& iFilterData)
{
  const auto optSpecifier = iRigidbody.MDY_PRIVATE(GetRigidbodySpecifier)();
  MDY_ASSERT_MSG_FORCE(optSpecifier.has_value() == true, "Unexpected error occurred. Target rigidbody must be activated and valid.");

  physx::PxFilterData resultFilterData;

  // Set rigidbody specifier id.
  const auto value24 = optSpecifier.value();
  resultFilterData.word0  = 0;            // Reset
  resultFilterData.word0 |= value24 << 8; // [31..8] Rigidbody specifier id.

  // Find layer integer id from iLayerName to [7..0] word0
  const auto& defaultSetting = MPhysics::GetInstance().GetDefaultSetting();

  const auto it = std::find(MDY_BIND_BEGIN_END(defaultSetting.mCollisionTag), iLayerName);
  MDY_ASSERT_MSG(it != defaultSetting.mCollisionTag.end(), "Unexpected error occurred.");

  const TU08 TagId = static_cast<TU08>(std::distance(defaultSetting.mCollisionTag.begin(), it));
  resultFilterData.word0 |= TagId;

  // Set filter data from word1... to word3.
  MDY_ASSERT_MSG_FORCE(this->mFilterValues.size() <= 48, "Filter value size is not valid. Must be equal or less than 48.");
  const auto valueCount = static_cast<TI32>(this->mFilterValues.size());
  for (TI32 i = 0; i < 16 && i < valueCount; ++i)
  {
    switch (this->mFilterValues[i])
    {
    case EDyCollisionFilter::Block:   break; // 00 (Do nothing because value is already initailzed 00.
    case EDyCollisionFilter::Overlap: { resultFilterData.word1 |= 0b01 << (2 * i); } break; // 01
    case EDyCollisionFilter::Ignore:  { resultFilterData.word1 |= 0b10 << (2 * i); } break; // 10
    }
  }
  for (TI32 i = 16; i < 32 && i < valueCount; ++i)
  {
    switch (this->mFilterValues[i])
    {
    case EDyCollisionFilter::Block:   break; // 00 (Do nothing because value is already initailzed 00.
    case EDyCollisionFilter::Overlap: { resultFilterData.word2 |= 0b01 << (2 * i); } break; // 01
    case EDyCollisionFilter::Ignore:  { resultFilterData.word2 |= 0b10 << (2 * i); } break; // 10
    }
  }  for (TI32 i = 32; i < 48 && i < valueCount; ++i)
  {
    switch (this->mFilterValues[i])
    {
    case EDyCollisionFilter::Block:   break; // 00 (Do nothing because value is already initailzed 00.
    case EDyCollisionFilter::Overlap: { resultFilterData.word3 |= 0b01 << (2 * i); } break; // 01
    case EDyCollisionFilter::Ignore:  { resultFilterData.word3 |= 0b10 << (2 * i); } break; // 10
    }
  }

  return resultFilterData;
}

} /// ::dy namespace