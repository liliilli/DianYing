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

#include <Type/DArea2D.h>

namespace dy::math
{

template <typename TType>
DArea2D<TType>::DArea2D(const DVector2<TValueType>& iStartPoint, const DVector2<TValueType>& iLength)
  : mStartPoint{iStartPoint},
    mLength{iLength}
{ }

template <typename TType>
DArea2D<TType>::DArea2D(TValueType x, TValueType y, TValueType width, TValueType height)
  : mStartPoint{DVector2<TValueType>{x, y}},
    mLength{DVector2<TValueType>{width, height}},
{ }

template <typename TType>
template <typename TAnotherType>
DArea2D<TType>::operator DArea2D<TAnotherType>() const noexcept
{
  using TAnotherType = typename DArea2D<TAnotherType>::ValueType;
  using TVectorType = DVector2<TAnotherType>;
  return DArea2D{Cast<TVectorType>(this->mStartPoint), Cast<TVectorType>(this->mLength)};
}

template <typename TType>
TReal DArea2D<TType>::GetArea() const noexcept
{
  return mLength.GetSquareLength();
}

} /// ::dy::math namespace