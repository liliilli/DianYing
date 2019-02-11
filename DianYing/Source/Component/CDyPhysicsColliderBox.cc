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
#include <Dy/Component/CDyPhysicsColliderBox.h>
#include <geometry/PxBoxGeometry.h>
#include <Dy/Management/PhysicsManager.h>
#include <Dy/Component/CDyPhysicsRigidbody.h>

namespace dy
{

EDySuccess CDyPhysicsColliderBox::Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc)
{
  // Call parent function first.
  MDY_CALL_ASSERT_SUCCESS(CDyPhysicsCollider::Initialize(desc));

  // Get Sphere reference instance and apply values.
  const auto refDetail = 
      std::get<TColliderBindingType<EDyColliderType::Box>::Type>(desc.mDetails.mColliderDetails);
  this->mHalfExtent = refDetail.mHalfExtent;

  if (desc.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CDyPhysicsColliderBox::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

std::string CDyPhysicsColliderBox::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CDyPhysicsColliderBox::InitializeInternalResource(_MINOUT_ CDyPhysicsRigidbody& iRefRigidbody)
{
  // PxSphereGeometry is value type.
  const physx::PxBoxGeometry geometry{this->mHalfExtent.X, this->mHalfExtent.Y, this->mHalfExtent.Z};

  // Create shape.
  auto& physics = MDyPhysics::GetInstance().MDY_PRIVATE_SPECIFIER(GetRefInternalSdk)();
  const auto& defaultMaterial = MDyPhysics::GetInstance().GetDefaultPhysicsMaterial();
  this->mPtrInternalShape = physics.createShape(geometry, defaultMaterial);
  MDY_ASSERT_FORCE(MDY_CHECK_ISNOTNULL(this->mPtrInternalShape), "Unexpected error occurred.");
  
  // Make filter value (PxFilterData)
  const physx::PxFilterData filterData = CreateFilterDataValue(
      iRefRigidbody, 
      this->mCollisionTagName, this->mFilterValues);
  // Apply Collision filter
  this->mPtrInternalShape->setSimulationFilterData(filterData);

  // Apply to iRefRigidbody.
  iRefRigidbody.BindShapeToRigidbody(*this->mPtrInternalShape);
}

const DDyVector3& CDyPhysicsColliderBox::GetHalfExtent() const noexcept
{
  return this->mHalfExtent;
}

void CDyPhysicsColliderBox::UpdateColliderMesh()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace