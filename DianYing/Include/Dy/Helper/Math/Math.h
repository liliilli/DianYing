#ifndef GUARD_DY_HELPER_MATH_MATH_H
#define GUARD_DY_HELPER_MATH_MATH_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Dy/Helper/Math/Math.h
///
/// @brief Restriction template struct type which has range.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-05 Create file.
///

//!
//! Forward declaration
//!

namespace dy
{
struct DVector2;
struct DVector3;
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
/// @brief aef
/// efef
///
template <typename TType>
constexpr TType Clamp(const TType& value, const TType& from, const TType& inclusive_to) noexcept {
  return (value > from ? (value < inclusive_to ? value : inclusive_to) : from);
}

///
/// @brief Check float lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two floating points.
///
constexpr bool IsNearlyEqual(const float lhs, const float rhs, const float error_tolerance = 0.0001f) noexcept {
  return (rhs < lhs ? lhs - rhs : rhs - lhs) < error_tolerance;
}

///
/// @brief Check double lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two double points.
///
constexpr bool IsNearlyEqual(const double lhs, const double rhs, const double error_tolerance = 0.0001) noexcept {
  return (rhs < lhs ? lhs - rhs : rhs - lhs) < error_tolerance;
}

///
/// @brief Check if vector is all zero or nearly equal to zero.
///
bool IsAllZero(const dy::DVector2& vector) noexcept;

///
/// @brief Check if vector is all zero or nearly equal to zero.
///
bool IsAllZero(const dy::DVector3& vector) noexcept;

// @todo Need to implement DVector4 && DVectorInt4 version IsAllZero().

///
/// @brief Do linear interpolation with float type.
/// If something wrong has been happened, return lowest value of float.
///
float Lerp(float lhs, float rhs, float offset);

///
/// @brief Do linear interpolation with double type.
/// If something wrong has been happened, return lowest value of double.
///
double Lerp(double lhs, double rhs, float offset);

///
/// @brief Do linear interpolation with DVector2 type.
///
DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float offset);

///
/// @brief Do linear interpolation with DVector3 type.
///
DVector3 Lerp(const DVector3& lhs, const DVector3& rhs, float offset);

///
/// @brief Get result point through quadratic bezier curve calculation.
///
DVector2 GetQuadBezierCurvePoint(const DVector2& lhs, const DVector2& rhs, const DVector2& control,
                                 float offset);

///
/// @brief Get result point through quadratic bezier curve calculation.
///
DVector3 GetQuadBezierCurvePoint(const DVector3& lhs, const DVector3& rhs, const DVector3& control,
                                 float offset);

//!
//! GetMinMax Functions
//!

template <typename TValueType>
using TMinMaxResult = std::pair<TValueType, TValueType>;

///
/// @brief Get min and max arithmetic value from list.
/// Result container's first value has min, second value has max.
///
template <typename TValueType,
          typename = std::enable_if_t<std::is_arithmetic_v<TValueType>>>
TMinMaxResult<TValueType> GetMinMax(const std::vector<TValueType>& list) {
  TValueType min = std::numeric_limits<TValueType>::max();
  TValueType max = std::numeric_limits<TValueType>::lowest();

  for (const auto& value : list) {
    if (value < min) min = value;
    if (value > max) max = value;
  }

  return {min, max};
}

///
/// @brief Get min and max arithmetic value from list.
/// Result container's first value has min, second value has max.
///
template <typename TValueType,
          typename = std::enable_if_t<std::is_arithmetic_v<TValueType>>>
TMinMaxResult<TValueType> GetMinMax(const std::list<TValueType>& list) {
  TValueType min = std::numeric_limits<TValueType>::max();
  TValueType max = std::numeric_limits<TValueType>::lowest();

  for (const auto& value : list) {
    if (value < min) min = value;
    if (value > max) max = value;
  }

  return {min, max};
}

///
/// @brief Get min and max arithmetic value from list.
/// Result container's first value has min, second value has max.
///
template <typename TValueType, int32_t TSize,
          typename = std::enable_if_t<std::is_arithmetic_v<TValueType>>>
TMinMaxResult<TValueType> GetMinMax(const std::array<TValueType, TSize>& list) {
  TValueType min = std::numeric_limits<TValueType>::max();
  TValueType max = std::numeric_limits<TValueType>::lowest();

  for (const auto& value : list) {
    if (value < min) min = value;
    if (value > max) max = value;
  }

  return {min, max};
}

///
/// @brief Get min and max arithmetic value from list.
/// Result container's first value has min, second value has max.
///
template <typename TValueType, int32_t TSize,
          typename = std::enable_if_t<std::is_arithmetic_v<TValueType>>>
TMinMaxResult<TValueType> GetMinMax(const TValueType (&list)[TSize]) {
  TValueType min = std::numeric_limits<TValueType>::max();
  TValueType max = std::numeric_limits<TValueType>::lowest();

  for (const auto& value : list) {
    if (value < min) min = value;
    if (value > max) max = value;
  }

  return {min, max};
}

///
/// @brief Get min and max arithmetic value from DVector2.
/// Result container's first value has min, second value has max.
///
TMinMaxResult<float> GetMinMax(const dy::DVector2& vector) noexcept;

///
/// @brief Get min and max arithmetic value from DVector3.
/// Result container's first value has min, second value has max.
///
TMinMaxResult<float> GetMinMax(const dy::DVector3& vector) noexcept;

///
/// @brief Get degree rotation angle from -180 to 180' automatically.
///
float GetRotationAngle(float angle_value) noexcept;

///
/// @brief Get degree rotation angle from -180 to 180' automatically.
///
double GetRotationAngle(double angle_value) noexcept;

///
/// @brief Get degree rotation angle from -180 to 180' automatically.
///
float GetRotationAngleRadian(float angle_value) noexcept;

///
/// @brief Get degree rotation angle from -180 to 180' automatically.
///
double GetRotationAngleRadian(double angle_value) noexcept;

///
/// @brief Convert radian to degree, return -180 to 180'.
///
float RadToDeg(float radian) noexcept;

///
/// @brief Convert radian to degree, return -180 to 180'.
///
double RadToDeg(double radian) noexcept;

///
/// @brief Convert degree to radian, return -pi to pi.
///
float DegToRad(float degree) noexcept;

///
/// @brief Convert degree to radian, return -pi to pi.
///
double DegToRad(double degree) noexcept;

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_MATH_MATH_H