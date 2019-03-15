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
#include <Dy/Component/CCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Dy/Element/Actor.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Component/CDyTransform.h>

namespace dy
{

EDySuccess CCamera::Initialize(const PDyCameraComponentMetaInfo& descriptor)
{
  // Setting initial properties values.
  this->mNear                   = descriptor.mDetails.mNear;
  this->mFar                    = descriptor.mDetails.mFar;
  this->mFieldOfView            = descriptor.mDetails.mInitialFieldOfView;
  this->mIsOrthographicCamera   = descriptor.mDetails.mProjectionType == EDyCameraProjectionType::Orthographic;
  this->mViewportRectXY         = descriptor.mDetails.mViewportSize.mLeftDown;
  this->mViewportRectWH         = descriptor.mDetails.mViewportSize.mRightUp;

  const auto& settingManager    = MSetting::GetInstance();
  this->mAspect                 = static_cast<float>(settingManager.GetWindowSizeWidth()) / settingManager.GetWindowSizeHeight();
  this->mIsMustBeFocusedInstantly = descriptor.mDetails.mIsFocusInstantly;

  // @TODO FIX) IMPLEMENT THIS.
  this->mIsEnableMeshUnClipped    = false;
  this->mIsEnableMeshUnClipped    = descriptor.mDetails.mIsEnableMeshUnClipped;

  this->mIsUsing3DListener        = descriptor.mDetails.mIs3DListener;

  // Set first time flag to false to use second time flag logics.
  if (descriptor.mInitiallyActivated) { this->Activate(); }

  return DY_SUCCESS;
}

void CCamera::Release()
{
  if (this->mIsFocused == true) { this->Unfocus(); }
}

void CCamera::Update(TF32)
{
  this->pUpdateCameraVectors();

  bool isNeedToUpdateFrustum = false;
  if (this->mIsViewMatrixDirty)
  { 
    this->pUpdateViewMatrix(); 
    isNeedToUpdateFrustum = true;
  }
  if (this->mIsProjectionMatrixDirty) 
  { 
    this->pUpdateProjectionMatrix(); 
    isNeedToUpdateFrustum = true;
  }

  if (isNeedToUpdateFrustum == true)
  { // If we need to update culling frustum, update it.
    this->mFrustum.UpdateFrustum(this->GetProjectionMatrix(), this->GetViewMatrix());
  }
}

std::string CCamera::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void CCamera::pUpdateCameraVectors()
{
  // Calculate the new Front vector
  const auto transform = this->GetBindedActor()->GetTransform();

  // Also re-calculate the Right and Up vector
  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  if (const auto& lookingDirection = transform->GetToChildBasis()[2];
      this->mLookingAtDirection != lookingDirection)
  {
    this->mLookingAtDirection = lookingDirection;
    this->mIsViewMatrixDirty  = true;
  }
  if (const auto& worldPos = transform->GetFinalWorldPosition(); this->mPosition != worldPos)
  {
    this->mPosition = transform->GetFinalWorldPosition();
    this->mIsViewMatrixDirty  = true;
  }
}

void CCamera::pUpdateViewMatrix()
{
  this->mViewMatrix = glm::lookAt(
      static_cast<glm::vec3>(this->mPosition),
      static_cast<glm::vec3>(this->mPosition + this->mLookingAtDirection),
      static_cast<glm::vec3>(DVector3::UpY())
  );

  this->mIsViewMatrixDirty = false;
}

void CCamera::pUpdateProjectionMatrix()
{
  if (this->mIsOrthographicCamera)
  {
    const auto& settingManager  = MSetting::GetInstance();
    const auto  width           = settingManager.GetWindowSizeWidth();
    const auto  height          = settingManager.GetWindowSizeHeight();

    this->mProjectionMatrix = glm::ortho(-static_cast<float>(width) / 2,
                                          static_cast<float>(width) / 2,
                                         -static_cast<float>(height) / 2,
                                          static_cast<float>(height) / 2,
                                          this->mNear, this->mFar);
  }
  else 
  { 
    this->mProjectionMatrix = glm::perspective(this->mFieldOfView * math::DegToRadVal<TF32>, this->mAspect, this->mNear, this->mFar); 
  }

  this->mIsProjectionMatrixDirty = false;
}

void CCamera::TryActivateInstance()
{
  this->pUpdateCameraVectors();
  this->pUpdateViewMatrix();
  this->pUpdateProjectionMatrix();
  this->mFrustum.UpdateFrustum(this->GetProjectionMatrix(), this->GetViewMatrix());

  // If camera must be focused instantly, set it to present focused camera reference ptr of manager.
  if (this->mIsMustBeFocusedInstantly == true) { this->Focus(); }
}

void CCamera::TryDeactivateInstance()
{
  if (this->mIsFocused == true) { this->Unfocus(); }
}

const DMatrix4x4& CCamera::GetViewMatrix() const noexcept
{
  return this->mViewMatrix;
}

const DMatrix4x4& CCamera::GetProjectionMatrix() const noexcept
{
  return this->mProjectionMatrix;
}

bool CCamera::IsEnabledMeshUnclipping() const noexcept
{
  return this->mIsEnableMeshUnClipped;
}

TF32 CCamera::GetNear() const noexcept
{
  return this->mNear;
}

TF32 CCamera::GetFar() const noexcept
{
  return this->mFar;
}

TF32 CCamera::GetFieldOfView() const noexcept
{
  return this->mFieldOfView;
}

const DVector3& CCamera::GetPosition() const noexcept
{
  return this->mPosition;
}

bool CCamera::IsPointInFrustum(const DVector3& iPoint) const noexcept
{
  return this->mFrustum.IsPointInFrustum(iPoint);
}

bool CCamera::IsSphereInFrustum(const DVector3& iPoint, TF32 iRadius) const noexcept
{
  return this->mFrustum.IsSphereInFrustum(iPoint, iRadius);
}

bool CCamera::IsUsing3DListener() const noexcept
{
  return this->mIsUsing3DListener;
}

const DVector2& CCamera::GetViewportRectScaleXy() const noexcept
{
  return this->mViewportRectXY;
}

const DVector2& CCamera::GetViewportRectScaleWh() const noexcept
{
  return this->mViewportRectWH;
}

std::array<TI32, 4> CCamera::GetPixelizedViewportRectangle() const noexcept
{
  const auto& setting                 = MSetting::GetInstance();
  const auto presentResolutionWidth   = setting.GetWindowSizeWidth();
  const auto presentResolutionHeight  = setting.GetWindowSizeHeight();

  const auto& scaleXy = this->mViewportRectXY;
  const auto& scaleWh = this->mViewportRectWH;

  return {static_cast<TI32>(scaleXy.X * presentResolutionWidth),
          static_cast<TI32>(scaleXy.Y * presentResolutionHeight),
          static_cast<TI32>(scaleWh.X * presentResolutionWidth),
          static_cast<TI32>(scaleWh.Y * presentResolutionHeight)
  };
}

EDySuccess CCamera::SetNear(TF32 newNear) noexcept
{
  if (newNear > this->mFar || newNear < 0) { return DY_FAILURE; }
  else
  {
    this->mNear = newNear;
    this->mIsProjectionMatrixDirty = true;
    this->pUpdateProjectionMatrix();
    return DY_SUCCESS;
  }
}

EDySuccess CCamera::SetFar(TF32 newFar) noexcept
{
  if (newFar < this->mNear || newFar < 0) { return DY_FAILURE; }
  else
  {
    this->mFar = newFar;
    this->mIsProjectionMatrixDirty = true;
    this->pUpdateProjectionMatrix();
    return DY_SUCCESS;
  }
}

EDySuccess CCamera::SetFieldOfView(TF32 degreeValue) noexcept
{
  if (degreeValue < 0 || degreeValue > 180.f) { return DY_FAILURE; }
  else
  {
    this->mFieldOfView = degreeValue;
    this->mIsProjectionMatrixDirty = true;
    this->pUpdateProjectionMatrix();
    return DY_SUCCESS;
  }
}

void CCamera::SetViewportX(TF32 x) noexcept
{
  this->mViewportRectXY.X = x;
}

void CCamera::SetViewportY(TF32 y) noexcept
{
  this->mViewportRectXY.Y = y;
}

void CCamera::SetViewportW(TF32 w) noexcept
{
  this->mViewportRectWH.X = w;
}

void CCamera::SetViewportH(TF32 h) noexcept
{
  this->mViewportRectWH.Y = h;
}

void CCamera::SetFeatureMeshUnclipping(bool flag) noexcept
{
  this->mIsEnableMeshUnClipped = flag;
}

void CCamera::Set3DListenerActivation(bool iActivated) noexcept
{
  if (this->mIsUsing3DListener != iActivated)
  {
    this->mIsUsing3DListener = iActivated;
  }
}

EDySuccess CCamera::Focus()
{
  if (this->mIsFocused == true)
  { // FAILURE!
    DyPushLogWarning("{}::{} | From FDyActor : {} | This camera is already binded to main camera.",
                    "CCamera", "Focus()", this->GetBindedActor()->GetActorName());
    return DY_FAILURE;
  }

  this->mActivatedUpdateListId = MWorld::GetInstance().pfEnrollActiveCamera(*this);
  this->mIsFocused = true;

  return DY_SUCCESS;
}

EDySuccess CCamera::Unfocus()
{
  if (this->mIsFocused == false)
  { // FAILURE!
    DyPushLogWarning("{}::{} | From FDyActor : {} | This camera is not binded to main camera.",
                    "CCamera", "Unfocus()", this->GetBindedActor()->GetActorName());
    return DY_FAILURE;
  }

  MWorld::GetInstance().pfUnenrollActiveCamera(this->mActivatedUpdateListId);
  this->mIsFocused = false;
  return DY_SUCCESS;
}

} /// ::dy namespace