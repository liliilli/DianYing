#ifndef GUARD_DY_COMPONENT_CDYCAMERA_H
#define GUARD_DY_COMPONENT_CDYCAMERA_H
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
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Component/Descriptor/ComponentMetaDescriptor.h>
#include <Dy/Helper/Type/Matrix4.h>

namespace dy
{

///
/// @enum   EDyCameraType
/// @brief  Camera projection type. \n
/// r = tan(\theta / 2) * d
///
enum class EDyCameraType
{
  /// Do projection matrix with perspective.         \n
  /// .- 2n/(r-l)    0     -(r+1)/(r-l)     0     -. \n
  /// |     0     2n/(t-b) -(t+b)/(t-b)     0      | \n
  /// |     0        0      (f+n)/(f-n) -2fn/(f-n) | \n
  /// `-    0        0           1          0     -` \n
  Perspective,
  /// Do projection matrix with orthographic.        \n
  ///
  Orthographic,
};

///
/// @class CDyCamera
/// @brief New camera component type.
///
class CDyCamera final : public ADyGeneralBaseComponent, public IDyInitializeHelper<DDyCameraMetaInformation>
{
public:
  CDyCamera(_MIN_ FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference) {};
  virtual ~CDyCamera() = default;

  CDyCamera(const CDyCamera&)                                 = delete;
  CDyCamera& operator=(const CDyCamera&)                      = delete;
  CDyCamera(CDyCamera&& instance)                   noexcept  = default;
  CDyCamera& operator=(CDyCamera&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyCamera);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyCamera);

  ///
  /// @brief  Get view matrix.
  /// @return Valid immutable view matrix reference.
  ///
  MDY_NODISCARD const DDyMatrix4x4& GetViewMatrix() const noexcept
  {
    return this->mViewMatrix;
  }

  ///
  /// @brief  Get projection matrix
  /// @return Valid immutable projection matirx reference.
  ///
  MDY_NODISCARD const DDyMatrix4x4& GetProjectionMatrix() const noexcept
  {
    return this->mProjectionMatrix;
  }

  ///
  /// @brief  Get near value of camera.
  /// @return Near value.
  ///
  MDY_NODISCARD float GetNear() const noexcept
  {
    return this->mNear;
  }

  ///
  /// @brief  Get far value of camera.
  /// @return Far value.
  ///
  MDY_NODISCARD float GetFar() const noexcept
  {
    return this->mFar;
  }

  ///
  /// @brief  Get field of view degree angle value.
  /// @return Field of view degree angle value. (horizontal)
  ///
  MDY_NODISCARD float GetFieldOfView() const noexcept
  {
    return this->mFieldOfView;
  }

  ///
  /// @brief  Set near value. If successful, return DY_SUCCESS.
  /// But when newNear value is bigger than mFar value, do nothing just return DY_FAILURE.
  /// When update near value, projection matrix will be updated also immediately.
  /// This will cause performance down.
  ///
  /// @param  newNear New value to update near value.
  /// @return Success flag.
  ///
  MDY_NODISCARD EDySuccess SetNear(_MIN_ const float newNear) noexcept;

  ///
  /// @brief  Set far value. If successful, return DY_SUCCESS.
  /// But when newFar svalue is smaller than mNear value, do nothing just return DY_FAILURE.
  /// When update near value, projection matrix will be updated also immediately.
  /// This will cause performance down.
  ///
  /// @param  newFar New value to update far value.
  /// @return Success flag.
  ///
  MDY_NODISCARD EDySuccess SetFar(_MIN_ const float newFar) noexcept;

  ///
  /// @brief  Set field of view value. If successful, return DY_SUCCESS.
  /// When degreeValue is smaller or equal to 0.f, or bigger than 180.f, do nothing and just return DY_FAILURE.
  /// When update field of view value, projection matrix will be updated also immediately.
  /// This will cause performance down.
  ///
  /// @param  degreeValue New value to update field of view value.
  /// @return Success flag.
  ///
  MDY_NODISCARD EDySuccess SetFieldOfView(_MIN_ const float degreeValue) noexcept;

  ///
  /// @brief
  /// @param  x
  ///
  void SetViewportX(_MIN_ const float x) noexcept;

  ///
  /// @brief
  /// @param  y
  ///
  void SetViewportY(_MIN_ const float y) noexcept;

  ///
  /// @brief
  /// @param  w
  ///
  void SetViewportW(_MIN_ const float w) noexcept;

  ///
  /// @brief
  /// @param  h
  ///
  void SetViewportH(_MIN_ const float h) noexcept;

