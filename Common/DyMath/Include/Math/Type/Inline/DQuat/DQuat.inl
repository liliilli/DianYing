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

#include <Math/Utility/XMath.h>
#include <Math/Utility/XLinearMath.h>
#include <Math/Utility/XMathTrigonometry.h>
#include "..\..\Math\DQuat.h"

namespace dy::math
{

template <typename TType>
DQuaternion<TType>::DQuaternion(const DVector3<TType>& eulerAngle, bool isDegree)
{
  const DVector3<TType> angle = isDegree == true 
    ? eulerAngle * kToRadian<TType> * TType(0.5) 
    : eulerAngle * TType(0.5);

  const auto cosAngle = Cos(angle, false);
  const auto sinAngle = Sin(angle, false);

  this->mW = cosAngle.X * cosAngle.Y * cosAngle.Z + sinAngle.X * sinAngle.Y * sinAngle.Z;
  this->mX = sinAngle.X * cosAngle.Y * cosAngle.Z - cosAngle.X * sinAngle.Y * sinAngle.Z;
  this->mY = cosAngle.X * sinAngle.Y * cosAngle.Z + sinAngle.X * cosAngle.Y * sinAngle.Z;
  this->mZ = cosAngle.X * cosAngle.Y * sinAngle.Z - sinAngle.X * sinAngle.Y * cosAngle.Z;
}

template <typename TType>
DQuaternion<TType>::DQuaternion(TValueType x, TValueType y, TValueType z, TValueType w)
  : mX{x}, mY{y}, mZ{z}, mW{w}
{ }

template <typename TType>
typename DQuaternion<TType>::TValueType& 
DQuaternion<TType>::operator[](TIndex index) 
{
  switch (index)
  {
  case 0: return this->mX;
  case 1: return this->mY;
  case 2: return this->mZ;
  case 3: return this->mW;
  default: 
    M_ASSERT_OR_THROW(false, "index must be 0, 1, 2 and 3.");
  }
}

template <typename TType>
const typename DQuaternion<TType>::TValueType& 
DQuaternion<TType>::operator[](TIndex index) const
{
  switch (index)
  {
  case 0: return this->mX;
  case 1: return this->mY;
  case 2: return this->mZ;
  case 3: return this->mW;
  default: 
    M_ASSERT_OR_THROW(false, "index must be 0, 1, 2 and 3.");
  }
}

template <typename TType>
DMatrix3<typename DQuaternion<TType>::TValueType> 
DQuaternion<TType>::ToMatrix3() const noexcept
{
  // 1-2y^2-2z^2 2xy+2wz      2xz-2wy
  // 2xy-2wz     1-2x^2-2z^2  2yz+2wx
  // 2xz+2wy     2yz-2wx      1-2x^2-2y^2 Transpose.
  DMatrix3<TType> result{};
  auto qxx = this->mX * this->mX;
  auto qyy = this->mY * this->mY;
  auto qzz = this->mZ * this->mZ;
  auto qxz = this->mX * this->mZ;
  auto qxy = this->mX * this->mY;
  auto qyz = this->mY * this->mZ;
  auto qwx = this->mW * this->mX;
  auto qwy = this->mW * this->mY;
  auto qwz = this->mW * this->mZ;

  result[0][0] = TType(1) - TType(2) * (qyy +  qzz);
  result[0][1] = TType(2) * (qxy + qwz);
  result[0][2] = TType(2) * (qxz - qwy);

  result[1][0] = TType(2) * (qxy - qwz);
  result[1][1] = TType(1) - TType(2) * (qxx +  qzz);
  result[1][2] = TType(2) * (qyz + qwx);

  result[2][0] = TType(2) * (qxz + qwy);
  result[2][1] = TType(2) * (qyz - qwx);
  result[2][2] = TType(1) - TType(2) * (qxx +  qyy);

  return result;
}

template <typename TType>
DMatrix4<typename DQuaternion<TType>::TValueType> 
DQuaternion<TType>::ToMatrix4() const noexcept
{
  const DMatrix3<TType> matrix3 = this->ToMatrix3();
  return 
  {
    DVector4<TType>{matrix3[0]}, 
    {matrix3[1]}, 
    {matrix3[2]}, 
    {0, 0, 0, 1}
  };
}

template <typename TType>
DQuaternion<TType> DQuaternion<TType>::Inverse() const
{
  // Get conjugate matrix. 
  const auto conjugate = DQuaternion<TType>{-this->mX, -this->mY, -this->mZ, this->mW};
  const auto dot = Dot(*this, *this);

  conjugate[0] /= dot;
  conjugate[1] /= dot;
  conjugate[2] /= dot;
  conjugate[3] /= dot;

  return conjugate;
}

template<typename TType>
DVector3<TType> DQuaternion<TType>::ToDegrees() const noexcept
{
  return this->ToRadians() * kToDegree<TType>;
}

template<typename TType>
DVector3<TType> DQuaternion<TType>::ToRadians() const noexcept
{
  // pitch (x-axis rotation)
  TType sinCosp = TType(2.0) * (this->mW * this->mX + this->mY * this->mZ);
  TType cosCosp = this->mW * this->mW + this->mZ * this->mZ * (this->mX * this->mX + this->mY * this->mY);
  TType pitchRad = std::atan2(sinCosp, cosCosp);

  // yaw (y-axis)
  // Why -2 instead of 2?
  TType yawRad = std::asin(TType(-2) * (this->mX * this->mZ - this->mY * this->mW));

  // roll (z-axis)
  TType rollRad = 
  std::atan2(
    TType(2) * (this->mX * this->mY + this->mW * this->mZ), 
    this->mW * this->mW + this->mX * this->mX - this->mY * this->mY - this->mZ * this->mZ);

  return {pitchRad, yawRad, rollRad};
}

template<typename TType>
void DQuaternion<TType>::AddAngle(const DVector3<TValueType>& angles, bool isDegree)
{
  if (isDegree == true)
  {
    const auto degree = this->ToDegrees() + angles;
    this->SetAngle(degree, true);
  }
  else
  {
    const auto radian = this->ToRadians() + angles;
    this->SetAngle(radian, false);
  }
}

template<typename TType>
void DQuaternion<TType>::AddAngle(const DQuaternion<TValueType>& quaternion)
{
  auto quat = quaternion * (*this);
  this->mX = quat.X();
  this->mY = quat.Y();
  this->mZ = quat.Z();
  this->mW = quat.W();
}

template<typename TType>
void DQuaternion<TType>::AddAngle(EAxis axis, TValueType iAngle, bool isDegree)
{
  auto angle = iAngle;
  if (isDegree == true)
  {
    angle = math::ToNormalizedRadian(angle * kToRadian<TType>);
  }

  switch (axis)
  {
  case EAxis::X:
  {
    const auto temp = AngleWithAxis(angle, DVector3<TType>{1, 0, 0}, false);
    const auto quat = temp * (*this);
    (*this) = quat;
  } break;
  case EAxis::Y:
  {
    const auto temp = AngleWithAxis(angle, DVector3<TType>{0, 1, 0}, false);
    const auto quat = temp * (*this);
    (*this) = quat;
  } break;
  case EAxis::Z:
  {
    const auto temp = AngleWithAxis(angle, DVector3<TType>{0, 0, 1}, false);
    const auto quat = temp * (*this);
    (*this) = quat;
  } break;
  }
}

template<typename TType>
void DQuaternion<TType>::SetAngle(const DVector3<TValueType>& eulerAngles, bool isDegree)
{
  auto angle = eulerAngles;
  if (isDegree == true)
  {
    angle *= math::kToRadian<TType>;
  }

  DQuaternion<TType> quat{angle, false};
  this->mX = quat.X();
  this->mY = quat.Y();
  this->mZ = quat.Z();
  this->mW = quat.W();
}

template <typename TType>
bool DQuaternion<TType>::HasNaN() const noexcept
{
  return std::isnan(this->mX) 
      || std::isnan(this->mY) 
      || std::isnan(this->mZ) 
      || std::isnan(this->mW);
}

template <typename TType>
bool DQuaternion<TType>::HasInfinity() const noexcept
{
  return std::isinf(this->mX) 
      || std::isinf(this->mY) 
      || std::isinf(this->mZ) 
      || std::isinf(this->mW);
}

template <typename TType>
bool DQuaternion<TType>::HasOnlyNormal() const noexcept
{
  return std::isnormal(this->mX) 
      && std::isnormal(this->mY) 
      && std::isnormal(this->mZ) 
      && std::isnormal(this->mW);
}

template <typename TType>
typename DQuaternion<TType>::TValueType* 
DQuaternion<TType>::Data() noexcept
{
  return &this->mX;
}

template <typename TType>
const typename DQuaternion<TType>::TValueType* 
DQuaternion<TType>::Data() const noexcept
{
  return &this->mX;
}

template <typename TType>
typename DQuaternion<TType>::TValueType 
DQuaternion<TType>::X() const noexcept
{
  return this->mX;
}

template <typename TType>
typename DQuaternion<TType>::TValueType 
DQuaternion<TType>::Y() const noexcept
{
  return this->mY;
}

template <typename TType>
typename DQuaternion<TType>::TValueType 
DQuaternion<TType>::Z() const noexcept
{
  return this->mZ;
}

template <typename TType>
typename DQuaternion<TType>::TValueType 
DQuaternion<TType>::W() const noexcept
{
  return this->mW;
}

} /// ::dy::math namespace