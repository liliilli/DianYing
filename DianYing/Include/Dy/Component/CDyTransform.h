#ifndef GUARD_DY_COMPONENT_CDYTRANSFORM_H
#define GUARD_DY_COMPONENT_CDYTRANSFORM_H
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

#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Element/Type/DDyTransform.h>
#include <Dy/Helper/Type/DVector3.h>
#include <Dy/Helper/Type/EAxis3D.h>
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Helper/Type/DQuaternion.h>
#include "Dy/Meta/Information/ComponentMetaInformation.h"

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyTransform
/// @brief
/// @TODO SCRIPT THIS
///
class CDyTransform final : public AGeneralBaseComponent, public DDyTransform
{
public:
  CDyTransform(FDyActor& actorReference) : AGeneralBaseComponent(actorReference) { }
  virtual ~CDyTransform() = default;

  CDyTransform(const CDyTransform&)                                 = delete;
  CDyTransform& operator=(const CDyTransform&)                      = delete;
  CDyTransform(CDyTransform&& instance)                   noexcept  = default;
  CDyTransform& operator=(CDyTransform&& instance)        noexcept  = default;

  ///
  /// @brief  Initilize component.
  /// @param  desc
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyTransformComponentMetaInfo& desc);
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const DDyTransform& desc);

  /// @brief Release component.
  void Release();

  /// @brief  Return local position.
  /// @return Actor's local position.
  MDY_NODISCARD const DVector3& GetRelativeLocalPosition() const noexcept;

	/// @brief  Return world position.
	/// @return Actor's world position from parent object's world basis.
  MDY_NODISCARD const DVector3& GetRelativeWorldPosition() const noexcept;

  /// @brief  Get Object's final position.
  /// @return Actor's final world position which to be rendered.
  MDY_NODISCARD const DVector3& GetFinalWorldPosition() noexcept;

  /// @brief  Get Object's to child basis (but not propagated to children FDyActor!)
  /// @return Actor's to child basis (this::mLocalEulerAngle + this::mToChildEulerAngle)
  MDY_NODISCARD const std::array<DVector3, 3>& GetToChildBasis() noexcept;

  /// @brief  The method gets local rotation angle value. (euler degree value)
  MDY_NODISCARD const DVector3& GetRelativeLocalEulerAngle() const noexcept;

  /// @brief Overloaded version of `GetRelativeLocalEulerAngle`.
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  MDY_NODISCARD float GetRelativeLocalEulerAngle(_MIN_ EAxis3D direction) const noexcept;

  /// @brief  The method gets world rotation angle value. (euler degree value)
  MDY_NODISCARD const DVector3& GetRelativeWorldEulerAngle() const noexcept;

  /// @brief Overloaded version of `GetRelativeLocalEulerAngle`.
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  MDY_NODISCARD float GetRelativeWorldEulerAngle(_MIN_ EAxis3D direction) const noexcept;

  /// @brief  The method gets final rendering angle value. (euler degree value)
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  MDY_NODISCARD float GetFinalEulerAngle(_MIN_ EAxis3D direction) noexcept;

  /// @brief  The method gets (x, y, z) DVector3 scaling axis factor.
  /// @return Object's scaling vector which has (x, y, z) axis factors.
  MDY_NODISCARD const DVector3& GetLocalScale() const noexcept;

  /// @brief  Returns world scale which able to affect child object's transform.
  /// @return Object's scaling vector which has (x, y, z) axis factors.
  MDY_NODISCARD const DVector3& GetWorldScale() const noexcept;

  /// @brief  Return final producted scale value vector.
  MDY_NODISCARD const DVector3& GetFinalScale() noexcept;

  /// @brief Set local position.
  /// @param[in] localPosition local position to set on.
  void SetRelativeLocalPosition(_MIN_ const DVector3& localPosition) noexcept;
  /// @brief Set local position with final world position. 
  void SetRelativeLocalPositionWithFinalWorldPosition(_MIN_ const DVector3& finalPosition);

	/// @brief Set world position.
	/// @param[in] worldPosition Winal position in Screen space and from parent' object.
	void SetRelativeWorldPosition(_MIN_ const DVector3& worldPosition) noexcept;

  /// @brief Add local position with input vector.
  void AddRelativeLocalPosition(_MIN_ const DVector3& localPosiiton) noexcept;

  /// @brief Add offset value with axis as local position.
  void AddRelativeLocalPosition(_MIN_ EAxis3D axis, _MIN_ TF32 value) noexcept;

  /// @brief Add world position with input vector.
  void AddRelativeWorldPosition(_MIN_ const DVector3& localPosiiton) noexcept;

  /// @brief Add offset value with axis as world position.
  void AddRelativeWorldPosition(_MIN_ EAxis3D axis, _MIN_ TF32 value) noexcept;

  /// @brief The method adds local rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  /// @param[in] eulerAngleValue Euler angle value to set on.
  void SetLocalEulerAngle(_MIN_ const DVector3& eulerAngleValue) noexcept;
  /// @brief Set local euler angle with quaternion.
  void SetLocalEulerAngleWithQuaternion(_MIN_ const DQuaternion& iQuat);

  /// @brief The method adds world rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  /// @param[in] eulerAngleValue Euler angle value to set on.
  void SetWorldEulerAngle(_MIN_ const DVector3& eulerAngleValue) noexcept;

  /// @brief Add offset eulerAngleValue with axis as local rotation angle.
  void AddLocalEulerAngle(_MIN_ EAxis3D axis, _MIN_ float eulerAngleValue) noexcept;

  /// @brief Add offset eulerAngleValue with axis as local rotation angle.
  void AddLocalEulerAngle(_MIN_ const DVector3& eulerAngleValue) noexcept;

  /// @brief Add offset eulerAngleValue with axis as world rotation angle.
  void AddWorldEulerAngle(_MIN_ EAxis3D axis, _MIN_ float eulerAngleValue) noexcept;

  /// @brief Add offset eulerAngleValue with axis as world rotation angle.
  void AddWorldEulerAngle(_MIN_ const DVector3& eulerAngleValue) noexcept;

  /// @brief The method sets scaling vector have (x, y, z) scaling factors.
  void SetLocalScale(_MIN_ const DVector3& xyz_value) noexcept;

  /// @brief Sets world scaling vector value which have (x, y, z).
  void SetWorldScale(_MIN_ const DVector3& xyz_value) noexcept;

  /// @brief Get rotation matrix (4x4)
  MDY_NODISCARD const DMatrix4x4 GetRotationMatrix() noexcept;

  /// @brief Get rotation quaternion instance.
  MDY_NODISCARD const DQuaternion& GetRotationQuaternion() noexcept;

  /// @brief The method returns Model matrix, M = TRS
  ///
  /// If any value which related to Translate, Rotation, Scaling has been changed,
  /// model matrix would be calculated newly and returned.
  ///
  /// @return Model matrix (M = TRS)
  MDY_NODISCARD const DMatrix4x4& GetTransform() noexcept;

  ///
  /// @brief
  /// @TODO NOT IMPLEMENTED YET
  ///
  MDY_NODISCARD std::string ToString() override final;

  ///
  void Update(_MIN_ float dt) override final {};

  /// @brief Try propagate information that is needed by children to update transform properly. \n
  /// If children is not exist, do nothing.
  void TryPropagateTransformToChildren();

  void MDY_PRIVATE(PropagateTransform)(
      _MIN_ const DVector3& iPosition,
      _MIN_ const DVector3& iAngle,
      _MIN_ const DVector3& iScale);
  /// @brief Set transform with physx::PxTransform.
  /// When set, degree x, y, z rotation value will be not valid. 
  void MDY_PRIVATE(SetPxTransform)(_MIN_ const physx::PxTransform& iTransform);

