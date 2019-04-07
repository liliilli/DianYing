#ifndef PHITOS_TYPES_PT_T_BYTES_H
#define PHITOS_TYPES_PT_T_BYTES_H

///
/// @file Phitos/Types/ptTByte.h
///
/// @brief
/// Safe byte type variable type implementation file.
///
/// @todo Resolve some potential pitfalls.
///
/// Executive summary: it's 64 bits, or larger.
/// unsigned long long is the same as unsigned long long int.
/// Its size is platform-dependent, but guaranteed by the C standard
/// (ISO C99) to be at least 64 bits.
///
/// There was no long long in C89,
/// but apparently even MSVC supports it, so it's quite portable.
///
/// In the current C++ standard (issued in 2003),
/// there is no long long, though many compilers support it as an extension.
/// The upcoming C++0x standard will support it and
/// its size will be the same as in C, so at least 64 bits.
///

#include <cstdint>

#include <type_traits>
#include <limits>

namespace phitos::type {

struct PtTByte {
  using underlying_type = unsigned long long;

  explicit constexpr PtTByte(const underlying_type value) :
    m_value{ value } {};

  constexpr explicit operator underlying_type() const noexcept {
    return m_value;
  }

  template <
    typename = std::enable_if_t<
    !std::is_same_v<underlying_type, uint64_t>
    >
  >
  constexpr explicit operator uint64_t() const noexcept {
    return static_cast<uint64_t>(m_value);
  }

  constexpr explicit operator uint32_t() const noexcept {
    return static_cast<uint32_t>(m_value);
  }

  ///
  /// @todo Implement overflow exception.
  ///
  constexpr PtTByte& operator+=(const PtTByte& value) noexcept {
    m_value = static_cast<underlying_type>(value);
    return *this;
  }

  constexpr PtTByte& operator-=(const PtTByte& value) {
    m_value = (this->m_value < value.m_value ?
        throw 0 :
        (this->m_value - value.m_value));
    return *this;
  }

  friend constexpr PtTByte operator+(const PtTByte& lhs, const PtTByte& rhs) noexcept;
  friend constexpr PtTByte operator-(const PtTByte& lhs, const PtTByte& rhs);

private:
  underlying_type m_value;
};

///
/// @todo Implement overflow exception.
///
constexpr PtTByte operator+(const PtTByte& lhs, const PtTByte& rhs) noexcept {
  return PtTByte{
      static_cast<PtTByte::underlying_type>(lhs) +
      static_cast<PtTByte::underlying_type>(rhs)
  };
}

constexpr PtTByte operator-(const PtTByte& lhs, const PtTByte& rhs) {
  return lhs.m_value < rhs.m_value ?
         throw 0 :
         PtTByte{lhs.m_value - rhs.m_value};
}

} /// ::phitos::type namespace.

constexpr phitos::type::PtTByte operator "" _pByte(
    const phitos::type::PtTByte::underlying_type value) {
  return phitos::type::PtTByte{value};
}

#endif /// PHITOS_TYPES_PT_T_BYTES_H
