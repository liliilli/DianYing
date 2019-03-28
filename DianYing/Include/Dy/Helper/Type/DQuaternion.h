#ifndef GUARD_DY_TYPE_QUATERNION_H
#define GUARD_DY_TYPE_QUATERNION_H
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

#include <glm/gtc/quaternion.hpp>
#include <assimp/quaternion.h>
#include <foundation/PxQuat.h>
#include <Dy/Helper/Type/EAxis.h>
#include <Dy/Helper/Type/DMatrix4x4.h>

//!
//! Implementation
//!

namespace dy {

/// @class DQuaternion
/// @brief Quaternion class
class DQuaternion final 
{
public:
  DQuaternion() = default;
  DQuaternion(const DQuaternion& quaternion)            = default;
  DQuaternion& operator=(const DQuaternion& quaternion) = default;
  DQuaternion(const DVec3& eulerAngleXYZ);

  DQuaternion(const aiQuaternion& aiQuaternion);
  DQuaternion& operator=(const aiQuaternion& auQuaternion);

  DQuaternion(const glm::quat& glmQuat);
  DQuaternion& operator=(const glm::quat& glmQuat);

  DQuaternion(const physx::PxQuat& pxQuat);
  DQuaternion& operator=(const physx::PxQuat& pxQuat);
  
  DQuaternion(TF32 x, TF32 y, TF32 z, TF32 w);

  ///
  /// @brief Get rotation matrix (4x4) from quaternion.
  ///
  DMatrix4x4 GetRotationMatrix4x4() const noexcept;

  ///
  /// @brief Get rotation matrix (3x3) from quaternion.
  ///
  DMat3 GetRotationMatrix3x3() const noexcept;

  ///
  /// @brief Return euler rotation angle (degrees) (x, y, z).
  /// Caution! Quaternion to euler angle does not guarante precise degree euler angle.
  /// If you want to get degree angle values, you must to write code for saving angle.
  ///
  DVec3 GetEulerRotationDegreeAngleVector() const noexcept;

  ///
  /// @brief Return euler rotation angle (radians) (x, y, z).
  /// Caution! Quaternion to euler angle does not guarante precise radian euler angle.
  /// If you want to get radian angle values, you must to write code for saving angle.
  ///
  DVec3 GetEulerRotationRadiansAngleVector() const noexcept;

  ///
  /// @brief Impulse rotation with eulerAngle vector (degrees).
  ///
  void AddRotationAngle(const DVec3& eulerAngle);

  ///
  /// @brief Impulse rotation with quaternion.
  ///
  void AddRotationAngle(const DQuaternion& quaternion);

  ///
  /// @brief Impulse rotation with axis and degree angle value.
  ///
  void AddRotationAngle(EAxis axis, float degreeEulerAngle);

  ///
  /// @brief Impulse rotation with (x, y, z) degree angle value.
  ///
  void SetRotationAngle(const DVec3& degreeEulerAngle);

  ///
  const glm::quat& pGetQuaternion() const noexcept;

  operator physx::PxQuat() const noexcept
  {
    return physx::PxQuat{this->mQuaternion.x, this->mQuaternion.y, this->mQuaternion.z, this->mQuaternion.w};
  }

  MDY_NODISCARD TF32 W() const noexcept { return this->mQuaternion.w; }
  MDY_NODISCARD TF32 X() const noexcept { return this->mQuaternion.x; }
  MDY_NODISCARD TF32 Y() const noexcept { return this->mQuaternion.y; }
  MDY_NODISCARD TF32 Z() const noexcept { return this->mQuaternion.z; }

private:
  glm::quat mQuaternion;
};

} /// ::opgs16 namespace

#endif /// GUARD_DY_TYPE_QUATERNION_H
