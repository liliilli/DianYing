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
#include <Dy/Component/CDyPhysicsColliderSphere.h>

#include <geometry/PxSphereGeometry.h>
#include <Dy/Management/PhysicsManager.h>
#include <Dy/Component/CDyPhysicsRigidbody.h>

namespace dy
{

EDySuccess CDyPhysicsColliderSphere::Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc)
{
  // Call parent function first.
  MDY_CALL_ASSERT_SUCCESS(CDyPhysicsCollider::Initialize(desc));

  // Get Sphere reference instance and apply values.
  const PDySphereColliderMetaInfo refSphere = 
      std::get<TColliderBindingType<EDyColliderType::Sphere>::Type>(desc.mDetails.mColliderDetails);
  this->mRadius = refSphere.mRadius;

  // Activation.
  if (desc.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CDyPhysicsColliderSphere::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

std::string CDyPhysicsColliderSphere::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CDyPhysicsColliderSphere::InitializeInternalResource(_MINOUT_ CDyPhysicsRigidbody& iRefRigidbody)
{
  // PxSphereGeometry is value type.
  const physx::PxSphereGeometry geometry{this->mRadius};

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

TF32 CDyPhysicsColliderSphere::GetRadius() const noexcept
{
  return this->mRadius;
}

void CDyPhysicsColliderSphere::UpdateColliderMesh()
{
  // First, clear all information.
  this->mColliderMeshInformation.clear();
  const TF32 step = math::Pi2<TF32> / (50 - 1);

  // (X, Y, 0)
  {
    TF32 angle = 0;
    for (TU32 i = 0; i < 50; ++i, angle += step)
    {
      this->mColliderMeshInformation.emplace_back(this->mRadius * sin(angle), this->mRadius * cos(angle), 0);
    }
  }

  // (0, Y, Z)
  {
    TF32 angle = 0;
    for (TU32 i = 0; i < 50; ++i, angle += step)
    {
      this->mColliderMeshInformation.emplace_back(0, this->mRadius * cos(angle), this->mRadius * sin(angle));
    }
  }
}

} /// ::dy namespace