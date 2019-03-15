#ifndef GUARD_DY_COMPONENT_CBasePhysicsColliderSPHERE_H
#define GUARD_DY_COMPONENT_CBasePhysicsColliderSPHERE_H
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

#include <Dy/Component/Internal/Physics/CBasePhysicsCollider.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

//!
//! Implementation
//!

namespace dy
{

/// @class CPhysicsColliderSphere
/// @brief Sphere collider component of physics simuation.
class CPhysicsColliderSphere final : public CBasePhysicsCollider
{
public:
  explicit CPhysicsColliderSphere(FDyActor& actorReference);
  virtual ~CPhysicsColliderSphere() = default;
      
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CPhysicsColliderSphere);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::CBasePhysicsCollider, CPhysicsColliderSphere)
  MDY_SET_CRC32_HASH_WITH_TYPE(CPhysicsColliderSphere);

  /// @brief Initialize component.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc) override final;
  /// @brief Release component.
  void Release() override final;

  /// @brief Get string information of this component.
  MDY_NODISCARD std::string ToString() override final;

  /// @brief Initialize internal (PhysX) resource.
  void InitializeInternalResource(_MINOUT_ CDyPhysicsRigidbody& iRefRigidbody) override final; 
  /// @brief Update collider mesh information.
  void UpdateColliderMesh() override;

  /// @brief
  MDY_NODISCARD TF32 GetRadius() const noexcept;

private:
  TF32 mRadius = 0.0f;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CBasePhysicsColliderSPHERE_H
