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

#include <Type/DVector2.h>

namespace dy::math
{

/// @struct DArea2D
/// @brief Area 2D (x, y) (start point) and width, height value types.
template <typename TType>
struct DArea2D final
{
  using TValueType = TType;
  DVector2<TValueType> mStartPoint = kMinValueOf<TValueType>;
  DVector2<TValueType> mLength = kMaxValueOf<TValueType>; // RangeOf may lead to overflow.

  DArea2D() = default;
  explicit DArea2D(const DVector2<TValueType>& iStartPoint, const DVector2<TValueType>& iLength);
  explicit DArea2D(TValueType x, TValueType y, TValueType width, TValueType height);

  template <typename TAnotherType>
  explicit operator DArea2D<TAnotherType>() const noexcept;

  /// @brief Get area of instance.
  TReal GetArea() const noexcept;
};

} /// ::dy::math namespace
#include <Type/Inline/DArea2D.inl>