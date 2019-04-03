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

#include <Math/Utility/XMath.h>

namespace dy::math
{
  
template <typename TLeft, typename TRight>
DBounds3D<GetBiggerType<TLeft, TRight>> 
operator+(const DBounds3D<TLeft>& lhs, const DBounds3D<TRight>& rhs) noexcept 
{
  using TBiggerType = GetBiggerType<TLeft, TRight>;
  return DBounds3D<TBiggerType>{
    ExtractMin(lhs.GetMinimumPoint(), rhs.GetMinimumPoint()), 
    ExtractMax(lhs.GetMaximumPoint(), rhs.GetMaximumPoint())
  };
}

template <typename TType>
bool operator==(const DBounds3D<TType>& lhs, const DBounds3D<TType>& rhs) noexcept
{
  return 
      lhs.GetMinimumPoint() == rhs.GetMinimumPoint()
  &&  lhs.GetMaximumPoint() == rhs.GetMaximumPoint();
}

template <typename TType>
bool operator!=(const DBounds3D<TType>& lhs, const DBounds3D<TType>& rhs) noexcept
{
  return !(lhs == rhs);
}

} /// ::dy::math namespace