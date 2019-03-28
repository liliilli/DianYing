#ifndef GUARD_DY_COMPONENT_CDYCAMERA_H
#define GUARD_DY_COMPONENT_CDYCAMERA_H
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

#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Component/Internal/Camera/FFrustumTester.h>

namespace dy
{

/// @enum   ECameraType
/// @brief  Camera projection type. \n
/// r = tan(\theta / 2) * d
enum class ECameraType
{
  /// Do projection matrix with perspective.         \n
  /// .- 2n/(r-l)    0     -(r+1)/(r-l)     0     -. \n
  /// |     0     2n/(t-b) -(t+b)/(t-b)     0      | \n
  /// |     0        0      (f+n)/(f-n) -2fn/(f-n) | \n
  /// `-    0        0           1          0     -` \n
  Perspective,
  /// Do projection matrix with orthographic.        \n
  /// .- 2/(r-l)     0           0    -(r+l)/(r-l)-. \n
  /// |     0      2/(t-b)       0    -(t+b)/(t-b) | \n
  /// |     0        0       -2/(f-n) -(f+n)/(f-n) | \n
  /// `-    0        0           0          1     -`
  Orthographic,
};

/// @class CCamera
/// @brief New camera component type.
class CCamera final : public AGeneralBaseComponent, public IInitializeHelper<PDyCameraComponentMetaInfo>
{
public:
  CCamera(FActor& actorReference) : AGeneralBaseComponent(actorReference) {};
  virtual ~CCamera() = default;

  CCamera(const CCamera&)                          = delete;
  CCamera& operator=(const CCamera&)               = delete;
  CCamera(CCamera&& instance)            noexcept  = default;
  CCamera& operator=(CCamera&& instance) noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CCamera);
  MDY_SET_CRC32_HASH_WITH_TYPE(CCamera);

  /// @brief  Get view matrix.
  /// @return Valid immutable view matrix reference.
  MDY_NODISCARD const DMatrix4x4& GetViewMatrix() const noexcept;

  /// @brief  Get projection matrix
  /// @return Valid immutable projection matirx reference.
  MDY_NODISCARD const DMatrix4x4& GetProjectionMatrix() const noexcept;

  /// @brief  Get feature flag about mesh unclipping of this camera component.
  MDY_NODISCARD bool IsEnabledMeshUnclipping() const noexcept;

  /// @brief  Get near value of camera.
  MDY_NODISCARD TF32 GetNear() const noexcept;

  /// @brief  Get far value of camera.
  MDY_NODISCARD TF32 GetFar() const noexcept;

  /// @brief  Get field of horizontal view degree angle value.
  MDY_NODISCARD TF32 GetFieldOfView() const noexcept;

  /// @brief  Get camera's world position.
  MDY_NODISCARD const DVec3& GetPosition() const noexcept;

  /// @brief Check point is in camera frustum.
  MDY_NODISCARD bool IsPointInFrustum(const DVec3& iPoint) const noexcept;

  /// @brief Check virtual sphere that is consisted of point and radius, is in camera frustum.
  MDY_NODISCARD bool IsSphereInFrustum(const DVec3& iPoint, TF32 iRadius) const noexcept;

  /// @brief Check this camera is using 3d listener.
  MDY_NODISCARD bool IsUsing3DListener() const noexcept;

  /// @brief  Get scale value of xy (start point) of viewport rectangle.
  /// @return Scaled start point of viewport rectangle.
  MDY_NODISCARD const DVec2& GetViewportRectScaleXy() const noexcept;

  /// @brief  Get scale value of wh (start point) of viewport rectangle.
  /// @return Scaled width and height of viewport rectangle.
  MDY_NODISCARD const DVec2& GetViewportRectScaleWh() const noexcept;

  /// @brief  Get (x, y, w, h) pixel value of viewport rectangle along with present resolution size.
  /// @return Pixelized (reflect present game resolution size) (x, y, w, h) integer container for representing viewport rectangle.
  MDY_NODISCARD std::array<TI32, 4> GetPixelizedViewportRectangle() const noexcept;

  /// @brief  Set near value. If successful, return DY_SUCCESS.
  /// But when newNear value is bigger than mFar value, do nothing just return DY_FAILURE.
  /// When update near value, projection matrix will be updated also immediately.
  /// This will cause performance down.
  ///
  /// @param  newNear New value to update near value.
  /// @return Success flag.
  MDY_NODISCARD EDySuccess SetNear(TF32 newNear) noexcept;

  /// @brief  Set far value. If successful, return DY_SUCCESS.
  /// But when newFar svalue is smaller than mNear value, do nothing just return DY_FAILURE.
  /// When update near value, projection matrix will be updated also immediately.
  /// This will cause performance down.
  ///
  /// @param  newFar New value to update far value.
  /// @return Success flag.
  MDY_NODISCARD EDySuccess SetFar(TF32 newFar) noexcept;

