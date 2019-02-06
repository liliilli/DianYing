#ifndef GUARD_DY_COMPONENT_CDYPHYSICSCOLLIDERSPHERE_H
#define GUARD_DY_COMPONENT_CDYPHYSICSCOLLIDERSPHERE_H
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

#include <Dy/Component/CDyPhysicsCollider.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

//!
//! Forward declaration
//!

namespace physx
{
class PxShape;
} /// ::physx namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyPhysicsColliderSphere
/// @brief Sphere collider component of physics simuation.
///
class CDyPhysicsColliderSphere final : public CDyPhysicsCollider
{
public:
  CDyPhysicsColliderSphere(_MIN_ FDyActor& actorReference) : CDyPhysicsCollider(actorReference) 
  { 
    this->mColliderType = EDyColliderType::Sphere; 
  };
  virtual ~CDyPhysicsColliderSphere() = default;
      
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDyPhysicsColliderSphere);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::CDyPhysicsCollider, CDyPhysicsColliderSphere)
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyPhysicsColliderSphere);

  /// @brief Initialize component.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc) override final;
  /// @brief Release component.
  void Release() override final;

  /// @brief Get string information of this component.
  MDY_NODISCARD std::string ToString() override final;

  /// @brief Initialize internal (PhysX) resource.
  void InitializeInternalResource(_MINOUT_ CDyPhysicsRigidbody& iRefRigidbody) override final; 
  /// @brief Release internal (PhysX) resource.
  void ReleaseInternalResource() override final;

  /// @brief
  MDY_NODISCARD TF32 GetRadius() const noexcept;

private:
  TF32 mRadius = 0.0f;

  /// @brief Internal shape pointer instance. 
  physx::PxShape* mPtrInternalShape = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYPHYSICSCOLLIDERSPHERE_H