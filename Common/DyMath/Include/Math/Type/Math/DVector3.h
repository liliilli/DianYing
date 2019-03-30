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
#include <Math/Common/XGlobalMacroes.h>
#include <Math/Type/Math/DVector2.h>

//!
//! Forward declaration 
//!

namespace dy::math
{

/// @struct DVector3
/// @brief 3-element vector struct type.
template <typename TType, typename TExpression = void> 
struct DVector3;

template <typename TType>
struct DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>>;

template <typename TType>
struct DVector3<TType, std::enable_if_t<kIsRealType<TType>>>;

} /// ::dy::math namespace

//!
//! Implementation
//!

namespace dy::math 
{

template <typename TType>
struct MATH_NODISCARD DVector3<TType, std::enable_if_t<kIsIntegerType<TType>>> final
{
  using TValueType = TType;
  TValueType X, Y, Z;

  DVector3() = default;
  DVector3(TValueType x, TValueType y, TValueType z) noexcept;
  DVector3(TValueType value) noexcept; 
  DVector3(const DVector2<TValueType>& value) noexcept;

  template <typename TAnotherType>
  explicit operator DVector3<TAnotherType>() const noexcept;

  /// @brief Narrow conversion.
  explicit operator DVector2<TValueType>() const noexcept;

  /// @brief Get values with index. index must be 0, 1 or 2.
  TValueType& operator[](TIndex index);
  /// @brief Get values with index. index must be 0, 1 or 2.
  const TValueType& operator[](TIndex index) const;

  /// @brief Return data chunk pointer of DVector3.
  TValueType* Data() noexcept;
  /// @brief Return data chunk pointer of DVector2.
  const TValueType* Data() const noexcept;

  /// @brief Return squared length of this vector.
  TReal GetSquareLength() const noexcept;
  /// @brief Returns the length of this vector.
  TReal GetLength() const noexcept;

  /// @brief Return new DVector3 instance of normalized input vector.
  DVector3<TReal> Normalize() const noexcept;

  DVector3& operator+=(const DVector3& value) noexcept;
  DVector3& operator-=(const DVector3& value) noexcept;
  DVector3& operator*=(TValueType value) noexcept;
  DVector3& operator*=(const DVector3& value) noexcept;
  DVector3& operator/=(TValueType value) noexcept;
  DVector3& operator/=(const DVector3& value) noexcept;

  /// @brief Return {1, 0, 0} right DVector3 vector.
  static DVector3 UnitX() noexcept;
  /// @brief Return {0, 1, 0} up DVector3 vector.
  static DVector3 UnitY() noexcept;
  /// @brief Return {0, 0, 1} front DVector3 vector.
  static DVector3 UnitZ() noexcept;
};

template <typename TType>
struct MATH_NODISCARD DVector3<TType, std::enable_if_t<kIsRealType<TType>>> final
{
  using TValueType = TType;
  TValueType X, Y, Z;

  DVector3() = default;
  DVector3(TValueType x, TValueType y, TValueType z) noexcept;
  DVector3(TValueType value) noexcept; 
  DVector3(const DVector2<TValueType>& value) noexcept;

  template <typename TAnotherType>
  explicit operator DVector3<TAnotherType>() const noexcept;

  /// @brief Narrow conversion.
  explicit operator DVector2<TValueType>() const noexcept;

  /// @brief Get values with index. index must be 0, 1 or 2.
  TValueType& operator[](TIndex index);
  /// @brief Get values with index. index must be 0, 1 or 2.
  const TValueType& operator[](TIndex index) const;

  /// @brief Return data chunk pointer of DVector3.
  TValueType* Data() noexcept;
  /// @brief Return data chunk pointer of DVector2.
  const TValueType* Data() const noexcept;

  /// @brief Return squared length of this vector.
  TReal GetSquareLength() const noexcept;
  /// @brief Returns the length of this vector.
  TReal GetLength() const noexcept;

  /// @brief Return new DVector3 instance of normalized input vector.
  DVector3<TReal> Normalize() const noexcept;

  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;
  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;
  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

  DVector3& operator+=(const DVector3& value) noexcept;
  DVector3& operator-=(const DVector3& value) noexcept;
  DVector3& operator*=(TValueType value) noexcept;
  DVector3& operator*=(const DVector3& value) noexcept;
  DVector3& operator/=(TValueType value) noexcept;
  DVector3& operator/=(const DVector3& value) noexcept;

  /// @brief Return {1, 0, 0} right DVector3 vector.
  static DVector3 UnitX() noexcept;
  /// @brief Return {0, 1, 0} up DVector3 vector.
  static DVector3 UnitY() noexcept;
  /// @brief Return {0, 0, 1} front DVector3 vector.
  static DVector3 UnitZ() noexcept;
};

} /// ::dy::math namespace
#include <Math/Type/Inline/DVector3/DVector3Common.inl>
#include <Math/Type/Inline/DVector3/DVector3Int.inl>
#include <Math/Type/Inline/DVector3/DVector3Real.inl>