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
#include <Dy/Component/CDyCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/WorldManager.h>

namespace dy
{

void CDyCamera::Activate() noexcept
{
  ADyGeneralBaseComponent::Activate();

  // FunctionBody ∨
  MDY_NOTUSED auto _ = pTryActivateCameraOperation();
}

void CDyCamera::Deactivate() noexcept
{
  ADyGeneralBaseComponent::Deactivate();

  // FunctionBody ∨
  MDY_NOTUSED auto _ = pTryDeactivateCameraOperation();
}

void CDyCamera::pPropagateParentActorActivation(_MIN_ const DDy3StateBool& actorBool) noexcept
{
  ADyGeneralBaseComponent::pPropagateParentActorActivation(actorBool);

  // FunctionBody ∨
  { MDY_NOTUSED auto _ = pTryActivateCameraOperation();   }
  { MDY_NOTUSED auto _ = pTryDeactivateCameraOperation(); }
}

EDySuccess CDyCamera::Initialize(_MIN_ const DDyCameraMetaInformation& descriptor)
{
  // Setting initial properties values.
  this->mNear                   = descriptor.mNear;
  this->mFar                    = descriptor.mFar;
  this->mFieldOfView            = descriptor.mInitialFieldOfView;
  this->mIsOrthographicCamera   = descriptor.mIsOrthographic;
  this->mViewportRectXY         = descriptor.mViewportSizeXY;
  this->mViewportRectWH         = descriptor.mViewportSizeWH;

  const auto& settingManager    = MDySetting::GetInstance();
  this->mAspect                 = static_cast<float>(settingManager.GetWindowSizeWidth()) / settingManager.GetWindowSizeHeight();
  this->mIsMustBeFocusedInstantly = descriptor.mIsFocusInstantly;

  // @TODO FIX) IMPLEMENT THIS.
  this->mIsEnableMeshUnClipped    = false;
  this->mIsEnableMeshUnClipped    = descriptor.mIsEnableMeshUnClipped;

  // Set first time flag to false to use second time flag logics.
  if (descriptor.mInitiallyActivated) { this->Activate(); }
  this->mIsFirstTime = false;

  return DY_SUCCESS;
}

void CDyCamera::Release()
{
  if (this->mIsFocused == true) { this->Unfocus(); }
}

void CDyCamera::Update(_MIN_ float dt)
{

}

std::string CDyCamera::ToString()
{
  PHITOS_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITILAIZE_EMPTYSTR;
}

void CDyCamera::pUpdateCameraVectors()
{
  // Calculate the new Front vector
  DDyVector3 front;
  front.X = cos(glm::radians(this->mRotationEulerAngle.Y)) * cos(glm::radians(this->mRotationEulerAngle.X));
  front.Y = sin(glm::radians(this->mRotationEulerAngle.X));
  front.Z = sin(glm::radians(this->mRotationEulerAngle.Y)) * cos(glm::radians(this->mRotationEulerAngle.X));

  // Also re-calculate the Right and Up vector
  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  this->mLookingAtDirection       = front.Normalize();
  this->mLookingAtRightDirection  = DDyVector3::Cross(front, DDyVector3::UpY()).Normalize();
  this->mLookingAtUpDirection     = DDyVector3::Cross(this->mLookingAtRightDirection, this->mLookingAtDirection).Normalize();

  this->mIsViewMatrixDirty        = true;
  this->pUpdateViewMatrix();
}

void CDyCamera::pUpdateViewMatrix()
{
  this->mViewMatrix = glm::lookAt(
      static_cast<glm::vec3>(this->mPosition),
      static_cast<glm::vec3>(this->mPosition + this->mLookingAtDirection),
      static_cast<glm::vec3>(DDyVector3::UpY())
  );

  this->mIsViewMatrixDirty = false;
}

void CDyCamera::pUpdateProjectionMatrix()
{
  if (this->mIsOrthographicCamera)
  {
    const auto& settingManager  = MDySetting::GetInstance();
    const auto  width           = settingManager.GetWindowSizeWidth();
    const auto  height          = settingManager.GetWindowSizeHeight();

    this->mProjectionMatrix = glm::ortho(-static_cast<float>(width) / 2,
                                          static_cast<float>(width) / 2,
                                         -static_cast<float>(height) / 2,
                                          static_cast<float>(height) / 2,
                                          this->mNear, this->mFar);
  }
  else { this->mProjectionMatrix = glm::perspective(this->mFieldOfView, this->mAspect, this->mNear, this->mFar); }

  this->mIsProjectionMatrixDirty = false;
}

EDySuccess CDyCamera::pTryActivateCameraOperation()
{
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  if (this->mActivateFlag.GetOutput() == false)             { return DY_FAILURE; }

  this->pUpdateCameraVectors();
  this->pUpdateViewMatrix();
  this->pUpdateProjectionMatrix();

  // If camera must be focused instantly, set it to present focused camera reference ptr of manager.
  if (this->mIsMustBeFocusedInstantly == true) { this->Focus(); }

  return DY_SUCCESS;
}

EDySuccess CDyCamera::pTryDeactivateCameraOperation()
{
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  if (this->mActivateFlag.GetOutput() == true)             { return DY_FAILURE; }

  if (this->mIsFocused == true) { this->Unfocus(); }
  return DY_SUCCESS;
}

std::array<TI32, 4> CDyCamera::GetPixelizedViewportRectangle() const noexcept
{
  const auto& setting                 = MDySetting::GetInstance();
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

EDySuccess CDyCamera::SetNear(_MIN_ const float newNear) noexcept
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

EDySuccess CDyCamera::SetFar(_MIN_ const float newFar) noexcept
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

EDySuccess CDyCamera::SetFieldOfView(const float degreeValue) noexcept
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

void CDyCamera::SetViewportX(const float x) noexcept
{
  this->mViewportRectXY.X = x;
}

void CDyCamera::SetViewportY(const float y) noexcept
{
  this->mViewportRectXY.Y = y;
}

void CDyCamera::SetViewportW(const float w) noexcept
{
  this->mViewportRectWH.X = w;
}

void CDyCamera::SetViewportH(const float h) noexcept
{
  this->mViewportRectWH.Y = h;
}

EDySuccess CDyCamera::Focus()
{
  if (this->mIsFocused == true)
  { // FAILURE!
    MDY_LOG_WARNING("{}::{} | From FDyActor : {} | This camera is already binded to main camera.",
                    "CDyCamera", "Focus()", this->GetBindedActor()->GetActorName());
    return DY_FAILURE;
  }

  this->mActivatedUpdateListId = MDyWorld::GetInstance().pfEnrollActiveCamera(*this);
  this->mIsFocused = true;

  return DY_SUCCESS;
}

EDySuccess CDyCamera::Unfocus()
{
  if (this->mIsFocused == false)
  { // FAILURE!
    MDY_LOG_WARNING("{}::{} | From FDyActor : {} | This camera is not binded to main camera.",
                    "CDyCamera", "Unfocus()", this->GetBindedActor()->GetActorName());
    return DY_FAILURE;
  }

  MDyWorld::GetInstance().pfUnenrollActiveCamera(this->mActivatedUpdateListId);
  this->mIsFocused = false;
  return DY_SUCCESS;
}

} /// ::dy namespace