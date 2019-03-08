#ifndef GUARD_DY_LIBRARY_HELPERSTRING_H
#define GUARD_DY_LIBRARY_HELPERSTRING_H
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

#include <string>
#include <Libraries/spdlog/include/spdlog/fmt/fmt.h>

namespace dy
{

template <typename... TArgs>
[[nodiscard]] std::string MakeStringU8(const std::string& iStringFormat, TArgs&&... iArgs)
{
  return fmt::format(iStringFormat, std::forward<TArgs>(iArgs)...);
}

constexpr uint16_t SURROGATE_SIGN = 0xFFFF;

/// @brief
/// @return
[[nodiscard]] constexpr uint32_t DyGetByteOfUtf8Char(const char* iUtf8StartChar)
{
  const char checkByte = (*iUtf8StartChar) & 0xF8;
  if (checkByte & 0x80)
  {
    const auto byte2 = checkByte >> 5; if (!(byte2 & 0x01)) { return 2; }
    const auto byte3 = checkByte >> 4; if (!(byte3 & 0x01)) { return 3; }
    return 4;
  }
  return 1;
}

/// @brief
/// @return
[[nodiscard]] constexpr uint16_t DyGetRawUtf16CharacterFrom(const char* iUtf8StartCharacter) noexcept
{
  switch (DyGetByteOfUtf8Char(iUtf8StartCharacter))
  {
  case 1: return uint16_t(*iUtf8StartCharacter);
  case 2:
  {
    const char16_t b1 = (*iUtf8StartCharacter) & 0b00001111;
    const char16_t b2 = *(iUtf8StartCharacter + 1) & 0b00111111;
    return uint16_t((b1 << 6) | b2);
  }
  case 3:
  {
    const char16_t b1 = (*iUtf8StartCharacter) & 0b00001111;
    const char16_t b2 = *(iUtf8StartCharacter + 1) & 0b00111111;
    const char16_t b3 = *(iUtf8StartCharacter + 2) & 0b00111111;
    return uint16_t((b1 << 12) | (b2 << 6) | b3);
  }
  case 4:
  {
    return SURROGATE_SIGN;
#ifdef false
    const char32_t codepage_10k = (
      ((*utf8StartCharacter) & 0b00000111) << 18 |
      (*(utf8StartCharacter + 1) & 0b00111111) << 12 |
      (*(utf8StartCharacter + 2) & 0b00111111) << 6 |
      (*(utf8StartCharacter + 3) & 0b00111111)) - 0x10000;
    return static_cast<TC16>((codepage_10k >> 10) + 0xD800);
#endif
  }
  default: assert(false); return 0;
  }
}

/// @brief
/// Get UTF-16 string length from argument length.
/// Returned length doesn't include null-terminate character.
[[nodiscard]] constexpr uint32_t DyGetUtf16LengthFrom(const char* iUtf8Text) noexcept
{
  uint32_t length = 0;
  for (const char* it = iUtf8Text; *it != '\0';)
  {
    const uint32_t utf8_char_byte = DyGetByteOfUtf8Char(it);
    it = it + utf8_char_byte;
    length += 1;
  }

  return length;
}

} /// ::dy namespace

#endif /// GUARD_DY_LIBRARY_HELPERSTRING_H