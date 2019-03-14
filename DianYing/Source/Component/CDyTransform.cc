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

EDySuccess CDyTransform::Initialize(const DDyTransform& desc)
{
  this->SetRelativeLocalPosition(desc.mLocalRelPosition);
  this->SetLocalEulerAngle(desc.mLocalEulerAngle);
  this->SetLocalScale(desc.mLocalScale);

  this->SetRelativeWorldPosition(desc.mWorldRelPosition);
  this->SetWorldEulerAngle(desc.mWorldEulerAngle);
  this->SetWorldScale(desc.mWorldScale);
  return DY_SUCCESS;
}

void CDyTransform::Release()
{

}

//!
//! Getter
//!

const DVector3& CDyTransform::GetRelativeLocalPosition() const noexcept
{
  return this->mLocalRelPosition;
}

const DVector3& CDyTransform::GetRelativeWorldPosition() const noexcept
{
  return this->mWorldRelPosition;
}

const DVector3& CDyTransform::GetFinalWorldPosition() noexcept
{
  this->MDY_PRIVATE(TryUpdateFinalPosition)();
  return this->mFinalRenderingPosition;
}

const std::array<DVector3, 3>& CDyTransform::GetToChildBasis() noexcept
{
  this->MDY_PRIVATE(TryUpdateToChildBasis)();
  return this->mToChildBasis;
}

const DVector3& CDyTransform::GetRelativeLocalEulerAngle() const noexcept
{
  return this->mLocalEulerAngle;
}

float CDyTransform::GetRelativeLocalEulerAngle(_MIN_ EAxis3D direction) const noexcept
{
  const auto& rlea = this->GetRelativeLocalEulerAngle();
  switch (direction)
  {
  case EAxis3D::X: return rlea.X;
  case EAxis3D::Y: return rlea.Y;
  case EAxis3D::Z: return rlea.Z;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(0.0f);
  }
}

const DVector3& CDyTransform::GetRelativeWorldEulerAngle() const noexcept
{
  return this->mWorldEulerAngle;
}

float CDyTransform::GetRelativeWorldEulerAngle(_MIN_ EAxis3D direction) const noexcept
{
  const auto& rwea = this->GetRelativeWorldEulerAngle();
  switch (direction)
  {
  case EAxis3D::X: return rwea.X;
  case EAxis3D::Y: return rwea.Y;
  case EAxis3D::Z: return rwea.Z;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(0.0f);
  }
}

float CDyTransform::GetFinalEulerAngle(_MIN_ EAxis3D direction) noexcept
{
  this->MDY_PRIVATE(TryUpdateFinalWorldRotAngle)();

  switch (direction)
  {
  case EAxis3D::X: return this->mFinalRenderingEulerAngle.X;
  case EAxis3D::Y: return this->mFinalRenderingEulerAngle.Y;
  case EAxis3D::Z: return this->mFinalRenderingEulerAngle.Z;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(0.0f);
  }
}

const DVector3& CDyTransform::GetLocalScale() const noexcept
{
  return this->mLocalScale;
}

const DVector3& CDyTransform::GetWorldScale() const noexcept
{
  return this->mWorldScale;
}

const DVector3& CDyTransform::GetFinalScale() noexcept
{
  this->MDY_PRIVATE(UpdateFinalRenderingScale)();
  return this->mFinalRenderingScale;
}

//!
//! Setter
//!

void CDyTransform::SetRelativeLocalPosition(_MIN_ const DVector3& localPosition) noexcept
{
  this->mLocalRelPosition         = localPosition;
  this->mIsLocalRelAlignedPosDirty= true;
  this->mIsFinalPositionDirty     = true;
  this->mIsModelMatrixDirty       = true;
}

void CDyTransform::SetRelativeLocalPositionWithFinalWorldPosition(const DVector3& finalPosition)
{
  // Position.
  this->mLocalRelAlignedPosition    = finalPosition - this->mWorldRelAlignedPosition;
  this->mLocalRelPosition           = this->mPresentPositionBasis.Inverse().MultiplyVector(this->mLocalRelAlignedPosition);
  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;
}

