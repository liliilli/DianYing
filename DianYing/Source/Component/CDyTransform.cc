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
#include <Dy/Element/Actor.h>

namespace dy
{


EDySuccess CDyTransform::Initialize(const PDyTransformComponentMetaInfo& desc)
{
  this->SetRelativeLocalPosition(desc.mDetails.mLocalPosition);
  this->SetLocalEulerAngle(desc.mDetails.mLocalRotation);
  this->SetLocalScale(desc.mDetails.mLocalScale);

  this->SetRelativeWorldPosition(desc.mDetails.mWorldPosition);
  this->SetWorldEulerAngle(desc.mDetails.mWorldRotation);
  this->SetWorldScale(desc.mDetails.mWorldScale);

  return DY_SUCCESS;
}

void CDyTransform::Release()
{

}

//!
//! Getter
//!

const DDyVector3& CDyTransform::GetRelativeLocalPosition() const noexcept
{
  return this->mLocalPosition;
}

const DDyVector3& CDyTransform::GetRelativeWorldPosition() const noexcept
{
  return this->mWorldPosition;
}

const DDyVector3& CDyTransform::GetFinalWorldPosition() const noexcept
{
  this->__UpdateFromParentWorldEulerAngle();
  this->pUpdateFinalRenderingAxis();
  this->pUpdateFinalRenderingPosition();

  return this->mFinalRenderingPosition;
}

const std::array<DDyVector3, 3>& CDyTransform::GetToChildBasis() const noexcept
{
  this->__UpdateFromParentWorldEulerAngle();
  this->pUpdateFinalRenderingEulerAngle();
  this->pUpdateToChildBasisAxis();

  return this->mToChildBasis;
}

const DDyVector3& CDyTransform::GetRelativeLocalEulerAngle() const noexcept
{
  return this->mLocalEulerAngle;
}

float CDyTransform::GetRelativeLocalEulerAngle(_MIN_ EDyAxis3D direction) const noexcept
{
  const auto& rlea = this->GetRelativeLocalEulerAngle();
  switch (direction)
  {
  case EDyAxis3D::X: return rlea.X;
  case EDyAxis3D::Y: return rlea.Y;
  case EDyAxis3D::Z: return rlea.Z;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(0.0f);
  }
}

const DDyVector3& CDyTransform::GetRelativeWorldEulerAngle() const noexcept
{
  return this->mWorldEulerAngle;
}

float CDyTransform::GetRelativeWorldEulerAngle(_MIN_ EDyAxis3D direction) const noexcept
{
  const auto& rwea = this->GetRelativeWorldEulerAngle();
  switch (direction)
  {
  case EDyAxis3D::X: return rwea.X;
  case EDyAxis3D::Y: return rwea.Y;
  case EDyAxis3D::Z: return rwea.Z;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(0.0f);
  }
}

float CDyTransform::GetFinalEulerAngle(_MIN_ EDyAxis3D direction) const noexcept
{
  this->MDY_PRIVATE_SPECIFIER(UpdateFromParentWorldEulerAngle)();
  this->pUpdateFinalRenderingEulerAngle();

  switch (direction)
  {
  case EDyAxis3D::X: return this->mFinalRenderingEulerAngle.X;
  case EDyAxis3D::Y: return this->mFinalRenderingEulerAngle.Y;
  case EDyAxis3D::Z: return this->mFinalRenderingEulerAngle.Z;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(0.0f);
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

void CDyTransform::SetRelativeLocalPosition(_MIN_ const DDyVector3& localPosition) noexcept
{
  this->mLocalPosition            = localPosition;
  this->mIsLocalPositionAlignDirty= true;
  this->mIsFinalPositionDirty     = true;
  this->mIsModelMatrixDirty       = true;
}

void CDyTransform::SetRelativeWorldPosition(_MIN_ const DDyVector3& worldPosition) noexcept
{
  this->mWorldPosition            = worldPosition;
  this->mIsWorldPositionAlignDirty= true;
  this->mIsWorldSumAlignedPosDirty= true;
  this->mIsFinalPositionDirty     = true;
  this->mIsModelMatrixDirty       = true;

  auto* validActorPtr = this->GetBindedActor();
  if (validActorPtr->IsHavingChildrenObject() == true)
  { 
    // (1) Try update movement basis.
    this->MDY_PRIVATE_SPECIFIER(TryUpdateMovementBasis)();
    // (2) Try update relative aligend world position.
    this->MDY_PRIVATE_SPECIFIER(TryUpdateWorldRelAlignedPos)();
    // (3) Try update final position without relative local position (aka fwpos wo rlpos)
    this->MDY_PRIVATE_SPECIFIER(TryUpdateAlignedWorldSumPosition)();

    // (4) Try update `fwpos wo rlpos` into children's transform.
    auto& childrenContainer = validActorPtr->GetChildrenContainer();
    for (auto& [specifier, ptrsmtObject] : childrenContainer)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
      ptrsmtObject->GetTransform()->MDY_PRIVATE_SPECIFIER(SetRelativeWorldOrigin(this->mWorldSumAlignedPosition));
    }
  }
}

void CDyTransform::AddRelativeLocalPosition(_MIN_ const DDyVector3& localPosiiton) noexcept
{
  this->SetRelativeWorldPosition(this->GetRelativeLocalPosition() + localPosiiton);
}

void CDyTransform::AddRelativeLocalPosition(_MIN_ EDyAxis3D axis, _MIN_ TF32 value) noexcept
{
  DDyVector3 offset = {};
  switch (axis)
  {
  case EDyAxis3D::X: offset.X += value; break;
  case EDyAxis3D::Y: offset.Y += value; break;
  case EDyAxis3D::Z: offset.Z += value; break;
  }

  this->SetRelativeWorldPosition(this->GetRelativeLocalPosition() + offset);
}

void CDyTransform::AddRelativeWorldPosition(_MIN_ const DDyVector3& localPosiiton) noexcept
{
  this->SetRelativeWorldPosition(this->GetRelativeWorldPosition() + localPosiiton);
}

void CDyTransform::AddRelativeWorldPosition(_MIN_ EDyAxis3D axis, _MIN_ TF32 value) noexcept
{
  DDyVector3 offset = {};
  switch (axis)
  {
  case EDyAxis3D::X: offset.X += value; break;
  case EDyAxis3D::Y: offset.Y += value; break;
  case EDyAxis3D::Z: offset.Z += value; break;
  }

  this->SetRelativeWorldPosition(this->GetRelativeWorldPosition() + offset);
}

void CDyTransform::SetLocalEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept
{
  this->AddLocalEulerAngle(eulerAngleValue - this->GetRelativeLocalEulerAngle());
}

void CDyTransform::SetWorldEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept
{
  this->AddWorldEulerAngle(eulerAngleValue - this->GetRelativeWorldEulerAngle());
}

void CDyTransform::AddLocalEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept
{
  this->mLocalEulerAngle += eulerAngleValue;
  this->mIsFinalRotationAngleDirty = true;
  this->mToChildBasisAxisDirty     = true;
  this->mIsModelMatrixDirty        = true;
}

void CDyTransform::AddWorldEulerAngle(_MIN_ const DDyVector3& eulerAngleValue) noexcept
{
  this->mWorldEulerAngle += eulerAngleValue;
  this->mIsWorldSumRotationAngleDirty = true;
  this->mIsFinalRotationAngleDirty    = true;
  this->mIsModelMatrixDirty           = true;

  auto* validActorPtr = this->GetBindedActor();
  if (validActorPtr->IsHavingChildrenObject() == true)
  { 
    // (1) Try update sum of world (without each local) rotation angle.
    this->MDY_PRIVATE_SPECIFIER(TryUpdateWorldSumRotAngle)();

    // (2) Try update `mFromParentWorldEulerAngle` into children's transform.
    auto& childrenContainer = validActorPtr->GetChildrenContainer();
    for (auto& [specifier, ptrsmtObject] : childrenContainer)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
      ptrsmtObject->GetTransform()->MDY_PRIVATE_SPECIFIER(SetRotationAngleOrigin(this->mWorldSumEulerAngle));
    }
  }
}

void CDyTransform::AddLocalEulerAngle(_MIN_ EDyAxis3D axis, _MIN_ float eulerAngleValue) noexcept
{
  DDyVector3 eulerAngle = {};
  switch (axis)
  {
  case EDyAxis3D::X: eulerAngle.X = eulerAngleValue; break;
  case EDyAxis3D::Y: eulerAngle.Y = eulerAngleValue; break;
  case EDyAxis3D::Z: eulerAngle.Z = eulerAngleValue; break;
  default: break;
  }
 
  this->AddLocalEulerAngle(eulerAngle);
}

void CDyTransform::AddWorldEulerAngle(_MIN_ EDyAxis3D axis, _MIN_ float eulerAngleValue) noexcept
{
  DDyVector3 eulerAngle = {};
  switch (axis)
  {
  case EDyAxis3D::X: eulerAngle.X = eulerAngleValue; break;
  case EDyAxis3D::Y: eulerAngle.Y = eulerAngleValue; break;
  case EDyAxis3D::Z: eulerAngle.Z = eulerAngleValue; break;
  default: break;
  }

  this->AddWorldEulerAngle(eulerAngle);
}

void CDyTransform::SetLocalScale(_MIN_ const DDyVector3& xyz_value) noexcept
{
  this->mLocalScale         = xyz_value;
  this->mIsFinalScaleDirty  = true;
  this->mIsModelMatrixDirty = true;
}

void CDyTransform::SetWorldScale(_MIN_ const DDyVector3& xyz_value) noexcept
{
  this->mWorldScale           = xyz_value;
  this->mIsWorldPrdScaleDirty = true;
  this->mIsFinalScaleDirty    = true;
  this->mIsModelMatrixDirty   = true;

  if (auto* validActorPtr = this->GetBindedActor(); validActorPtr->IsHavingChildrenObject() == true)
  { 
    // (1) Try update product of world (without each local) scale.
    this->MDY_PRIVATE_SPECIFIER(TryUpdateWorldPrdScale)();

    // (2) Try update `mFromParentWorldScale` into children's transform.
    auto& childrenContainer = validActorPtr->GetChildrenContainer();
    for (auto& [specifier, ptrsmtObject] : childrenContainer)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
      ptrsmtObject->GetTransform()->MDY_PRIVATE_SPECIFIER(SetWorldScaleOrigin(this->mWorldProductScale));
    }
  }
}

