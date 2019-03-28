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
#include <Math/Common/TGlobalTypes.h>

namespace dy::math
{

/// @struct DClamp
/// @brief Value type for clamping from given start value to end value.
/// This value type does not support unary bit operators.
/// @tparam TType Actual value type of this type.
/// @tparam TStart Start value.
/// @tparam TEnd End value. End value is inclusive to value range. (Closed range)
template <typename TType, TI64 TStart, TI64 TEnd>
struct DClamp final
{ 
private:
  // Validation test (compile time)
  static_assert(std::is_arithmetic_v<TType>, "DClamp can only get arithmetic type as TType.");
  static_assert(TStart <= TEnd,              "TStart must be less or equal to TEnd.");

  static constexpr bool IsNotUsingNarrowConversion() noexcept
  {
    constexpr bool testA = kMaxValueOf<TType> >= TStart;
    constexpr bool testB = kMaxValueOf<TType> >= TEnd;
    constexpr bool testC = kMinValueOf<TType> <= TStart;
    constexpr bool testD = kMinValueOf<TType> <= TEnd;
    return testA && testB && testC && testD;
  }
  static_assert(
    IsNotUsingNarrowConversion() == true, 
    "Specified value range is not matched specified range of given TType.");

public:
  using TValueType = TType;
  static constexpr TI64 kStart = TStart;
  static constexpr TI64 kEnd = TEnd;

  constexpr DClamp() noexcept = default;
  constexpr DClamp(TType value) noexcept;
  constexpr DClamp& operator=(TType value) noexcept;

  /// @brief Convert to actual value type.
  constexpr operator TValueType() const noexcept;
  /// @brief Get value like using functor.
  constexpr TValueType operator()() const noexcept;
  /// @brief Return the pointer of Data.
  const TValueType* Data() const noexcept;

private:
  TValueType mValue = std::clamp(TValueType{}, TValueType(kStart), TValueType(kEnd));
};

} /// ::dy::math namespace

#include <Math/Type/Inline/DClamp/DClamp.inl>
#include <Math/Type/Inline/DClamp/DClampOperator.inl>