void CDyTransform::SetRelativeWorldPosition(_MIN_ const DVector3& worldPosition) noexcept
{
  this->mWorldRelPosition         = worldPosition;
  this->mIsWorldPositionAlignDirty= true;
  this->mIsWorldRelAlignedPosDirty= true;
  this->mIsWorldSumAlignedPosDirty= true;
  this->mIsFinalPositionDirty     = true;
  this->mIsModelMatrixDirty       = true;

  auto* validActorPtr = this->GetBindedActor();
  if (validActorPtr->HasChildrenActor() == true)
  { 
    // (1) Try update movement basis.
    this->MDY_PRIVATE(TryUpdateMovementBasis)();
    // (2) Try update relative aligend world position.
    this->MDY_PRIVATE(TryUpdateWorldRelAlignedPos)();
    // (3) Try update final position without relative local position (aka fwpos wo rlpos)
    this->MDY_PRIVATE(TryUpdateAlignedWorldSumPosition)();

    // (4) Try update `fwpos wo rlpos` into children's transform.
    auto& childrenContainer = validActorPtr->GetChildrenContainer();
    for (auto& [specifier, ptrsmtObject] : childrenContainer)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
      ptrsmtObject->GetTransform()->MDY_PRIVATE(SetRelativeWorldOrigin(this->mWorldSumAlignedPosition));
    }
  }
}

void CDyTransform::AddRelativeLocalPosition(_MIN_ const DVector3& localPosiiton) noexcept
{
  this->SetRelativeWorldPosition(this->GetRelativeLocalPosition() + localPosiiton);
}

void CDyTransform::AddRelativeLocalPosition(_MIN_ EAxis3D axis, _MIN_ TF32 value) noexcept
{
  DVector3 offset = {};
  switch (axis)
  {
  case EAxis3D::X: offset.X += value; break;
  case EAxis3D::Y: offset.Y += value; break;
  case EAxis3D::Z: offset.Z += value; break;
  }

  this->SetRelativeWorldPosition(this->GetRelativeLocalPosition() + offset);
}

void CDyTransform::AddRelativeWorldPosition(_MIN_ const DVector3& localPosiiton) noexcept
{
  this->SetRelativeWorldPosition(this->GetRelativeWorldPosition() + localPosiiton);
}

void CDyTransform::AddRelativeWorldPosition(_MIN_ EAxis3D axis, _MIN_ TF32 value) noexcept
{
  DVector3 offset = {};
  switch (axis)
  {
  case EAxis3D::X: offset.X += value; break;
  case EAxis3D::Y: offset.Y += value; break;
  case EAxis3D::Z: offset.Z += value; break;
  }

  this->SetRelativeWorldPosition(this->GetRelativeWorldPosition() + offset);
}

void CDyTransform::SetLocalEulerAngle(_MIN_ const DVector3& eulerAngleValue) noexcept
{
  this->AddLocalEulerAngle(eulerAngleValue - this->GetRelativeLocalEulerAngle());
}

void CDyTransform::SetLocalEulerAngleWithQuaternion(_MIN_ const DQuaternion& iQuat)
{
  const auto angle = math::ConvertQuaternionToRadianEuler(iQuat) * math::RadToDegVal<TF32>;

  // Set local.
  this->mLocalEulerAngle = angle - this->mWorldSumEulerAngle;
  this->mIsFinalRotationAngleDirty = true;
  this->mToChildBasisAxisDirty     = true;
  this->mIsModelMatrixDirty        = true;
}

void CDyTransform::SetWorldEulerAngle(_MIN_ const DVector3& eulerAngleValue) noexcept
{
  this->AddWorldEulerAngle(eulerAngleValue - this->GetRelativeWorldEulerAngle());
}

void CDyTransform::AddLocalEulerAngle(_MIN_ const DVector3& eulerAngleValue) noexcept
{
  this->mLocalEulerAngle += eulerAngleValue;
  this->mIsFinalRotationAngleDirty = true;
  this->mToChildBasisAxisDirty     = true;
  this->mIsModelMatrixDirty        = true;
}