std::string CDyTransform::ToString()
{
  return MDY_INITIALIZE_EMPTYSTR;
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
    if (this->mIsMovementBasisDirty       == true) { this->pUpdateFinalRenderingAxis(); }
    if (this->mIsFinalPositionDirty       == true) { this->pUpdateFinalRenderingPosition();   }

    // Assemble not obsoleted updated value.
    this->mFinalRenderingTransform      = this->mFinalRenderRotationQuaternion.GetRotationMatrix4x4();

    this->mFinalRenderingTransform[0]   *= this->mFinalRenderingScale.X;
    this->mFinalRenderingTransform[1]   *= this->mFinalRenderingScale.Y;
    this->mFinalRenderingTransform[2]   *= this->mFinalRenderingScale.Z;

    this->mFinalRenderingTransform[3][0] = this->mFinalRenderingPosition.X;
    this->mFinalRenderingTransform[3][1] = this->mFinalRenderingPosition.Y;
    this->mFinalRenderingTransform[3][2] = this->mFinalRenderingPosition.Z;

    this->mIsModelMatrixDirty = false;
  }

  return this->mFinalRenderingTransform;
}

//!
//! Rotation lazy evaluation
//!

void CDyTransform::MDY_PRIVATE_SPECIFIER(TryUpdateMovementBasis)()
{
  if (this->mIsMovementBasisDirty == false) { return; }

    // Update position movement basis of this FDyActor.
  const auto& m = this->mFinalPositionBasisMatrix;
  this->mMovementBasis[0] = DDyVector3{m[0][0], m[0][1], m[0][2]};
  this->mMovementBasis[1] = DDyVector3{m[1][0], m[1][1], m[1][2]};
  this->mMovementBasis[2] = DDyVector3{m[2][0], m[2][1], m[2][2]};
  
  this->mIsMovementBasisDirty       = false;
  this->mIsWorldRelAlignedPosDirty  = true;
  this->mIsWorldSumAlignedPosDirty  = true;
  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;
}

