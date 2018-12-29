#ifndef GUARD_DY_COMPONENT_CDYTRANSFORM_H
#define GUARD_DY_COMPONENT_CDYTRANSFORM_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/Type/MathEnum.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Helper/Type/Quaternion.h>
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
class CDyTransform final : public ADyGeneralBaseComponent
{
public:
  CDyTransform(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference) { }
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

  /// @brief Release component.
  void Release();

  /// @brief  Return local position.
  /// @return Actor's local position.
  MDY_NODISCARD const DDyVector3& GetRelativeLocalPosition() const noexcept;

	/// @brief  Return world position.
	/// @return Actor's world position from parent object's world basis.
  MDY_NODISCARD const DDyVector3& GetRelativeWorldPosition() const noexcept;

  /// @brief  Get Object's final position.
  /// @return Actor's final world position which to be rendered.
  MDY_NODISCARD const DDyVector3& GetFinalWorldPosition() const noexcept;

  /// @brief  Get Object's to child basis (but not propagated to children FDyActor!)
  /// @return Actor's to child basis (this::mLocalEulerAngle + this::mToChildEulerAngle)
  MDY_NODISCARD const std::array<DDyVector3, 3>& GetToChildBasis() const noexcept;

  /// @brief
  MDY_NODISCARD const DDyVector3& GetLocalRotationAngle() const noexcept;

  /// @brief
  MDY_NODISCARD const DDyVector3& GetWorldRotationAngle() const noexcept;

  /// @brief
  MDY_NODISCARD const DDyVector3& GetFinalRotationAngle() const noexcept;

  /// @brief  The method gets local rotation angle value. (euler degree value)
  MDY_NODISCARD const DDyVector3& GetRelativeLocalEulerAngle() const noexcept;

  /// @brief Overloaded version of `GetRelativeLocalEulerAngle`.
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  MDY_NODISCARD float GetRelativeLocalEulerAngle(_MIN_ EDyAxis3D direction) const noexcept;

  /// @brief  The method gets world rotation angle value. (euler degree value)
  MDY_NODISCARD const DDyVector3& GetRelativeWorldEulerAngle() const noexcept;

  /// @brief Overloaded version of `GetRelativeLocalEulerAngle`.
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  MDY_NODISCARD float GetRelativeWorldEulerAngle(_MIN_ EDyAxis3D direction) const noexcept;

  /// @brief  The method gets final rendering angle value. (euler degree value)
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  MDY_NODISCARD float GetFinalEulerAngle(_MIN_ EDyAxis3D direction) const noexcept;

  ///
  /// @brief  The method gets (x, y, z) DDyVector3 scaling axis factor.
  /// @return Object's scaling vector which has (x, y, z) axis factors.
  ///
  MDY_NODISCARD const DDyVector3& GetLocalScale() const noexcept;

  ///
  /// @brief  Returns world scale which able to affect child object's transform.
  /// @return Object's scaling vector which has (x, y, z) axis factors.
  ///
  MDY_NODISCARD const DDyVector3& GetWorldScale() const noexcept;

  ///
  /// @brief  Return final producted scale value vector.
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD const DDyVector3& GetFinalScale() const noexcept;

  //!
  //! Setter
  //!

  /// @brief Sets local position.
  /// @param[in] localPosition local position to set on.
  void SetRelativeLocalPosition(_MIN_ const DDyVector3& localPosition) noexcept;

	/// @brief Set world position.
	/// @param[in] worldPosition Winal position in Screen space and from parent' object.
	void SetRelativeWorldPosition(_MIN_ const DDyVector3& worldPosition) noexcept;

  /// @brief Add local position with input vector.
  void AddRelativeLocalPosition(_MIN_ const DDyVector3& localPosiiton) noexcept;

  /// @brief Add offset value with axis as local position.
  void AddRelativeLocalPosition(_MIN_ EDyAxis3D axis, _MIN_ TF32 value) noexcept;

  /// @brief Add world position with input vector.
  void AddRelativeWorldPosition(_MIN_ const DDyVector3& localPosiiton) noexcept;

  /// @brief Add offset value with axis as world position.
  void AddRelativeWorldPosition(_MIN_ EDyAxis3D axis, _MIN_ TF32 value) noexcept;

  ///
  /// @brief The method adds local rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  ///
  /// @param[in] eulerAngleValue Euler angle value to set on.
  ///
  void SetLocalEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept;

  /// @brief The method adds world rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  /// @param[in] eulerAngleValue Euler angle value to set on.
  void SetWorldEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept;

  /// @brief Add offset eulerAngleValue with axis as local rotation angle.
  void AddLocalEulerAngle(_MIN_ EDyAxis3D axis, _MIN_ float eulerAngleValue) noexcept;

  /// @brief Add offset eulerAngleValue with axis as local rotation angle.
  void AddLocalEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept;