void CDyTransform::AddWorldEulerAngle(_MIN_ const DVector3& eulerAngleValue) noexcept
{
  this->mWorldEulerAngle += eulerAngleValue;
  this->mIsWorldSumRotationAngleDirty = true;
  this->mToChildBasisAxisDirty        = true;
  this->mIsFinalRotationAngleDirty    = true;
  this->mIsModelMatrixDirty           = true;

  auto* validActorPtr = this->GetBindedActor();
  if (validActorPtr->HasChildrenActor() == true)
  { 
    // (1) Try update sum of world (without each local) rotation angle.
    this->MDY_PRIVATE(TryUpdateWorldSumRotAngle)();

    // (2) Try update `mFromParentWorldEulerAngle` into children's transform.
    auto& childrenContainer = validActorPtr->GetChildrenContainer();
    for (auto& [specifier, ptrsmtObject] : childrenContainer)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
      ptrsmtObject->GetTransform()->MDY_PRIVATE(SetRotationAngleOrigin(this->mWorldSumEulerAngle));
    }
  }
}

void CDyTransform::AddLocalEulerAngle(_MIN_ EAxis3D axis, _MIN_ float eulerAngleValue) noexcept
{
  DVector3 eulerAngle = {};
  switch (axis)
  {
  case EAxis3D::X: eulerAngle.X = eulerAngleValue; break;
  case EAxis3D::Y: eulerAngle.Y = eulerAngleValue; break;
  case EAxis3D::Z: eulerAngle.Z = eulerAngleValue; break;
  default: break;
  }
 
  this->AddLocalEulerAngle(eulerAngle);
}

void CDyTransform::AddWorldEulerAngle(_MIN_ EAxis3D axis, _MIN_ float eulerAngleValue) noexcept
{
  DVector3 eulerAngle = {};
  switch (axis)
  {
  case EAxis3D::X: eulerAngle.X = eulerAngleValue; break;
  case EAxis3D::Y: eulerAngle.Y = eulerAngleValue; break;
  case EAxis3D::Z: eulerAngle.Z = eulerAngleValue; break;
  default: break;
  }

  this->AddWorldEulerAngle(eulerAngle);
}

void CDyTransform::SetLocalScale(_MIN_ const DVector3& xyz_value) noexcept
{
  this->mLocalScale         = xyz_value;
  this->mIsFinalScaleDirty  = true;
  this->mIsModelMatrixDirty = true;
}

void CDyTransform::SetWorldScale(_MIN_ const DVector3& xyz_value) noexcept
{
  this->mWorldScale           = xyz_value;
  this->mIsWorldPrdScaleDirty = true;
  this->mIsFinalScaleDirty    = true;
  this->mIsModelMatrixDirty   = true;

  if (auto* validActorPtr = this->GetBindedActor(); validActorPtr->HasChildrenActor() == true)
  { 
    // (1) Try update product of world (without each local) scale.
    this->MDY_PRIVATE(TryUpdateWorldPrdScale)();

    // (2) Try update `mFromParentWorldScale` into children's transform.
    auto& childrenContainer = validActorPtr->GetChildrenContainer();
    for (auto& [specifier, ptrsmtObject] : childrenContainer)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
      ptrsmtObject->GetTransform()->MDY_PRIVATE(SetWorldScaleOrigin(this->mWorldProductScale));
    }
  }
}

const DMatrix4x4 CDyTransform::GetRotationMatrix() noexcept
{
  MDY_NOUSE_RTVAL_EXPR(this->GetTransform());
  return this->mFinalRenderRotationQuaternion.GetRotationMatrix4x4();
}

const DQuaternion& CDyTransform::GetRotationQuaternion() noexcept
{
  MDY_NOUSE_RTVAL_EXPR(this->GetTransform());
  return this->mFinalRenderRotationQuaternion;
}

std::string CDyTransform::ToString()
{
  return MDY_INITIALIZE_EMPTYSTR;
}

