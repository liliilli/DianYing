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

#include <array>
#include <Math/Type/Math/DVector4.h>

namespace dy::math
{

/// @class DMatrix4
/// @brief Column-major matrix4 type.
/// Matrix type only support Real type.
template <typename TType>
struct MATH_NODISCARD DMatrix4 final
{
private:
  static_assert(
    kCategoryOf<TType> == EValueCategory::Real, 
    "Failed to make DMatrix, DMatrix only supports Real type.");

public:
  using TValueType = TType;
  DMatrix4()  = default;

  DMatrix4(
    TValueType _00, TValueType _01, TValueType _02, TValueType _03,
    TValueType _10, TValueType _11, TValueType _12, TValueType _13,
    TValueType _20, TValueType _21, TValueType _22, TValueType _23,
    TValueType _30, TValueType _31, TValueType _32, TValueType _33);
  DMatrix4(
    const DVector4<TValueType>& col0, 
    const DVector4<TValueType>& col1,
    const DVector4<TValueType>& col2,
    const DVector4<TValueType>& col3);

  DVector4<TValueType>& operator[](TIndex index) noexcept;
  const DVector4<TValueType>& operator[](TIndex index) const noexcept;

  /// @brief Transpose matrix.
  DMatrix4 Transpose() const noexcept;
  /// @brief Get inverse matrix of this matrix.
  /// This does not check determinant is 0 or not.
  DMatrix4 Inverse() const;

  /// @brief Check this matrix can be inversed.
  bool IsInvertible() const noexcept;
  /// @brief Get determinant of matrix.
  TValueType GetDeterminant() const noexcept;

  /// @brief Check value has NaN.
  bool HasNaN() const noexcept;
  /// @brief Check value has Infinity.
  bool HasInfinity() const noexcept;
  /// @brief Check values are normal value, neither NaN nor Inf.
  bool HasOnlyNormal() const noexcept;

  /// @brief Get start pointer of matrix sequence.
  TValueType* Data() noexcept;
  /// @brief Get start pointer of matrix sequence.
  const TValueType* Data() const noexcept;

  /// @brief Convert to vector that has values.
  std::vector<DVector4<TValueType>> ToVector() const noexcept;

  /// @brief Get identity matrix.
  static DMatrix4 Identity() noexcept;
  
private:
  /// Column major
  std::array<DVector4<TValueType>, 4> mValues;
};

static_assert(sizeof(DMatrix4<TF32>) == 64);
static_assert(sizeof(DMatrix4<TF64>) == 128);

} /// ::dy::math namespace
#include <Math/Type/Inline/DMatrix4/DMatrix4Operator.inl>
#include <Math/Type/Inline/DMatrix4/DMatrix4.inl>