  /// @brief Add offset eulerAngleValue with axis as world rotation angle.
  void AddWorldEulerAngle(_MIN_ EDyAxis3D axis, _MIN_ float eulerAngleValue) noexcept;

  /// @brief Add offset eulerAngleValue with axis as world rotation angle.
  void AddWorldEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept;

  /// @brief The method sets scaling vector have (x, y, z) scaling factors.
  /// @TODO SCRIPT THIS
  void SetLocalScale(_MIN_ const DDyVector3& xyz_value) noexcept;

  ///
  /// @brief Sets world scaling vector value which have (x, y, z).
  /// @TODO SCRIPT THIS
  ///
  void SetWorldScale(_MIN_ const DDyVector3& xyz_value) noexcept;

  ///
  /// @brief The method returns Model matrix, M = TRS
  ///
  /// If any value which related to Translate, Rotation, Scaling has been changed,
  /// model matrix would be calculated newly and returned.
  ///
  /// @return Model matrix (M = TRS)
  ///
  MDY_NODISCARD const DDyMatrix4x4& GetTransform() const noexcept;

  ///
  /// @brief
  /// @TODO NOT IMPLEMENTED YET
  ///
  MDY_NODISCARD std::string ToString() override final;

  ///
  void Update(_MIN_ float dt) override final {};

  /// @brief Try update movement basis.
  void MDY_PRIVATE_SPECIFIER(TryUpdateMovementBasis)();
  /// @brief
  void MDY_PRIVATE_SPECIFIER(TryUpdateWorldRelAlignedPos)();
  /// @brief 
  void MDY_PRIVATE_SPECIFIER(TryUpdateAlignedWorldSumPosition)();
  /// @brief
  void MDY_PRIVATE_SPECIFIER(SetRelativeWorldOrigin)(_MIN_ const DDyVector3& iParentPosition);

  /// @brief Try update sum of world (without each local) rotation angle.
  void MDY_PRIVATE_SPECIFIER(TryUpdateWorldSumRotAngle)();
  /// @brief 
  void MDY_PRIVATE_SPECIFIER(SetRotationAngleOrigin)(_MIN_ const DDyVector3& iParentSumRotAngle);

  /// @brief
  void MDY_PRIVATE_SPECIFIER(TryUpdateWorldPrdScale)();
  /// @brief
  void MDY_PRIVATE_SPECIFIER(SetWorldScaleOrigin)(_MIN_ const DDyVector3& iParentPrdScale);

private:
  /// @brief
  void pUpdateFinalRenderingEulerAngle() const noexcept;
  /// @brief
  void pUpdateToChildWorldEulerAngle() const noexcept;
  /// @brief
  void MDY_PRIVATE_SPECIFIER(UpdateFromParentWorldEulerAngle)() const noexcept;

  /// @brief
  void pUpdateFinalRenderingAxis() const noexcept;
  /// @brief
  void pUpdateToChildBasisAxis() const noexcept;

  /// @brief
  void pUpdateFinalRenderingPosition() const noexcept;
  /// @brief
  void pUpdateAxisAlignedLocalPosition() const noexcept;
  /// @brief
  void pUpdateAxisAlignedWorldPosition() const noexcept;
  /// @brief
  void pUpdateToChildPosition() const noexcept;
  /// @brief
  void pUpdateFromParentWorldPosition() const noexcept;

  /// @brief
  void pUpdateFinalRenderingScale() const noexcept;
  /// @brief
  void pUpdateToChildWorldScale() const noexcept;
  /// @brief
  void pUpdateFromParentWorldScale() const noexcept;

  //!
  //! Friend function
  //!

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD FORCEINLINE bool pfIsToChildPositionUpdated() const noexcept
  {
    return this->mIsFromChildPositionUpdated;
  }

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD FORCEINLINE bool pfIsToChildRotationUpdate() const noexcept
  {
    return this->mIsFromChildRotationUpdated;
  }

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD FORCEINLINE bool pfIsToChildScaleUpdate() const noexcept
  {
    return this->mIsFromChildScaleUpdated;
  }

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD const DDyVector3& pfGetToChildPosition() const noexcept;

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD const DDyVector3& pfGetToChildWorldEulerAngle() const noexcept;

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD const DDyVector3& pfGetToChildScale() const noexcept;

  //!
  //! Variables
  //!

  /// Local position
  DDyVector3 mLocalPosition   = {0, 0, 0};
  /// World position.
  DDyVector3 mWorldPosition   = {0, 0, 0};

  /// Local euler angle
  DDyVector3 mLocalEulerAngle = {0, 0, 0};
  /// World euler angle which affects to position basis to child actor.
  DDyVector3 mWorldEulerAngle = {0, 0, 0};

  /// Scale local factor, default is (1, 1, 1)
  DDyVector3 mLocalScale      = {1, 1, 1};
  /// Scale world factor. default is (1, 1, 1)
  DDyVector3 mWorldScale      = {1, 1, 1};

