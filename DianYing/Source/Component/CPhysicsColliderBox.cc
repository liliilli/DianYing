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
#include <Dy/Component/CPhysicsColliderBox.h>
#include <geometry/PxBoxGeometry.h>
#include <Dy/Management/MPhysics.h>
#include <Dy/Component/CPhysicsRigidbody.h>
#include <PxPhysics.h>

namespace dy
{

/*
 *{
    "Type": "PhysicsCollider", "Activated": true,
    "Details": {
      "IsNotifyEvent": false, "IsOverlapEvent": false, "ColliderType": "Box",
      "ColliderDetails": { "HalfExtent": { "X": 10, "Y": 10, "Z": 30 }
      },
      "FilterPresetSpecifier": "", "CollisionFilter": [ 0, 0, 0, 0, 0 ], "LayerName": "",
      "Transform": {
        "Position": { "X": 0, "Y": 0, "Z": 0 },
        "Rotation": { "X": 0, "Y": 0, "Z": 0 }
      }
    }
  }
 */

EDySuccess CPhysicsColliderBox::Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc)
{
  // Call parent function first.
  MDY_CALL_ASSERT_SUCCESS(CBasePhysicsCollider::Initialize(desc));

  // Get Sphere reference instance and apply values.
  const auto refDetail = 
      std::get<TColliderBindingType<EDyColliderType::Box>::Type>(desc.mDetails.mColliderDetails);
  this->mHalfExtent = refDetail.mHalfExtent;

  if (desc.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CPhysicsColliderBox::Release()
{
  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

std::string CPhysicsColliderBox::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CPhysicsColliderBox::InitializeInternalResource(_MINOUT_ CPhysicsRigidbody& iRefRigidbody)
{
  // PxSphereGeometry is value type.
  const physx::PxBoxGeometry geometry{this->mHalfExtent.X, this->mHalfExtent.Y, this->mHalfExtent.Z};

  // Create shape.
  auto& physics = MPhysics::GetInstance().MDY_PRIVATE(GetRefInternalSdk)();
  const auto& defaultMaterial = MPhysics::GetInstance().GetDefaultPhysicsMaterial();
  this->mPtrInternalShape = physics.createShape(geometry, defaultMaterial);
  MDY_ASSERT_MSG_FORCE(MDY_CHECK_ISNOTNULL(this->mPtrInternalShape), "Unexpected error occurred.");
  
  // Make filter value (PxFilterData)
  const physx::PxFilterData filterData = CreateFilterDataValue(
      iRefRigidbody, 
      this->mCollisionTagName, this->mFilterValues);
  // Apply Collision filter
  this->mPtrInternalShape->setSimulationFilterData(filterData);

  // Apply to iRefRigidbody.
  iRefRigidbody.BindShapeToRigidbody(*this);
}

const DVector3& CPhysicsColliderBox::GetHalfExtent() const noexcept
{
  return this->mHalfExtent;
}

void CPhysicsColliderBox::UpdateColliderMesh()
{
  // First, clear all information.
  this->mColliderMeshInformation.clear();

  //!     lud-----rud \n
  //!    /|      /|   \n
  //! luf-----ruf |   \n
  //!   | lbd---|-rbd \n
  //!   |/      |/    \n
  //! lbf-----rbf

  const auto ruf = DVector3{this->mHalfExtent};
  const auto rud = DVector3{this->mHalfExtent.X, this->mHalfExtent.Y, -this->mHalfExtent.Z};
  const auto lud = DVector3{-this->mHalfExtent.X, this->mHalfExtent.Y, -this->mHalfExtent.Z};
  const auto luf = DVector3{-this->mHalfExtent.X, this->mHalfExtent.Y, this->mHalfExtent.Z};
 
  const auto rbf = DVector3{this->mHalfExtent.X, -this->mHalfExtent.Y, this->mHalfExtent.Z};
  const auto rbd = DVector3{this->mHalfExtent.X, -this->mHalfExtent.Y, -this->mHalfExtent.Z};
  const auto lbd = DVector3{-this->mHalfExtent.X, -this->mHalfExtent.Y, -this->mHalfExtent.Z};
  const auto lbf = DVector3{-this->mHalfExtent.X, -this->mHalfExtent.Y, this->mHalfExtent.Z};

  // We do not need insert information for up & bottom plane.
  // Becasuse collider mesh will be drawn by line_strip.

  this->mColliderMeshInformation.emplace_back(luf);
  this->mColliderMeshInformation.emplace_back(lbf);
  this->mColliderMeshInformation.emplace_back(rbf);
  this->mColliderMeshInformation.emplace_back(ruf);
  this->mColliderMeshInformation.emplace_back(luf);

  this->mColliderMeshInformation.emplace_back(lud);
  this->mColliderMeshInformation.emplace_back(lbd);
  this->mColliderMeshInformation.emplace_back(lbf);
  this->mColliderMeshInformation.emplace_back(luf);
  this->mColliderMeshInformation.emplace_back(lud);

  this->mColliderMeshInformation.emplace_back(rud);
  this->mColliderMeshInformation.emplace_back(rbd);
  this->mColliderMeshInformation.emplace_back(lbd);
  this->mColliderMeshInformation.emplace_back(lud);
  this->mColliderMeshInformation.emplace_back(rud);

  this->mColliderMeshInformation.emplace_back(ruf);
  this->mColliderMeshInformation.emplace_back(rbf);
  this->mColliderMeshInformation.emplace_back(rbd);
  this->mColliderMeshInformation.emplace_back(rud);
  this->mColliderMeshInformation.emplace_back(ruf);
}

} /// ::dy namespace