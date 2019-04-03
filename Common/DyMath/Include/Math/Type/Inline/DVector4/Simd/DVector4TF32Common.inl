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
 
inline DVector4<TF32>
operator+(const DVector4<TF32>& lhs, const DVector4<TF32>& rhs) noexcept 
{
  return {_mm_add_ps(lhs.__mVal, rhs.__mVal)};
}

inline DVector4<TF32>
operator-(const DVector4<TF32>& lhs, const DVector4<TF32>& rhs) noexcept 
{
  return {_mm_sub_ps(lhs.__mVal, rhs.__mVal)};
}

inline DVector4<TF32>
operator*(const DVector4<TF32>& lhs, TF32 rhs) noexcept
{
  return {_mm_mul_ps(lhs.__mVal, _mm_set_ps1(rhs))};
}

inline DVector4<TF32>
operator*(TF32 lhs, const DVector4<TF32>& rhs) noexcept
{
  return rhs * lhs;
}

inline DVector4<TF32>
operator*(const DVector4<TF32>& lhs, const DVector4<TF32>& rhs) noexcept 
{
  return {_mm_mul_ps(lhs.__mVal, rhs.__mVal)};
}

inline DVector4<TF32>
operator/(DVector4<TF32> lhs, TF32 rhs) noexcept 
{
  return {_mm_div_ps(lhs.__mVal, _mm_set_ps1(rhs))};
}

inline DVector4<TF32>
operator/(TF32 lhs, const DVector4<TF32>& rhs) noexcept
{
  return {_mm_div_ps(_mm_set_ps1(lhs), rhs.__mVal)};
}

inline DVector4<TF32>
operator/(DVector4<TF32> lhs, const DVector4<TF32>& rhs) noexcept 
{
  return {_mm_div_ps(lhs.__mVal, rhs.__mVal)};
}

inline bool 
operator==(const DVector4<TF32>& lhs, const DVector4<TF32>& rhs) noexcept
{
  return !(_mm_movemask_ps(_mm_cmpeq_ps(lhs.__mVal, rhs.__mVal)) != 0xF);
}

} /// ::dy::math namespace
#endif /// MATH_ENABLE_SIMD