void CDyTransform::MDY_PRIVATE_SPECIFIER(TryUpdateWorldRelAlignedPos)()
{
  if (this->mIsWorldRelAlignedPosDirty == false) { return; }

  this->mWorldRelativeAlignedPosition = this->mFinalPositionBasisMatrix.MultiplyVector(this->mWorldPosition);

  this->mIsWorldRelAlignedPosDirty = false;
  this->mIsWorldSumAlignedPosDirty = true;
  this->mIsFinalPositionDirty      = true;
  this->mIsModelMatrixDirty        = true;
}

void CDyTransform::MDY_PRIVATE_SPECIFIER(TryUpdateAlignedWorldSumPosition)()
{
  if (this->mIsWorldSumAlignedPosDirty == false) { return; }

  // Update value.
  this->mWorldSumAlignedPosition = this->mWorldRelativeOriginPosition + this->mWorldRelativeAlignedPosition;

  // Update flag.
  this->mIsWorldSumAlignedPosDirty  = false;
  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;
}

void CDyTransform::MDY_PRIVATE_SPECIFIER(SetRelativeWorldOrigin)(_MIN_ const DDyVector3& iParentPosition)
{
  this->mWorldRelativeOriginPosition = iParentPosition;

  this->mIsWorldSumAlignedPosDirty  = true;
  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;

  // (3) Try update final position without relative local position (aka fwpos wo rlpos)
  this->MDY_PRIVATE_SPECIFIER(TryUpdateAlignedWorldSumPosition)();

  // (4) Try update `fwpos wo rlpos` into children's transform.
  auto& childrenContainer = this->GetBindedActor()->GetChildrenContainer();
  for (auto& [specifier, ptrsmtObject] : childrenContainer)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
    ptrsmtObject->GetTransform()->MDY_PRIVATE_SPECIFIER(SetRelativeWorldOrigin(this->mWorldSumAlignedPosition));
  }
}

