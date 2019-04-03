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
DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DBounds3D(const DVector3<TValueType>& point)
  : mMin{point}, mMax{point}
{ }

template <typename TType>
DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::DBounds3D(const DVector3<TValueType>& point1, const DVector3<TValueType>& point2)
  : mMin{ExtractMin(point1, point2)},
    mMax{ExtractMax(point1, point2)}
{ }

template <typename TType>
template <typename TAnotherType>
DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::operator DBounds3D<TAnotherType>() const noexcept
{
  return {
    static_cast<DVector3<TAnotherType>>(this->GetMinimumPoint()), 
    static_cast<DVector3<TAnotherType>>(this->GetMaximumPoint())
  };
}

template <typename TType>
TReal DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetArea() const noexcept
{
  const auto diagonal = this->GetDiagonal();
  return static_cast<TReal>(diagonal.X * diagonal.Y * diagonal.Z);
}

template <typename TType>
const DVector3<TType>& DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetMinimumPoint() const noexcept
{
  return this->mMin;
}

template <typename TType>
const DVector3<typename DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType>& 
DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetMaximumPoint() const noexcept
{
  return this->mMax;
}

template <typename TType>
DVector3<typename DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>::TValueType> 
DBounds3D<TType, std::enable_if_t<kIsIntegerType<TType>>>
::GetDiagonal() const noexcept
{
  return this->mMax - this->mMin;
}

} /// ::dy::math namespace
