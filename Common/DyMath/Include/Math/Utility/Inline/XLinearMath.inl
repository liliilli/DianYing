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

#include <Math/Utility/XLinearMath.h>

namespace dy::math
{

template <typename TLeft, typename TRight>
TReal Dot(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs) noexcept
{
  return Cast<TReal>(lhs.X * rhs.X + lhs.Y * rhs.Y);
}

template <typename TLeft, typename TRight>
TReal Dot(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept
{
  return Cast<TReal>(lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z);
}

template <typename TLeft, typename TRight>
DVector3<GetBiggerType<TLeft, TRight>> 
Cross(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept
{
  // (0) X Y Z 
  // (1) X Y Z => (0Y1Z - 0Z1Y, 0Z1X - 0X1Z, 0X1Y - 0Y1X). 
  return
  {
    lhs.Y * rhs.Z - rhs.Y * lhs.Z,
    lhs.Z * rhs.X - rhs.Z * lhs.X,
    lhs.X * rhs.Y - rhs.X * lhs.Y
  };
}

template <typename TLeft, typename TRight>
DVector2<GetRealTypeOf<GetBiggerType<TLeft, TRight>>>
Lerp(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs, TReal factor) noexcept
{
  return 
  {
    Lerp(lhs.X, rhs.X, factor), 
    Lerp(lhs.Y, rhs.Y, factor)
  };
}

template <typename TLeft, typename TRight>
DVector3<GetRealTypeOf<GetBiggerType<TLeft, TRight>>>
Lerp(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs, TReal factor) noexcept
{
  return 
  {
    Lerp(lhs.X, rhs.X, factor), 
    Lerp(lhs.Y, rhs.Y, factor), 
    Lerp(lhs.Z, rhs.Z, factor)
  };
}

template <typename TLeft, typename TRight>
DVector4<GetRealTypeOf<GetBiggerType<TLeft, TRight>>>
Lerp(const DVector4<TLeft>& lhs, const DVector4<TRight>& rhs, TReal factor) noexcept
{
  return 
  {
    Lerp(lhs.X, rhs.X, factor), 
    Lerp(lhs.Y, rhs.Y, factor), 
    Lerp(lhs.Z, rhs.Z, factor),
    Lerp(lhs.W, rhs.W, factor),
  };
}

template <typename TType>
DVector2<TType> 
GetQuadBezierCurvePoint(
  const DVector2<TType>& lhs, 
  const DVector2<TType>& rhs,
  const DVector2<TType>& control, TReal offset)
{
  return Lerp(Lerp(lhs, control, offset), Lerp(control, rhs, offset), offset);
}

template <typename TType>
DVector3<TType> 
GetQuadBezierCurvePoint(
  const DVector3<TType>& lhs,
  const DVector3<TType>& rhs,
  const DVector3<TType>& control, 
  TReal offset)
{
  return Lerp(Lerp(lhs, control, offset), Lerp(control, rhs, offset), offset);
}

template <typename TType>
inline DQuaternion<TType> 
AngleWithAxis(TType angle, const DVector3<TType>& axis, bool isDegree)
{
  const auto s = std::sin(angle * (isDegree ? kToRadian<TType> : 1) * TType(0.5));
  const auto w = std::cos(angle * (isDegree ? kToRadian<TType> : 1) * TType(0.5));
  const auto x = axis.X * s;
  const auto y = axis.Y * s;
  const auto z = axis.Z * s;
  return {x, y, z, w};
}

} /// ::dy::math namespace