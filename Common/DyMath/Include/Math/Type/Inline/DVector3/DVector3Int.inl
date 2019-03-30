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

namespace dy::math
{

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DVector3(TValueType x, TValueType y, TValueType z) noexcept
  : X{x}, Y{y}, Z{z}
{ }

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DVector3(TValueType value) noexcept
  : X{value}, Y{value}, Z{value} 
{ }

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DVector3(const DVector2<TValueType>& value) noexcept
  : X{value.X}, Y{value.Y}, Z{TValueType{}} 
{ };

template <typename TType>
template <typename TAnotherType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator DVector3<TAnotherType>() const noexcept
{
  using AnotherType = typename DVector3<
    TAnotherType, 
    std::enable_if_t<kIsIntegerType<TAnotherType> || kIsRealType<TAnotherType>>>::TValueType; 

  return DVector3<TAnotherType>
  {
    Cast<AnotherType>(this->X), Cast<AnotherType>(this->Y), Cast<AnotherType>(this->Z)
  };
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator DVector2<TType>() const noexcept
{
  return DVector2<TType>{this->X, this->Y};
}

template <typename TType>
typename DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType& 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator[](TIndex index)
{
  switch (index)
  {
  case 0: return this->X;
  case 1: return this->Y;
  case 2: return this->Z;
  default: 
    M_ASSERT_OR_THROW(false, "index must be 0, 1 and 2.");
  }
}

template <typename TType>
const typename DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType& 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator[](TIndex index) const
{
  switch (index)
  {
  case 0: return this->X;
  case 1: return this->Y;
  case 2: return this->Z;
  default: 
    M_ASSERT_OR_THROW(false, "index must be 0, 1 and 2.");
  }
}

template <typename TType>
typename DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType* 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::Data() noexcept
{
  return &this->X;
}

template <typename TType>
const typename DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType* 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::Data() const noexcept
{
  return &this->X;
}

template <typename TType>
TReal
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetSquareLength() const noexcept
{
  return Cast<TReal>(std::pow(this->X, 2) + std::pow(this->Y, 2) + std::pow(this->Z, 2));
}

template <typename TType>
TReal
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetLength() const noexcept
{
  return std::sqrt(this->GetSquareLength());
}

template <typename TType>
DVector3<TReal> 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::Normalize() const noexcept
{
  const auto length = this->GetLength();
  return DVector3<TReal>{
    static_cast<TReal>(this->X) / length, 
    static_cast<TReal>(this->Y) / length,  
    static_cast<TReal>(this->Z) / length
  };  
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>& 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator+=(const DVector3& value) noexcept
{
  this->X += value.X; this->Y += value.Y; this->Z += value.Z;
  return *this;
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>& 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator-=(const DVector3& value) noexcept
{
  this->X -= value.X; this->Y -= value.Y; this->Z -= value.Z;
  return *this;
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>&
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator*=(TValueType value) noexcept
{
  this->X *= value; this->Y *= value; this->Z *= value;
  return *this;
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>& 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator*=(const DVector3& value) noexcept
{
  this->X *= value.X; this->Y *= value.Y; this->Z *= value.Z;
  return *this;
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>&
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator/=(TValueType value) noexcept
{
  this->X /= value; this->Y /= value; this->Z /= value;
  return *this;
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>& 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator/=(const DVector3& value) noexcept
{
  this->X /= value.X; this->Y /= value.Y; this->Z /= value.Z;
  return *this;
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>> 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>::UnitX() noexcept
{
  static DVector3 ret{1, 0, 0};
  return ret;
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>> 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>::UnitY() noexcept
{
  static DVector3 ret{0, 1, 0};
  return ret;
}

template <typename TType>
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>> 
DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>::UnitZ() noexcept
{
  static DVector3 ret{0, 0, 1};
  return ret;
}

} /// ::dy::math namespace