  //!
  //! Parent depdendent information
  //!

  /// parent::mMovementBasis
  MDY_TRANSIENT DDyVector3    mFromParentPositionBasis          = {};
  /// parent::mWorldSumAlignedPosition
  MDY_TRANSIENT DDyVector3    mWorldRelativeOriginPosition          = {};
  /// parent::mWorldSumEulerAngle
  MDY_TRANSIENT DDyVector3    mFromParentWorldEulerAngle        = {};
  /// parent::mWorldProductScale
  MDY_TRANSIENT DDyVector3    mFromParentWorldScale             = DDyVector3{ 1.f };

  //!
  //! Lazy evlauated variables by flag activation.
  //!

  /// (x, y, z) parent position to bring child. \n
  /// from this::mWorldRelativeOriginPosition + this::mWorldRelativeAlignedPosition.
  MDY_TRANSIENT DDyVector3    mWorldSumAlignedPosition             = {};
  /// this::mWorldEulerAngle + this::mFromParentWorldEulerAngle
  MDY_TRANSIENT DDyVector3    mWorldSumEulerAngle           = {};
  /// this::mWorldScale * this::mFromParentWorldScale
  MDY_TRANSIENT DDyVector3    mWorldProductScale                = DDyVector3{ 1.f };

  /// from this::mFromParentWorldEulerAngle (parent::mWorldSumEulerAngle)
  MDY_TRANSIENT std::array<DDyVector3, 3> mMovementBasis= { DDyVector3::RightX(), DDyVector3::UpY(), DDyVector3::FrontZ() };
  /// from this::mFromParentWorldEulerAngle (parent::mWorldSumEulerAngle)
  MDY_TRANSIENT std::array<DDyVector3, 3> mToChildBasis         = { DDyVector3::RightX(), DDyVector3::UpY(), DDyVector3::FrontZ() };

  /// from this::mMovementBasis[0][1][2] * this::mLocalPosition.XYZ
  MDY_TRANSIENT DDyVector3    mFinalBasisAlignedLocalPosition   = {};
  /// from this::mMovementBasis[0][1][2] * this::mWorldPosition.XYZ
  MDY_TRANSIENT DDyVector3    mWorldRelativeAlignedPosition   = {};

  /// (x, y, z) final position in hierarchy. \n
  /// from this::mWorldSumAlignedPosition + this::mFinalBasisAlignedLocalPosition
  MDY_TRANSIENT DDyVector3    mFinalRenderingPosition           = {};
  /// from this::mLocalEulerAngle + this::mWorldSumEulerAngle
  MDY_TRANSIENT DDyVector3    mFinalRenderingEulerAngle         = {};
  /// from this::mFinalRenderingEulerAngle
  MDY_TRANSIENT DDyQuaternion mFinalRenderRotationQuaternion    = {DDyVector3{0, 0, 0}};
  /// this::mLocalScale * this::mWorldProductScale
  MDY_TRANSIENT DDyVector3    mFinalRenderingScale              = DDyVector3{ 1.f };

  //!
  //! Result
  //!

  ///
  MDY_TRANSIENT DDyMatrix3x3  mFinalPositionBasisMatrix;
  /// Final model matrix also reflected by parent's and world rot.
  MDY_TRANSIENT DDyMatrix4x4  mFinalRenderingTransform;

  //!
  //! Flags
  //!

  ///
  MDY_TRANSIENT bool mIsMovementBasisDirty            = true;
  ///
  MDY_TRANSIENT bool mToChildBasisAxisDirty       = true;

  ///
  MDY_TRANSIENT bool mIsWorldSumRotationAngleDirty = true;
  ///
  MDY_TRANSIENT bool mIsFinalRotationAngleDirty   = true;

  ///
  MDY_TRANSIENT bool mIsWorldPrdScaleDirty    = true;
  ///
  MDY_TRANSIENT bool mIsFinalScaleDirty           = true;

  ///
  MDY_TRANSIENT bool mIsLocalPositionAlignDirty   = true;
  ///
  MDY_TRANSIENT bool mIsWorldPositionAlignDirty   = true;
  MDY_TRANSIENT bool mIsWorldRelAlignedPosDirty   = true;
  ///
  MDY_TRANSIENT bool mIsWorldSumAlignedPosDirty   = true;
  ///
  MDY_TRANSIENT bool mIsFinalPositionDirty        = true;

  ///
  MDY_TRANSIENT bool mIsModelMatrixDirty          = true;

  //!
  //! Related to friend function called from children FDyActor's transform.
  //!

  ///
  MDY_TRANSIENT bool mIsFromChildPositionUpdated  = true;
  ///
  MDY_TRANSIENT bool mIsFromChildRotationUpdated   = true;
  ///
  MDY_TRANSIENT bool mIsFromChildScaleUpdated      = true;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyTransform);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyTransform);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYTRANSFORM_H