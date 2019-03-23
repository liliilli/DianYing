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
bool IsNearlyEqual(const DVector2<TLeft>& lhs, const DVector2<TRight>& rhs, TReal tolerance)
{
  using TCheckType = GetRealTypeOf<GetBiggerType<TLeft, TRight>>;

  if (std::abs(TCheckType(lhs.X - rhs.X)) > tolerance) { return false; }
  if (std::abs(TCheckType(lhs.Y - rhs.Y)) > tolerance) { return false; }
  return true;
}

template <typename TLeft, typename TRight>
bool IsNearlyEqual(const DVector3<TLeft>& lhs, const DVector3<TRight>& rhs, TReal tolerance)
{
  using TCheckType = GetRealTypeOf<GetBiggerType<TLeft, TRight>>;

  if (std::abs(TCheckType(lhs.X - rhs.X)) > tolerance) { return false; }
  if (std::abs(TCheckType(lhs.Y - rhs.Y)) > tolerance) { return false; }
  if (std::abs(TCheckType(lhs.Z - rhs.Z)) > tolerance) { return false; }
  return true;
}

template <typename TLeft, typename TRight>
bool IsNearlyEqual(const DVector4<TLeft>& lhs, const DVector4<TRight>& rhs, TReal tolerance)
{
  using TCheckType = GetRealTypeOf<GetBiggerType<TLeft, TRight>>;

  if (std::abs(TCheckType(lhs.X - rhs.X)) > tolerance) { return false; }
  if (std::abs(TCheckType(lhs.Y - rhs.Y)) > tolerance) { return false; }
  if (std::abs(TCheckType(lhs.Z - rhs.Z)) > tolerance) { return false; }
  if (std::abs(TCheckType(lhs.W - rhs.W)) > tolerance) { return false; }
  return true;
}

} /// ::dy::math namespace
