#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Type/DString.h>
#include <sstream>
#include <Dy/Helper/System/Assertion.h>

//!
//! Forward declaration & local functions
//!

namespace
{

constexpr TChr16 SURROGATE_SIGN = 0xFFFF;

///
/// @brief
/// @param  utf8StartChar
/// @return
///
MDY_NODISCARD constexpr TU32 DyGetByteOfUtf8Char(_MIN_ const char* utf8StartChar)
{
  const char check_byte = (*utf8StartChar) & 0xF8;
  if (check_byte & 0x80)
  {
    const auto byte2 = check_byte >> 5;
    if (!(byte2 & 0x01)) { return 2; }

    const auto byte3 = check_byte >> 4;
    if (!(byte3 & 0x01)) { return 3; }

    return 4;
  }

  return 1;
}

///
/// @brief
/// @param  utf8StartCharacter
/// @return
///
MDY_NODISCARD constexpr TChr16 DyGetRawUtf16CharacterFrom(_MIN_ const char* utf8StartCharacter) noexcept
{
  switch (DyGetByteOfUtf8Char(utf8StartCharacter))
  {
  case 1: return TChr16(*utf8StartCharacter);
  case 2:
  {
    const char16_t b1 = (*utf8StartCharacter) & 0b00001111;
    const char16_t b2 = *(utf8StartCharacter + 1) & 0b00111111;
    return TChr16((b1 << 6) | b2);
  }
  case 3:
  {
    const char16_t b1 = (*utf8StartCharacter) & 0b00001111;
    const char16_t b2 = *(utf8StartCharacter + 1) & 0b00111111;
    const char16_t b3 = *(utf8StartCharacter + 2) & 0b00111111;
    return TChr16((b1 << 12) | (b2 << 6) | b3);
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
  default: MDY_UNEXPECTED_BRANCH(); return 0;
  }
}

///
/// @brief
/// Get UTF-16 string length from argument length.
/// Returned length doesn't include null-terminate character.
/// @param  utf8Text
/// @return Length;
///
MDY_NODISCARD constexpr TU32 DyGetUtf16LengthFrom(_MIN_ const char* utf8Text) noexcept
{
  TU32 length = 0;
  for (const char* it = utf8Text; *it != '\0';)
  {
    const TU32 utf8_char_byte = DyGetByteOfUtf8Char(it);
    it = it + utf8_char_byte;
    length += 1;
  }

  return length;
}

} /// unnamed namespace

//!
//! Implementation
//!

namespace dy
{

DString::~DString()
{
  this->mString.clear();
}

DString::DString(_MIN_ const char* cString) noexcept
{
  this->pUpdateString(cString);
}

DString::DString(_MIN_ const std::string& string) noexcept 
{
  if (string.empty() == false)
  {
    this->pUpdateString(string.c_str());
  }
}

void DString::SetText(_MIN_ const char* string) noexcept
{
  this->pUpdateString(string);
}

void DString::SetText(_MIN_ const std::string& string) noexcept
{
  this->SetText(string.c_str());
}

void DString::SetText(_MIN_ const std::string_view& string) noexcept
{
  this->SetText(string.data());
}

void DString::pUpdateString(_MIN_ const char* string) noexcept
{
  this->mString.clear();
  this->mString.reserve(DyGetUtf16LengthFrom(string));

  for (auto it = string; *it != '\0'; it += DyGetByteOfUtf8Char(it))
  {
    const auto chr = DyGetRawUtf16CharacterFrom(it);
    if (chr != SURROGATE_SIGN) { this->mString.emplace_back(chr); }
  }
}

} /// ::dy namespace