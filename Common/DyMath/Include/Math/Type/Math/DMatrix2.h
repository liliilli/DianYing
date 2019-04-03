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
#include <Math/Type/Math/DVector2.h>

namespace dy::math
{

/// @class DMatrix2
/// @brief Column-major matrix2 type.
/// Matrix type only support Real type.
template <typename TType>
struct MATH_NODISCARD DMatrix2 final
{
private:
  static_assert(
    kCategoryOf<TType> == EValueCategory::Real, 
    "Failed to make DMatrix, DMatrix only supports Real type.");

public:
  using TValueType = TType;
  DMatrix2()  = default;

  DMatrix2(TValueType _00, TValueType _01, TValueType _10, TValueType _11);
  DMatrix2(const DVector2<TValueType>& _0, const DVector2<TValueType>& _1);

  DVector2<TValueType>& operator[](TIndex index) noexcept;
  const DVector2<TValueType>& operator[](TIndex index) const noexcept;

  /// @brief Transpose matrix.
  DMatrix2 Transpose() const noexcept;
  /// @brief Get inverse matrix of this matrix.
  /// This does not check determinant is 0 or not.
  DMatrix2 Inverse() const;

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
  std::vector<DVector2<TValueType>> ToVector() const noexcept;

  /// @brief Get identity matrix.
  static DMatrix2 Identity() noexcept;
  
private:
  /// Column major
  std::array<DVector2<TValueType>, 2> mValues;
};

static_assert(sizeof(DMatrix2<TF32>) == 16);
static_assert(sizeof(DMatrix2<TF64>) == 32);

} /// ::dy::math namespace
#include <Math/Type/Inline/DMatrix2/DMatrix2Operator.inl>
#include <Math/Type/Inline/DMatrix2/DMatrix2.inl>
