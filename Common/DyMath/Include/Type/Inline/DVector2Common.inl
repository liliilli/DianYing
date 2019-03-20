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

template <typename TType>
DVector2<TType> operator+(const DVector2<TType>& lhs, const DVector2<TType>& rhs) noexcept 
{
  return DVector2<TType>{lhs.X + rhs.X, lhs.Y + rhs.Y};
}

template <typename TType>
DVector2<TType> operator-(const DVector2<TType>& lhs, const DVector2<TType>& rhs) noexcept 
{
  return DVector2<TType>{lhs.X - rhs.X, lhs.Y - rhs.Y};
}

/// DVector2<TType> $$ v = (x, y) $$ and value $$ a $$, $$ av $$.
template <typename TType, typename TValue>
auto operator*(const DVector2<TType>& lhs, TValue rhs) noexcept -> DVector2<TType>
{
  using TValueType = decltype(lhs)::TValueType;
  if constexpr (TValueCategory<TValue>::kValue == EValueCategory::Real)
  {

  }

  return DVector2<TType>{lhs.X * rhs, lhs.Y * rhs};
}

/// If lhs and rhs are DVector2<TType>, element multiplication happens.
template <typename TType>
DVector2<TType> operator*(const DVector2<TType>& lhs, const DVector2<TType>& rhs) noexcept 
{
  return DVector2<TType>{lhs.X * rhs.X, lhs.Y * rhs.Y};
}

/// If rhs has 0 value, this function just do nothing.
template <typename TType>
DVector2<TType> operator/(DVector2<TType> lhs, const float rhs) noexcept 
{
  return DVector2<TType>{lhs.X / rhs, lhs.Y / rhs};
}

/// If rhs vector has any 0 value, this function just do nothing.
template <typename TType>
DVector2<TType> operator/(DVector2<TType> lhs, const DVector2<TType>& rhs) noexcept 
{
  return DVector2<TType>{lhs.X / rhs.X, lhs.Y / rhs.Y};
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