private:
  void TryActivateInstance() override final   { /* Do nothing */ };
  void TryDeactivateInstance() override final { /* Do nothing */ };
  
  /// @brief Try update movement basis.
  void MDY_PRIVATE(TryUpdateMovementBasis)();
  /// @brief
  void MDY_PRIVATE(TryUpdateLocalRelAlignedPos)();
  /// @brief
  void MDY_PRIVATE(TryUpdateWorldRelAlignedPos)();
  /// @brief 
  void MDY_PRIVATE(TryUpdateAlignedWorldSumPosition)();
  /// @brief
  void MDY_PRIVATE(TryUpdateFinalPosition)();
  /// @brief
  void MDY_PRIVATE(SetRelativeWorldOrigin)(_MIN_ const DVector3& iParentPosition);

  /// @brief Try update sum of world (without each local) rotation angle.
  void MDY_PRIVATE(TryUpdateWorldSumRotAngle)();
  /// @brief
  void MDY_PRIVATE(TryUpdateFinalWorldRotAngle)() noexcept;
  /// @brief
  void MDY_PRIVATE(TryUpdateToChildBasis)() noexcept;
  /// @brief 
  void MDY_PRIVATE(SetRotationAngleOrigin)(_MIN_ const DVector3& iParentSumRotAngle);

  /// @brief
  void MDY_PRIVATE(TryUpdateWorldPrdScale)();
  /// @brief
  void MDY_PRIVATE(UpdateFinalRenderingScale)() noexcept;
  /// @brief
  void MDY_PRIVATE(SetWorldScaleOrigin)(_MIN_ const DVector3& iParentPrdScale);

