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

#include <StringUtil/DU16String.h>
#include <cassert>

namespace 
{

constexpr dy::DU16String::TCharType SURROGATE_SIGN = 0xFFFF;

/// @brief Get allocated character byte of given utf-8 char start point.
/// @param utf8StartChar UTF-8 character start point.
/// @return 1~4.
constexpr unsigned
DyGetByteOfUtf8Char(const char* utf8StartChar)
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

/// @brief Get UTF-16 (UCS2) character from UTF-8 character.
/// @param utf8StartCharacter
/// @return If utf-8 character is 4-byte character, just return surrogated sign.
constexpr dy::DU16String::TCharType 
DyGetRawUtf16CharacterFrom(const char* utf8StartCharacter) noexcept
{
  using TChr16 = dy::DU16String::TCharType;

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
  default: assert(false); return 0;
  }
}

/// @brief
/// Get UTF-16 string length from argument length.
/// Returned length doesn't include null-terminate character.
/// @param  utf8Text
/// @return Length;
constexpr dy::DU16String::TSize
DyGetUtf16LengthFrom(const char* utf8Text) noexcept
{
  using TSize = dy::DU16String::TSize;

  TSize length = 0;
  for (const char* it = utf8Text; *it != '\0';)
  {
    const TSize utf8_char_byte = DyGetByteOfUtf8Char(it);
    it = it + utf8_char_byte;
    length += 1;
  }

  return length;
}

} /// ::anonymous namespace

namespace dy
{
  
DU16String::~DU16String()
{
  this->mString.clear();
}

DU16String::DU16String(const char* cString) noexcept
{
  this->pUpdateString(cString);
}

DU16String::DU16String(const std::string& string) noexcept 
{
  if (string.empty() == false)
  {
    this->pUpdateString(string.c_str());
  }
}

void DU16String::SetText(const char* string) noexcept
{
  this->pUpdateString(string);
}

void DU16String::SetText(const std::string& string) noexcept
{
  this->SetText(string.c_str());
}

void DU16String::SetText(const std::string_view& string) noexcept
{
  this->SetText(string.data());
}

DU16String::TSize DU16String::GetLength() const noexcept
{
  return static_cast<TSize>(this->mString.size());
}

DU16String::TCharType DU16String::operator[](TIndex id) const
{
  // Validation check
  if (id > this->mString.size())
  {
    throw std::out_of_range("Id is out of range from DU16String.");
  }

  // If all okay, just return.
  return this->mString[id];
}

void DU16String::pUpdateString(const char* string) noexcept
{
  this->mString.clear();
  this->mString.reserve(DyGetUtf16LengthFrom(string));

  for (auto it = string; *it != '\0'; it += DyGetByteOfUtf8Char(it))
  {
    const auto chr = DyGetRawUtf16CharacterFrom(it);
    if (chr != SURROGATE_SIGN) { this->mString.emplace_back(chr); }
  }
}

decltype(DU16String::mString)::const_iterator DU16String::begin() const noexcept
{
  return mString.begin();
}

decltype(DU16String::mString)::const_iterator DU16String::cbegin() const noexcept
{
  return mString.cbegin();
}

decltype(DU16String::mString)::const_iterator DU16String::end() const noexcept
{
  return mString.end();
}

decltype(DU16String::mString)::const_iterator DU16String::cend() const noexcept
{
  return mString.cend();
}

} /// ::dy namespace