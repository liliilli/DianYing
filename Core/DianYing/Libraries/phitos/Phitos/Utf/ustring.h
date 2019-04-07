#ifndef INCLUDE_PHITOS_UTF_USTRING_H
#define INCLUDE_PHITOS_UTF_USTRING_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Utf/ustring.h
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-28 Create file.
///

#include <cassert>
#include <cstdint>
#include <cstring>

namespace phitos::utf
{

///
/// @class U16Char
///
/// @brief
/// Container for saving UTF-16 character.
///
class U16Char final
{
public:
  constexpr explicit U16Char() noexcept = default;
  ~U16Char() = default;

  ///
  /// This constructor does not check surrogate because of input value is
  /// restricted by one char16_t type value.
  ///
  constexpr explicit U16Char(const char16_t input_value) noexcept
  {
    m_char[0] = ((input_value & 0xFF00) >> 8) | ((input_value & 0x00FF) << 8);
  }

  ///
  /// This constructor , must be starting point of character
  /// This constructor does not guarantee compile time expression.
  ///
  constexpr explicit U16Char(const char16_t* input_value) noexcept
  {
    // At first, check former 8bit is out of range from D0~D7.
    // If true, input_value will be regarded as surrogate character.
    const auto former_bits = static_cast<char>((*input_value) >> 8);
    //const char former_bits = reinterpret_cast<const char*>(input_value)[1];
    if ((former_bits & 0xD0) && ((former_bits & 0x0F) >= 0x08))
    {
      /// it's surrogate.
      m_char[1] = ((input_value[1] & 0xFF00) >> 8) | ((input_value[1] & 0x00FF) << 8);
      m_surrogate = true;
    }

    m_char[0] = ((input_value[0] & 0xFF00) >> 8) | ((input_value[0] & 0x00FF) << 8);
  }

  constexpr U16Char(const U16Char&) = default;
  constexpr U16Char& operator=(const U16Char&) = default;
  constexpr U16Char(U16Char&&) = default;
  constexpr U16Char& operator=(U16Char&&) = default;

  constexpr bool operator==(const U16Char& rhs) const noexcept {
      return (m_char[0] == rhs.m_char[0])
          && (m_char[1] == rhs.m_char[1])
          && (m_surrogate == rhs.m_surrogate);
  }

  constexpr bool operator!=(const U16Char& rhs) const noexcept {
      return !operator==(rhs);
  }

  constexpr bool IsSurrogate() const noexcept
  {
    return m_surrogate;
  }

