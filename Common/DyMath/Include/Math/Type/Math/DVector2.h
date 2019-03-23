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

#include <Math/Common/TGlobalTypes.h>
#include <Math/Common/XGlobalMacroes.h>

//!
//! Forward declaration 
//!

namespace dy::math
{

/// @struct DVector2
/// @brief 2-element vector struct type.
template <typename TType, typename TExpression = void> 
struct DVector2;

template <typename TType>
struct DVector2<TType, std::enable_if_t<kIsIntegerType<TType>>>;

template <typename TType>
struct DVector2<TType, std::enable_if_t<kIsRealType<TType>>>;

} /// ::dy::math namespace

//!
//! Implementation
//!

namespace dy::math
{

template <typename TType>
struct MATH_NODISCARD DVector2<TType, std::enable_if_t<kIsIntegerType<TType>>> final
{
  using TValueType = TType;
  TValueType X, Y;

  DVector2() = default;
  DVector2(TValueType x, TValueType y) noexcept;
  DVector2(TValueType value) noexcept;

  template <typename TAnotherType>
  explicit operator DVector2<TAnotherType>() const noexcept;

  /// @brief Get values with index. index must be 0 or 1.
  TValueType& operator[](TIndex index);
  /// @brief Get values with index. index must be 0 or 1.
  const TValueType& operator[](TIndex index) const;

  /// @brief Return data chunk pointer of DVector2.
  TValueType* Data() noexcept;
  /// @brief Return data chunk pointer of DVector2.
  const TValueType* Data() const noexcept;

  /// @brief Return squared length of this vector.
  TReal GetSquareLength() const noexcept;
  /// @brief Returns the length of this vector.
  TReal GetLength() const noexcept;

  /// @brief Return new DVector2 instance of normalized input vector.
  /// @return Normalized DVector2 vector.
  DVector2<TReal> Normalize() const noexcept;

  DVector2& operator+=(const DVector2& value) noexcept;
  DVector2& operator-=(const DVector2& value) noexcept;
  DVector2& operator*=(TValueType value) noexcept;
  DVector2& operator*=(const DVector2& value) noexcept;
  DVector2& operator/=(TValueType value);
  DVector2& operator/=(const DVector2& value);
};

template <typename TType>
struct MATH_NODISCARD DVector2<TType, std::enable_if_t<kIsRealType<TType>>> final
{
  using TValueType = TType;
  TValueType X, Y;

  DVector2() = default;
  DVector2(TValueType x, TValueType y) noexcept;
  DVector2(TValueType value) noexcept;

  template <typename TAnotherType>
  explicit operator DVector2<TAnotherType>() const noexcept;

  /// @brief Get values with index. index must be 0 or 1.
  TValueType& operator[](TIndex index);
  /// @brief Get values with index. index must be 0 or 1.
  const TValueType& operator[](TIndex index) const;

  /// @brief Return data chunk pointer of DVector2.
  TValueType* Data() noexcept;
  /// @brief Return data chunk pointer of DVector2.
  const TValueType* Data() const noexcept;

  /// @brief Return squared length of this vector.
  TReal GetSquareLength() const noexcept;
  /// @brief Returns the length of this vector.
  TReal GetLength() const noexcept;

  /// @brief Return new DVector2 instance of normalized input vector.
  /// @return Normalized DVector2 vector.
  DVector2<TReal> Normalize() const noexcept;

  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;
  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;
  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

  DVector2& operator+=(const DVector2& value) noexcept;
  DVector2& operator-=(const DVector2& value) noexcept;
  DVector2& operator*=(TValueType value) noexcept;
  DVector2& operator*=(const DVector2& value) noexcept;
  DVector2& operator/=(TValueType value);
  DVector2& operator/=(const DVector2& value);
};

} /// ::dy::math namespace
#include <Math/Type/Inline/DVector2/DVector2Int.inl>
#include <Math/Type/Inline/DVector2/DVector2Real.inl>
#include <Math/Type/Inline/DVector2/DVector2Common.inl>
