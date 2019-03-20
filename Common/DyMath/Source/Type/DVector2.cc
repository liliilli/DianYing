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

/// Header file
#include <Type/DVector2.h>

namespace dy::math
{

//! Explicit specialization to use outside world. 

template <> struct DVector2<TU8>;
template <> struct DVector2<TU16>;
template <> struct DVector2<TU32>;
template <> struct DVector2<TU64>;

template <> struct DVector2<TI8>;
template <> struct DVector2<TI16>;
template <> struct DVector2<TI32>;
template <> struct DVector2<TI64>;

#if defined(M_USE_REAL_AS_DOUBLE) == true
template <> struct DVector2<TF32>;
#else
template <> struct DVector2<TF64>;
#endif

} /// ::dy::math namespace