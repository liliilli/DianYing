#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Math/Math.h>

#include <cfenv>
#include <limits>
#include <glm/gtc/quaternion.hpp>

#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Helper/Type/DVector3.h>
#include <Dy/Helper/Type/DVector4.h>
#include <Dy/Helper/Type/DQuaternion.h>

namespace dy::math {

bool IsAllZero(const dy::DVector2& vector) noexcept
{
  if (!IsNearlyEqual(vector.X, 0.f)) return false;
  if (!IsNearlyEqual(vector.Y, 0.f)) return false;
  return true;
}

bool IsAllZero(const dy::DVector3& vector) noexcept
{
  if (!IsNearlyEqual(vector.X, 0.f)) return false;
  if (!IsNearlyEqual(vector.Y, 0.f)) return false;
  if (!IsNearlyEqual(vector.Z, 0.f)) return false;
  return true;
}

bool IsAllZero(const dy::DVector4& vector) noexcept
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

DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float offset) noexcept
{
  return lhs * (1 - offset) + rhs * offset;
}

DVector3 Lerp(const DVector3& lhs, const DVector3& rhs, float offset) noexcept
{
  return lhs * (1 - offset) + rhs * offset;
}

DVector4 Lerp(const DVector4& lhs, const DVector4& rhs, float offset) noexcept
{
  return lhs * (1 - offset) + rhs * offset;
}

DQuaternion Slerp(const DQuaternion& lhs, const DQuaternion& rhs, TF32 offset) noexcept
{
  return glm::slerp(lhs.pGetQuaternion(), rhs.pGetQuaternion(), offset);
}

DVector2 GetQuadBezierCurvePoint(const DVector2& lhs, const DVector2& rhs, const DVector2& control, float offset)
{
  return Lerp(Lerp(lhs, control, offset), Lerp(control, rhs, offset), offset);
}

DVector3 GetQuadBezierCurvePoint(const DVector3& lhs, const DVector3& rhs, const DVector3& control, float offset)
{
  return Lerp(Lerp(lhs, control, offset), Lerp(control, rhs, offset), offset);
}

TMinMaxResult<float> GetMinMax(const dy::DVector2& vector) noexcept
{
  if (vector.X < vector.Y)
  {
    return {vector.X, vector.Y};
  }
  else {
    return {vector.Y, vector.X};
  }
}

TMinMaxResult<float> GetMinMax(const dy::DVector3& vector) noexcept
{
  auto min = vector.X;
  auto max = min;

  if (vector.Y < min) min = vector.Y;
  if (vector.Y > max) max = vector.Y;
  if (vector.Z < min) min = vector.Z;
  if (vector.Z > max) max = vector.Z;
  return {min, max};
}

TMinMaxResult<float> GetMinMax(const dy::DVector4& vector) noexcept
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

DVector3 ConvertQuaternionToRadianEuler(_MIN_ const DQuaternion& iQuat)
{
  return ConvertQuaternionToRadianEuler(iQuat.W(), iQuat.X(), iQuat.Y(), iQuat.Z());
}

DVector3 ConvertQuaternionToRadianEuler(_MIN_ TF32 w, _MIN_ TF32 x, _MIN_ TF32 y, _MIN_ TF32 z)
{
  DVector3 radianAngle{};

  // roll (x-axis rotation)
  const TF64 sinrCosp = +2.0 * (w * x + y * z);
  const TF64 cosrCosp = +1.0 - 2.0 * (x * x + y * y);
	radianAngle.X = atan2(sinrCosp, cosrCosp);

	// pitch (y-axis rotation)
	const TF64 sinp = +2.0 * (w * y - z * x);
	if (fabs(sinp) >= 1)
  { // use 90 degrees if out of range
    radianAngle.Y = copysign(math::Pi<TF32> / 2, sinp); 
  }
	else { radianAngle.Y = asin(sinp); }

	// yaw (z-axis rotation)
	const TF64 sinyCosp = +2.0 * (w * z + x * y);
	const TF64 cosyCosp = +1.0 - 2.0 * (y * y + z * z);  
	radianAngle.Z = atan2(sinyCosp, cosyCosp);

  return radianAngle;
}

} /// ::dy::math namespace