const DMatrix4x4& CDyTransform::GetTransform() noexcept
{
  if (this->mIsModelMatrixDirty == true)
  { // Update dependent transform information.
    if (this->mIsFinalScaleDirty == true) 
    { this->MDY_PRIVATE(UpdateFinalRenderingScale)(); }
    if (this->mIsFinalRotationAngleDirty == true) 
    { this->MDY_PRIVATE(TryUpdateFinalWorldRotAngle)(); }
    if (this->mIsFinalPositionDirty == true) 
    { this->MDY_PRIVATE(TryUpdateFinalPosition)();   }

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

void CDyTransform::TryPropagateTransformToChildren()
{
  MDY_CALL_BUT_NOUSE_RESULT(this->GetTransform());

  if (auto* validActorPtr = this->GetBindedActor(); validActorPtr->HasChildrenActor() == true)
  { 
    // (1) Try update movement basis.
    this->MDY_PRIVATE(TryUpdateMovementBasis)();
    // (2) Try update relative aligend world position.
    this->MDY_PRIVATE(TryUpdateWorldRelAlignedPos)();
    // (3) Try update final position without relative local position (aka fwpos wo rlpos)
    this->MDY_PRIVATE(TryUpdateAlignedWorldSumPosition)();
    // (4) Try update sum of world (without each local) rotation angle.
    this->MDY_PRIVATE(TryUpdateWorldSumRotAngle)();
    // (5) Try update sum of world (without each local) rotation angle.
    this->MDY_PRIVATE(TryUpdateWorldPrdScale)();

    auto& childrenContainer = validActorPtr->GetChildrenContainer();
    for (auto& [specifier, ptrsmtObject] : childrenContainer)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
      ptrsmtObject->GetTransform()->MDY_PRIVATE(PropagateTransform)(
          this->mWorldSumAlignedPosition,
          this->mWorldSumEulerAngle,
          this->mWorldProductScale);
    }
  }
}

void CDyTransform::MDY_PRIVATE(PropagateTransform)(
    _MIN_ const DVector3& iPosition,
    _MIN_ const DVector3& iAngle,
    _MIN_ const DVector3& iScale)
{
  this->mFromParentWorldEulerAngle = iAngle;
  this->mWorldRelativeOriginPosition = iPosition;
  this->mFromParentWorldScale = iScale;

  {
    this->mIsWorldPrdScaleDirty = true;
    this->mIsFinalScaleDirty    = true;
    this->mIsModelMatrixDirty   = true;
    this->mIsWorldSumAlignedPosDirty  = true;
    this->mIsFinalPositionDirty       = true;
    this->mIsModelMatrixDirty         = true;
    this->mIsWorldSumRotationAngleDirty = true;
    this->mIsMovementBasisDirty      = true;
    this->mIsLocalRelAlignedPosDirty = true;
    this->mIsWorldPositionAlignDirty = true;
    this->mIsWorldRelAlignedPosDirty = true;
    this->mIsWorldSumAlignedPosDirty = true;
    this->mIsFinalPositionDirty      = true;
    this->mToChildBasisAxisDirty     = true;
    this->mIsFinalRotationAngleDirty = true;
    this->mIsModelMatrixDirty        = true;
  }

  if (auto* validActorPtr = this->GetBindedActor(); validActorPtr->HasChildrenActor() == true)
  { 
    // (1) Try update movement basis.
    this->MDY_PRIVATE(TryUpdateMovementBasis)();
    // (2) Try update relative aligend world position.
    this->MDY_PRIVATE(TryUpdateWorldRelAlignedPos)();
    // (3) Try update final position without relative local position (aka fwpos wo rlpos)
    this->MDY_PRIVATE(TryUpdateAlignedWorldSumPosition)();
    // (4) Try update sum of world (without each local) rotation angle.
    this->MDY_PRIVATE(TryUpdateWorldSumRotAngle)();
    // (5) Try update sum of world (without each local) rotation angle.
    this->MDY_PRIVATE(TryUpdateWorldPrdScale)();

    auto& childrenContainer = validActorPtr->GetChildrenContainer();
    for (auto& [specifier, ptrsmtObject] : childrenContainer)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
      ptrsmtObject->GetTransform()->MDY_PRIVATE(PropagateTransform)(
          this->mWorldSumAlignedPosition,
          this->mWorldSumEulerAngle,
          this->mWorldProductScale);
    }
  }
}

void CDyTransform::MDY_PRIVATE(SetPxTransform)(_MIN_ const physx::PxTransform& iTransform)
{
  this->SetRelativeLocalPositionWithFinalWorldPosition(iTransform.p);

  // Set local.
  const auto angle = 
      math::ConvertQuaternionToRadianEuler(iTransform.q.w, iTransform.q.x, iTransform.q.y, iTransform.q.z) 
    * math::RadToDegVal<TF32>;

  this->mLocalEulerAngle = angle - this->mWorldSumEulerAngle;
  this->mIsFinalRotationAngleDirty = true;
  this->mToChildBasisAxisDirty     = true;
  this->mIsModelMatrixDirty        = true;
}

