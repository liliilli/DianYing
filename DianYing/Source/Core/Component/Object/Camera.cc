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
#include <Dy/Core/Component/Object/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/ImmutableSetting.h>

#include <Dy/Management/InputManager.h>
#include <Dy/Management/SceneManager.h>
#include <Phitos/Dbg/assert.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

CDyCamera::CDyCamera(const PDyCameraConstructionDescriptor& descriptor)
{
  this->mLookingAtDirection = dy::DVector3{0, 0, -1};
  MDY_CALL_ASSERT_SUCCESS(this->UpdateSetting(descriptor));
}

CDyCamera::~CDyCamera()
{
  // Unbind focus camera if this is being binded to camera focus of scene.
  auto& sceneManager = MDyScene::GetInstance();
  if (this->IsBeingFocused())
  {
    sceneManager.__pfUnbindCameraFocus();
  }
}

EDySuccess CDyCamera::UpdateSetting(const PDyCameraConstructionDescriptor& descriptor)
{
  this->mIsMoveable = descriptor.mIsMoveable;

  // Make lookat view.
  this->pUpdateViewMatrix();

  // Make projection view.
  const float fov = descriptor.mInitialFieldOfView;
  this->SetFieldOfView(fov);

  float aspect = static_cast<float>(kScreenWidth) / kScreenHeight;
  if (descriptor.mUseCustomViewport)
  {
    const auto& customViewportSize = descriptor.mViewportSize;
    aspect = customViewportSize.X / customViewportSize.Y;
  }
  this->SetAspect(aspect);
  this->pUpdateProjectionMatrix();

  this->mIsEnableMeshUnClipped  = descriptor.mIsEnableMeshUnClipped;

  // If camera must be focused instantly, set it to present focused camera reference ptr of manager.
  if (descriptor.mIsFocusInstantly)
  {
    auto& sceneManager = MDyScene::GetInstance();
    sceneManager.__pfBindFocusCamera(this);
    this->mIsFocused = true;
  }

  this->pUpdateCameraVectors();

  // Set first time flag to false to use second time flag logics.
  this->mIsFirstTime = false;
  return DY_SUCCESS;
}

const DDyMatrix4x4& CDyCamera::GetViewMatrix() noexcept
{
  if (this->mIsViewMatrixDirty)
  {
    this->pUpdateViewMatrix();
  }

  return this->mViewMatrix;
}

const DDyMatrix4x4& CDyCamera::GetProjectionMatrix() noexcept
{
  if (this->mIsPerspectiveMatrixDirty)
  {
    this->pUpdateProjectionMatrix();
  }

  return this->mProjectionMatrix;
}

void CDyCamera::SetFieldOfView(float newFov) noexcept
{
  if (newFov < 0.1f)   newFov = 0.1f;
  if (newFov > 180.f)  newFov = 180.f;
  this->mFieldOfView = newFov;
  this->mIsPerspectiveMatrixDirty = true;
}

void CDyCamera::SetAspect(float newAspect) noexcept
{
  PHITOS_ASSERT(newAspect > 0.f, "Aspect value must bigger than 0.0f");

  this->mAspect = newAspect;
  this->mIsPerspectiveMatrixDirty = true;
}

bool CDyCamera::IsOrthographicCamera() const noexcept
{
  return this->mIsOrthographicCamera;
}

bool CDyCamera::IsBeingFocused() const noexcept
{
  auto& sceneManager = MDyScene::GetInstance();
  return sceneManager.GetCamera() == this;
}

bool CDyCamera::IsMoveable() const noexcept
{
  return this->mIsMoveable;
}

void CDyCamera::Update(float dt)
{
  if (this->mIsMoveable)
  {
    auto& input = MDyInput::GetInstance();

    const auto xVal = input.GetKeyValue("XAxis");
    const auto yVal = input.GetKeyValue("YAxis");

    if (yVal != 0.0f || xVal != 0.0f)
    {
      MDY_LOG_DEBUG_D("Moved {} , {}", xVal, yVal);

      this->mPosition += this->mLookingAtRightDirection * xVal * dt * mSpeed;
      this->mPosition += this->mLookingAtDirection * yVal * dt * mSpeed;
      this->mIsViewMatrixDirty = true;
    }

    if (input.IsMouseMoved())
    {
      const auto& present = input.GetPresentMousePosition();
      const auto& last = input.GetPresentLastPosition();
      const auto offset = DVector2(present.X - last.X, last.Y - present.Y);
      this->pProcessMouseMovement(offset, true);
    }
  }
}

void CDyCamera::pProcessMouseMovement(const DVector2& offset, bool constrainPitch)
{
  this->mRotationEulerAngle.Y += offset.X * this->mMouseSensitivity;
  this->mRotationEulerAngle.X += offset.Y * this->mMouseSensitivity;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (this->mRotationEulerAngle.X > 89.0f)
      this->mRotationEulerAngle.X = 89.0f;
    if (this->mRotationEulerAngle.X < -89.0f)
      this->mRotationEulerAngle.X = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Euler angles
  MDY_LOG_DEBUG_D("Euler angle | X : {}, Y : {}, Z : {}", this->mRotationEulerAngle.X, this->mRotationEulerAngle.Y, this->mRotationEulerAngle.Z);
  this->pUpdateCameraVectors();
}

void CDyCamera::pUpdateCameraVectors()
{
  // Calculate the new Front vector
  DVector3 front;
  front.X = cos(glm::radians(this->mRotationEulerAngle.Y)) * cos(glm::radians(this->mRotationEulerAngle.X));
  front.Y = sin(glm::radians(this->mRotationEulerAngle.X));
  front.Z = sin(glm::radians(this->mRotationEulerAngle.Y)) * cos(glm::radians(this->mRotationEulerAngle.X));
  this->mLookingAtDirection = front.Normalize();

  // Also re-calculate the Right and Up vector
  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  this->mLookingAtRightDirection  = DVector3::Cross(front, DVector3::UpY()).Normalize();
  this->mLookingAtUpDirection     = DVector3::Cross(this->mLookingAtRightDirection, this->mLookingAtDirection).Normalize();
  this->mIsViewMatrixDirty        = true;
}

void CDyCamera::pUpdateViewMatrix()
{
  this->mViewMatrix = glm::lookAt(
      static_cast<glm::vec3>(mPosition),
      static_cast<glm::vec3>(mPosition + mLookingAtDirection),
      static_cast<glm::vec3>(DVector3::UpY())
  );
  this->mIsViewMatrixDirty = false;
}

void CDyCamera::pUpdateProjectionMatrix()
{
  if (this->mIsOrthographicCamera)
  {
    this->mProjectionMatrix = glm::ortho(-static_cast<float>(kScreenWidth) / 2,
                                          static_cast<float>(kScreenWidth) / 2,
                                         -static_cast<float>(kScreenHeight) / 2,
                                          static_cast<float>(kScreenHeight) / 2,
                                          this->mNear, this->mFar);
    this->mIsOrthographicCamera = true;
  }
  else
  {
    this->mProjectionMatrix     = glm::perspective(this->mFieldOfView, this->mAspect, this->mNear, this->mFar);
    this->mIsOrthographicCamera = false;
  }
  this->mIsPerspectiveMatrixDirty = false;
}

} /// ::dy namespace