  /// @brief  Set field of view value. If successful, return DY_SUCCESS.
  /// When degreeValue is smaller or equal to 0.f, or bigger than 180.f, do nothing and just return DY_FAILURE.
  /// When update field of view value, projection matrix will be updated also immediately.
  /// This will cause performance down.
  ///
  /// @param  degreeValue New value to update field of view value.
  /// @return Success flag.
  MDY_NODISCARD EDySuccess SetFieldOfView(TF32 degreeValue) noexcept;

  /// @brief  Get unscaled viewport value x.
  void SetViewportX(TF32 x) noexcept;

  /// @brief  Get unscaled viewport value y.
  void SetViewportY(TF32 y) noexcept;

  /// @brief  Get unscaled viewport value w.
  void SetViewportW(TF32 w) noexcept;

  /// @brief  Get unscaled viewport value h.
  void SetViewportH(TF32 h) noexcept;

  /// @brief  Set mesh unclipping feature of this camera component instance.
  void SetFeatureMeshUnclipping(bool flag) noexcept;

  /// @brief Set 3D Listener activation.
  void Set3DListenerActivation(bool iActivated) noexcept;

  /// @brief  Do focusing camera as main camera.
  /// @return If camera component is not binded main cam before Focus(), return true or false.
  EDySuccess Focus();

  /// @brief  remove focusing camera as main camera.
  /// @return If camera component is being binded as main cam before Focus(), return true or false.
  EDySuccess Unfocus();

  /// @brief  Initilaize component property.
  /// @param  descriptor Descriptor which has a values.
  /// @return If succeeded, return true or false.
  MDY_NODISCARD EDySuccess Initialize(const PDyCameraComponentMetaInfo& descriptor) override final;

  /// @brief  Release component safely.
  void Release() override final;

  /// @brief  Update camera properties, like updating view matrix following final position of FActor.
  void Update(TF32) override final;

  /// @brief  Return information string of this component.
  /// @return Information string.
  /// @TODO IMPLEMENT THIS, NOT IMPLEMENTED YET.
  MDY_NODISCARD std::string ToString() override final;

private:
  /// Try activate camera operation.
  /// If parent is activate and itself activated, update all properties newly.
  void TryActivateInstance() override final;

  /// Try deactivate camera operation.
  /// If either parent or itself is deactivated, disconnect it with all outside system.
  void TryDeactivateInstance() override final;

  /// @brief Update camera vectors
  void pUpdateCameraVectors();

  /// @brief Update view matrix.
  void pUpdateViewMatrix();

  /// @brief Update projection matrix.
  void pUpdateProjectionMatrix();

  /// View matrix (In OpenGL, final result must be transposed because of matrix must be column-major as ragard to specification.)
  /// .- rx ry rz  0 -..-  0  0  0 -x -.    .- rx ry rz -r*t -.T r(x, y, z) is camera's x-axis normal vector.
  /// |  ux uy uz  0  ||   0  0  0 -y  | == |  ux uy uz -u*t  |  v(x, y, z) is camera's z-axis normal vector.
  /// |  vx vy vz  0  ||   0  0  0 -z  | == |  vx vy vz -v*t  |  u(x, y, z) is camera's y-axis normal vector.
  /// '-  0  0  0  0 -``-  0  0  0  1 -`    `-  0  0  0    1 -`  We need to translate all object's in view frustum as camera's origin.
  DMatrix4x4      mViewMatrix;
  /// Projection matrix.
  DMatrix4x4      mProjectionMatrix;
  /// Normalized Lookat direction vector
  DVec3        mLookingAtDirection = {};
  /// Camera final posittion
  DVec3        mPosition = {};
  /// Frustum varaible.
  FFrustumTester  mFrustum  = {};

  //! (Camera) -> |Near| >>>>>>>>>>>>>>>|Far|
  //! Ground --------------------------------

  /// Camera's near value.
  float mNear                     = 0.1f;
  /// Camera's far value.
  float mFar                      = 1000;
  /// Camera's field of view (vertical)
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
  DVec2  mViewportRectXY         = {0, 0};
  /// Viewport rect W, H
  DVec2  mViewportRectWH         = {1, 1};

  /// Check flag if mesh is unclipped even though vertex position is out of bound.
  bool        mIsEnableMeshUnClipped  = false;
  /// Boolean flag whether or not camera projection is orthographic or perspective.
  bool        mIsOrthographicCamera   = false;
  /// Check flag whether or not this CCamera instance is on focused.
  bool        mIsFocused              = false;
  ///
  MDY_TRANSIENT bool mIsMustBeFocusedInstantly  = false;
  MDY_TRANSIENT bool mIsViewMatrixDirty         = true;
  MDY_TRANSIENT bool mIsProjectionMatrixDirty   = true;

  /// @brief Using 3D Listener.
  bool mIsUsing3DListener = false;

#ifdef false
  float mMouseSensitivity         = 0.25f;
  float mSpeed                    = 10.f;
#endif
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYCAMERA_H