void CDyTransform::MDY_PRIVATE(TryUpdateMovementBasis)()
{
  if (this->mIsMovementBasisDirty == false) { return; }

  // Update position movement basis of this FDyActor.
  this->mPresentPositionBasis = DQuaternion(this->mFromParentWorldEulerAngle).GetRotationMatrix3x3();
  const auto& m = this->mPresentPositionBasis;

  // Update display movement basis.
  this->mDisplayMovementBasis[0] = DVector3{m[0][0], m[0][1], m[0][2]};
  this->mDisplayMovementBasis[1] = DVector3{m[1][0], m[1][1], m[1][2]};
  this->mDisplayMovementBasis[2] = DVector3{m[2][0], m[2][1], m[2][2]};
  
  this->mIsMovementBasisDirty       = false;
  this->mIsWorldRelAlignedPosDirty  = true;
  this->mIsWorldSumAlignedPosDirty  = true;
  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;
}

void CDyTransform::MDY_PRIVATE(TryUpdateLocalRelAlignedPos)()
{
  if (this->mIsLocalRelAlignedPosDirty == false)  { return; }
  if (this->mIsMovementBasisDirty == true)        { this->MDY_PRIVATE(TryUpdateMovementBasis)(); }

  this->mLocalRelAlignedPosition    = this->mPresentPositionBasis.MultiplyVector(this->mLocalRelPosition);
  this->mIsLocalRelAlignedPosDirty  = false;
  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;
}

void CDyTransform::MDY_PRIVATE(TryUpdateWorldRelAlignedPos)()
{
  if (this->mIsWorldRelAlignedPosDirty == false)  { return; }
  if (this->mIsMovementBasisDirty == true)        { this->MDY_PRIVATE(TryUpdateMovementBasis)(); }

  this->mWorldRelAlignedPosition = this->mPresentPositionBasis.MultiplyVector(this->mWorldRelPosition);
  this->mIsWorldRelAlignedPosDirty = false;
  this->mIsWorldSumAlignedPosDirty = true;
  this->mIsFinalPositionDirty      = true;
  this->mIsModelMatrixDirty        = true;
}

void CDyTransform::MDY_PRIVATE(TryUpdateAlignedWorldSumPosition)()
{
  if (this->mIsWorldSumAlignedPosDirty == false)  { return; }
  if (this->mIsWorldRelAlignedPosDirty == true)   { this->MDY_PRIVATE(TryUpdateWorldRelAlignedPos)(); }

  // Update value.
  this->mWorldSumAlignedPosition    = this->mWorldRelativeOriginPosition + this->mWorldRelAlignedPosition;
  this->mIsWorldSumAlignedPosDirty  = false;
  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;
}

void CDyTransform::MDY_PRIVATE(TryUpdateFinalPosition)()
{
  if (this->mIsFinalPositionDirty == false)     { return; }
  if (this->mIsLocalRelAlignedPosDirty == true) { this->MDY_PRIVATE(TryUpdateLocalRelAlignedPos)(); }
  if (this->mIsWorldSumAlignedPosDirty == true) { this->MDY_PRIVATE(TryUpdateAlignedWorldSumPosition)(); }

  this->mFinalRenderingPosition = this->mWorldSumAlignedPosition + this->mLocalRelAlignedPosition;
  this->mIsFinalPositionDirty   = false;
  this->mIsModelMatrixDirty     = true;
}

void CDyTransform::MDY_PRIVATE(SetRelativeWorldOrigin)(_MIN_ const DVector3& iParentPosition)
{
  this->mWorldRelativeOriginPosition = iParentPosition;

  this->mIsWorldSumAlignedPosDirty  = true;
  this->mIsFinalPositionDirty       = true;
  this->mIsModelMatrixDirty         = true;

  // (3) Try update final position without relative local position (aka fwpos wo rlpos)
  this->MDY_PRIVATE(TryUpdateAlignedWorldSumPosition)();

  // (4) Try update `fwpos wo rlpos` into children's transform.
  auto& childrenContainer = this->GetBindedActor()->GetChildrenContainer();
  for (auto& [specifier, ptrsmtObject] : childrenContainer)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
    ptrsmtObject->GetTransform()->MDY_PRIVATE(SetRelativeWorldOrigin(this->mWorldSumAlignedPosition));
  }
}

