#pragma once
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

#include <Math/Common/Inline/TGlobalTypes.inl>
#include <Math/Common/XGlobalMacroes.h>
#include <Math/Type/Math/DVector3.h>
#include <Math/Type/Math/DMatrix4.h>
#include <Math/Type/Math/DMatrix3.h>
#include <Math/Type/Math/EAxis.h>

namespace dy::math 
{

/// @class DQuaternion
/// @brief Quaternion class
template <typename TType> 
struct MATH_NODISCARD DQuaternion final 
{
public:
  static_assert(
    kCategoryOf<TType> == EValueCategory::Real, 
    "Failed to make DQuaternion, DQuaternion only supports Real type.");

  using TValueType = TType;

  DQuaternion() = default;
  /// @brief Contsructor for eulerAngle, pitch, yaw and roll.
  DQuaternion(const DVector3<TValueType>& eulerAngle, bool isDegree = true);
  DQuaternion(TValueType x, TValueType y, TValueType z, TValueType w);

  TValueType& operator[](TIndex index);
  const TValueType& operator[](TIndex index) const;

  /// @brief Get rotation matrix (3x3) from quaternion.
  DMatrix3<TValueType> ToMatrix3() const noexcept;
  /// @brief Get rotation matrix (4x4) from quaternion.
  DMatrix4<TValueType> ToMatrix4() const noexcept;
  /// @brief Get inverse quaternion of this quaternion.
  DQuaternion Inverse() const;

  /// @brief Return euler rotation angle (degrees) (x, y, z).
  /// Note that Quaternion to euler angle does not guarantee precise degree euler angle.
  /// If you want to get degree angle values, you must to write code for saving angle.
  DVector3<TValueType> ToDegrees() const noexcept;

  /// @brief Return euler rotation angle (radians) (x, y, z).
  /// Note that Quaternion to euler angle does not guarantee precise radian euler angle.
  /// If you want to get radian angle values, you must to write code for saving angle.
  DVector3<TValueType> ToRadians() const noexcept;

  /// @brief Impulse rotation with eulerAngle vector (degrees or radians).
  void AddAngle(const DVector3<TValueType>& angles, bool isDegree = true);
  /// @brief Impulse rotation with quaternion.
  void AddAngle(const DQuaternion<TValueType>& quaternion);
  /// @brief Impulse rotation with axis and degree or radian sangle value.
  void AddAngle(EAxis axis, TValueType angle, bool isDegree = true);

  /// @brief Impulse rotation with (x, y, z) angle value.
  void SetAngle(const DVector3<TValueType>& eulerAngles, bool isDegree = true);

  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;
  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;
  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

  /// @brief Get start pointer of matrix sequence.
  TValueType* Data() noexcept;
  /// @brief Get start pointer of matrix sequence.
  const TValueType* Data() const noexcept;

  TValueType X() const noexcept;
  TValueType Y() const noexcept;
  TValueType Z() const noexcept;
  TValueType W() const noexcept;

private:
  TValueType mX = 0, mY = 0, mZ = 0, mW = TValueType(1);
};

template<typename TType>
DQuaternion<TType> operator+(const DQuaternion<TType>& q, const DQuaternion<TType>& p)
{
  return { q.X() + p.X(), q.Y() + p.Y(), q.Z() + p.Z(), q.W() + p.W() };
}

template<typename TType>
DQuaternion<TType> operator*(const DQuaternion<TType>& q, const DQuaternion<TType>& p)
{
  return 
  {
    p.W() * q.X() + p.X() * q.W() + p.Y() * q.Z() - p.Z() * q.Y(),
    p.W() * q.Y() + p.Y() * q.W() + p.Z() * q.X() - p.X() * q.Z(),
    p.W() * q.Z() + p.Z() * q.W() + p.X() * q.Y() - p.Y() * q.X(),
    p.W() * q.W() - p.X() * q.X() - p.Y() * q.Y() - p.Z() * q.Z()
  };
}

template<typename TType>
DQuaternion<TType> operator*(const DQuaternion<TType>& q, const TType& p)
{
  return { q.X() * p, q.Y() * p, q.Z() * p, q.W() * p };
}

template<typename TType>
DQuaternion<TType> operator*(const TType& p, const DQuaternion<TType>& q)
{
  return q * p;
}

template<typename TType>
DQuaternion<TType> operator/(const DQuaternion<TType>& q, const TType& p)
{
  return { q.X() / p, q.Y() / p, q.Z() / p, q.W() / p };
}

} /// ::dY()::math namespace
#include <Math/Type/Inline/DQuat/DQuat.inl>