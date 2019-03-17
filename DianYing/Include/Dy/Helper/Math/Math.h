#ifndef GUARD_DY_HELPER_MATH_MATH_H
#define GUARD_DY_HELPER_MATH_MATH_H
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

#include <list>

//!
//! Forward declaration
//!

namespace dy
{
class DQuaternion;
struct DVector2;
struct DVector3;
struct DVector4;
}

//!
//! Implementation
//!

#ifdef max
#undef max
#endif

namespace dy::math {

template <typename TType>
constexpr TType Pi = TType(3.1415926535897932385L);

template <typename TType>
constexpr TType Pi2 = TType(3.1415926535897932385L) * TType(2);

template <typename TType>
constexpr TType Exp = TType(2.71828182845904523536L);

template <typename TType>
constexpr TType DegToRadVal = Pi<TType> / TType(180.0);

template <typename TType>
constexpr TType RadToDegVal = TType(180.0) / Pi<TType>;

//!
//! Functions
//!

///
/// @brief Clamp value from 'from' and 'inclusive_to'.
///
template <typename TType>
[[nodiscard]] constexpr TType Clamp(const TType& value, const TType& from, const TType& inclusive_to) noexcept
{
  return (value > from ? (value < inclusive_to ? value : inclusive_to) : from);
}

///
/// @brief Check float lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two floating points.
///
[[nodiscard]] constexpr bool IsNearlyEqual(const float lhs, const float rhs, const float error_tolerance = 0.0001f) noexcept
{
  return (rhs < lhs ? lhs - rhs : rhs - lhs) < error_tolerance;
}

///
/// @brief Check double lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two double points.
///
[[nodiscard]] constexpr bool IsNearlyEqual(const double lhs, const double rhs, const double error_tolerance = 0.0001) noexcept
{
  return (rhs < lhs ? lhs - rhs : rhs - lhs) < error_tolerance;
}

///
/// @brief Check float '0' is nearly equal to real actual 0.
///
[[nodiscard]] constexpr bool IsNearlyZero(const float lhs, const float errorTolerance = 0.0001f) noexcept
{
  return lhs < errorTolerance ? (lhs > -errorTolerance ? true : false) : false;
}

///
/// @brief Check float '0' is nearly equal to real actual 0.
///
[[nodiscard]] constexpr bool IsNearlyZero(const double lhs, const double errorTolerance = 0.0001) noexcept
{
  return lhs < errorTolerance ? (lhs > -errorTolerance ? true : false) : false;
}

///
/// @brief Check if vector is all zero or nearly equal to zero.
///
[[nodiscard]] bool IsAllZero(const DVector2& vector) noexcept;

///
/// @brief Check if vector is all zero or nearly equal to zero.
///
[[nodiscard]] bool IsAllZero(const DVector3& vector) noexcept;

///
/// @brief Check if vector is all zero or nearly equal to zero.
///
[[nodiscard]] bool IsAllZero(const DVector4& vector) noexcept;

///
/// @brief Do linear interpolation with float type.
/// If something wrong has been happened, return lowest value of float.
///
[[nodiscard]] float Lerp(float lhs, float rhs, float offset);

///
/// @brief Do linear interpolation with double type.
/// If something wrong has been happened, return lowest value of double.
///
[[nodiscard]] double Lerp(double lhs, double rhs, float offset);

///
/// @brief Do linear interpolation with DVector2 type.
///
[[nodiscard]] DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float offset) noexcept;

///
/// @brief Do linear interpolation with DVector3 type.
///
[[nodiscard]] DVector3 Lerp(const DVector3& lhs, const DVector3& rhs, float offset) noexcept;

///
/// @brief Do linear interpolation with DVector4 type.
///
[[nodiscard]] DVector4 Lerp(const DVector4& lhs, const DVector4& rhs, float offset) noexcept;

/// @brief Do sphere-linear interpolation with DQuaternion.
MDY_NODISCARD DQuaternion Slerp(_MIN_ const DQuaternion& lhs, _MIN_ const DQuaternion& rhs, _MIN_ TF32 offset) noexcept;

///
/// @brief Get result point through quadratic bezier curve calculation.
///
[[nodiscard]] DVector2 GetQuadBezierCurvePoint(const DVector2& lhs, const DVector2& rhs, const DVector2& control, float offset);

///
/// @brief Get result point through quadratic bezier curve calculation.
///
[[nodiscard]] DVector3 GetQuadBezierCurvePoint(const DVector3& lhs, const DVector3& rhs, const DVector3& control, float offset);

//!
//! GetMinMax Functions
//!

template <typename TValueType>
using TMinMaxResult = std::pair<TValueType, TValueType>;

