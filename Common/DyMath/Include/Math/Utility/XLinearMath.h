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

#include <Math/Type/Math/DVector2.h>
#include <Math/Type/Math/DVector3.h>
#include <Math/Type/Math/DVector4.h>
#include <Math/Type/Math/DQuat.h>

namespace dy::math
{

/// @brief Do dot product of (x, y) R^2 vector.
/// @return Dot product float value.
template <typename TLeft, typename TRight>
TReal Dot(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs) noexcept;

/// @brief Do dot product of (x, y, z) R^3 vector.
/// @return Dot product float value.
template <typename TLeft, typename TRight>
TReal Dot(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept;

/// @brief Do dot product of (x, y, z, w) R^4 vector.
/// @return Dot product float value.
template <typename TLeft, typename TRight>
TReal Dot(const DVector4<TLeft>& lhs, const DVector4<TRight>& rhs) noexcept;

/// @brief Do dot product of (w, ijk(x, y, z)) Quaternion.
/// @return Dot product float value.
template <typename TLeft, typename TRight>
TReal Dot(const DQuaternion<TLeft>& lhs, const DQuaternion<TRight>& rhs) noexcept;

/// @brief Cross product of (x, y, z) R^3 vector.
/// @return Cross product float value.
template <typename TLeft, typename TRight>
DVector3<GetBiggerType<TLeft, TRight>> 
Cross(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept;

/// @brief Do linear interpolation with plain value type.
/// result = lhs ( 1 - factor ) + rhs
///
/// @return Interpolated value, value type is real type and covers both input type bit. 
template <
  typename TLeft, typename TRight,
  typename = std::enable_if_t<std::is_arithmetic_v<TLeft> && std::is_arithmetic_v<TRight>>
>
GetRealTypeOf<GetBiggerType<TLeft, TRight>>
Lerp(TLeft lhs, TRight right, TReal factor) noexcept
{
  return TReal(lhs) * (1 - factor) + TReal(right) * factor;
}

/// @brief Do linear interpolation with vector2 value type.
/// result = lhs ( 1 - factor ) + rhs
///
/// @return Interpolated vector value, value type is real type and covers both input type bit. 
template <typename TLeft, typename TRight>
DVector2<GetRealTypeOf<GetBiggerType<TLeft, TRight>>>
Lerp(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs, TReal factor) noexcept;

/// @brief Do linear interpolation with vector3 value type.
/// result = lhs ( 1 - factor ) + rhs
///
/// @return Interpolated vector value, value type is real type and covers both input type bit. 
template <typename TLeft, typename TRight>
DVector3<GetRealTypeOf<GetBiggerType<TLeft, TRight>>>
Lerp(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs, TReal factor) noexcept;

/// @brief Do linear interpolation with vector4 value type.
/// result = lhs ( 1 - factor ) + rhs
///
/// @return Interpolated vector value, value type is real type and covers both input type bit. 
template <typename TLeft, typename TRight>
DVector4<GetRealTypeOf<GetBiggerType<TLeft, TRight>>>
Lerp(const DVector4<TLeft>& lhs, const DVector4<TRight>& rhs, TReal factor) noexcept;

/// @brief Get result point through quadratic bezier curve calculation.
template <typename TType>
DVector2<TType>
GetQuadBezierCurvePoint(
  const DVector2<TType>& lhs, 
  const DVector2<TType>& rhs, 
  const DVector2<TType>& control, 
  TReal offset);

/// @brief Get result point through quadratic bezier curve calculation.
template <typename TType>
DVector3<TType>
GetQuadBezierCurvePoint(
  const DVector3<TType>& lhs, 
  const DVector3<TType>& rhs, 
  const DVector3<TType>& control, 
  TReal offset);

template <typename TType>
DQuaternion<TType>
AngleWithAxis(TType angle, const DVector3<TType>& axis, bool isDegree = true);

template <typename TType>
DQuaternion<TType>
Slerp(const DQuaternion<TType>& lhs, const DQuaternion<TType>& rhs, TReal factor);

} /// ::dy::math namespace
#include <Math/Utility/Inline/XLinearMath.inl>