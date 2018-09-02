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

#include <Dy/Management/SceneManager.h>

namespace dy
{

CDyCamera::CDyCamera(const PDyCameraConstructionDescriptor& descriptor)
{
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
  // Bind or unbind controller to main system (from main system).
  if (descriptor.mIsMoveable)
  {

  }
  else if (this->mIsMoveable && !this->mIsFirstTime)
  {

  }

  // Make lookat view.
  this->mViewMatrix = glm::lookAt(
      static_cast<glm::vec3>(mPosition),
      static_cast<glm::vec3>(mLookingAt),
      static_cast<glm::vec3>(DVector3::UpY())
  );

  // Make projection view.
  float fov = descriptor.mInitialFieldOfView;
  if (fov < 0.1f)   fov = 0.1f;
  if (fov > 180.f)  fov = 180.f;

  float aspect = static_cast<float>(kScreenWidth) / kScreenHeight;
  if (descriptor.mUseCustomViewport)
  {
    const auto& customViewportSize = descriptor.mViewportSize;
    aspect = customViewportSize.X / customViewportSize.Y;
  }

  if (descriptor.mIsOrthographic)
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
    this->mProjectionMatrix     = glm::perspective(fov, aspect, this->mNear, this->mFar);
    this->mIsOrthographicCamera = false;
  }

  //
  this->mIsEnableMeshUnClipped  = descriptor.mIsEnableMeshUnClipped;

  // If camera must be focused instantly, set it to present focused camera reference ptr of manager.
  if (descriptor.mIsFocusInstantly)
  {
    auto& sceneManager = MDyScene::GetInstance();
    sceneManager.__pfBindFocusCamera(this);
  }

  // Set first time flag to false to use second time flag logics.
  this->mIsFirstTime = false;
  return DY_SUCCESS;
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

}

} /// ::dy namespace