///
/// @brief Get min and max arithmetic value from list.
/// Result container's first value has min, second value has max.
///
template <typename TValueType, typename = std::enable_if_t<std::is_arithmetic_v<TValueType>>>
[[nodiscard]] TMinMaxResult<TValueType> GetMinMax(const std::vector<TValueType>& list)
{
  TValueType min = std::numeric_limits<TValueType>::max();
  TValueType max = std::numeric_limits<TValueType>::lowest();

  for (const auto& value : list)
  {
    if (value < min) min = value;
    if (value > max) max = value;
  }

  return {min, max};
}

///
/// @brief Get min and max arithmetic value from list.
/// Result container's first value has min, second value has max.
///
template <typename TValueType, typename = std::enable_if_t<std::is_arithmetic_v<TValueType>>>
[[nodiscard]] TMinMaxResult<TValueType> GetMinMax(const std::list<TValueType>& list)
{
  TValueType min = std::numeric_limits<TValueType>::max();
  TValueType max = std::numeric_limits<TValueType>::lowest();

  for (const auto& value : list)
  {
    if (value < min) min = value;
    if (value > max) max = value;
  }

  return {min, max};
}

///
/// @brief Get min and max arithmetic value from list.
/// Result container's first value has min, second value has max.
///
template <typename TValueType, int32_t TSize, typename = std::enable_if_t<std::is_arithmetic_v<TValueType>>>
[[nodiscard]] TMinMaxResult<TValueType> GetMinMax(const std::array<TValueType, TSize>& list)
{
  TValueType min = std::numeric_limits<TValueType>::max();
  TValueType max = std::numeric_limits<TValueType>::lowest();

  for (const auto& value : list)
  {
    if (value < min) min = value;
    if (value > max) max = value;
  }

  return {min, max};
}

///
/// @brief Get min and max arithmetic value from list.
/// Result container's first value has min, second value has max.
///
template <typename TValueType, int32_t TSize, typename = std::enable_if_t<std::is_arithmetic_v<TValueType>>>
[[nodiscard]] TMinMaxResult<TValueType> GetMinMax(const TValueType (&list)[TSize])
{
  TValueType min = std::numeric_limits<TValueType>::max();
  TValueType max = std::numeric_limits<TValueType>::lowest();

  for (const auto& value : list)
  {
    if (value < min) min = value;
    if (value > max) max = value;
  }

  return {min, max};
}

///
/// @brief Get min and max arithmetic value from DVector2.
/// Result container's first value has min, second value has max.
///
[[nodiscard]] TMinMaxResult<float> GetMinMax(const dy::DVector2& vector) noexcept;

///
/// @brief Get min and max arithmetic value from DVector3.
/// Result container's first value has min, second value has max.
///
[[nodiscard]] TMinMaxResult<float> GetMinMax(const dy::DVector3& vector) noexcept;

///
/// @brief Get min and max arithmetic value from DVector3.
/// Result container's first value has min, second value has max.
///
[[nodiscard]] TMinMaxResult<float> GetMinMax(const dy::DVector4& vector) noexcept;

///
/// @brief Get degree rotation angle from 0 to 360' automatically.
///
[[nodiscard]] float GetClampedRotationDegreeAngle(float degreeAngle) noexcept;

///
/// @brief Get degree rotation angle from 0 to 360' automatically.
///
[[nodiscard]] double GetClampedRotationDegreeAngle(double degreeAngle) noexcept;

///
/// @brief Get degree rotation angle from 0 to 2pi automatically.
///
[[nodiscard]] float GetClampedRotationRadianAngle(float radianAngle) noexcept;

///
/// @brief Get degree rotation angle from 0 to 2pi automatically.
///
[[nodiscard]] double GetClampedRotationRadianAngle(double radianAngle) noexcept;

///
/// @brief Convert radian to degree, return -180 to 180'.
///
[[nodiscard]] float ConvertRadianToDegree(float radian) noexcept;

///
/// @brief Convert radian to degree, return -180 to 180'.
///
[[nodiscard]] double ConvertRadianToDegree(double radian) noexcept;

///
/// @brief Convert degree to radian, return -pi to pi.
///
[[nodiscard]] float ConvertDegreeToRadian(float degree) noexcept;

///
/// @brief Convert degree to radian, return -pi to pi.
///
[[nodiscard]] double ConvertDegreeToRadian(double degree) noexcept;

/// @brief Convert quaternion to radian degree.
[[nodiscard]] DVector3 ConvertQuaternionToRadianEuler(_MIN_ const DQuaternion& iQuat);
/// @brief Convert quaterion w,x,y,z to radian degree.
[[nodiscard]] DVector3 ConvertQuaternionToRadianEuler(_MIN_ TF32 w, _MIN_ TF32 x, _MIN_ TF32 y, _MIN_ TF32 z);

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_MATH_MATH_H