private:
  MDY_TRANSIENT DVector3    mWorldRelativeOriginPosition  = {};
  MDY_TRANSIENT DVector3    mLocalRelAlignedPosition = {};
  MDY_TRANSIENT DVector3    mWorldRelAlignedPosition = {};
  MDY_TRANSIENT DVector3    mWorldSumAlignedPosition = {};
  MDY_TRANSIENT DVector3    mFinalRenderingPosition  = {};

  MDY_TRANSIENT DVector3    mFromParentWorldEulerAngle        = {};
  MDY_TRANSIENT DMatrix3x3  mPresentPositionBasis;
  MDY_TRANSIENT std::array<DVector3, 3> mDisplayMovementBasis = { DVector3::RightX(), DVector3::UpY(), DVector3::FrontZ() };
  MDY_TRANSIENT DVector3    mWorldSumEulerAngle               = {};
  MDY_TRANSIENT DVector3    mFinalRenderingEulerAngle         = {};
  MDY_TRANSIENT DQuaternion mFinalRenderRotationQuaternion    = {DVector3{0, 0, 0}};
  MDY_TRANSIENT std::array<DVector3, 3> mToChildBasis         = { DVector3::RightX(), DVector3::UpY(), DVector3::FrontZ() };

  MDY_TRANSIENT DVector3    mFromParentWorldScale = DVector3{ 1.f };
  MDY_TRANSIENT DVector3    mWorldProductScale    = DVector3{ 1.f };
  MDY_TRANSIENT DVector3    mFinalRenderingScale  = DVector3{ 1.f };

  MDY_TRANSIENT DMatrix4x4  mFinalRenderingTransform;

  MDY_TRANSIENT bool mIsMovementBasisDirty        = true;
  MDY_TRANSIENT bool mIsWorldSumRotationAngleDirty= true;
  MDY_TRANSIENT bool mIsFinalRotationAngleDirty   = true;
  MDY_TRANSIENT bool mToChildBasisAxisDirty       = true;

  MDY_TRANSIENT bool mIsWorldPrdScaleDirty        = true;
  MDY_TRANSIENT bool mIsFinalScaleDirty           = true;

  MDY_TRANSIENT bool mIsLocalRelAlignedPosDirty   = true;
  MDY_TRANSIENT bool mIsWorldPositionAlignDirty   = true;
  MDY_TRANSIENT bool mIsWorldRelAlignedPosDirty   = true;
  MDY_TRANSIENT bool mIsWorldSumAlignedPosDirty   = true;
  MDY_TRANSIENT bool mIsFinalPositionDirty        = true;

  MDY_TRANSIENT bool mIsModelMatrixDirty          = true;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CDyTransform);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyTransform);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYTRANSFORM_H