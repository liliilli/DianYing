#ifndef GUARD_DY_HELPER_TYPE_CLAMP_H
#define GUARD_DY_HELPER_TYPE_CLAMP_H
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

#include <nlohmann/json.hpp>
#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/Math/Math.h>

#if defined(max)
#undef max
#endif // Undef max
#if defined(min)
#undef min
#endif // Undef min

namespace dy
{

template <typename TType, TI32 TStart, TI32 TEnd>
class DClamp final
{ // Integrity test (compile time)
  static_assert(std::is_arithmetic_v<TType>,  "DClamp can only get arithmetic type as TType.");
  static_assert(TStart <= TEnd,               "TStart must be less or equal to TEnd.");
  static constexpr bool IsNotUsingNarrowConversion() noexcept
  {
    constexpr bool testA = std::numeric_limits<TType>::max()    >= TStart;
    constexpr bool testB = std::numeric_limits<TType>::max()    >= TEnd;
    constexpr bool testC = std::numeric_limits<TType>::lowest() <= TStart;
    constexpr bool testD = std::numeric_limits<TType>::lowest() <= TEnd;
    return testA && testB && testC && testD;
  }
  static_assert(IsNotUsingNarrowConversion() == true, "Specified value range is not matched specified range of given TType.");

public:
  constexpr DClamp() noexcept = default;
  constexpr DClamp(_MIN_ const TType& value) noexcept
  {
    this->mValue = math::Clamp<TType>(static_cast<TType>(value), TStart, TEnd);
  }

  template <typename TConvType, typename = std::enable_if_t<std::is_convertible_v<TConvType, TType>>>
  constexpr DClamp(_MIN_ const TConvType& value) noexcept
  {
    this->mValue = math::Clamp<TType>(static_cast<TType>(value), TStart, TEnd);
  }
  ~DClamp() = default;

  constexpr DClamp(_MIN_ const DClamp& instance)            = default;
  constexpr DClamp& operator=(_MIN_ const DClamp& instance) = default;

  constexpr DClamp& operator=(_MIN_ const TType& value) noexcept
  {
    this->mValue = math::Clamp<TType>(static_cast<TType>(value), TStart, TEnd);
    return *this;
  }

  template <typename TConvType, typename = std::enable_if_t<std::is_convertible_v<TConvType, TType>>>
  constexpr DClamp& operator=(_MIN_ const TConvType& value) noexcept
  {
    this->mValue = math::Clamp<TType>(static_cast<TType>(value), TStart, TEnd);
    return *this;
  }

  constexpr operator TType() const noexcept
  {
    return this->mValue;
  }

  constexpr TType operator()() const noexcept
  {
    return this->mValue;
  }

  constexpr friend bool operator==(_MIN_ const DClamp& lhs, _MIN_ const TType& value) noexcept
  {
    return lhs.mValue == value;
  }

  constexpr friend TType operator*(_MIN_ const TType& lhs, _MIN_ const DClamp& value) noexcept
  {
    return lhs * value.mValue;
  }

  ///
  /// @brief  Data pointer
  /// @return Data pointer sequence.
  ///
  MDY_NODISCARD const TType* Data() const noexcept
  {
    return &this->mValue;
  }

private:
  TType mValue = (TType() < TStart ? TType() : TStart);
};

template <typename TType, TI32 TStart, TI32 TEnd>
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DClamp<TType, TStart, TEnd>& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

template <typename TType, TI32 TStart, TI32 TEnd>
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DClamp<TType, TStart, TEnd>& p)
{
  p = j.get<TType>();
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_CLAMP_H
