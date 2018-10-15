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

//!
//! Forward declaration
//!

namespace dy
{
struct DDyTransformMetaInformation;
} /// ::dy namespace

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
  CDyTransform(FDyActor& actorReference);
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
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const DDyTransformMetaInformation& desc);

  /// @brief Release component.
  void Release();

  ///
  /// @brief  Return local position.
  /// @return Actor's local position.
  ///
  MDY_NODISCARD const DDyVector3& GetLocalPosition() const noexcept;

	///
	/// @brief  Return world position.
	/// @return Actor's world position from parent object's world basis.
	///
  MDY_NODISCARD const DDyVector3& GetWorldPosition() const noexcept;

  ///
  /// @brief  Get Object's final position.
  /// @return Actor's final world position which to be rendered.
  ///
  MDY_NODISCARD const DDyVector3& GetFinalPosition() const noexcept;

  ///
  /// @brief  Get Object's to child basis (but not propagated to children FDyActor!)
  /// @return Actor's to child basis (this::mLocalEulerAngle + this::mToChildEulerAngle)
  ///
  MDY_NODISCARD const std::array<DDyVector3, 3>& GetToChildBasis() const noexcept;

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD const DDyVector3& GetLocalRotationAngle() const noexcept;

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD const DDyVector3& GetWorldRotationAngle() const noexcept;

  ///
  /// @brief
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD const DDyVector3& GetFinalRotationAngle() const noexcept;

  ///
  /// @brief  The method gets local rotation angle value. (euler degree value)
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  ///
  MDY_NODISCARD float GetLocalEulerAngle(_MIN_ EDyAxis3D direction) const noexcept;

  ///
  /// @brief  The method gets world rotation angle value. (euler degree value)
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  ///
  MDY_NODISCARD float GetWorldEulerAngle(_MIN_ EDyAxis3D direction) const noexcept;

  ///
  /// @brief  The method gets final rendering angle value. (euler degree value)
  /// @param  direction Direction axis to get degree angle value.
  /// @return Object's rotation angle value.
  ///
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

  ///
  /// @brief Sets local position.
  /// @param[in] localPosition local position to set on.
  ///
  void SetLocalPosition(_MIN_ const DDyVector3& localPosition) noexcept;

	///
	/// @brief Set world position.
	/// @param[in] worldPosition Winal position in Screen space and from parent' object.
	///
	void SetWorldPosition(_MIN_ const DDyVector3& worldPosition) noexcept;

  ///
  /// @brief The method set final position but keeping parent position
  /// but adjusting FDyPawn's world position.
  /// @TODO SCRIPT THIS
  ///
  void SetWorldPositionFromFinalPosition(_MIN_ const DDyVector3& finalPosition) noexcept;

  ///
  /// @brief Add offset value with axis as local position.
  /// @TODO SCRIPT THIS
  ///
  void AddOffsetLocalPosition(_MIN_ EDyAxis3D axis, _MIN_ float value) noexcept;

  ///
  /// @brief Add offset value with axis as world position.
  /// @TODO SCRIPT THIS
  ///
  void AddOffsetWorldPosition(_MIN_ EDyAxis3D axis, _MIN_ float value) noexcept;

  ///
  /// @brief The method adds local rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  ///
  /// @param[in] eulerAngleValue Euler angle value to set on.
  ///
  void SetLocalEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept;

  ///
  /// @brief The method adds world rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  ///
  /// @param[in] eulerAngleValue Euler angle value to set on.
  ///
  void SetWorldEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept;

  ///
  /// @brief Add offset eulerAngleValue with axis as local rotation angle.
  /// @TODO SCRIPT THIS
  ///
  void AddLocalEulerAngle(_MIN_ EDyAxis3D axis, _MIN_ float eulerAngleValue) noexcept;

  ///
  /// @brief Add offset eulerAngleValue with axis as world rotation angle.
  /// @TODO SCRIPT THIS
  ///
  void AddWorldEulerAngle(_MIN_ EDyAxis3D axis, _MIN_ float eulerAngleValue) noexcept;

  ///
  /// @brief The method sets scaling vector have (x, y, z) scaling factors.
  /// @TODO SCRIPT THIS
  ///
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

