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

#ifdef MATH_ENABLE_SIMD
#include <emmintrin.h>

namespace dy::math
{

template <>
struct MATH_NODISCARD DVector4<TI32, void> final
{
  using TValueType = TI32;
  union
  {
    __m128i __mVal;
    struct { TValueType X, Y, Z, W; };
  };

  DVector4() = default;
  DVector4(TValueType x, TValueType y, TValueType z, TValueType w) noexcept;
  DVector4(TValueType value) noexcept;
  DVector4(const DVector2<TValueType>& value, TValueType z = TValueType{}, TValueType w = TValueType{}) noexcept;
  DVector4(const DVector3<TValueType>& value, TValueType w = TValueType{}) noexcept;
  DVector4(__m128i __iSimd) noexcept;

  template <typename TAnotherType>
  explicit operator DVector4<TAnotherType>() const noexcept;
  /// @brief Narrow conversion.
  explicit operator DVector2<TValueType>() const noexcept;
  /// @brief Narrow conversion.
  explicit operator DVector3<TValueType>() const noexcept;

  /// @brief Get values with index. index must be 0, 1 or 2.
  TValueType& operator[](TIndex index);
  /// @brief Get values with index. index must be 0, 1 or 2.
  const TValueType& operator[](TIndex index) const;

  /// @brief Return data chunk pointer of DVector4.
  TValueType* Data() noexcept;
  /// @brief Return data chunk pointer of DVector2.
  const TValueType* Data() const noexcept;

  /// @brief Return squared length of this vector.
  TReal GetSquareLength() const noexcept;
  /// @brief Return the length of this vector.
  TReal GetLength() const noexcept;
  /// @brief Return the length of this vector as homogeneous coordinate position.
  TReal GetHomogeneousLength() const noexcept;

  /// @brief Return new DVector4 instance of normalized input vector.
  DVector4<TReal> Normalize() const noexcept;

  DVector4& operator+=(const DVector4& value) noexcept;
  DVector4& operator-=(const DVector4& value) noexcept;
  DVector4& operator*=(TValueType value) noexcept;
  DVector4& operator*=(const DVector4& value) noexcept;
  DVector4& operator/=(TValueType value) noexcept;
  DVector4& operator/=(const DVector4& value) noexcept;
};

template <typename TAnotherType>
DVector4<TI32, void>::operator DVector4<TAnotherType>() const noexcept
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

inline DVector4<TI32, void>  // NOLINT
::DVector4(TValueType x, TValueType y, TValueType z, TValueType w) noexcept 
  : __mVal{_mm_set_epi32(w, z, y, x)}
{ }

inline DVector4<TI32, void>::DVector4(TValueType value) noexcept  // NOLINT
  : __mVal{_mm_set_epi32(value, value, value, value)}
{ }

inline DVector4<TI32, void> // NOLINT
::DVector4(const DVector2<TValueType>& value, TValueType z, TValueType w) noexcept
  : __mVal{_mm_set_epi32(w, z, value.Y, value.X)}
{ }

inline DVector4<TI32, void> // NOLINT
::DVector4(const DVector3<TValueType>& value, TValueType w) noexcept
  : __mVal{_mm_set_epi32(w, value.Z, value.Y, value.X)}
{ }

inline DVector4<TI32, void>::DVector4(__m128i __iSimd) noexcept // NOLINT
  : __mVal{__iSimd}
{ }

inline DVector4<TI32>::TValueType& 
DVector4<TI32, void>::operator[](TIndex index) // NOLINT
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

inline const DVector4<TI32>::TValueType& 
DVector4<TI32, void>::operator[](TIndex index) const // NOLINT
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

inline DVector4<TI32>::TValueType* 
DVector4<TI32, void>::Data() noexcept
{
  return &this->X;
}

inline const DVector4<TI32>::TValueType* 
DVector4<TI32, void>::Data() const noexcept
{
  return &this->X;
}

} /// ::dy::math namespace
#include "DVector4TI32Common.inl"
#endif /// MATH_ENABLE_SIMD