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

#include <algorithm>

namespace dy::math
{

template <typename TType>
DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DBounds2D(const DVector2<TValueType>& point)
  : mMin{point}, mMax{point}
{ }

template <typename TType>
DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DBounds2D(const DVector2<TValueType>& point1, const DVector2<TValueType>& point2)
  : mMin{std::min(point1.X, point2.X), std::min(point1.Y, point2.Y)},
    mMax{std::max(point1.X, point2.X), std::max(point1.Y, point2.Y)}
{ }

template <typename TType>
template <typename TAnotherType>
DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator DBounds2D<TAnotherType>() const noexcept
{
  return {
    static_cast<DVector2<TAnotherType>>(this->GetMinimumPoint()), 
    static_cast<DVector2<TAnotherType>>(this->GetMaximumPoint())
  };
}

template <typename TType>
TReal DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetArea() const noexcept
{
  const auto diagonal = this->GetDiagonal();
  return static_cast<TReal>(diagonal.X * diagonal.Y);
}

template <typename TType>
const DVector2<TType>& DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetMinimumPoint() const noexcept
{
  return this->mMin;
}

template <typename TType>
const DVector2<typename DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType>& 
DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetMaximumPoint() const noexcept
{
  return this->mMax;
}

template <typename TType>
DVector2<typename DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType> 
DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetDiagonal() const noexcept
{
  return this->mMax - this->mMin;
}

} /// ::dy::math namespace
