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

#ifndef M_NOUSE_THROW
#include <stdexcept>
#else
#include <cassert>
#endif
#include <Common/XGlobalMacroes.h>
#include <Common/XGlobalUtilites.h>
#include <Type/DVector2.h>

namespace dy::math
{

template <typename TType>
DVector2<
  TType, 
  std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::DVector2(TValueType x, TValueType y) noexcept
  : X{x}, Y{y}
{ }

template <typename TType>
DVector2<
  TType, 
  std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::DVector2(TValueType value) noexcept
  : X{value}, Y{value} 
{ }

template <typename TType>
template <typename TAnotherType>
DVector2<
  TType, 
  std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator DVector2<TAnotherType>() const noexcept
{
  using AnotherType = typename DVector2<
    TAnotherType, 
    std::enable_if_t<
        TValueCategory<TAnotherType>::kValue == EValueCategory::Integer
    ||  TValueCategory<TAnotherType>::kValue == EValueCategory::Real>>::TValueType; 

  return DVector2<TAnotherType>
  {
    Cast<AnotherType>(this->X), 
    Cast<AnotherType>(this->Y)
  };
}

template <typename TType>
typename DVector2<
  TType, 
  std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>::TValueType& 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator[](TIndex index)
{
  switch (index)
  {
  case 0: return this->X;
  case 1: return this->Y;
  default: 
    M_ASSERT_OR_THROW(false, "index must be 0, 1.");
  }
}

template <typename TType>
const
typename DVector2<
  TType, 
  std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>::TValueType& 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator[](TIndex index) const 
{
  switch (index) 
  {
  case 0: return this->X;
  case 1: return this->Y;
  default: 
    M_ASSERT_OR_THROW(false, "index must be 0, 1.");
  }
}

template <typename TType>
typename DVector2<
  TType, 
  std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>::TValueType* 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::Data() noexcept
{
  return &this->X;
}

template <typename TType>
const
typename DVector2<
  TType, 
  std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>::TValueType* 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::Data() const noexcept
{
  return &this->Y;
}

template <typename TType>
TReal
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::GetSquareLength() const noexcept
{
  return Cast<TReal>(std::pow(this->X, 2) + std::pow(this->Y, 2));
}

template <typename TType>
TReal
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::GetLength() const noexcept
{
  return std::sqrt(this->GetSquareLength());
}

template <typename TType>
DVector2<TReal> 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::Normalize() const noexcept
{
  const auto length = this->GetLength();
  return DVector2<TReal>{static_cast<TReal>(this->X) / length, static_cast<TReal>(this->Y) / length};
}

template <typename TType>
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>& 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator+=(const DVector2& value) noexcept
{
  this->X += value.X; this->Y += value.Y;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>& 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator-=(const DVector2& value) noexcept
{
  this->X -= value.X; this->Y -= value.Y;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>& 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator*=(TValueType value) noexcept
{
  this->X *= value; this->Y *= value;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>& 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator*=(const DVector2& value) noexcept
{
  this->X *= value.X; this->Y *= value.Y;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>& 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator/=(TValueType value)
{
  this->X /= value; this->Y /= value;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>& 
DVector2<TType, std::enable_if_t<TValueCategory<TType>::kValue == EValueCategory::Integer>>
::operator/=(const DVector2& value)
{
  this->X /= value.X; this->Y /= value.Y;
  return *this;
}

} /// ::dy::math namespace