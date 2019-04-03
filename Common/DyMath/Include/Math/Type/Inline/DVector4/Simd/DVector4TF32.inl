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
#include <smmintrin.h>

namespace dy::math
{

template <>
struct MATH_NODISCARD DVector4<TF32, void> final
{
  using TValueType = TF32;
  union
  {
    __m128 __mVal;
    struct { TValueType X, Y, Z, W; };
  };

  DVector4() = default;
  DVector4(TValueType x, TValueType y, TValueType z, TValueType w) noexcept;
  DVector4(TValueType value) noexcept; 
  DVector4(const DVector2<TValueType>& value, TValueType z = TValueType{}, TValueType w = TValueType{}) noexcept;
  DVector4(const DVector3<TValueType>& value, TValueType w = TValueType{}) noexcept;
  DVector4(__m128 __iSimd) noexcept;

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
  /// @brief Returns the length of this vector.
  TReal GetLength() const noexcept;
  /// @brief Return the length of this vector as homogeneous coordinate position.
  TReal GetHomogeneousLength() const noexcept;

  /// @brief Return new DVector4 instance of normalized input vector.
  DVector4<TReal> Normalize() const noexcept;

  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;
  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;
  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

  DVector4& operator+=(const DVector4& value) noexcept;
  DVector4& operator-=(const DVector4& value) noexcept;
  DVector4& operator*=(TValueType value) noexcept;
  DVector4& operator*=(const DVector4& value) noexcept;
  DVector4& operator/=(TValueType value) noexcept;
  DVector4& operator/=(const DVector4& value) noexcept;
};

template <typename TAnotherType>
DVector4<TF32, void>::operator DVector4<TAnotherType>() const noexcept
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

inline DVector4<TF32, void>  // NOLINT
::DVector4(TValueType x, TValueType y, TValueType z, TValueType w) noexcept
  : __mVal{_mm_set_ps(w, z, y, x)}  // NOLINT
{ }

inline DVector4<TF32, void> // NOLINT
::DVector4(TValueType value) noexcept
  : __mVal{_mm_set_ps1(value)}  
{ }

inline DVector4<TF32, void> // NOLINT
::DVector4(const DVector2<TValueType>& value, TValueType z, TValueType w) noexcept
  : __mVal{_mm_set_ps(w, z, value.Y, value.X)}  
{ }

inline DVector4<float, void> // NOLINT
::DVector4(const DVector3<TValueType>& value, TValueType w) noexcept
  : __mVal{_mm_set_ps(w, value.Z, value.Y, value.X)}  
{ }

inline DVector4<TF32, void> // NOLINT
::DVector4(__m128 __iSimd) noexcept
  : __mVal{__iSimd}
{ }

inline DVector4<TF32>::TValueType& 
DVector4<TF32, void>::operator[](TIndex index) // NOLINT
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

inline const DVector4<TF32>::TValueType& 
DVector4<TF32, void>::operator[](TIndex index) const // NOLINT
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

inline DVector4<TF32>::TValueType* 
DVector4<TF32, void>::Data() noexcept
{
  return &this->X;
}

inline const DVector4<TF32>::TValueType* 
DVector4<TF32, void>::Data() const noexcept
{
  return &this->X;
}

inline TReal 
DVector4<TF32, void>::GetSquareLength() const noexcept
{
  // Do dot product. (SSE4.1)
  const auto result = _mm_dp_ps(this->__mVal, this->__mVal, 0xFF);    
  // Store float into values. (SSE)
  TF32 value[4];
  _mm_store_ps(value, result);                        
  return value[3];
}

inline TReal 
DVector4<TF32, void>::GetLength() const noexcept
{
  return std::sqrt(this->GetSquareLength());
}

inline TReal 
DVector4<TF32, void>::GetHomogeneousLength() const noexcept
{
  TF32 vectorValues[4]; _mm_store_ps(vectorValues, this->__mVal);                        
  // Do dot product. (SSE4.1)
  const auto rejectedWSimd = _mm_set_ps(0, vectorValues[2], vectorValues[1], vectorValues[0]);
  const auto result = _mm_sqrt_ps(_mm_dp_ps(rejectedWSimd, rejectedWSimd, 0xFF));    

#ifndef MATH_USE_REAL_AS_DOUBLE
  // Store float into values. (SSE)
  TF32 value[4]; _mm_store_ps(value, result);                        
  return value[3];
#else
  // Store double into values. (SSE)
  TF64 value[4]; _mm_store_pd(value, _mm_cvtps_pd(result));                        
  return value[3];
#endif
}

inline DVector4<TReal> 
DVector4<TF32, void>::Normalize() const noexcept
{
#ifndef MATH_USE_REAL_AS_DOUBLE
  // Do dot product. (SSE4.1) and sqrt (SSE2) to get length.
  const auto result = _mm_sqrt_ps(_mm_dp_ps(this->__mVal, this->__mVal, 0xFF));    
  return DVector4<TReal>{_mm_div_ps(this->__mVal, result)};
#else
  // Do dot product. (SSE4.1) and sqrt (SSE2) to get length.
  const auto result = _mm_sqrt_pd(_mm_dp_pd(this->__mVal, this->__mVal, 0xFF));    
  return DVector4<TReal>{_mm_div_pd(simd, result)};
#endif
}

inline DVector4<TF32>& 
DVector4<TF32, void>::operator+=(const DVector4& value) noexcept
{
  this->__mVal = _mm_add_ps(this->__mVal, value.__mVal);
  return *this;
}

inline DVector4<TF32>& 
DVector4<TF32, void>::operator-=(const DVector4& value) noexcept
{
  this->__mVal = _mm_sub_ps(this->__mVal, value.__mVal);
  return *this;
}

inline DVector4<TF32>& 
DVector4<TF32, void>::operator*=(TValueType value) noexcept
{
  this->__mVal = _mm_mul_ps(this->__mVal, _mm_set_ps1(value));
  return *this;
}

inline DVector4<TF32>& 
DVector4<float, void>::operator*=(const DVector4& value) noexcept
{
  this->__mVal = _mm_mul_ps(this->__mVal, value.__mVal);
  return *this;
}

inline DVector4<TF32>& 
DVector4<TF32, void>::operator/=(TValueType value) noexcept
{
  this->__mVal = _mm_div_ps(this->__mVal, _mm_set_ps1(value));
  return *this;
}

inline DVector4<TF32>& 
DVector4<TF32, void>::operator/=(const DVector4& value) noexcept
{
  this->__mVal = _mm_div_ps(this->__mVal, value.__mVal);
  return *this;
}

} /// ::dy::math namespace
#include "DVector4TF32Common.inl"
#endif /// MATH_ENABLE_SIMD