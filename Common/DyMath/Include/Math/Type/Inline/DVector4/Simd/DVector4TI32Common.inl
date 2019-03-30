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
  const auto neq = _mm_xor_si128(lhs.__mVal, rhs.__mVal);
  return static_cast<bool>(_mm_test_all_zeros(neq, neq));
}

}
#endif /// MATH_ENABLE_SIMD