void CDyTransform::MDY_PRIVATE_SPECIFIER(TryUpdateWorldSumRotAngle)()
{
  if (this->mIsWorldSumRotationAngleDirty == false) { return; }

  this->mWorldSumEulerAngle = this->mFromParentWorldEulerAngle + this->mWorldEulerAngle;

  this->mIsWorldSumRotationAngleDirty = false;
  this->mIsFinalRotationAngleDirty    = true;
  this->mIsModelMatrixDirty           = true;
}

void CDyTransform::MDY_PRIVATE_SPECIFIER(SetRotationAngleOrigin)(_MIN_ const DDyVector3& iParentSumRotAngle)
{
  this->mFromParentWorldEulerAngle = iParentSumRotAngle;

  this->mIsWorldSumRotationAngleDirty = true;
  this->mIsMovementBasisDirty      = true;
  this->mIsLocalPositionAlignDirty = true;
  this->mIsWorldPositionAlignDirty = true;
  this->mIsWorldRelAlignedPosDirty = true;
  this->mIsWorldSumAlignedPosDirty = true;
  this->mIsFinalPositionDirty      = true;
  this->mIsFinalRotationAngleDirty = true;
  this->mIsModelMatrixDirty        = true;

  // (1) Try update final position without relative local position (aka fwpos wo rlpos)
  this->MDY_PRIVATE_SPECIFIER(TryUpdateWorldSumRotAngle)();

  // (2) Try update children's transform recursively.
  auto& childrenContainer = this->GetBindedActor()->GetChildrenContainer();
  for (auto& [specifier, ptrsmtObject] : childrenContainer)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
    ptrsmtObject->GetTransform()->MDY_PRIVATE_SPECIFIER(SetRotationAngleOrigin)(this->mWorldSumAlignedPosition);
  }
}

void CDyTransform::MDY_PRIVATE_SPECIFIER(TryUpdateWorldPrdScale)()
{
  this->mWorldProductScale = this->mFromParentWorldScale * this->mWorldScale;

  this->mIsWorldPrdScaleDirty = false;
  this->mIsFinalScaleDirty    = true;
  this->mIsModelMatrixDirty   = true;
}

