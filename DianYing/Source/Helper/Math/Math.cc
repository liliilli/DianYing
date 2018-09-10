#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Math/Math.h>

#include <cfenv>
#include <limits>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/Type/Vector4.h>

namespace dy::math {

bool IsAllZero(const dy::DDyVector2& vector) noexcept
{
  if (!IsNearlyEqual(vector.X, 0.f)) return false;
  if (!IsNearlyEqual(vector.Y, 0.f)) return false;
  return true;
}

bool IsAllZero(const dy::DDyVector3& vector) noexcept
{
  if (!IsNearlyEqual(vector.X, 0.f)) return false;
  if (!IsNearlyEqual(vector.Y, 0.f)) return false;
  if (!IsNearlyEqual(vector.Z, 0.f)) return false;
  return true;
}

bool IsAllZero(const dy::DDyVector4& vector) noexcept
{
  if (!IsNearlyEqual(vector.X, 0.f)) return false;
  if (!IsNearlyEqual(vector.Y, 0.f)) return false;
  if (!IsNearlyEqual(vector.Z, 0.f)) return false;
  if (!IsNearlyEqual(vector.W, 0.f)) return false;
  return true;
}

float Lerp(float lhs, float rhs, float offset)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  const auto result = lhs * (1 - offset) + rhs * offset;
  if (std::fetestexcept(FE_INVALID | FE_OVERFLOW)) {
    return std::numeric_limits<float>::lowest();
  }
  return result;
}

double Lerp(double lhs, double rhs, float offset)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  const auto result = lhs * (1 - offset) + rhs * offset;
  if (std::fetestexcept(FE_INVALID | FE_OVERFLOW)) {
    return std::numeric_limits<double>::lowest();
  }
  return result;
}

DDyVector2 Lerp(const DDyVector2& lhs, const DDyVector2& rhs, float offset) noexcept
{
  return lhs * (1 - offset) + rhs * offset;
}

DDyVector3 Lerp(const DDyVector3& lhs, const DDyVector3& rhs, float offset) noexcept
{
  return lhs * (1 - offset) + rhs * offset;
}

DDyVector4 Lerp(const DDyVector4& lhs, const DDyVector4& rhs, float offset) noexcept
{
  return lhs * (1 - offset) + rhs * offset;
}

DDyVector2 GetQuadBezierCurvePoint(const DDyVector2& lhs, const DDyVector2& rhs, const DDyVector2& control, float offset)
{
  return Lerp(Lerp(lhs, control, offset), Lerp(control, rhs, offset), offset);
}

DDyVector3 GetQuadBezierCurvePoint(const DDyVector3& lhs, const DDyVector3& rhs, const DDyVector3& control, float offset)
{
  return Lerp(Lerp(lhs, control, offset), Lerp(control, rhs, offset), offset);
}

TMinMaxResult<float> GetMinMax(const dy::DDyVector2& vector) noexcept
{
  if (vector.X < vector.Y)
  {
    return {vector.X, vector.Y};
  }
  else {
    return {vector.Y, vector.X};
  }
}

TMinMaxResult<float> GetMinMax(const dy::DDyVector3& vector) noexcept
{
  auto min = vector.X;
  auto max = min;

  if (vector.Y < min) min = vector.Y;
  if (vector.Y > max) max = vector.Y;
  if (vector.Z < min) min = vector.Z;
  if (vector.Z > max) max = vector.Z;
  return {min, max};
}

TMinMaxResult<float> GetMinMax(const dy::DDyVector4& vector) noexcept
{
  auto min = vector.X;
  auto max = min;

  if (vector.Y < min) min = vector.Y;
  if (vector.Y > max) max = vector.Y;
  if (vector.Z < min) min = vector.Z;
  if (vector.Z > max) max = vector.Z;
  if (vector.W < min) min = vector.W;
  if (vector.W > max) max = vector.W;
  return {min, max};
}

float GetClampedRotationDegreeAngle(float angle_value) noexcept
{
  const float angle = std::fmod(angle_value, 360.f);
  return (angle < 0.f) ? angle + 360.f : angle;
}

double GetClampedRotationDegreeAngle(double angle_value) noexcept
{
  const double angle = std::fmod(angle_value, 360.0);
  return (angle <= 0.0) ? angle + 360.0 : angle;
}

float GetClampedRotationRadianAngle(float angle_value) noexcept
{
  const float angle = std::fmod(angle_value, math::Pi2<float>);
  return (angle <= 0.f) ? angle + (Pi2<float>) : angle;
}

double GetClampedRotationRadianAngle(double angle_value) noexcept
{
  const double angle = std::fmod(angle_value, math::Pi2<double>);
  return (angle <= 0.0) ? angle + (Pi2<float>) : angle;
}

float ConvertRadianToDegree(float radian) noexcept
{
  return GetClampedRotationDegreeAngle(radian * RadToDegVal<float>);
}

double ConvertRadianToDegree(double radian) noexcept
{
  return GetClampedRotationDegreeAngle(radian * RadToDegVal<double>);
}

float ConvertDegreeToRadian(float degree) noexcept
{
  return GetClampedRotationRadianAngle(degree * DegToRadVal<float>);
}

double ConvertDegreeToRadian(double degree) noexcept
{
  return GetClampedRotationRadianAngle(degree * DegToRadVal<double>);
}

} /// ::dy::math namespace