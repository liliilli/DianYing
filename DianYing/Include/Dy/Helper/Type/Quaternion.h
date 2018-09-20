#ifndef GUARD_DY_TYPE_QUATERNION_H
#define GUARD_DY_TYPE_QUATERNION_H
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

#include <glm/gtc/quaternion.hpp>
#include <assimp/quaternion.h>
#include <Dy/Helper/Type/MathEnum.h>
#include <Dy/Helper/Type/Matrix4.h>

//!
//! Implementation
//!

namespace dy {

///
/// @class DDyQuaternion
/// @brief Quaternion class
///
class DDyQuaternion final {
public:
  DDyQuaternion() = default;
  DDyQuaternion(const DDyQuaternion& quaternion)            = default;
  DDyQuaternion& operator=(const DDyQuaternion& quaternion) = default;
  DDyQuaternion(const DDyVector3& eulerAngleXYZ);

  DDyQuaternion(const aiQuaternion& aiQuaternion);
  DDyQuaternion& operator=(const aiQuaternion& auQuaternion);

  DDyQuaternion(const glm::quat& glmQuat);
  DDyQuaternion& operator=(const glm::quat& glmQuat);

  ///
  /// @brief Get rotation matrix (4x4) from quaternion.
  ///
  DDyMatrix4x4 GetRotationMatrix4x4() const noexcept;

  ///
  /// @brief Get rotation matrix (3x3) from quaternion.
  ///
  DDyMatrix3x3 GetRotationMatrix3x3() const noexcept;

  ///
  /// @brief Return euler rotation angle (degrees) (x, y, z).
  /// Caution! Quaternion to euler angle does not guarante precise degree euler angle.
  /// If you want to get degree angle values, you must to write code for saving angle.
  ///
  DDyVector3 GetEulerRotationDegreeAngleVector() const noexcept;

  ///
  /// @brief Return euler rotation angle (radians) (x, y, z).
  /// Caution! Quaternion to euler angle does not guarante precise radian euler angle.
  /// If you want to get radian angle values, you must to write code for saving angle.
  ///
  DDyVector3 GetEulerRotationRadiansAngleVector() const noexcept;

  ///
  /// @brief Impulse rotation with eulerAngle vector (degrees).
  ///
  void AddRotationAngle(const DDyVector3& eulerAngle);

  ///
  /// @brief Impulse rotation with quaternion.
  ///
  void AddRotationAngle(const DDyQuaternion& quaternion);

  ///
  /// @brief Impulse rotation with axis and degree angle value.
  ///
  void AddRotationAngle(EDyAxis3D axis, float degreeEulerAngle);

  ///
  /// @brief Impulse rotation with (x, y, z) degree angle value.
  ///
  void SetRotationAngle(const DDyVector3& degreeEulerAngle);

  ///
  const glm::quat& pGetQuaternion() const noexcept;

private:
  glm::quat mQuaternion;
};

} /// ::opgs16 namespace

#endif /// GUARD_DY_TYPE_QUATERNION_H
