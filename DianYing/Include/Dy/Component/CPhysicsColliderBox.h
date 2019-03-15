#ifndef GUARD_DY_COMPONENT_CBasePhysicsColliderBOX_H
#define GUARD_DY_COMPONENT_CBasePhysicsColliderBOX_H
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

namespace dy
{

/// @class CPhysicsColliderBox
/// @brief Box collider component of physics simuation.
class CPhysicsColliderBox final : public CBasePhysicsCollider
{
public:
  CPhysicsColliderBox(FActor& actorReference) : CBasePhysicsCollider(actorReference) {};
  virtual ~CPhysicsColliderBox() = default;
      
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CPhysicsColliderBox);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::CBasePhysicsCollider, CPhysicsColliderBox)
  MDY_SET_CRC32_HASH_WITH_TYPE(CPhysicsColliderBox);

  /// @brief Initialize component.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc) override final;
  /// @brief Release component.
  void Release() override final;

  /// @brief Get string information of this component.
  MDY_NODISCARD std::string ToString() override final;

  /// @brief Initialize internal (PhysX) resource.
  void InitializeInternalResource(_MINOUT_ CPhysicsRigidbody& iRefRigidbody) override final; 
  /// @brief Update collider mesh information.
  void UpdateColliderMesh() override;

  /// @brief Get half extent.
  MDY_NODISCARD const DVector3& GetHalfExtent() const noexcept;

private:
  DVector3 mHalfExtent;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CBasePhysicsColliderBOX_H
