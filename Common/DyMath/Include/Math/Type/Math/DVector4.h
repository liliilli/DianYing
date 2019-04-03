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
#include <Math/Type/Math/DVector3.h>

//!
//! Forward declaration 
//!

namespace dy::math
{

/// @struct DVector4
/// @brief 4-element vector struct type.
/// @todo Need to apply simd __m128 (SSE), __m128i, and two __m128d.
template <typename TType, typename TExpression = void> 
struct DVector4;

template <typename TType>
struct DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>>;

template <typename TType>
struct DVector4<TType, std::enable_if_t<kIsRealType<TType>>>;

} /// ::dy::math namespace

//!
//! Implementation
//!

namespace dy::math 
{

template <typename TType>
struct MATH_NODISCARD DVector4<TType, std::enable_if_t<kIsIntegerType<TType>>> final
{
  using TValueType = TType;
  TValueType X, Y, Z, W;

  DVector4() = default;
  DVector4(TValueType x, TValueType y, TValueType z, TValueType w) noexcept;
  DVector4(TValueType value) noexcept; 
  DVector4(const DVector2<TValueType>& value, TValueType z = TValueType{}, TValueType w = TValueType{}) noexcept;
  DVector4(const DVector3<TValueType>& value, TValueType w = TValueType{}) noexcept;

  template <typename TAnotherType>
  explicit operator DVector4<TAnotherType>() const noexcept;

  /// @brief Narrow conversion.
  explicit operator DVector2<TValueType>() const noexcept;
  /// @brief Narrow conversion.
  explicit operator DVector3<TValueType>() const noexcept;

  /// @brief Get values with index. index must be 0, 1 or 2.
  TValueType& operator[](TIndex index);
  /// @brief Get values with index. index must be 0, 1 or 2.
  const TValueType& operator[](TIndex index) const;

  /// @brief Return data chunk pointer of DVector4.
  TValueType* Data() noexcept;
  /// @brief Return data chunk pointer of DVector2.
  const TValueType* Data() const noexcept;

  /// @brief Return squared length of this vector.
  TReal GetSquareLength() const noexcept;
  /// @brief Return the length of this vector.
  TReal GetLength() const noexcept;
  /// @brief Return the length of this vector as homogeneous coordinate position.
  TReal GetHomogeneousLength() const noexcept;

  /// @brief Return new DVector4 instance of normalized input vector.
  DVector4<TReal> Normalize() const noexcept;

  DVector4& operator+=(const DVector4& value) noexcept;
  DVector4& operator-=(const DVector4& value) noexcept;
  DVector4& operator*=(TValueType value) noexcept;
  DVector4& operator*=(const DVector4& value) noexcept;
  DVector4& operator/=(TValueType value) noexcept;
  DVector4& operator/=(const DVector4& value) noexcept;
};

template <typename TType>
struct MATH_NODISCARD DVector4<TType, std::enable_if_t<kIsRealType<TType>>> final
{
  using TValueType = TType;
  TValueType X, Y, Z, W;

  DVector4() = default;
  DVector4(TValueType x, TValueType y, TValueType z, TValueType w) noexcept;
  DVector4(TValueType value) noexcept; 
  DVector4(const DVector2<TValueType>& value, TValueType z = TValueType{}, TValueType w = TValueType{}) noexcept;
  DVector4(const DVector3<TValueType>& value, TValueType w = TValueType{}) noexcept;

  template <typename TAnotherType>
  explicit operator DVector4<TAnotherType>() const noexcept;
  
  /// @brief Narrow conversion.
  explicit operator DVector2<TValueType>() const noexcept;
  /// @brief Narrow conversion.
  explicit operator DVector3<TValueType>() const noexcept;

  /// @brief Get values with index. index must be 0, 1 or 2.
  TValueType& operator[](TIndex index);
  /// @brief Get values with index. index must be 0, 1 or 2.
  const TValueType& operator[](TIndex index) const;

  /// @brief Return data chunk pointer of DVector4.
  TValueType* Data() noexcept;
  /// @brief Return data chunk pointer of DVector2.
  const TValueType* Data() const noexcept;

  /// @brief Return squared length of this vector.
  TReal GetSquareLength() const noexcept;
  /// @brief Returns the length of this vector.
  TReal GetLength() const noexcept;
  /// @brief Return the length of this vector as homogeneous coordinate position.
  TReal GetHomogeneousLength() const noexcept;

  /// @brief Return new DVector4 instance of normalized input vector.
  DVector4<TReal> Normalize() const noexcept;

  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;
  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;
  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

  DVector4& operator+=(const DVector4& value) noexcept;
  DVector4& operator-=(const DVector4& value) noexcept;
  DVector4& operator*=(TValueType value) noexcept;
  DVector4& operator*=(const DVector4& value) noexcept;
  DVector4& operator/=(TValueType value) noexcept;
  DVector4& operator/=(const DVector4& value) noexcept;
};

} /// ::dy::math namespace
#include <Math/Type/Inline/DVector4/Simd/DVector4TF32.inl>
#include <Math/Type/Inline/DVector4/Simd/DVector4TI32.inl>
#include <Math/Type/Inline/DVector4/DVector4Common.inl>
#include <Math/Type/Inline/DVector4/DVector4Int.inl>
#include <Math/Type/Inline/DVector4/DVector4Real.inl>
