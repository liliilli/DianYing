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

#include <Math/Type/Math/DVector3.h>
#include "..\..\Math\DMatrix3.h"

namespace dy::math
{

template <typename TType>
DMatrix3<TType>::DMatrix3(
  TValueType _00, TValueType _01, TValueType _02, 
  TValueType _10, TValueType _11, TValueType _12, 
  TValueType _20, TValueType _21, TValueType _22)
  : mValues{
    DVector3<TValueType>{_00, _10, _20}, 
    DVector3<TValueType>{_01, _11, _21},
    DVector3<TValueType>{_02, _12, _22}} 
{ }

template<typename TType>
DMatrix3<TType>::DMatrix3(
  const DVector3<TValueType>& _0, 
  const DVector3<TValueType>& _1,
  const DVector3<TValueType>& _2)
  : mValues{_0, _1, _2}
{ }

template <typename TType>
DVector3<typename DMatrix3<TType>::TValueType>& 
DMatrix3<TType>::operator[](TIndex index) noexcept
{
  return this->mValues[index];
}

template <typename TType>
const DVector3<typename DMatrix3<TType>::TValueType>& 
DMatrix3<TType>::operator[](TIndex index) const noexcept
{
  return this->mValues[index];
}

template <typename TType>
DMatrix3<TType> DMatrix3<TType>::Transpose() const noexcept
{
  return DMatrix3
  {
    this->mValues[0][0], this->mValues[0][1], this->mValues[0][2],
    this->mValues[1][0], this->mValues[1][1], this->mValues[1][2],
    this->mValues[2][0], this->mValues[2][1], this->mValues[2][2]
  };
}

template <typename TType>
bool DMatrix3<TType>::IsInvertible() const noexcept
{
  return this->GetDeterminant() != TType(0.0);
}

template <typename TType>
TType DMatrix3<TType>::GetDeterminant() const noexcept
{
  return 
    + (*this)[0][0] * ((*this)[1][1] * (*this)[2][2] - (*this)[2][1] * (*this)[1][2])
    - (*this)[1][0] * ((*this)[0][1] * (*this)[2][2] - (*this)[2][1] * (*this)[0][2])
    + (*this)[2][0] * ((*this)[0][1] * (*this)[1][2] - (*this)[1][1] * (*this)[0][2]);
}

template <typename TType>
DMatrix3<TType> DMatrix3<TType>::Inverse() const
{
  const auto det = this->GetDeterminant();

  DMatrix3<TType> result{};
  result[0][0] = + ((*this)[1][1] * (*this)[2][2] - (*this)[2][1] * (*this)[1][2]);
  result[1][0] = - ((*this)[1][0] * (*this)[2][2] - (*this)[2][0] * (*this)[1][2]);
  result[2][0] = + ((*this)[1][0] * (*this)[2][1] - (*this)[2][0] * (*this)[1][1]);
  result[0][1] = - ((*this)[0][1] * (*this)[2][2] - (*this)[2][1] * (*this)[0][2]);
  result[1][1] = + ((*this)[0][0] * (*this)[2][2] - (*this)[2][0] * (*this)[0][2]);
  result[2][1] = - ((*this)[0][0] * (*this)[2][1] - (*this)[2][0] * (*this)[0][1]);
  result[0][2] = + ((*this)[0][1] * (*this)[1][2] - (*this)[1][1] * (*this)[0][2]);
  result[1][2] = - ((*this)[0][0] * (*this)[1][2] - (*this)[1][0] * (*this)[0][2]);
  result[2][2] = + ((*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1]);
  result /= det;

  return result;
}

template <typename TType>
bool DMatrix3<TType>::HasNaN() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasNaN() == true) { return true; }
  }

  return false;
}

template <typename TType>
bool DMatrix3<TType>::HasInfinity() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasInfinity() == true) { return true; }
  }

  return false;
}

template <typename TType>
bool DMatrix3<TType>::HasOnlyNormal() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasOnlyNormal() == false) { return false; }
  }

  return true;
}

template <typename TType>
typename DMatrix3<TType>::TValueType* DMatrix3<TType>::Data() noexcept
{
  return this->mValues.front().Data();
}

template <typename TType>
const typename DMatrix3<TType>::TValueType* DMatrix3<TType>::Data() const noexcept
{
  return this->mValues.front().Data();
}

template<typename TType>
std::vector<DVector3<TType>> DMatrix3<TType>::ToVector() const noexcept
{
  return std::vector<DVector3<TType>>
  {
    this->operator[](0),
    this->operator[](1),
    this->operator[](2)
  };
}

template <typename TType>
DMatrix3<TType> DMatrix3<TType>::Identity() noexcept
{
  static DMatrix3<TType> identity
  {
    1, 0, 0, 
    0, 1, 0,
    0, 0, 1
  };
  return identity;
}

} /// ::dy::math namespace