void CDyTransform::MDY_PRIVATE(TryUpdateWorldSumRotAngle)()
{
  if (this->mIsWorldSumRotationAngleDirty == false) { return; }

  this->mWorldSumEulerAngle = this->mFromParentWorldEulerAngle + this->mWorldEulerAngle;
  this->mIsWorldSumRotationAngleDirty = false;
  this->mIsFinalRotationAngleDirty    = true;
  this->mToChildBasisAxisDirty        = true;
  this->mIsModelMatrixDirty           = true;
}

void CDyTransform::MDY_PRIVATE(TryUpdateFinalWorldRotAngle)() noexcept
{
  if (this->mIsFinalRotationAngleDirty == false)   { return; }
  if (this->mIsWorldSumRotationAngleDirty == true) { this->MDY_PRIVATE(TryUpdateWorldSumRotAngle)(); }

  // Set final rendering angles, and... update quaternion to setup new model matrix.
  this->mFinalRenderingEulerAngle  = this->mWorldSumEulerAngle + this->mLocalEulerAngle;
  this->mFinalRenderRotationQuaternion.SetRotationAngle(this->mFinalRenderingEulerAngle);

  this->mIsFinalRotationAngleDirty = false;
  this->mToChildBasisAxisDirty     = true;
  this->mIsModelMatrixDirty        = true;
}

void CDyTransform::MDY_PRIVATE(TryUpdateToChildBasis)() noexcept
{
  if (this->mToChildBasisAxisDirty == false)    { return; }
  if (this->mIsFinalRotationAngleDirty == true) { this->MDY_PRIVATE(TryUpdateFinalWorldRotAngle)(); }

  const auto m = DQuaternion{this->mFinalRenderingEulerAngle}.GetRotationMatrix3x3();
  this->mToChildBasis[0] = DVector3{m[0][0], m[0][1], m[0][2]};
  this->mToChildBasis[1] = DVector3{m[1][0], m[1][1], m[1][2]};
  this->mToChildBasis[2] = DVector3{m[2][0], m[2][1], m[2][2]};
  this->mToChildBasisAxisDirty = false;
}

void CDyTransform::MDY_PRIVATE(SetRotationAngleOrigin)(_MIN_ const DVector3& iParentSumRotAngle)
{
  this->mFromParentWorldEulerAngle = iParentSumRotAngle;

  this->mIsWorldSumRotationAngleDirty = true;
  this->mIsMovementBasisDirty      = true;
  this->mIsLocalRelAlignedPosDirty = true;
  this->mIsWorldPositionAlignDirty = true;
  this->mIsWorldRelAlignedPosDirty = true;
  this->mIsWorldSumAlignedPosDirty = true;
  this->mIsFinalPositionDirty      = true;
  this->mToChildBasisAxisDirty     = true;
  this->mIsFinalRotationAngleDirty = true;
  this->mIsModelMatrixDirty        = true;

  // (1) Try update final position without relative local position (aka fwpos wo rlpos)
  this->MDY_PRIVATE(TryUpdateWorldSumRotAngle)();

  // (2) Try update children's transform recursively.
  auto& childrenContainer = this->GetBindedActor()->GetChildrenContainer();
  for (auto& [specifier, ptrsmtObject] : childrenContainer)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
    ptrsmtObject->GetTransform()->MDY_PRIVATE(SetRotationAngleOrigin)(this->mWorldSumAlignedPosition);
  }
}

void CDyTransform::MDY_PRIVATE(TryUpdateWorldPrdScale)()
{
  if (this->mIsWorldPrdScaleDirty == false) { return; }
  
  this->mWorldProductScale    = this->mFromParentWorldScale * this->mWorldScale;
  this->mIsWorldPrdScaleDirty = false;
  this->mIsFinalScaleDirty    = true;
  this->mIsModelMatrixDirty   = true;
}

