#ifndef GUARD_DY_HELPER_TYPE_CLAMP_H
#define GUARD_DY_HELPER_TYPE_CLAMP_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

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
class DDyClamp final
{ // Integrity test (compile time)
  static_assert(std::is_arithmetic_v<TType>,  "DDyClamp can only get arithmetic type as TType.");
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
  template <typename TConvType, typename = std::enable_if_t<std::is_convertible_v<TConvType, TType>>>
  constexpr DDyClamp(_MIN_ const TConvType value) noexcept
  {
    this->mValue = math::Clamp<TType>(static_cast<TType>(value), TStart, TEnd);
  }
  ~DDyClamp() = default;

  constexpr DDyClamp(_MIN_ const DDyClamp& instance)            = default;
  constexpr DDyClamp& operator=(_MIN_ const DDyClamp& instance) = default;

  template <typename TConvType, typename = std::enable_if_t<std::is_convertible_v<TConvType, TType>>>
  constexpr DDyClamp& operator=(_MIN_ const TConvType value) noexcept
  {
    this->mValue = math::Clamp<TType>(static_cast<TType>(value), TStart, TEnd);
    return *this;
  }

  constexpr TType operator()() const noexcept
  {
    return this->mValue;
  }

  constexpr friend bool operator==(_MIN_ const DDyClamp& lhs, _MIN_ const TType& value) noexcept
  {
    return lhs.mValue == value;
  }

private:
  TType mValue = (TType() < TStart ? TType() : TStart);
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_CLAMP_H
