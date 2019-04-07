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
#include <Dy/Component/CPhysicsColliderSphere.h>

#include <geometry/PxSphereGeometry.h>
#include <PxPhysics.h>

#include <Dy/Management/MPhysics.h>
#include <Dy/Component/CPhysicsRigidbody.h>
#include <Math/Utility/XMath.h>

namespace dy
{

/*
 *{
  "Type": "PhysicsCollider", "Activated": true,
  "Details": {
    "IsNotifyEvent": false, "IsOverlapEvent": false,
    "ColliderType": "Sphere", "ColliderDetails": { "Radius": 20.0 },
    "FilterPresetSpecifier": "", "CollisionFilter": [ 0, 0, 0, 0, 0 ], "LayerName": "",
    "Transform": {
      "Position": { "X": 0, "Y": 0, "Z": 0 },
      "Rotation": { "X": 0, "Y": 0, "Z": 0 }
    }
  }
 */

CPhysicsColliderSphere::CPhysicsColliderSphere(FActor& actorReference)
  : CBasePhysicsCollider(actorReference) 
{ 
  this->mColliderType = EDyColliderType::Sphere; 
};

EDySuccess CPhysicsColliderSphere::Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc)
{
  // Call parent function first.
  MDY_CALL_ASSERT_SUCCESS(CBasePhysicsCollider::Initialize(desc));

  // Get Sphere reference instance and apply values.
  const PDySphereColliderMetaInfo refSphere = 
      std::get<TColliderBindingType<EDyColliderType::Sphere>::Type>(desc.mDetails.mColliderDetails);
  this->mRadius = refSphere.mRadius;

  // Activation.
  if (desc.mInitiallyActivated == true) { this->Activate(); }
  return EDySuccess::DY_SUCCESS;
}

void CPhysicsColliderSphere::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

std::string CPhysicsColliderSphere::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CPhysicsColliderSphere::InitializeInternalResource(_MINOUT_ CPhysicsRigidbody& iRefRigidbody)
{
  // PxSphereGeometry is value type.
  const physx::PxSphereGeometry geometry{this->mRadius};

  // Create shape.
  auto& physics = MPhysics::GetInstance().MDY_PRIVATE(GetRefInternalSdk)();
  const auto& defaultMaterial = MPhysics::GetInstance().GetDefaultPhysicsMaterial();
  this->mPtrInternalShape = physics.createShape(geometry, defaultMaterial);
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNOTNULL(this->mPtrInternalShape), "Unexpected error occurred.");
  
  // Make filter value (PxFilterData)
  this->mInternalFilterData = CreateFilterDataValue(iRefRigidbody, this->mCollisionTagName, this->mFilterValues);
  // Apply Collision filter
  this->mPtrInternalShape->setSimulationFilterData(this->mInternalFilterData);

  // Apply to iRefRigidbody.
  iRefRigidbody.BindShapeToRigidbody(*this);
}

TF32 CPhysicsColliderSphere::GetRadius() const noexcept
{
  return this->mRadius;
}

void CPhysicsColliderSphere::UpdateColliderMesh()
{
  // First, clear all information.
  this->mColliderMeshInformation.clear();
  const TF32 step = math::k2Pi<TF32> / (50 - 1);

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