private:
  /// @brief
  void pUpdateFinalRenderingEulerAngle() const noexcept;
  /// @brief
  void pUpdateToChildWorldEulerAngle() const noexcept;
  /// @brief
  void pUpdateFromParentWorldEulerAngle() const noexcept;

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

  /// parent::mPositionMovementBasis
  MDY_TRANSIENT DDyVector3    mFromParentPositionBasis          = {};
  /// parent::mToChildWorldPosition
  MDY_TRANSIENT DDyVector3    mFromParentWorldPosition          = {};
  /// parent::mToChildWorldEulerAngle
  MDY_TRANSIENT DDyVector3    mFromParentWorldEulerAngle        = {};
  /// parent::mToChildWorldScale
  MDY_TRANSIENT DDyVector3    mFromParentWorldScale             = DDyVector3{ 1.f };

  //!
  //! Lazy evlauated variables by flag activation.
  //!

  /// (x, y, z) parent position to bring child. \n
  /// from this::mFromParentWorldPosition + this::mFinalBasisAlignedWorldPosition.
  MDY_TRANSIENT DDyVector3    mToChildWorldPosition             = {};
  /// this::mWorldEulerAngle + this::mFromParentWorldEulerAngle
  MDY_TRANSIENT DDyVector3    mToChildWorldEulerAngle           = {};
  /// this::mWorldScale * this::mFromParentWorldScale
  MDY_TRANSIENT DDyVector3    mToChildWorldScale                = DDyVector3{ 1.f };

  /// from this::mFromParentWorldEulerAngle (parent::mToChildWorldEulerAngle)
  MDY_TRANSIENT std::array<DDyVector3, 3> mPositionMovementBasis= { DDyVector3::RightX(), DDyVector3::UpY(), DDyVector3::FrontZ() };
  /// from this::mFromParentWorldEulerAngle (parent::mToChildWorldEulerAngle)
  MDY_TRANSIENT std::array<DDyVector3, 3> mToChildBasis         = { DDyVector3::RightX(), DDyVector3::UpY(), DDyVector3::FrontZ() };

  /// from this::mPositionMovementBasis[0][1][2] * this::mLocalPosition.XYZ
  MDY_TRANSIENT DDyVector3    mFinalBasisAlignedLocalPosition   = {};
  /// from this::mPositionMovementBasis[0][1][2] * this::mWorldPosition.XYZ
  MDY_TRANSIENT DDyVector3    mFinalBasisAlignedWorldPosition   = {};

  /// (x, y, z) final position in hierarchy. \n
  /// from this::mToChildWorldPosition + this::mFinalBasisAlignedLocalPosition
  MDY_TRANSIENT DDyVector3    mFinalRenderingPosition           = {};
  /// from this::mLocalEulerAngle + this::mToChildWorldEulerAngle
  MDY_TRANSIENT DDyVector3    mFinalRenderingEulerAngle         = {};
  /// from this::mFinalRenderingEulerAngle
  MDY_TRANSIENT DDyQuaternion mFinalRenderRotationQuaternion    = {DDyVector3{0, 0, 0}};
  /// this::mLocalScale * this::mToChildWorldScale
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
  MDY_TRANSIENT bool mIsFinalAxisDirty            = true;
  ///
  MDY_TRANSIENT bool mToChildBasisAxisDirty       = true;

  ///
  MDY_TRANSIENT bool mIsToChildRotationAngleDirty = true;
  ///
  MDY_TRANSIENT bool mIsFinalRotationAngleDirty   = true;

  ///
  MDY_TRANSIENT bool mIsToChildWorldScaleDirty    = true;
  ///
  MDY_TRANSIENT bool mIsFinalScaleDirty           = true;

  ///
  MDY_TRANSIENT bool mIsLocalPositionAlignDirty   = true;
  ///
  MDY_TRANSIENT bool mIsWorldPositionAlignDirty   = true;
  ///
  MDY_TRANSIENT bool mIsToChildPositionDirty      = true;
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