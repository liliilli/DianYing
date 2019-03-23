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

namespace dy::math
{

template <typename TLeft, typename TRight>
DVector3<GetBiggerType<TLeft, TRight>> 
operator+(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DVector3<TBiggerType>{lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z};
}

template <typename TLeft, typename TRight>
DVector3<GetBiggerType<TLeft, TRight>> 
operator-(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DVector3<TBiggerType>{lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z};
}

/// DVector3<TType> $$ v = (x, y) $$ and value $$ a $$, $$ av $$.
template <
  typename TType, typename TValue, 
  typename = std::enable_if_t<std::is_arithmetic_v<TValue> == true>
>
DVector3<GetBiggerType<TType, TValue>> 
operator*(const DVector3<TType>& lhs, TValue rhs) noexcept
{
  using TBiggerType = GetBiggerType<TType, TValue>;
  return DVector3<TBiggerType>{lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs};
}

template <
  typename TType, typename TValue, 
  typename = std::enable_if_t<std::is_arithmetic_v<TValue> == true>
>
DVector3<GetBiggerType<TType, TValue>> 
operator*(TValue lhs, const DVector3<TType>& rhs) noexcept
{
  using TBiggerType = GetBiggerType<TType, TValue>;
  return DVector3<TBiggerType>{lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z};
}

/// If lhs and rhs are DVector3<TType>, element multiplication happens.
template <typename TLeft, typename TRight>
DVector3<GetBiggerType<TLeft, TRight>> 
operator*(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DVector3<TBiggerType>{lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z};
}

/// If rhs has 0 value, this function just do nothing.
template <
  typename TType, typename TValue, 
  typename = std::enable_if_t<std::is_arithmetic_v<TValue> == true>
>
DVector3<GetBiggerType<TType, TValue>> 
operator/(DVector3<TType> lhs, TValue rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TType, TValue>;
  return DVector3<TBiggerType>{lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs};
}

template <
  typename TType, typename TValue, 
  typename = std::enable_if_t<std::is_arithmetic_v<TValue> == true>
>
DVector3<GetBiggerType<TType, TValue>> 
operator/(TValue lhs, const DVector3<TType>& rhs) noexcept
{
  using TBiggerType = GetBiggerType<TType, TValue>;
  return DVector3<TBiggerType>{lhs / rhs.X, lhs / rhs.Y, lhs / rhs.Z};
}

/// If rhs vector has any 0 value, this function just do nothing.
template <typename TLeft, typename TRight>
DVector3<GetBiggerType<TLeft, TRight>> 
operator/(DVector3<TLeft> lhs, const DVector3<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DVector3<TBiggerType>{lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z};
}

template <typename TType>
bool operator==(const DVector3<TType>& lhs, const DVector3<TType>& rhs) noexcept
{
  return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
}

template <typename TType>
bool operator!=(const DVector3<TType>& lhs, const DVector3<TType>& rhs) noexcept
{
  return !(lhs == rhs);
}

} /// ::dy::math namespace