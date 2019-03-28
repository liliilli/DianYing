#ifndef GUARD_DY_HELPER_MATH_MATH_H
#define GUARD_DY_HELPER_MATH_MATH_H
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

#include <Dy/Helper/Type/DVector3.h>

//!
//! Forward declaration
//!

namespace dy
{
class DQuaternion;
}

//!
//! Implementation
//!

#ifdef max
#undef max
#endif

namespace dy::math 
{

/// @brief Do sphere-linear interpolation with DQuaternion.
DQuaternion Slerp(const DQuaternion& lhs, const DQuaternion& rhs, TF32 offset) noexcept;

/// @brief Convert quaternion to radian degree.
[[nodiscard]] DVec3 ConvertQuaternionToRadianEuler(const DQuaternion& iQuat);
/// @brief Convert quaterion w,x,y,z to radian degree.
[[nodiscard]] DVec3 ConvertQuaternionToRadianEuler(TF32 w, TF32 x, TF32 y, TF32 z);

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_MATH_MATH_H