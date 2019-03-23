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
DVector2<GetBiggerType<TLeft, TRight>> 
operator+(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DVector2<TBiggerType>{lhs.X + rhs.X, lhs.Y + rhs.Y};
}

template <typename TLeft, typename TRight>
DVector2<GetBiggerType<TLeft, TRight>> 
operator-(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DVector2<TBiggerType>{lhs.X - rhs.X, lhs.Y - rhs.Y};
}

/// DVector2<TType> $$ v = (x, y) $$ and value $$ a $$, $$ av $$.
template <
  typename TType, typename TValue, 
  typename = std::enable_if_t<std::is_arithmetic_v<TValue> == true>
>
DVector2<GetBiggerType<TType, TValue>> 
operator*(const DVector2<TType>& lhs, TValue rhs) noexcept
{
  using TBiggerType = GetBiggerType<TType, TValue>;
  return DVector2<TBiggerType>{lhs.X * rhs, lhs.Y * rhs};
}

template <
  typename TType, typename TValue, 
  typename = std::enable_if_t<std::is_arithmetic_v<TValue> == true>
>
DVector2<GetBiggerType<TType, TValue>> 
operator*(TValue lhs, const DVector2<TType>& rhs) noexcept
{
  using TBiggerType = GetBiggerType<TType, TValue>;
  return DVector2<TBiggerType>{lhs * rhs.X, lhs * rhs.Y};
}

/// If lhs and rhs are DVector2<TType>, element multiplication happens.
template <typename TLeft, typename TRight>
DVector2<GetBiggerType<TLeft, TRight>> 
operator*(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DVector2<TBiggerType>{lhs.X * rhs.X, lhs.Y * rhs.Y};
}

/// If rhs has 0 value, this function just do nothing.
template <
  typename TType, typename TValue, 
  typename = std::enable_if_t<std::is_arithmetic_v<TValue> == true>
>
DVector2<GetBiggerType<TType, TValue>> 
operator/(DVector2<TType> lhs, TValue rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TType, TValue>;
  return DVector2<TBiggerType>{lhs.X / rhs, lhs.Y / rhs};
}

template <
  typename TType, typename TValue, 
  typename = std::enable_if_t<std::is_arithmetic_v<TValue> == true>
>
DVector2<GetBiggerType<TType, TValue>> 
operator/(TValue lhs, const DVector2<TType>& rhs) noexcept
{
  using TBiggerType = GetBiggerType<TType, TValue>;
  return DVector2<TBiggerType>{lhs / rhs.X, lhs / rhs.Y};
}

/// If rhs vector has any 0 value, this function just do nothing.
template <typename TLeft, typename TRight>
DVector2<GetBiggerType<TLeft, TRight>> 
operator/(DVector2<TLeft> lhs, const DVector2<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DVector2<TBiggerType>{lhs.X / rhs.X, lhs.Y / rhs.Y};
}

template <typename TType>
bool operator==(const DVector2<TType>& lhs, const DVector2<TType>& rhs) noexcept
{
  return lhs.X == rhs.X && lhs.Y == rhs.Y;
}

template <typename TType>
bool operator!=(const DVector2<TType>& lhs, const DVector2<TType>& rhs) noexcept
{
  return !(lhs == rhs);
}

} /// ::dy::math namespace