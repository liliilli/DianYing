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
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DVector4(TValueType x, TValueType y, TValueType z, TValueType w) noexcept
  : X{x}, Y{y}, Z{z}, W{w}
{ }

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DVector4(TValueType value) noexcept
  : X{value}, Y{value}, Z{value}, W{value}
{ }

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DVector4(const DVector2<TValueType>& value, TValueType z, TValueType w) noexcept
  : X{value.X}, Y{value.Y}, Z{z}, W{w} 
{ }

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DVector4(const DVector3<TValueType>& value, TValueType w) noexcept
  : X{value.X}, Y{value.Y}, Z{value.Z}, W{w}
{ };

template <typename TType>
template <typename TAnotherType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator DVector4<TAnotherType>() const noexcept
{
  using AnotherType = typename DVector4<
    TAnotherType, 
    std::enable_if_t<kIsIntegerType<TAnotherType> || kIsRealType<TAnotherType>>>::TValueType; 

  return DVector4<TAnotherType>
  {
    Cast<AnotherType>(this->X), 
    Cast<AnotherType>(this->Y), 
    Cast<AnotherType>(this->Z),
    Cast<AnotherType>(this->W)
  };
}

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator DVector2<TType>() const noexcept
{
  return DVector2<TType>{this->X, this->Y};
}

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator DVector3<TType>() const noexcept
{
  return DVector3<TType>{this->X, this->Y, this->Z};
}

template <typename TType>
typename DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType& 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator[](TIndex index)
{
  switch (index)
  {
  case 0: return this->X;
  case 1: return this->Y;
  case 2: return this->Z;
  case 3: return this->W;
  default: 
    M_ASSERT_OR_THROW(false, "index must be 0, 1, 2 and 3.");
  }
}

template <typename TType>
const typename DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType& 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator[](TIndex index) const
{
  switch (index)
  {
  case 0: return this->X;
  case 1: return this->Y;
  case 2: return this->Z;
  case 3: return this->W;
  default: 
    M_ASSERT_OR_THROW(false, "index must be 0, 1, 2 and 3.");
  }
}

template <typename TType>
typename DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType* 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::Data() noexcept
{
  return &this->X;
}

template <typename TType>
const typename DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType* 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::Data() const noexcept
{
  return &this->X;
}

template <typename TType>
TReal
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetSquareLength() const noexcept
{
  return Cast<TReal>(
    std::pow(this->X, 2) 
  + std::pow(this->Y, 2) 
  + std::pow(this->Z, 2)
  + std::pow(this->W, 2));
}

template <typename TType>
TReal
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetLength() const noexcept
{
  return std::sqrt(this->GetSquareLength());
}

template <typename TType>
TReal DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetHomogeneousLength() const noexcept
{
  const auto w = Cast<TReal>(this->W);
  return DVector3<TReal>{this->X / w, this->Y / w, this->Z / w}.GetLength();
}

template <typename TType>
DVector4<TReal> 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::Normalize() const noexcept
{
  const auto length = this->GetLength();
  return 
  {
    static_cast<TReal>(this->X) / length, 
    static_cast<TReal>(this->Y) / length,  
    static_cast<TReal>(this->Z) / length,
    static_cast<TReal>(this->W) / length
  };  
}

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>& 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator+=(const DVector4& value) noexcept
{
  this->X += value.X; this->Y += value.Y; this->Z += value.Z; this->W += value.W;
  return *this;
}

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>& 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator-=(const DVector4& value) noexcept
{
  this->X -= value.X; this->Y -= value.Y; this->Z -= value.Z; this->W -= value.W;
  return *this;
}

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>&
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator*=(TValueType value) noexcept
{
  this->X *= value; this->Y *= value; this->Z *= value; this->W *= value;
  return *this;
}

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>& 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator*=(const DVector4& value) noexcept
{
  this->X *= value.X; this->Y *= value.Y; this->Z *= value.Z; this->W *= value.W;
  return *this;
}

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>&
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator/=(TValueType value) noexcept
{
  this->X /= value; this->Y /= value; this->Z /= value; this->W /= value;
  return *this;
}

template <typename TType>
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>& 
DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator/=(const DVector4& value) noexcept
{
  this->X /= value.X; this->Y /= value.Y; this->Z /= value.Z; this->W /= value.W;
  return *this;
}

} /// ::dy::math namespace