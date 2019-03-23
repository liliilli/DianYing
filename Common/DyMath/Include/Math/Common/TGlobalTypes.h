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

#include <cstdint>
#include <limits>
#include <Math/Common/XRemoveMacroes.h>

namespace dy::math
{

using TWChr  = wchar_t;   // A wide character. ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms. 
using TChr8  = char;      // An ansi character. 8-bit fixed-width representation of 7-bit characters.
using TChr16 = char16_t;  // 16-bit representations.
using TChr32 = char32_t;  // 32-bit representations.

using TU8  = uint8_t;   // 8-bit unsigned.  [0, 256)
using TU16 = uint16_t;  // 16-bit unsigend. [0, 65536)
using TU32 = uint32_t;  // 32-bit unsigend. [0, 2^32)
using TU64 = uint64_t;  // 64-bit unsigned. [0, 2^64)

using TI8  = int8_t;    // 8-bit integer.   [-128, 128)
using TI16 = int16_t;   // 16-bit integer.  [-32768, 32768)
using TI32 = int32_t;   // 32-bit integer.  [-2^31, 2^31)
using TI64 = int64_t;   // 64-bit integer.  [-2^63, 2^63)

using TF32 = float;       // 32-bit single precision type.
using TF64 = double;      // 64-bit double precision type.
using TPointer = void*;   // Pointer Address type.
using TIndex   = size_t;  // Index or size type.

//! To use TReal type as double (TF64) type, define MATH_USE_REAL_AS_DOUBLE macro
//! above of math library header file.
using TReal =
#if defined(MATH_USE_REAL_AS_DOUBLE)
  TF64;
#else
  TF32;
#endif

/// @brief Get maximum value of given type.
/// @tparam TType type.
template <typename TType>
constexpr auto kMaxValueOf = std::numeric_limits<TType>::max();

/// @brief Get minimum value of given type.
/// @tparam TType type.
template <typename TType>
constexpr auto kMinValueOf = std::numeric_limits<TType>::lowest();

} /// ::dy::math namespace
#include <Math/Common/Inline/TGlobalTypes.inl>