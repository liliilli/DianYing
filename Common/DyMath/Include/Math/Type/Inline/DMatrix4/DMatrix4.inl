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

#include <Math/Type/Math/DVector4.h>
#include "..\..\Math\DMatrix4.h"

namespace dy::math
{

template <typename TType>
DMatrix4<TType>::DMatrix4(
  TValueType _00, TValueType _01, TValueType _02, TValueType _03,
  TValueType _10, TValueType _11, TValueType _12, TValueType _13,
  TValueType _20, TValueType _21, TValueType _22, TValueType _23,
  TValueType _30, TValueType _31, TValueType _32, TValueType _33)
  : mValues{
    DVector4<TValueType>{_00, _10, _20, _30}, 
    DVector4<TValueType>{_01, _11, _21, _31},
    DVector4<TValueType>{_02, _12, _22, _32},
    DVector4<TValueType>{_03, _13, _23, _33}} 
{ }

template<typename TType>
DMatrix4<TType>::DMatrix4(
  const DVector4<TValueType>& _0, 
  const DVector4<TValueType>& _1,
  const DVector4<TValueType>& _2,
  const DVector4<TValueType>& _3)
  : mValues{_0, _1, _2, _3}
{ }

template <typename TType>
DVector4<typename DMatrix4<TType>::TValueType>& 
DMatrix4<TType>::operator[](TIndex index) noexcept
{
  return this->mValues[index];
}

template <typename TType>
const DVector4<typename DMatrix4<TType>::TValueType>& 
DMatrix4<TType>::operator[](TIndex index) const noexcept
{
  return this->mValues[index];
}

template <typename TType>
DMatrix4<TType> DMatrix4<TType>::Transpose() const noexcept
{
  return DMatrix4
  {
    this->mValues[0][0], this->mValues[0][1], this->mValues[0][2], this->mValues[0][3],
    this->mValues[1][0], this->mValues[1][1], this->mValues[1][2], this->mValues[1][3],
    this->mValues[2][0], this->mValues[2][1], this->mValues[2][2], this->mValues[2][3],
    this->mValues[3][0], this->mValues[3][1], this->mValues[3][2], this->mValues[3][3],
  };
}

template <typename TType>
bool DMatrix4<TType>::IsInvertible() const noexcept
{
  return this->GetDeterminant() != TType(0.0);
}

template <typename TType>
TType DMatrix4<TType>::GetDeterminant() const noexcept
{
  TType SubFactor00 = (*this)[2][2] * (*this)[3][3] - (*this)[3][2] * (*this)[2][3];
  TType SubFactor01 = (*this)[2][1] * (*this)[3][3] - (*this)[3][1] * (*this)[2][3];
  TType SubFactor02 = (*this)[2][1] * (*this)[3][2] - (*this)[3][1] * (*this)[2][2];
  TType SubFactor03 = (*this)[2][0] * (*this)[3][3] - (*this)[3][0] * (*this)[2][3];
  TType SubFactor04 = (*this)[2][0] * (*this)[3][2] - (*this)[3][0] * (*this)[2][2];
  TType SubFactor05 = (*this)[2][0] * (*this)[3][1] - (*this)[3][0] * (*this)[2][1];

  DVector4<TType> DetCof
  {
    + ((*this)[1][1] * SubFactor00 - (*this)[1][2] * SubFactor01 + (*this)[1][3] * SubFactor02),
    - ((*this)[1][0] * SubFactor00 - (*this)[1][2] * SubFactor03 + (*this)[1][3] * SubFactor04),
    + ((*this)[1][0] * SubFactor01 - (*this)[1][1] * SubFactor03 + (*this)[1][3] * SubFactor05),
    - ((*this)[1][0] * SubFactor02 - (*this)[1][1] * SubFactor04 + (*this)[1][2] * SubFactor05)
  };

  return
    (*this)[0][0] * DetCof[0] + (*this)[0][1] * DetCof[1] +
    (*this)[0][2] * DetCof[2] + (*this)[0][3] * DetCof[3];
}

template <typename TType>
DMatrix4<TType> DMatrix4<TType>::Inverse() const
{
  TType Coef00 = (*this)[2][2] * (*this)[3][3] - (*this)[3][2] * (*this)[2][3];
  TType Coef02 = (*this)[1][2] * (*this)[3][3] - (*this)[3][2] * (*this)[1][3];
  TType Coef03 = (*this)[1][2] * (*this)[2][3] - (*this)[2][2] * (*this)[1][3];

  TType Coef04 = (*this)[2][1] * (*this)[3][3] - (*this)[3][1] * (*this)[2][3];
  TType Coef06 = (*this)[1][1] * (*this)[3][3] - (*this)[3][1] * (*this)[1][3];
  TType Coef07 = (*this)[1][1] * (*this)[2][3] - (*this)[2][1] * (*this)[1][3];

  TType Coef08 = (*this)[2][1] * (*this)[3][2] - (*this)[3][1] * (*this)[2][2];
  TType Coef10 = (*this)[1][1] * (*this)[3][2] - (*this)[3][1] * (*this)[1][2];
  TType Coef11 = (*this)[1][1] * (*this)[2][2] - (*this)[2][1] * (*this)[1][2];

  TType Coef12 = (*this)[2][0] * (*this)[3][3] - (*this)[3][0] * (*this)[2][3];
  TType Coef14 = (*this)[1][0] * (*this)[3][3] - (*this)[3][0] * (*this)[1][3];
  TType Coef15 = (*this)[1][0] * (*this)[2][3] - (*this)[2][0] * (*this)[1][3];

  TType Coef16 = (*this)[2][0] * (*this)[3][2] - (*this)[3][0] * (*this)[2][2];
  TType Coef18 = (*this)[1][0] * (*this)[3][2] - (*this)[3][0] * (*this)[1][2];
  TType Coef19 = (*this)[1][0] * (*this)[2][2] - (*this)[2][0] * (*this)[1][2];

  TType Coef20 = (*this)[2][0] * (*this)[3][1] - (*this)[3][0] * (*this)[2][1];
  TType Coef22 = (*this)[1][0] * (*this)[3][1] - (*this)[3][0] * (*this)[1][1];
  TType Coef23 = (*this)[1][0] * (*this)[2][1] - (*this)[2][0] * (*this)[1][1];

  DVector4<TType> fac0(Coef00, Coef00, Coef02, Coef03);
  DVector4<TType> fac1(Coef04, Coef04, Coef06, Coef07);
  DVector4<TType> fac2(Coef08, Coef08, Coef10, Coef11);
  DVector4<TType> fac3(Coef12, Coef12, Coef14, Coef15);
  DVector4<TType> fac4(Coef16, Coef16, Coef18, Coef19);
  DVector4<TType> fac5(Coef20, Coef20, Coef22, Coef23);

  DVector4<TType> vec0((*this)[1][0], (*this)[0][0], (*this)[0][0], (*this)[0][0]);
  DVector4<TType> vec1((*this)[1][1], (*this)[0][1], (*this)[0][1], (*this)[0][1]);
  DVector4<TType> vec2((*this)[1][2], (*this)[0][2], (*this)[0][2], (*this)[0][2]);
  DVector4<TType> vec3((*this)[1][3], (*this)[0][3], (*this)[0][3], (*this)[0][3]);

  DVector4<TType> inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
  DVector4<TType> inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
  DVector4<TType> inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
  DVector4<TType> inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

  DVector4<TType> signA(+1, -1, +1, -1);
  DVector4<TType> signB(-1, +1, -1, +1);
  DMatrix4<TType> inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

  DVector4<TType> row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

  DVector4<TType> dot0((*this)[0] * row0);
  TType det = (dot0.X + dot0.Y) + (dot0.Z + dot0.W);

  return inverse / det;
}

template <typename TType>
bool DMatrix4<TType>::HasNaN() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasNaN() == true) { return true; }
  }

  return false;
}

template <typename TType>
bool DMatrix4<TType>::HasInfinity() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasInfinity() == true) { return true; }
  }

  return false;
}

template <typename TType>
bool DMatrix4<TType>::HasOnlyNormal() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasOnlyNormal() == false) { return false; }
  }

  return true;
}

template <typename TType>
typename DMatrix4<TType>::TValueType* DMatrix4<TType>::Data() noexcept
{
  return this->mValues.front().Data();
}

template <typename TType>
const typename DMatrix4<TType>::TValueType* DMatrix4<TType>::Data() const noexcept
{
  return this->mValues.front().Data();
}

template<typename TType>
std::vector<DVector4<TType>> DMatrix4<TType>::ToVector() const noexcept
{
  return std::vector<DVector4<TType>>
  {
    (*this)[0],
    (*this)[1],
    (*this)[2],
    (*this)[3]
  };
}

template <typename TType>
DMatrix4<TType> DMatrix4<TType>::Identity() noexcept
{
  static DMatrix4<TType> identity
  {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  };
  return identity;
}

} /// ::dy::math namespace
