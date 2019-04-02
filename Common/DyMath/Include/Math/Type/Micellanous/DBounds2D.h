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

#include <Math/Common/Inline/TGlobalTypes.inl>
#include <Math/Type/Math/DVector2.h>

namespace dy::math
{

/// @struct DBounds2D
/// @brief 2-element area struct type that has min, max point (DVector2) 
/// as axis aligned bounding box.
template <typename TType, typename TExpression = void> 
struct DBounds2D;

template <typename TType>
struct DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>>;

template <typename TType>
struct DBounds2D<TType, std::enable_if_t<kIsRealType<TType>>>;

} /// ::dy::math namespace

namespace dy::math
{

template <typename TType>
struct MATH_NODISCARD DBounds2D<TType, std::enable_if_t<kIsIntegerType<TType>>> final
{
public:
  using TValueType  = TType;
  using TItem = DVector2<TValueType>;

  DBounds2D() = default;
  DBounds2D(const DVector2<TValueType>& point);
  DBounds2D(const DVector2<TValueType>& point1, const DVector2<TValueType>& point2);

  template <typename TAnotherType>
  explicit operator DBounds2D<TAnotherType>() const noexcept;

  /// @brief Get area of bounding box.
  TReal GetArea() const noexcept;
  /// @brief Get diagonal.
  DVector2<TValueType> GetDiagonal() const noexcept;

  /// @brief Get minimum point of bounding box.
  const DVector2<TValueType>& GetMinimumPoint() const noexcept;
  /// @brief Get maximum point of bounding box.
  const DVector2<TValueType>& GetMaximumPoint() const noexcept;

private:
  DVector2<TValueType> mMin = kMinValueOf<TValueType>;
  DVector2<TValueType> mMax = kMaxValueOf<TValueType>;
};

template <typename TType>
struct MATH_NODISCARD DBounds2D<TType, std::enable_if_t<kIsRealType<TType>>> final
{
public:
  using TValueType  = TType;
  using TItem = DVector2<TValueType>;

  DBounds2D() = default;
  DBounds2D(const DVector2<TValueType>& point);
  DBounds2D(const DVector2<TValueType>& point1, const DVector2<TValueType>& point2);

  template <typename TAnotherType>
  explicit operator DBounds2D<TAnotherType>() const noexcept;

  /// @brief Get area of bounding box.
  TReal GetArea() const noexcept;
  /// @brief Get diagonal.
  DVector2<TValueType> GetDiagonal() const noexcept;

  /// @brief Get minimum point of bounding box.
  const DVector2<TValueType>& GetMinimumPoint() const noexcept;
  /// @brief Get maximum point of bounding box.
  const DVector2<TValueType>& GetMaximumPoint() const noexcept;
  
  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;
  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;
  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

private:
  DVector2<TValueType> mMin = kMinValueOf<TValueType>;
  DVector2<TValueType> mMax = kMaxValueOf<TValueType>;
};

} /// ::dy::math namespace
#include <Math/Type/Inline/DBounds2D/DBounds2DCommon.inl>
#include <Math/Type/Inline/DBounds2D/DBounds2DInt.inl>
#include <Math/Type/Inline/DBounds2D/DBounds2DReal.inl>