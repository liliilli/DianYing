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
#include <smmintrin.h>

namespace dy::math
{
 
inline DVector4<TI32> 
operator+(const DVector4<TI32>& lhs, const DVector4<TI32>& rhs) noexcept 
{
  return {_mm_add_epi32(lhs.__mVal, rhs.__mVal)};
}

inline DVector4<TI32>
operator-(const DVector4<TI32>& lhs, const DVector4<TI32>& rhs) noexcept 
{
  return {_mm_sub_epi32(lhs.__mVal, rhs.__mVal)};
}

inline bool 
operator==(const DVector4<TI32>& lhs, const DVector4<TI32>& rhs) noexcept
{
  // Do XOR and if not matched, neq has at least '1' value onto 128 bits.
  const auto neq = _mm_xor_si128(lhs.__mVal, rhs.__mVal);
  return static_cast<bool>(_mm_test_all_zeros(neq, neq));
}

inline DVector4<TI32>& 
DVector4<TI32>::operator+=(const DVector4<TI32>& value) noexcept
{
  this->__mVal = _mm_add_epi32(this->__mVal, value.__mVal);
  return *this;
}

inline DVector4<TI32>& 
DVector4<TI32>::operator-=(const DVector4<TI32>& value) noexcept
{
  this->__mVal = _mm_sub_epi32(this->__mVal, value.__mVal);
  return *this;
}

inline DVector4<TI32>& 
DVector4<TI32>::operator*=(const DVector4<TI32>& value) noexcept
{
  this->__mVal = _mm_mul_epi32(this->__mVal, value.__mVal);
  return *this;
}

inline DVector4<TI32>& 
DVector4<TI32>::operator*=(TI32 value) noexcept
{
  // Mulitply but stores low-bit [31:0] into __mVal.
  this->__mVal = _mm_mullo_epi32(this->__mVal, _mm_set_epi32(value, value, value, value));
  return *this;
}

inline DVector4<TI32>& 
DVector4<TI32>::operator/=(const DVector4<TI32>& value) noexcept
{
  this->X /= value.X;
  this->Y /= value.Y; 
  this->Z /= value.Z; 
  this->W /= value.W;
  return *this;
}

inline DVector4<TI32>& 
DVector4<TI32>::operator/=(TI32 value) noexcept
{
  this->X /= value;
  this->Y /= value;
  this->Z /= value; 
  this->W /= value;
  return *this;
}

}
#endif /// MATH_ENABLE_SIMD
