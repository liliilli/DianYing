#ifndef GUARD_DY_GLOBAL_TYPE_H
#define GUARD_DY_GLOBAL_TYPE_H
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

#include <string>
#include <unordered_map>

//! If you want to use real type as double, remove comment "//" to activate macro option.
//#define MATH_USE_REAL_AS_DOUBLE
#include <Math/Common/TGlobalTypes.h>

#if defined(max) == true
#undef max
#endif

#if defined(min) == true
#undef min
#endif

enum EDySuccess : bool 
{
  DY_FAILURE = false,
  DY_SUCCESS = true
};

//!
//! Types in function.
//!

using ::dy::math::TWChr;
using ::dy::math::TChr8;
using ::dy::math::TChr16;
using ::dy::math::TChr32;

using ::dy::math::TU8;
using ::dy::math::TU16;
using ::dy::math::TU32;
using ::dy::math::TU64;

using ::dy::math::TI8;
using ::dy::math::TI16;
using ::dy::math::TI32;
using ::dy::math::TI64;

using ::dy::math::TF32;
using ::dy::math::TF64;
using ::dy::math::TPointer;
using ::dy::math::TIndex;

using ::dy::math::TReal;
using ::dy::math::kMaxValueOf;
using ::dy::math::kMinValueOf;

/// @brief GLenum has 32bit depth.
/// https://www.khronos.org/opengl/wiki/OpenGL_Type
using TGlEnum = TI32;
using TGlSize = TU32;

template <
    typename TValue,
    typename THash = std::hash<std::string>,
    typename TEqualTo = std::equal_to<std::string>,
    typename TAllocator = std::allocator<std::pair<const std::string, TValue>>
>
using TStringHashMap = std::unordered_map<std::string, TValue, THash, TEqualTo, TAllocator>;

#endif /// GUARD_DY_GLOBAL_TYPE_H