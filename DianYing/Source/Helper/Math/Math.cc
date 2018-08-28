#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Math/Math.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Create file
///

/// Header file
#include <Dy/Helper/Math/Math.h>

#include <cfenv>
#include <limits>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Type/Vector3.h>

namespace dy::math {

bool IsAllZero(const dy::DVector2& vector) noexcept {
  if (!IsNearlyEqual(vector.X, 0.f)) return false;
  if (!IsNearlyEqual(vector.Y, 0.f)) return false;
  return true;
}

bool IsAllZero(const dy::DVector3& vector) noexcept {
  if (!IsNearlyEqual(vector.X, 0.f)) return false;
  if (!IsNearlyEqual(vector.Y, 0.f)) return false;
  if (!IsNearlyEqual(vector.Z, 0.f)) return false;
  return true;
}

float Lerp(float lhs, float rhs, float offset) {
  std::feclearexcept(FE_ALL_EXCEPT);
  const auto result = lhs * (1 - offset) + rhs * offset;
  if (std::fetestexcept(FE_INVALID | FE_OVERFLOW)) {
    return std::numeric_limits<float>::lowest();
  }
  return result;
}

double Lerp(double lhs, double rhs, float offset) {
  std::feclearexcept(FE_ALL_EXCEPT);
  const auto result = lhs * (1 - offset) + rhs * offset;
  if (std::fetestexcept(FE_INVALID | FE_OVERFLOW)) {
    return std::numeric_limits<double>::lowest();
  }
  return result;
}

DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float offset) {
  return lhs * (1 - offset) + rhs * offset;
}

DVector3 Lerp(const DVector3& lhs, const DVector3& rhs, float offset) {
  return lhs * (1 - offset) + rhs * offset;
}

DVector2 GetQuadBezierCurvePoint(const DVector2& lhs, const DVector2& rhs, const DVector2& control,
                                 float offset) {
  return Lerp(Lerp(lhs, control, offset), Lerp(control, rhs, offset), offset);
}

DVector3 GetQuadBezierCurvePoint(const DVector3& lhs, const DVector3& rhs, const DVector3& control,
                                 float offset) {
  return Lerp(Lerp(lhs, control, offset), Lerp(control, rhs, offset), offset);
}

TMinMaxResult<float> GetMinMax(const dy::DVector2& vector) noexcept {
  if (vector.X < vector.Y) {
    return {vector.X, vector.Y};
  }
  else {
    return {vector.Y, vector.X};
  }
}

TMinMaxResult<float> GetMinMax(const dy::DVector3& vector) noexcept {
  auto min = vector.X;
  auto max = min;

  if (vector.Y < min) min = vector.Y;
  if (vector.Y > max) max = vector.Y;
  if (vector.Z < min) min = vector.Z;
  if (vector.Z > max) max = vector.Z;
  return {min, max};
}

float GetRotationAngle(float angle_value) noexcept {
  const float angle = std::fmod(angle_value, 360.f);
  return (angle < 0.f) ? angle + 360.f : angle;
}

double GetRotationAngle(double angle_value) noexcept {
  const double angle = std::fmod(angle_value, 360.0);
  return (angle <= 0.0) ? angle + 360.0 : angle;
}

float GetRotationAngleRadian(float angle_value) noexcept {
  const float angle = std::fmod(angle_value, math::Pi2<float>);
  return (angle <= 0.f) ? angle + (Pi2<float>) : angle;
}

double GetRotationAngleRadian(double angle_value) noexcept {
  const double angle = std::fmod(angle_value, math::Pi2<double>);
  return (angle <= 0.0) ? angle + (Pi2<float>) : angle;
}

float RadToDeg(float radian) noexcept {
  return GetRotationAngle(radian * RadToDegVal<float>);
}

double RadToDeg(double radian) noexcept {
  return GetRotationAngle(radian * RadToDegVal<double>);
}

float DegToRad(float degree) noexcept {
  return GetRotationAngleRadian(degree * DegToRadVal<float>);
}

double DegToRad(double degree) noexcept {
  return GetRotationAngleRadian(degree * DegToRadVal<double>);
}

} /// ::dy::math namespace