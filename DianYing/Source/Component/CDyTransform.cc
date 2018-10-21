#include <precompiled.h>
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

/// Header file
#include <Dy/Component/CDyTransform.h>
#include <Dy/Component/Descriptor/ComponentMetaDescriptor.h>
#include <Dy/Element/Actor.h>

namespace dy
{

CDyTransform::CDyTransform(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyTransform::Initialize(const DDyTransformMetaInformation& desc)
{
  this->SetLocalPosition(desc.mLocalPosition);
  this->SetLocalEulerAngle(desc.mLocalRotation);
  this->SetLocalScale(desc.mLocalScale);

  this->SetWorldPosition(desc.mWorldPosition);
  this->SetWorldEulerAngle(desc.mWorldRotation);
  this->SetWorldScale(desc.mWorldScale);

  return DY_SUCCESS;
}

void CDyTransform::Release()
{

}

//!
//! Getter
//!

const DDyVector3& CDyTransform::GetLocalPosition() const noexcept
{
  return this->mLocalPosition;
}

const DDyVector3& CDyTransform::GetWorldPosition() const noexcept
{
  return this->mWorldPosition;
}

const DDyVector3& CDyTransform::GetFinalPosition() const noexcept
{
  this->pUpdateFromParentWorldEulerAngle();
  this->pUpdateFinalRenderingAxis();
  this->pUpdateFinalRenderingPosition();

  return this->mFinalRenderingPosition;
}

const std::array<DDyVector3, 3>& CDyTransform::GetToChildBasis() const noexcept
{
  this->pUpdateFromParentWorldEulerAngle();
  this->pUpdateFinalRenderingEulerAngle();
  this->pUpdateToChildBasisAxis();

  return this->mToChildBasis;
}

float CDyTransform::GetLocalEulerAngle(_MIN_ EDyAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EDyAxis3D::X: return this->mLocalEulerAngle.X;
  case EDyAxis3D::Y: return this->mLocalEulerAngle.Y;
  case EDyAxis3D::Z: return this->mLocalEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

float CDyTransform::GetWorldEulerAngle(_MIN_ EDyAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EDyAxis3D::X: return this->mWorldEulerAngle.X;
  case EDyAxis3D::Y: return this->mWorldEulerAngle.Y;
  case EDyAxis3D::Z: return this->mWorldEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

float CDyTransform::GetFinalEulerAngle(_MIN_ EDyAxis3D direction) const noexcept
{
  this->pUpdateFromParentWorldEulerAngle();
  this->pUpdateFinalRenderingEulerAngle();

  switch (direction)
  {
  case EDyAxis3D::X: return this->mFinalRenderingEulerAngle.X;
  case EDyAxis3D::Y: return this->mFinalRenderingEulerAngle.Y;
  case EDyAxis3D::Z: return this->mFinalRenderingEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

const DDyVector3& CDyTransform::GetLocalScale() const noexcept
{
  return this->mLocalScale;
}

const DDyVector3& CDyTransform::GetWorldScale() const noexcept
{
  return this->mWorldScale;
}

const DDyVector3& CDyTransform::GetFinalScale() const noexcept
{
  this->pUpdateFromParentWorldScale();
  this->pUpdateFinalRenderingScale();

  return this->mFinalRenderingScale;
}

//!
//! Setter
//!

void CDyTransform::SetLocalPosition(_MIN_ const DDyVector3& localPosition) noexcept
{
  this->mLocalPosition            = localPosition;
  this->mIsModelMatrixDirty       = true;
  this->mIsLocalPositionAlignDirty     = true;
  this->mIsFinalPositionDirty     = true;
}

void CDyTransform::SetWorldPosition(_MIN_ const DDyVector3& worldPosition) noexcept
{
  this->mWorldPosition            = worldPosition;
  this->mIsModelMatrixDirty       = true;
  this->mIsWorldPositionAlignDirty     = true;
  this->mIsFinalPositionDirty     = true;
  this->mIsToChildPositionDirty   = true;
}

void CDyTransform::SetWorldPositionFromFinalPosition(_MIN_ const DDyVector3& finalPosition) noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void CDyTransform::AddOffsetLocalPosition(_MIN_ EDyAxis3D axis, _MIN_ float value) noexcept
{
  switch (axis)
  {
  case EDyAxis3D::X: this->mLocalPosition.X += value; break;
  case EDyAxis3D::Y: this->mLocalPosition.Y += value; break;
  case EDyAxis3D::Z: this->mLocalPosition.Z += value; break;
  }

  this->mIsModelMatrixDirty       = true;
  this->mIsLocalPositionAlignDirty     = true;
  this->mIsFinalPositionDirty     = true;
}

void CDyTransform::AddOffsetWorldPosition(_MIN_ EDyAxis3D axis, _MIN_ float value) noexcept
{
  switch (axis)
  {
  case EDyAxis3D::X: this->mWorldPosition.X += value; break;
  case EDyAxis3D::Y: this->mWorldPosition.Y += value; break;
  case EDyAxis3D::Z: this->mWorldPosition.Z += value; break;
  }

  this->mIsModelMatrixDirty       = true;
  this->mIsWorldPositionAlignDirty     = true;
  this->mIsFinalPositionDirty     = true;
  this->mIsToChildPositionDirty   = true;
}

void CDyTransform::SetLocalEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept
{
  this->mLocalEulerAngle                = eulerAngleValue;
  this->mIsModelMatrixDirty             = true;
  this->mToChildBasisAxisDirty          = true;
  this->mIsFinalRotationAngleDirty      = true;
}

void CDyTransform::SetWorldEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept
{
  this->mWorldEulerAngle                = eulerAngleValue;
  this->mIsModelMatrixDirty             = true;
  this->mIsFinalRotationAngleDirty      = true;
  this->mIsToChildRotationAngleDirty    = true;
  this->mToChildBasisAxisDirty          = true;
  this->mIsFinalAxisDirty               = true;
}

void CDyTransform::AddLocalEulerAngle(_MIN_ EDyAxis3D axis, _MIN_ float eulerAngleValue) noexcept
{
  switch (axis)
  {
  case EDyAxis3D::X: this->mLocalEulerAngle.X += eulerAngleValue; break;
  case EDyAxis3D::Y: this->mLocalEulerAngle.Y += eulerAngleValue; break;
  case EDyAxis3D::Z: this->mLocalEulerAngle.Z += eulerAngleValue; break;
  default: break;
  }

  this->mIsFinalRotationAngleDirty      = true;
  this->mToChildBasisAxisDirty          = true;
  this->mIsModelMatrixDirty             = true;
}

void CDyTransform::AddWorldEulerAngle(_MIN_ EDyAxis3D axis, _MIN_ float eulerAngleValue) noexcept
{
  switch (axis) {
  case EDyAxis3D::X: this->mWorldEulerAngle.X += eulerAngleValue; break;
  case EDyAxis3D::Y: this->mWorldEulerAngle.Y += eulerAngleValue; break;
  case EDyAxis3D::Z: this->mWorldEulerAngle.Z += eulerAngleValue; break;
  default: break;
  }

  this->mIsFinalRotationAngleDirty      = true;
  this->mIsFinalAxisDirty               = true;
  this->mIsToChildRotationAngleDirty    = true;
  this->mToChildBasisAxisDirty          = true;
  this->mIsModelMatrixDirty             = true;
}

void CDyTransform::SetLocalScale(_MIN_ const DDyVector3& xyz_value) noexcept
{
  this->mLocalScale                     = xyz_value;
  this->mIsFinalScaleDirty              = true;
  this->mIsModelMatrixDirty             = true;
}

void CDyTransform::SetWorldScale(_MIN_ const DDyVector3& xyz_value) noexcept
{
  this->mWorldScale                     = xyz_value;
  this->mIsToChildWorldScaleDirty       = true;
  this->mIsFinalScaleDirty              = true;
  this->mIsModelMatrixDirty             = true;
}

std::string CDyTransform::ToString()
{
  return MDY_INITILAIZE_EMPTYSTR;
}

//!
//! Lazy evaluation phase
//!

const DDyMatrix4x4& CDyTransform::GetTransform() const noexcept
{
  if (this->mIsModelMatrixDirty == true)
  { // Update dependent transform information.
    if (this->mIsFinalScaleDirty          == true) { this->pUpdateFinalRenderingScale();      }
    if (this->mIsFinalRotationAngleDirty  == true) { this->pUpdateFinalRenderingEulerAngle(); }
    if (this->mIsFinalAxisDirty           == true) { this->pUpdateFinalRenderingAxis(); }
    if (this->mIsFinalPositionDirty       == true) { this->pUpdateFinalRenderingPosition();   }

    // Assemble not obsoleted updated value.
    this->mFinalRenderingTransform      = this->mFinalRenderRotationQuaternion.GetRotationMatrix4x4();

    this->mFinalRenderingTransform[0]   *= this->mFinalRenderingScale.X;
    this->mFinalRenderingTransform[1]   *= this->mFinalRenderingScale.Y;
    this->mFinalRenderingTransform[2]   *= this->mFinalRenderingScale.Z;

    this->mFinalRenderingTransform[3][0] = this->mFinalRenderingPosition.X;
    this->mFinalRenderingTransform[3][1] = this->mFinalRenderingPosition.Y;
    this->mFinalRenderingTransform[3][2] = this->mFinalRenderingPosition.X;

    this->mIsModelMatrixDirty = false;
  }

  return this->mFinalRenderingTransform;
}

//!
//! Rotation lazy evaluation
//!

void CDyTransform::pUpdateFinalRenderingEulerAngle() const noexcept
{
  if (this->mIsFinalRotationAngleDirty == true)
  { // Update dependent transform information.
    this->pUpdateToChildWorldEulerAngle();

    // Set final rendering angles, and... update quaternion to setup new model matrix.
    this->mFinalRenderingEulerAngle       = this->mToChildWorldEulerAngle + this->mLocalEulerAngle;
    this->mFinalRenderRotationQuaternion.SetRotationAngle(this->mFinalRenderingEulerAngle);

    this->mIsFinalRotationAngleDirty      = false;
    this->mToChildBasisAxisDirty          = true;
    this->mIsModelMatrixDirty             = true;
  }
}

void CDyTransform::pUpdateToChildWorldEulerAngle() const noexcept
{
  if (this->mIsToChildRotationAngleDirty == true)
  { // Update dependent transform information.
    this->pUpdateFromParentWorldEulerAngle();

    // Update CDyTransform::mToChildWorldEulerAngle
    this->mToChildWorldEulerAngle         = this->mFromParentWorldEulerAngle + this->mWorldEulerAngle;

    this->mIsToChildRotationAngleDirty    = false;
    this->mIsFromChildRotationUpdated     = true;
    this->mIsFinalRotationAngleDirty      = true;
    this->mToChildBasisAxisDirty          = true;
    this->mIsModelMatrixDirty             = true;
  }
}

void CDyTransform::pUpdateFromParentWorldEulerAngle() const noexcept
{
  auto* validActorPtr = const_cast<FDyActor*>(this->GetBindedActor());

  if (validActorPtr->IsHaveParent() == false)
  { // If binded actor have not a parent.
    this->mFromParentWorldEulerAngle  = DDyVector3{0, 0, 0};
  }
  else
  { // If binded actor has a parent.
    auto parent                         = validActorPtr->GetParent();
    CDyTransform* parentTransformPtr    = parent.value()->GetTransform();

    if (parentTransformPtr->pfIsToChildRotationUpdate() == true)
    { // Get updated parent::mToChildWorldEulerAngle vector.
      this->mFromParentWorldEulerAngle  = parentTransformPtr->pfGetToChildWorldEulerAngle();
    }
  }

  this->mIsFinalAxisDirty           = true;
  this->mIsFinalPositionDirty       = true;
  this->mToChildBasisAxisDirty      = true;
  this->mIsModelMatrixDirty         = true;
}

//!
//! Axis lazy evaluation
//!

void CDyTransform::pUpdateFinalRenderingAxis() const noexcept
{
  if (this->mIsFinalAxisDirty == true)
  { //
    const auto quat = DDyQuaternion(this->mFromParentWorldEulerAngle);
    this->mFinalPositionBasisMatrix = quat.GetRotationMatrix3x3();

    // Update position movement basis of this FDyActor.
    const auto& m                   = this->mFinalPositionBasisMatrix;
    this->mPositionMovementBasis[0] = DDyVector3{m[0][0], m[0][1], m[0][2]};
    this->mPositionMovementBasis[1] = DDyVector3{m[1][0], m[1][1], m[1][2]};
    this->mPositionMovementBasis[2] = DDyVector3{m[2][0], m[2][1], m[2][2]};

    this->mIsFinalAxisDirty         = false;
    this->mIsFinalPositionDirty     = true;
    this->mIsToChildPositionDirty   = true;
  }
}

void CDyTransform::pUpdateToChildBasisAxis() const noexcept
{
  if (this->mToChildBasisAxisDirty == true)
  { //
    const auto m = DDyQuaternion{this->mFinalRenderingEulerAngle}.GetRotationMatrix3x3();

    //
    this->mToChildBasis[0] = DDyVector3{m[0][0], m[0][1], m[0][2]};
    this->mToChildBasis[1] = DDyVector3{m[1][0], m[1][1], m[1][2]};
    this->mToChildBasis[2] = DDyVector3{m[2][0], m[2][1], m[2][2]};

    //
    this->mToChildBasisAxisDirty = false;
  }
}

//!
//! Position lazy evaluation
//!

void CDyTransform::pUpdateFinalRenderingPosition() const noexcept
{
  if (this->mIsFinalPositionDirty == true)
  { // Update dependent transform information.
    if (this->mIsLocalPositionAlignDirty == true) { this->pUpdateAxisAlignedLocalPosition(); }
    if (this->mIsWorldPositionAlignDirty == true) { this->pUpdateAxisAlignedWorldPosition(); }
    if (this->mIsToChildPositionDirty == true)    { this->pUpdateToChildPosition(); }

    // Update value.
    this->mFinalRenderingPosition = this->mToChildWorldPosition + this->mFinalBasisAlignedLocalPosition;

    // Update flag.
    this->mIsFinalPositionDirty   = false;
    this->mIsModelMatrixDirty     = true;
  }
}

void CDyTransform::pUpdateAxisAlignedLocalPosition() const noexcept
{
  if (this->mIsLocalPositionAlignDirty == true)
  { // Update dependent transform information.
    if (this->mIsFinalAxisDirty == true) { this->pUpdateFinalRenderingAxis(); }

    // Update value.
    this->mFinalBasisAlignedLocalPosition = this->mFinalPositionBasisMatrix.MultiplyVector(this->mLocalPosition);

    // Update flag.
    this->mIsLocalPositionAlignDirty  = false;
    this->mIsFinalPositionDirty       = true;
    this->mIsModelMatrixDirty         = true;
  }
}

void CDyTransform::pUpdateAxisAlignedWorldPosition() const noexcept
{
  if (this->mIsWorldPositionAlignDirty == true)
  { // Update dependent transform information.
    if (this->mIsFinalAxisDirty == true) { this->pUpdateFinalRenderingAxis(); }

    // Update value.
    this->mFinalBasisAlignedWorldPosition = this->mFinalPositionBasisMatrix.MultiplyVector(this->mWorldPosition);

    // Update flag.
    this->mIsWorldPositionAlignDirty  = false;
    this->mIsFinalPositionDirty       = true;
    this->mIsModelMatrixDirty         = true;
  }
}

void CDyTransform::pUpdateToChildPosition() const noexcept
{
  if (this->mIsToChildPositionDirty == true)
  { // Update dependent transform information.
    this->pUpdateFinalRenderingAxis();
    this->pUpdateAxisAlignedWorldPosition();
    this->pUpdateFromParentWorldPosition();

    // Update value.
    this->mToChildWorldPosition       = this->mFromParentWorldPosition + this->mFinalBasisAlignedWorldPosition;

    // Update flag.
    this->mIsToChildPositionDirty     = false;
    this->mIsFromChildPositionUpdated = true;
    this->mIsFinalPositionDirty       = true;
    this->mIsModelMatrixDirty         = true;
  }
}

void CDyTransform::pUpdateFromParentWorldPosition() const noexcept
{
  if (auto* validActorPtr = const_cast<FDyActor*>(this->GetBindedActor());
      validActorPtr->IsHaveParent() == false)
  { // If binded actor have not a parent.
    this->mFromParentWorldEulerAngle = DDyVector3{0, 0, 0};
  }
  else
  { // If binded actor has a parent.
    auto parent                      = validActorPtr->GetParent();
    CDyTransform* parentTransformPtr = parent.value()->GetTransform();

    if (parentTransformPtr->pfIsToChildPositionUpdated() == true)
    { // Get updated parent::mToChildPosition vector.
      this->mFromParentWorldPosition = parentTransformPtr->pfGetToChildPosition();
    }
  }

  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;
}

//!
//! Scale lazy evaluation
//!

void CDyTransform::pUpdateFinalRenderingScale() const noexcept
{
  if (this->mIsFinalScaleDirty == true)
  { // Update dependent transform information.
    if (this->mIsToChildWorldScaleDirty == true) { this->pUpdateToChildWorldScale(); }

    this->mFinalRenderingScale  = this->mToChildWorldScale * this->mLocalScale;
    this->mIsFinalScaleDirty    = false;
    this->mIsModelMatrixDirty   = true;
  }
}

void CDyTransform::pUpdateToChildWorldScale() const noexcept
{
  if (this->mIsToChildWorldScaleDirty == true)
  { // Update dependent transform information.
    this->pUpdateFromParentWorldScale();

    this->mToChildWorldScale = this->mWorldScale * this->mFromParentWorldScale;

    this->mIsToChildWorldScaleDirty = false;
    this->mIsFromChildScaleUpdated  = true;
    this->mIsFinalScaleDirty        = true;
    this->mIsModelMatrixDirty       = true;
  }
}

void CDyTransform::pUpdateFromParentWorldScale() const noexcept
{
  if (auto* validActorPtr = const_cast<FDyActor*>(this->GetBindedActor());
      validActorPtr->IsHaveParent() == false)
  { // If binded actor have not a parent.
    this->mFromParentWorldScale = DDyVector3{1, 1, 1};
  }
  else
  { // If binded actor has a parent.
    auto parent                      = validActorPtr->GetParent();
    CDyTransform* parentTransformPtr = parent.value()->GetTransform();

    if (parentTransformPtr->pfIsToChildScaleUpdate() == true)
    { // Get updated parent::mToChildPosition vector.
      this->mFromParentWorldScale = parentTransformPtr->pfGetToChildScale();
    }
  }

  this->mIsFinalScaleDirty          = true;
  this->mIsModelMatrixDirty         = true;
}

//!
//! Friend function
//!

const DDyVector3& CDyTransform::pfGetToChildPosition() const noexcept
{
  if (this->mIsFromChildPositionUpdated == true)
  { // Update dependent information.
    if (this->mIsToChildPositionDirty == true) { this->pUpdateToChildPosition(); }
    // Update flag
    this->mIsFromChildPositionUpdated = false;
  }

  return this->mToChildWorldPosition;
}

const DDyVector3& CDyTransform::pfGetToChildWorldEulerAngle() const noexcept
{
  if (this->mIsFromChildRotationUpdated == true)
  { // Update dependent information.
    if (this->mIsToChildRotationAngleDirty == true) { this->pUpdateToChildWorldEulerAngle(); }
    // Update flag
    this->mIsFromChildRotationUpdated = false;
  }

  return this->mToChildWorldEulerAngle;
}

const DDyVector3& CDyTransform::pfGetToChildScale() const noexcept
{
  if (this->mIsFromChildScaleUpdated == true)
  { // Update dependent information.
    if (this->mIsToChildWorldScaleDirty == true) { this->pUpdateToChildWorldScale(); }
    // Update flag
    this->mIsFromChildScaleUpdated = false;
  }

  return this->mToChildWorldScale;
}

} /// ::dy namespace