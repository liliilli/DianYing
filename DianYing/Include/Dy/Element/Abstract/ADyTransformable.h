#ifndef GUARD_DY_ABSTRACT_TRANFORMABLE_H
#define GUARD_DY_ABSTRACT_TRANFORMABLE_H
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

#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/Type/MathEnum.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Helper/Type/Quaternion.h>

namespace dy
{

///
/// @class ADyTransformable
/// @brief Add transformability.
///
/// ADyTransformable 타입은 전파가 가능해야 한다.
///
class ADyTransformable
{
public:
  virtual ~ADyTransformable() = default;

  //!
  //! Getter
  //!

  ///
  /// @brief Return local position.
  /// @return Object's local position.
  ///
  [[nodiscard]] const DDyVector3& GetLocalSpacePosition() const noexcept;

	///
	/// @brief Return world position.
	/// @return Object's world position from parent object's position.
	///
  [[nodiscard]] const DDyVector3& GetWorldSpacePosition() const noexcept;

  ///
  /// @brief Get Object's final position.
  ///
  [[nodiscard]] const DDyVector3& GetFinalWorldSpacePosition() const noexcept;

  ///
  /// @brief Return object final position not included local position.
  ///
  [[nodiscard]] const DDyVector3& GetParentWorldSpacePosition() const noexcept;

  ///
  /// @brief The method gets local rotation angle value. (euler degree value)
  /// @return Object's rotation angle value.
  ///
  [[nodiscard]] float GetLocalEulerAngle(EDyAxis3D direction) const noexcept;

  ///
  /// @brief The method gets world rotation angle value. (euler degree value)
  ///
  [[nodiscard]] float GetWorldEulerAngle(EDyAxis3D direction) const noexcept;

  ///
  /// @brief The method gets final rendering angle value. (euler degree value)
  ///
  [[nodiscard]] float GetFinalEulerAngle(EDyAxis3D direction) const noexcept;

  ///
  /// @brief The method gets (x, y, z) DDyVector3 scaling axis factor.
  /// @return Object's scaling vector which has (x, y, z) axis factors.
  ///
  [[nodiscard]] const DDyVector3& GetLocalScale() const noexcept;

  ///
  /// @brief Returns world scale which able to affect child object's transform.
  ///
  [[nodiscard]] const DDyVector3& GetWorldScale() const noexcept;

  //!
  //! Setter
  //!

  ///
  /// @brief Sets local position.
  /// @param[in] localPosition local position to set on.
  ///
  void SetLocalSpacePosition(const DDyVector3& localPosition) noexcept;

	///
	/// @brief Set world position.
	/// @param[in] worldPosition Winal position in Screen space and from parent' object.
	///
	void SetWorldSpacePosition(const DDyVector3& worldPosition) noexcept;

  ///
  /// @brief The method set final position but keeping parent position
  /// but adjusting FDyPawn's world position.
  ///
  void SetWorldPositionFromFinalPosition(const DDyVector3& finalPosition) noexcept;

  ///
  /// @brief Add offset value with axis as local position.
  ///
  void AddOffsetLocalSpacePosition(EDyAxis3D axis, float value) noexcept;

  ///
  /// @brief Add offset value with axis as world position.
  ///
  void AddOffsetWorldSpacePosition(EDyAxis3D axis, float value) noexcept;

  ///
  /// @brief The method adds local rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  ///
  /// @param[in] eulerAngleValue Euler angle value to set on.
  ///
  void SetLocalEulerAngle(const DDyVector3& eulerAngleValue) noexcept;

  ///
  /// @brief The method adds world rotation angle values.
  /// When input value is positive, axis rotates clockwise.
  /// input value is negative, axis rotates counter-clockwise.
  ///
  /// @param[in] eulerAngleValue Euler angle value to set on.
  ///
  void SetWorldEulerAngle(const DDyVector3& eulerAngleValue) noexcept;

  ///
  /// @brief Add offset eulerAngleValue with axis as local rotation angle.
  ///
  void AddLocalEulerAngle(EDyAxis3D axis, float eulerAngleValue) noexcept;

  ///
  /// @brief Add offset eulerAngleValue with axis as world rotation angle.
  ///
  void AddWorldEulerAngle(EDyAxis3D axis, float eulerAngleValue) noexcept;

  ///
  /// @brief The method sets scaling vector have (x, y, z) scaling factors.
  ///
  void SetLocalScale(const DDyVector3& xyz_value) noexcept;

  ///
  /// @brief Sets world scaling vector value which have (x, y, z).
  ///
  void SetWorldScale(const DDyVector3& xyz_value) noexcept;

  ///
  /// @brief The method returns Model matrix, M = TRS
  ///
  /// If any value which related to Translate, Rotation, Scaling has been changed,
  /// model matrix would be calculated newly and returned.
  ///
  /// @return Model matrix (M = TRS)
  ///
  [[nodiscard]] const DDyMatrix4x4& GetTransform() const noexcept;

private:

  //!
  //! Variables
  //!

  DDyVector3 mLocalPosition = {};
  DDyVector3 mWorldPosition = {};
  DDyVector3 m_propagated_world_basis_position;

  /// m_object_space_axis * local_position;
  mutable DDyVector3 m_local_axis_arranged_position;
  /// m_object_space_axis * world_position;
  mutable DDyVector3 m_world_axis_arranged_position;
  /// (x, y, z) parent position to bring child. m_propagated_world_basis_position + m_world_axis_arranged_position.
  mutable DDyVector3 m_summed_world_position;
  /// (x, y, z) final position in hierarchy. m_summed_world_position + m_local_axis_arranged_position
  mutable DDyVector3 m_axis_aligned_final_position;

  DDyVector3 mLocalEulerAngle;
  DDyVector3 mWorldEulerAngle;
  /// parent's propagated world rotation summed angle;
  DDyVector3 m_propagated_world_rotation_euler_angle;
  /// propagated_world_rotation_euler_angle + world
  mutable DDyVector3 m_summed_world_rotation_euler_angle;
  /// local + summed_world
  mutable DDyVector3 m_object_final_rotation_euler_angle;

  mutable DDyQuaternion m_local_rotation_quaternion;
  mutable glm::mat3     m_local_rotation_matrix;

  mutable DDyQuaternion m_propagated_rotation_quaternion;
  mutable glm::mat3     m_propagated_rotation_matrix;

  mutable DDyQuaternion m_summed_rotation_quaternion;

  /// Scale local factor, default is (1, 1, 1)
  DDyVector3 mLocalScale = DDyVector3{ 1.f };
  /// Scale world factor. default is (1, 1, 1)
  DDyVector3 mWorldScale = DDyVector3{ 1.f };
  /// Scale factor from parent.
  mutable DDyVector3 m_propagated_producted_scale = DDyVector3{ 1.f };
  /// mWorldScale * m_propagated_producted_scale
  mutable DDyVector3 m_summed_producted_scale     = DDyVector3{ 1.f };
  /// m_summed_producted_scale * mLocalScale
  mutable DDyVector3 m_final_producted_scale      = DDyVector3{ 1.f };

  /// World + Parent rotation matrix.
  mutable glm::mat3 m_summed_rotation_matrix;
  /// Final model matrix also reflected by parent's and world rot.
  MDY_TRANSIENT DDyMatrix4x4 mFinalTransform;

};

} /// ::dy namespace

#endif /// GUARD_DY_ABSTRACT_TRANFORMABLE_H