  ///
  /// @brief  Do focusing camera as main camera.
  /// @return If camera component is not binded main cam before Focus(), return true or false.
  ///
  EDySuccess Focus();

  ///
  /// @brief  remove focusing camera as main camera.
  /// @return If camera component is being binded as main cam before Focus(), return true or false.
  ///
  EDySuccess Unfocus();

  /// @brief Activate CDyCamera. Final activation value is also dependent on FDyActor activation flag.
  void Activate() noexcept override final;

  /// @brief Deactivate CDyCamera. Final activation value is also dependent on FDyActor activation flag.
  void Deactivate() noexcept override final;

  ///
  /// @brief
  /// @param  actorBool
  ///
  void pPropagateParentActorActivation(_MIN_ const DDy3StateBool& actorBool) noexcept override final;

  ///
  /// @brief
  /// @param  descriptor
  /// @return
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const DDyCameraMetaInformation& descriptor) override final;

  ///
  /// @brief
  ///
  void Release() override final;

  ///
  /// @brief
  /// @param dt
  ///
  void Update(_MIN_ float dt) override final;

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD std::string ToString() override final;

private:
  /// @brief Update camera vectors
  void pUpdateCameraVectors();

  /// @brief Update view matrix.
  void pUpdateViewMatrix();

  /// @brief Update projection matrix.
  void pUpdateProjectionMatrix();

  /// Try activate camera operation.
  /// If parent is activate and itself activated, update all properties newly.
  MDY_NODISCARD EDySuccess pTryActivateCameraOperation();

  /// Try deactivate camera operation.
  /// If either parent or itself is deactivated, disconnect it with all outside system.
  MDY_NODISCARD EDySuccess pTryDeactivateCameraOperation();

  /// View matrix (In OpenGL, final result must be transposed because of matrix must be column-major as ragard to specification.)
  /// .- rx ry rz  0 -..-  0  0  0 -x -.    .- rx ry rz -r*t -.T r(x, y, z) is camera's x-axis normal vector.
  /// |  ux uy uz  0  ||   0  0  0 -y  | == |  ux uy uz -u*t  |  v(x, y, z) is camera's z-axis normal vector.
  /// |  vx vy vz  0  ||   0  0  0 -z  | == |  vx vy vz -v*t  |  u(x, y, z) is camera's y-axis normal vector.
  /// '-  0  0  0  0 -``-  0  0  0  1 -`    `-  0  0  0    1 -`  We need to translate all object's in view frustum as camera's origin.
  DDyMatrix4x4    mViewMatrix;
  ///
  DDyMatrix4x4    mProjectionMatrix;

  ///
  DDyVector3      mRotationEulerAngle       = {};
  ///
  DDyVector3      mLookingAtDirection       = {};
  ///
  DDyVector3      mLookingAtRightDirection  = {};
  ///
  DDyVector3      mLookingAtUpDirection     = {};

  //! (Camera) -> |Near| >>>>>>>>>>>>>>>|Far|
  //! Ground --------------------------------

  /// Camera's near value.
  float mNear                     = 0.1f;
  /// Camera's far value.
  float mFar                      = 1000;
  /// Camera's field of view (horizontal)
  float mFieldOfView              = 0.f;
  /// Camera's aspect (from screen)
  MDY_TRANSIENT float mAspect     = 0.f;

  //!
  //! Viewport Rect    (W, H)
  //! -------------------+-- (1, 1)
  //! |     |////////////| |
  //! |     |////////////| |
  //! |     +------------- |
  //! |   (X, Y)           |
  //! ----------------------
  //! (0, 0)
  //!

  /// Viewport rect X, Y
  DDyVector2  mViewportRectXY         = {0, 0};
  /// Viewport rect W, H
  DDyVector2  mViewportRectWH         = {1, 1};

  /// Check flag if mesh is unclipped even though vertex position is out of bound.
  bool        mIsEnableMeshUnClipped  = false;
  /// Boolean flag whether or not camera projection is orthographic or perspective.
  bool        mIsOrthographicCamera   = false;
  /// Check flag whether or not this CDyCamera instance is on focused.
  bool        mIsFocused              = false;
  ///
  MDY_TRANSIENT bool mIsMustBeFocusedInstantly  = false;
  MDY_TRANSIENT bool mIsViewMatrixDirty         = true;
  MDY_TRANSIENT bool mIsProjectionMatrixDirty   = true;

#ifdef false
  float mMouseSensitivity         = 0.25f;
  float mSpeed                    = 10.f;
#endif
  ///
  MDY_TRANSIENT bool mIsFirstTime = true;

  // @TODO TEMPORAL
  DDyVector3 mPosition = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYCAMERA_H