void CDyTransform::MDY_PRIVATE(UpdateFinalRenderingScale)() noexcept
{
  if (this->mIsFinalScaleDirty == false)   { return; }
  if (this->mIsWorldPrdScaleDirty == true) { this->MDY_PRIVATE(TryUpdateWorldPrdScale)(); }

  this->mFinalRenderingScale  = this->mWorldProductScale * this->mLocalScale;
  this->mIsFinalScaleDirty    = false;
  this->mIsModelMatrixDirty   = true;
}

void CDyTransform::MDY_PRIVATE(SetWorldScaleOrigin)(_MIN_ const DVector3& iParentPrdScale)
{
  this->mFromParentWorldScale = iParentPrdScale;
  this->mIsWorldPrdScaleDirty = true;
  this->mIsFinalScaleDirty    = true;
  this->mIsModelMatrixDirty   = true;

  // (1) Try update scale.
  this->MDY_PRIVATE(TryUpdateWorldPrdScale)();

  // (2) Try update children's transform recursively.
  auto& childrenContainer = this->GetBindedActor()->GetChildrenContainer();
  for (auto& [specifier, ptrsmtObject] : childrenContainer)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtObject)) { continue; }
    ptrsmtObject->GetTransform()->MDY_PRIVATE(SetWorldScaleOrigin)(this->mWorldProductScale);
  }
}

#ifdef false
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

//!
//! Axis lazy evaluation
//!

void CDyTransform::pUpdateFinalRenderingAxis() const noexcept
{
  if (this->mIsMovementBasisDirty == true)
  { //
    const auto quat = DQuaternion(this->mFromParentWorldEulerAngle);
    this->mPresentPositionBasis = quat.GetRotationMatrix3x3();

    // Update position movement basis of this FDyActor.
    const auto& m                   = this->mPresentPositionBasis;
    this->mDisplayMovementBasis[0] = DVector3{m[0][0], m[0][1], m[0][2]};
    this->mDisplayMovementBasis[1] = DVector3{m[1][0], m[1][1], m[1][2]};
    this->mDisplayMovementBasis[2] = DVector3{m[2][0], m[2][1], m[2][2]};

    this->mIsMovementBasisDirty         = false;
    this->mIsFinalPositionDirty     = true;
    this->mIsWorldSumAlignedPosDirty   = true;
  }
}

void CDyTransform::pUpdateToChildBasisAxis() const noexcept
{
  if (this->mToChildBasisAxisDirty == true)
  { //
    const auto m = DQuaternion{this->mFinalRenderingEulerAngle}.GetRotationMatrix3x3();

    //
    this->mToChildBasis[0] = DVector3{m[0][0], m[0][1], m[0][2]};
    this->mToChildBasis[1] = DVector3{m[1][0], m[1][1], m[1][2]};
    this->mToChildBasis[2] = DVector3{m[2][0], m[2][1], m[2][2]};

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
    if (this->mIsLocalRelAlignedPosDirty == true) { this->pUpdateAxisAlignedLocalPosition(); }
    if (this->mIsWorldPositionAlignDirty == true) { this->pUpdateAxisAlignedWorldPosition(); }
    if (this->mIsWorldSumAlignedPosDirty == true)    { this->pUpdateToChildPosition(); }

    // Update value.
    this->mFinalRenderingPosition = this->mWorldSumAlignedPosition + this->mLocalRelAlignedPosition;

    // Update flag.
    this->mIsFinalPositionDirty   = false;
    this->mIsModelMatrixDirty     = true;
  }
}

void CDyTransform::pUpdateAxisAlignedLocalPosition() const noexcept
{
  if (this->mIsLocalRelAlignedPosDirty == true)
  { // Update dependent transform information.
    if (this->mIsMovementBasisDirty == true) { this->pUpdateFinalRenderingAxis(); }

    // Update value.
    this->mLocalRelAlignedPosition = this->mPresentPositionBasis.MultiplyVector(this->mLocalRelPosition);

    // Update flag.
    this->mIsLocalRelAlignedPosDirty  = false;
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
    this->mWorldRelAlignedPosition = this->mPresentPositionBasis.MultiplyVector(this->mWorldRelPosition);

    // Update flag.
    this->mIsWorldPositionAlignDirty  = false;
    this->mIsFinalPositionDirty       = true;
    this->mIsModelMatrixDirty         = true;
  }
}
#endif

} /// ::dy namespace