void CDyTransform::MDY_PRIVATE_SPECIFIER(SetWorldScaleOrigin)(_MIN_ const DDyVector3& iParentPrdScale)
{
  this->mFromParentWorldScale = iParentPrdScale;

  this->mIsWorldPrdScaleDirty = true;
  this->mIsFinalScaleDirty    = true;
  this->mIsModelMatrixDirty   = true;

  // (1) Try update scale.
  this->MDY_PRIVATE_SPECIFIER(TryUpdateWorldPrdScale)();

  // (2) Try update children's transform recursively.
  auto& childrenContainer = this->GetBindedActor()->GetChildrenContainer();
  for (auto& [specifier, ptrsmtObject] : childrenContainer)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
    ptrsmtObject->GetTransform()->MDY_PRIVATE_SPECIFIER(SetWorldScaleOrigin)(this->mWorldProductScale);
  }
}

void CDyTransform::pUpdateFinalRenderingEulerAngle() const noexcept
{
  if (this->mIsFinalRotationAngleDirty == true)
  { // Update dependent transform information.
    this->pUpdateToChildWorldEulerAngle();

    // Set final rendering angles, and... update quaternion to setup new model matrix.
    this->mFinalRenderingEulerAngle       = this->mWorldSumEulerAngle + this->mLocalEulerAngle;
    this->mFinalRenderRotationQuaternion.SetRotationAngle(this->mFinalRenderingEulerAngle);

    this->mIsFinalRotationAngleDirty      = false;
    this->mToChildBasisAxisDirty          = true;
    this->mIsModelMatrixDirty             = true;
  }
}

void CDyTransform::pUpdateToChildWorldEulerAngle() const noexcept
{
  if (this->mIsWorldSumRotationAngleDirty == true)
  { // Update dependent transform information.
    this->__UpdateFromParentWorldEulerAngle();

    // Update CDyTransform::mWorldSumEulerAngle
    this->mWorldSumEulerAngle         = this->mFromParentWorldEulerAngle + this->mWorldEulerAngle;

    this->mIsWorldSumRotationAngleDirty    = false;
    this->mIsFromChildRotationUpdated     = true;
    this->mIsFinalRotationAngleDirty      = true;
    this->mToChildBasisAxisDirty          = true;
    this->mIsModelMatrixDirty             = true;
  }
}

void CDyTransform::__UpdateFromParentWorldEulerAngle() const noexcept
{
  auto* validActorPtr = const_cast<FDyActor*>(this->GetBindedActor());

  if (validActorPtr->IsHaveParent() == false)
  { // If binded actor have not a parent.
    this->mFromParentWorldEulerAngle  = DDyVector3{0, 0, 0};
  }
  else
  { // If binded actor has a parent.
    CDyTransform* ptrParentTransform = validActorPtr->GetParent()->GetTransform();
    if (ptrParentTransform->pfIsToChildRotationUpdate() == true)
    { // Get updated parent::mWorldSumEulerAngle vector.
      this->mFromParentWorldEulerAngle  = ptrParentTransform->pfGetToChildWorldEulerAngle();
    }
  }

  this->mIsMovementBasisDirty           = true;
  this->mIsFinalPositionDirty       = true;
  this->mToChildBasisAxisDirty      = true;
  this->mIsModelMatrixDirty         = true;
}

//!
//! Axis lazy evaluation
//!

void CDyTransform::pUpdateFinalRenderingAxis() const noexcept
{
  if (this->mIsMovementBasisDirty == true)
  { //
    const auto quat = DDyQuaternion(this->mFromParentWorldEulerAngle);
    this->mFinalPositionBasisMatrix = quat.GetRotationMatrix3x3();

    // Update position movement basis of this FDyActor.
    const auto& m                   = this->mFinalPositionBasisMatrix;
    this->mMovementBasis[0] = DDyVector3{m[0][0], m[0][1], m[0][2]};
    this->mMovementBasis[1] = DDyVector3{m[1][0], m[1][1], m[1][2]};
    this->mMovementBasis[2] = DDyVector3{m[2][0], m[2][1], m[2][2]};

    this->mIsMovementBasisDirty         = false;
    this->mIsFinalPositionDirty     = true;
    this->mIsWorldSumAlignedPosDirty   = true;
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
    if (this->mIsWorldSumAlignedPosDirty == true)    { this->pUpdateToChildPosition(); }

    // Update value.
    this->mFinalRenderingPosition = this->mWorldSumAlignedPosition + this->mFinalBasisAlignedLocalPosition;

    // Update flag.
    this->mIsFinalPositionDirty   = false;
    this->mIsModelMatrixDirty     = true;
  }
}

