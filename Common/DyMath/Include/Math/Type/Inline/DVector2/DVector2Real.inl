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
#include <Math/Common/XGlobalMacroes.h>
#include <Math/Common/XGlobalUtilites.h>

namespace dy::math
{

template <typename TType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::DVector2(TValueType x, TValueType y) noexcept
  : X{x}, Y{y}
{ }

template <typename TType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::DVector2(TValueType value) noexcept
  : X{value}, Y{value} 
{ }

template <typename TType>
template <typename TAnotherType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::operator DVector2<TAnotherType>() const noexcept
{
  using AnotherType = typename DVector2<
    TAnotherType, 
    std::enable_if_t<
        TValueCategory<TAnotherType>::kValue == EValueCategory::Signed
    ||  TValueCategory<TAnotherType>::kValue == EValueCategory::Real>>::TValueType; 

  return DVector2<TAnotherType>
  {
    Cast<AnotherType>(this->X), 
    Cast<AnotherType>(this->Y)
  };
}

template <typename TType>
typename DVector2<TType, std::enable_if_t<kIsRealType<TType>>>::TValueType& 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
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
const typename DVector2<TType, std::enable_if_t<kIsRealType<TType>>>::TValueType& 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
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
typename DVector2<TType, std::enable_if_t<kIsRealType<TType>>>::TValueType* 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::Data() noexcept
{
  return &this->X;
}

template <typename TType>
const typename DVector2<TType, std::enable_if_t<kIsRealType<TType>>>::TValueType* 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::Data() const noexcept
{
  return &this->Y;
}

template <typename TType>
TReal DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::GetSquareLength() const noexcept
{
  return Cast<TReal>(std::pow(this->X, 2) + std::pow(this->Y, 2));
}

template <typename TType>
TReal DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::GetLength() const noexcept
{
  return std::sqrt(this->GetSquareLength());
}

template <typename TType>
DVector2<TReal> 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::Normalize() const noexcept
{
  const auto length = this->GetLength();
  return DVector2<TReal>{static_cast<TReal>(this->X) / length, static_cast<TReal>(this->Y) / length};
}

template <typename TType>
bool DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::HasNaN() const noexcept
{
  return std::isnan(this->X) || std::isnan(this->Y);
}

template <typename TType>
bool 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::HasInfinity() const noexcept
{
  return std::isinf(this->X) || std::isinf(this->Y);
}

template<typename TType>
bool 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::HasOnlyNormal() const noexcept
{
  return std::isnormal(this->X) && std::isnormal(this->Y);
}

template <typename TType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>& 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::operator+=(const DVector2& value) noexcept
{
  this->X += value.X; this->Y += value.Y;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>& 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::operator-=(const DVector2& value) noexcept
{
  this->X -= value.X; this->Y -= value.Y;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>& 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::operator*=(TValueType value) noexcept
{
  this->X *= value; this->Y *= value;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>& 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::operator*=(const DVector2& value) noexcept
{
  this->X *= value.X; this->Y *= value.Y;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>& 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::operator/=(TValueType value)
{
  this->X /= value; this->Y /= value;
  return *this;
}

template <typename TType>
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>& 
DVector2<TType, std::enable_if_t<kIsRealType<TType>>>
::operator/=(const DVector2& value)
{
  this->X /= value.X; this->Y /= value.Y;
  return *this;
}

} /// ::dy::math namespace