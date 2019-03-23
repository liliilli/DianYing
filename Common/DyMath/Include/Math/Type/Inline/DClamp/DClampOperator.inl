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

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr DClamp<TType, TStart, TEnd>& 
operator+=(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  a = a + value;
  return a;
}

template <typename TType, TI64 TStart, TI64 TEnd, TI64 TRStart, TI64 TREnd>
constexpr DClamp<TType, TStart, TEnd>& 
operator+=(DClamp<TType, TStart, TEnd>& a, const DClamp<TType, TRStart, TREnd>& value) noexcept
{
  a = a + value;
  return a;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr DClamp<TType, TStart, TEnd>& 
operator-=(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  a = a - value;
  return a;
}

template <typename TType, TI64 TStart, TI64 TEnd, TI64 TRStart, TI64 TREnd>
constexpr DClamp<TType, TStart, TEnd>& 
operator-=(DClamp<TType, TStart, TEnd>& a, const DClamp<TType, TRStart, TREnd>& value) noexcept
{
  a = a - value;
  return a;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr DClamp<TType, TStart, TEnd>& 
operator*=(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  a = a * value;
  return a;
}

template <typename TType, TI64 TStart, TI64 TEnd, TI64 TRStart, TI64 TREnd>
constexpr DClamp<TType, TStart, TEnd>& 
operator*=(DClamp<TType, TStart, TEnd>& a, const DClamp<TType, TRStart, TREnd>& value) noexcept
{
  a = a * value;
  return a;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr DClamp<TType, TStart, TEnd>& 
operator/=(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  a = a / value;
  return a;
}

template <typename TType, TI64 TStart, TI64 TEnd, TI64 TRStart, TI64 TREnd>
constexpr DClamp<TType, TStart, TEnd>& 
operator/=(DClamp<TType, TStart, TEnd>& a, const DClamp<TType, TRStart, TREnd>& value) noexcept
{
  a = a / value;
  return a;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr bool 
operator==(const DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  return a() == value;
}

template <typename TType, TI64 TStart, TI64 TEnd, TI64 TRStart, TI64 TREnd>
constexpr bool 
operator==(const DClamp<TType, TStart, TEnd>& a, const DClamp<TType, TRStart, TREnd>& b) noexcept
{
  return a() == b();
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr bool
operator!=(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  return a() != value;
}

template <typename TType, TI64 TStart, TI64 TEnd, TI64 TRStart, TI64 TREnd>
constexpr bool 
operator!=(const DClamp<TType, TStart, TEnd>& a, const DClamp<TType, TRStart, TREnd>& b) noexcept
{
  return a() != b();
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr bool 
operator>=(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  return a >= value;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr bool 
operator<=(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  return a <= value;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr bool 
operator>(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  return a > value;
}

template <typename TType, TI64 TStart, TI64 TEnd>
constexpr bool 
operator<(DClamp<TType, TStart, TEnd>& a, const TType& value) noexcept
{
  return a < value;
}

} /// ::dy::math namesapce