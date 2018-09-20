#ifndef GUARD_DY_GLOBAL_TYPE_H
#define GUARD_DY_GLOBAL_TYPE_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <string>

//!
//! If you want to use real type as double, remove comment "//" to activate macro option.
//!

//#define MDY_FLAG_USING_REAL_AS_DOUBLE

//!
//! Types in function.
//!

using TC8     = char;
using TC16    = unsigned short;

using TUTF8   = std::string;
using TUTF16  = std::u16string;

using TU08    = uint8_t;
using TU16    = uint16_t;
using TU32    = uint32_t;
using TU64    = uint64_t;

using TI08    = int8_t;
using TI16    = int16_t;
using TI32    = int32_t;
using TI64    = int64_t;

using TF32    = float;
using TF64    = double;

using Real    =
#if defined(MDY_FLAG_USING_REAL_AS_DOUBLE)
TF64;
#else
TF32;
#endif

#endif /// GUARD_DY_GLOBAL_TYPE_H