void CDyTransform::pUpdateAxisAlignedLocalPosition() const noexcept
{
  if (this->mIsLocalPositionAlignDirty == true)
  { // Update dependent transform information.
    if (this->mIsMovementBasisDirty == true) { this->pUpdateFinalRenderingAxis(); }

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
    if (this->mIsMovementBasisDirty == true) { this->pUpdateFinalRenderingAxis(); }

    // Update value.
    this->mWorldRelativeAlignedPosition = this->mFinalPositionBasisMatrix.MultiplyVector(this->mWorldPosition);

    // Update flag.
    this->mIsWorldPositionAlignDirty  = false;
    this->mIsFinalPositionDirty       = true;
    this->mIsModelMatrixDirty         = true;
  }
}

void CDyTransform::pUpdateToChildPosition() const noexcept
{
  if (this->mIsWorldSumAlignedPosDirty == true)
  { // Update dependent transform information.
    this->pUpdateFinalRenderingAxis();
    this->pUpdateAxisAlignedWorldPosition();
    this->pUpdateFromParentWorldPosition();

    // Update value.
    this->mWorldSumAlignedPosition       = this->mWorldRelativeOriginPosition + this->mWorldRelativeAlignedPosition;

    // Update flag.
    this->mIsWorldSumAlignedPosDirty     = false;
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
    CDyTransform* ptrParentTransform = validActorPtr->GetParent()->GetTransform();
    if (ptrParentTransform->pfIsToChildPositionUpdated() == true)
    { // Get updated parent::mToChildPosition vector.
      this->mWorldRelativeOriginPosition = ptrParentTransform->pfGetToChildPosition();
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
    if (this->mIsWorldPrdScaleDirty == true) { this->pUpdateToChildWorldScale(); }

    this->mFinalRenderingScale  = this->mWorldProductScale * this->mLocalScale;
    this->mIsFinalScaleDirty    = false;
    this->mIsModelMatrixDirty   = true;
  }
}

void CDyTransform::pUpdateToChildWorldScale() const noexcept
{
  if (this->mIsWorldPrdScaleDirty == true)
  { // Update dependent transform information.
    this->pUpdateFromParentWorldScale();

    this->mWorldProductScale = this->mWorldScale * this->mFromParentWorldScale;

    this->mIsWorldPrdScaleDirty = false;
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
    CDyTransform* ptrParentTransform = validActorPtr->GetParent()->GetTransform();
    if (ptrParentTransform->pfIsToChildScaleUpdate() == true)
    { // Get updated parent::mToChildPosition vector.
      this->mFromParentWorldScale = ptrParentTransform->pfGetToChildScale();
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
    if (this->mIsWorldSumAlignedPosDirty == true) { this->pUpdateToChildPosition(); }
    // Update flag
    this->mIsFromChildPositionUpdated = false;
  }

  return this->mWorldSumAlignedPosition;
}

const DDyVector3& CDyTransform::pfGetToChildWorldEulerAngle() const noexcept
{
  if (this->mIsFromChildRotationUpdated == true)
  { // Update dependent information.
    if (this->mIsWorldSumRotationAngleDirty == true) { this->pUpdateToChildWorldEulerAngle(); }
    // Update flag
    this->mIsFromChildRotationUpdated = false;
  }

  return this->mWorldSumEulerAngle;
}

const DDyVector3& CDyTransform::pfGetToChildScale() const noexcept
{
  if (this->mIsFromChildScaleUpdated == true)
  { // Update dependent information.
    if (this->mIsWorldPrdScaleDirty == true) { this->pUpdateToChildWorldScale(); }
    // Update flag
    this->mIsFromChildScaleUpdated = false;
  }

  return this->mWorldProductScale;
}

} /// ::dy namespace