  constexpr const char16_t* GetData() const noexcept
  {
    return m_char;
  }

private:
  ///
  /// Byte array values must be sorted by little endian by default.
  /// for example, U+0031 '1' must be saved as like a 31 00 00 00.
  /// U+1092E with surrogate must be saved as like a 02 D8 2E DD.
  ///
  char16_t m_char[2] = {0x0000,};
  bool m_surrogate = false;
};

///
/// @brief
///
/// @param[in] utf8_start_char
///
/// @return
///
constexpr uint32_t GetByteOfUtf8Char(const char* utf8_start_char)
{
  const char check_byte = (*utf8_start_char) & 0xF8;
  if (check_byte & 0x80)
  {
    const auto byte2 = check_byte >> 5;
    if (!(byte2 & 0x01)) return 2;

    const auto byte3 = check_byte >> 4;
    if (!(byte3 & 0x01)) return 3;

    return 4;
  }

  return 1;
}

///
/// @brief
/// Return little-endian UTF-16 character byte array, with surrogate flag.
/// input argument must be a start pointer of UTF-8 character,
///
/// @param[in] utf8_start_point
///
/// @todo Optimization to get values at compile time completely.
///
constexpr U16Char GetUtf16CharacterFrom(const char* utf8_start_point) noexcept
{
  switch (GetByteOfUtf8Char(utf8_start_point))
  {
  case 1: {
    return U16Char(utf8_start_point[0]);
  }
  case 2: {
    const char16_t b1 = (*utf8_start_point) & 0b00001111;
    const char16_t b2 = *(utf8_start_point + 1) & 0b00111111;
    return U16Char(char16_t((b1 << 6) | b2));
  }
  case 3: {
    const char16_t b1 = (*utf8_start_point) & 0b00001111;
    const char16_t b2 = *(utf8_start_point + 1) & 0b00111111;
    const char16_t b3 = *(utf8_start_point + 2) & 0b00111111;
    return U16Char(char16_t((b1 << 12) | (b2 << 6) | b3));
  }
  case 4: {
    const char16_t b1 = (*utf8_start_point) & 0b00000111;
    const char16_t b2 = *(utf8_start_point + 1) & 0b00111111;
    const char16_t b3 = *(utf8_start_point + 2) & 0b00111111;
    const char16_t b4 = *(utf8_start_point + 3) & 0b00111111;
    const char32_t codepage = b1 << 18 | b2 << 12 | b3 << 6 | b4;
    const char16_t data[2] {
        static_cast<char16_t>(((codepage - 0x10000) >> 10) + 0xD800),
        static_cast<char16_t>(((codepage - 0x10000) & 0x3FF) + 0xDC00)
    };
    return U16Char{data};
  }
  default:
    assert(false && "Unexpected byte value from GetByteOfUtf8Char.");
    return U16Char();
  }
}

///
/// @brief
/// Get UTF-16 string length from argument length.
/// Returned length doesn't include null-terminate character.
///
/// @param[in] utf8_text
///
/// @return Length;
///
constexpr uint32_t GetUtf16LengthFrom(const char* utf8_text) noexcept
{
  uint32_t length = 0;
  for (const char* it = utf8_text; *it != '\0';)
  {
    const uint32_t utf8_char_byte = GetByteOfUtf8Char(it);
    it = it + utf8_char_byte;
    length += 1;
  }

  return length;
}

constexpr uint16_t Swap(uint16_t input) noexcept {
    return ((input & 0xFF00) >> 8) | ((input & 0x00FF) << 8);
}

constexpr char16_t GetRawUtf16CharacterFrom(const char* utf8_start_point) noexcept
{
  switch (GetByteOfUtf8Char(utf8_start_point))
  {
  case 1:
    return *utf8_start_point;
#ifdef false
    return Swap(*utf8_start_point);
#endif
  case 2: {
    const char16_t b1 = (*utf8_start_point) & 0b00001111;
    const char16_t b2 = *(utf8_start_point + 1) & 0b00111111;
    return char16_t((b1 << 6) | b2);
#ifdef false
    return Swap(char16_t((b1 << 6) | b2));
#endif
  }
  case 3: {
    const char16_t b1 = (*utf8_start_point) & 0b00001111;
    const char16_t b2 = *(utf8_start_point + 1) & 0b00111111;
    const char16_t b3 = *(utf8_start_point + 2) & 0b00111111;
    return char16_t((b1 << 12) | (b2 << 6) | b3);
#ifdef false
    return Swap(char16_t((b1 << 12) | (b2 << 6) | b3));
#endif
  }
  case 4: {
    const char32_t codepage_10k = (
        ((*utf8_start_point) & 0b00000111) << 18 |
        (*(utf8_start_point + 1) & 0b00111111) << 12 |
        (*(utf8_start_point + 2) & 0b00111111) << 6 |
        (*(utf8_start_point + 3) & 0b00111111)) - 0x10000;
    return static_cast<char16_t>((codepage_10k >> 10) + 0xD800);
#ifdef false
    return Swap(static_cast<char16_t>((codepage_10k >> 10) + 0xD800));
#endif
  }
  default:
    return 0;
  }
}

///
/// @class UString
///
/// @brief
///
///
/// @author Jongmin Yun
///
///
///
class UString final
{
public:
  explicit UString(const char* string)
  {
    m_data = new U16Char[GetUtf16LengthFrom(string) + 1];
    uint32_t i = 0;
    for (const char* it = string; *it != '\0';)
    {

    }
  }

  uint32_t Length()
  {

  }

private:
  U16Char* m_data = nullptr;
};

} /// ::phitos::utf namespace

namespace phitos::utf::for_test
{

uint32_t TGetUtf16LengthFrom(const char* length) noexcept;

constexpr U16Char TGetUtf16CharacterFrom(const char* utf8_start_point) noexcept {
  return phitos::utf::GetUtf16CharacterFrom(utf8_start_point);
}

} /// ::phitos::utf::for_test namespace

#endif /// INCLUDE_PHITOS_UTF_USTRING_H