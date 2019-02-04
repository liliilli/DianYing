#ifndef GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H
#define GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H
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

#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

namespace dy
{

/// @class CDyPhysicsRigidbody
/// @brief Rigidbody component that process physics simulation & manages collision callback with colliders of actor.
class CDyPhysicsRigidbody final : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDyRigidbodyComponentMetaInfo>
{
public:
  CDyPhysicsRigidbody(FDyActor& actorReference);
  virtual ~CDyPhysicsRigidbody() = default;
    
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDyPhysicsRigidbody);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyPhysicsRigidbody)
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyPhysicsRigidbody);

  /// @brief Initialize component.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyRigidbodyComponentMetaInfo& descriptor) override;
  /// @brief Release component.
  void Release() override final;

  /// @brief Update anyway.
  void Update(_MIN_ TF32 dt) override final {};

  /// @brief Get string information of this component.
  MDY_NODISCARD std::string ToString() override final;

private:
  /// Check this rigidbody (and collider) simulate physics.
  bool mIsSimulatePhysics = false;
  /// Enable gravity or not.
  bool mIsEnableGravity = false;
  /// Mass of sum of collider.
  DDyClamp<TF32, 0, 100'000>  mMassInKg = 0.001f;
  /// Linear damping of rigidbody
  DDyClamp<TF32, 0, 10'000>   mLinearDamping = 1.0f;
  /// Angular damping of rigidbody
  DDyClamp<TF32, 0, 10'000>   mAngularDamping = 1.0f; 
  /// Lock position axis.
  DLockPreset::D3DAxis mLockPosition;
  /// Lock rotation axis.
  DLockPreset::D3DAxis mLockRotation;

  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYPHYSICSRIGIDBODYSTATIC_H