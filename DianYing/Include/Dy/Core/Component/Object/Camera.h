#ifndef GUARD_DY_CORE_COMPONENT_OBJECT_CAMERA_H
#define GUARD_DY_CORE_COMPONENT_OBJECT_CAMERA_H
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

#include <Dy/Helper/enum_flags.h>
#include <Dy/Helper/Type/Matrix4.h>

namespace dy
{

///
/// @struct
///
struct PDyCameraConstructionDescriptor
{
  /// Fov must be range from 0.1f to 180.f
  float mInitialFieldOfView   = 60.f;
  /// Custom viewport size, but mUseCustomViewport must be enabled.
  DVector2 mViewportSize      = {};

  /// Just use main viewport as a size.
  bool mUseCustomViewport     = false;
  /// Camera will project objects as orthographic without vanishing point.
  bool mIsOrthographic        = false;
  /// Clipped camera will be unclipped.
  bool mIsEnableMeshUnClipped = false;
  /// Camera will be focused instantly.
  bool mIsFocusInstantly      = false;
  /// Whether camera can be moveable using key input signal.
  bool mIsMoveable            = false;
};

///
/// @class CDyCamera
/// @brief
///
class CDyCamera
{
public:
  CDyCamera(const PDyCameraConstructionDescriptor& descriptor);
  CDyCamera(const CDyCamera&)             = delete;
  CDyCamera& operator=(const CDyCamera&)  = delete;
  CDyCamera(CDyCamera&&)                  = default;
  CDyCamera& operator=(CDyCamera&&)       = default;
  ~CDyCamera();

  ///
  /// @brief
  ///
  EDySuccess UpdateSetting(const PDyCameraConstructionDescriptor& descriptor);

  ///
  /// @brief
  ///
  bool IsOrthographicCamera() const noexcept;

  ///
  /// @brief
  ///
  bool IsBeingFocused() const noexcept;

  ///
  /// @brief
  ///
  bool IsMoveable() const noexcept;

  ///
  /// @brief
  ///
  void Update(float dt);

private:
  DDyMatrix4x4  mViewMatrix;
  DDyMatrix4x4  mProjectionMatrix;

  DVector3      mPosition         = {};
  DVector3      mLookingAt        = {};

  //! (Camera) -> |Near| >>>>>>>>>>>>>>>|Far|
  //! Ground --------------------------------

  /// Camera's near value.
  float mNear = 0;
  /// Camera's far value.
  float mFar  = 100;

  bool mIsMoveable                = false;
  bool mIsEnableMeshUnClipped     = false;
  bool mIsOrthographicCamera      = false;
  ///
  bool mIsFirstTime = true;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_OBJECT_CAMERA_H