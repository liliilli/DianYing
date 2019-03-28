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

#include <Math/Type/Math/DVector2.h>
#include "..\..\Math\DMatrix2.h"

namespace dy::math
{

template <typename TType>
DMatrix2<TType>::DMatrix2(TValueType _00, TValueType _01, TValueType _10, TValueType _11)
  : mValues{DVector2<TValueType>{_00, _10}, DVector2<TValueType>{_01, _11}} 
{ }

template<typename TType>
DMatrix2<TType>::DMatrix2(const DVector2<TValueType>& _0, const DVector2<TValueType>& _1)
  : mValues{_0, _1}
{ }

template <typename TType>
DVector2<typename DMatrix2<TType>::TValueType>& 
DMatrix2<TType>::operator[](TIndex index) noexcept
{
  return this->mValues[index];
}

template <typename TType>
const DVector2<typename DMatrix2<TType>::TValueType>& 
DMatrix2<TType>::operator[](TIndex index) const noexcept
{
  return this->mValues[index];
}

template <typename TType>
DMatrix2<TType> DMatrix2<TType>::Transpose() const noexcept
{
  return DMatrix2
  {
    this->mValues[0][0], 
    this->mValues[0][1],
    this->mValues[1][0], 
    this->mValues[1][1]
  };
}

template <typename TType>
bool DMatrix2<TType>::IsInvertible() const noexcept
{
  return this->GetDeterminant() != TType(0.0);
}

template <typename TType>
TType DMatrix2<TType>::GetDeterminant() const noexcept
{
  return (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1];
}

template <typename TType>
DMatrix2<TType> DMatrix2<TType>::Inverse() const
{
  const auto det = this->GetDeterminant();

  DMatrix2<TType> result;
  result[0][0] =  (*this)[1][1];
  result[1][1] =  (*this)[0][0];
  result[1][0] = -(*this)[1][0];
  result[0][1] = -(*this)[0][1];

  result /= det;
  return result;
}

template <typename TType>
bool DMatrix2<TType>::HasNaN() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasNaN() == true) { return true; }
  }

  return false;
}

template <typename TType>
bool DMatrix2<TType>::HasInfinity() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasInfinity() == true) { return true; }
  }

  return false;
}

template <typename TType>
bool DMatrix2<TType>::HasOnlyNormal() const noexcept
{
  for (const auto& value : this->mValues)
  {
    if (value.HasOnlyNormal() == false) { return false; }
  }

  return true;
}

template <typename TType>
typename DMatrix2<TType>::TValueType* DMatrix2<TType>::Data() noexcept
{
  return this->mValues.front().Data();
}

template <typename TType>
const typename DMatrix2<TType>::TValueType* DMatrix2<TType>::Data() const noexcept
{
  return this->mValues.front().Data();
}

template<typename TType>
std::vector<DVector2<TType>> DMatrix2<TType>::ToVector() const noexcept
{
  return std::vector<DVector2<TType>>{this->operator[](0), this->operator[](1)};
}

template <typename TType>
DMatrix2<TType> DMatrix2<TType>::Identity() noexcept
{
  // 1 0
  // 0 1
  static DMatrix2<TType> identity{1, 0, 0, 1};
  return identity;
}

} /// ::dy::math namespace
