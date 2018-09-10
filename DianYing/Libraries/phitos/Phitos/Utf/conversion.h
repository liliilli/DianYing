#ifndef INCLUDE_PHITOS_UTF_CONVERSION_H
#define INCLUDE_PHITOS_UTF_CONVERSION_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Utf/conversion.h
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-27 Create file.
///

#include <optional>
#include <string>

namespace phitos::utf
{

template <
    typename TType, typename TTraits, typename TAlloc,
    template <typename...> class TContainer = std::basic_string>
void PrintStringBytesByHex(const TContainer<TType, TTraits, TAlloc>& string,
                           const char* postfix)
{
  const auto string_byte_array = reinterpret_cast<const char*>(string.c_str());

  uint32_t length = 0;
  if constexpr (std::is_same_v<TType, char>)
    length = static_cast<uint32_t>(string.length());
  else if constexpr (std::is_same_v<TType, char16_t>)
    length = static_cast<uint32_t>(string.length()) << 1;
  else if constexpr (std::is_same_v<TType, char32_t>)
    length = static_cast<uint32_t>(string.length()) << 2;

  for (uint32_t i = 0; i < length; ++i)
  {
    printf("%2x ", static_cast<int>(string_byte_array[i]) & 0x000000FF);
  }
  printf(postfix);
}

#if defined(_WIN32)

///
/// @brief
///
/// @param[in]
///
/// @return
///
std::optional<std::string>
    ConvertUtf16StringToUtf8(const std::u16string& string);

///
/// @brief
///
/// @param[in]
///
/// @return
///
std::optional<std::u16string>
    ConvertUtf8StringToUtf16(const std::string& string);

#else
#define ConvertUtf16StringToUtf8(...) \
    std::optional<std::string>(std::nullopt);\
    static_assert( \
        false, \
        "This function is not used on any other OS except for Windows.")

#define ConvertUtf8StringToUtf16(...) \
    std::optional<std::string>(std::nullopt); \
    static_assert( \
        false, \
        "This function is not used on any other OS except for Windows.")
#endif /// defined(_WIN32)

} /// namespace ::phitos::utf

#endif /// INCLUDE_PHITOS_UTF_CONVERSION_H
