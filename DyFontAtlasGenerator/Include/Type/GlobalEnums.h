#ifndef GUARD_DY_GLOBALENUMS_H
#define GUARD_DY_GLOBALENUMS_H
/// Created at 2018-10-31 AM 12:00:25

#include <type_traits>
#include <cstdint>

namespace dy
{

///
/// @enum   EDyCharmapCollections
/// @brief  Charmap collection enumeration type (can be set flag.)
///
enum class EDyCharmapCollections : uint32_t
{
  None      = 0b00000000,
  English   = 0b00000001,
  Hangul    = 0b00000010,
  Kana      = 0b00000100,
  CJK       = 0b00001000,
  Automatic = 0b00010000 
};

///
/// @brief  `OR` operation of enum type.
/// @return `OR`ed enum type (don't use `OR`ed enum type value directly, because this causes UB)
///
inline EDyCharmapCollections operator|(EDyCharmapCollections lhs, EDyCharmapCollections rhs)
{ ///
  using TCharmapCollections = std::underlying_type<EDyCharmapCollections>::type;
  return static_cast<EDyCharmapCollections>(static_cast<TCharmapCollections>(lhs) | static_cast<TCharmapCollections>(rhs));
}

///
/// @brief  `OR` operation of enum type.
/// @return `OR`ed enum type (don't use `OR`ed enum type value directly, because this causes UB)
///
inline EDyCharmapCollections& operator|=(EDyCharmapCollections& lhs, EDyCharmapCollections rhs)
{ ///
  using TCharmapCollections = std::underlying_type<EDyCharmapCollections>::type;
  lhs = static_cast<EDyCharmapCollections>(static_cast<TCharmapCollections>(lhs) | static_cast<TCharmapCollections>(rhs));
  return lhs;
}

///
/// @enum   EDyOptionCollections
/// @brief  Option collection enumration type (can be set flag.)
///
enum class EDyOptionCollections : uint32_t
{
  None                = 0b0000,
  SeparateJsonAndPng  = 0b0001,
  /// @deprecated This is deprecated because of compression is default and must be.
  CompressJsonString_Deprecated  = 0b0010,
};

///
/// @brief  `OR` operation of enum type.
/// @return `OR`ed enum type (don't use `OR`ed enum type value directly, because this causes UB)
///
inline EDyOptionCollections operator|(EDyOptionCollections lhs, EDyOptionCollections rhs)
{ ///
  using T = std::underlying_type<EDyOptionCollections>::type;
  return static_cast<EDyOptionCollections>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

///
/// @brief  `OR` operation of enum type.
/// @return `OR`ed enum type (don't use `OR`ed enum type value directly, because this causes UB)
///
inline EDyOptionCollections& operator|=(EDyOptionCollections& lhs, EDyOptionCollections rhs)
{ ///
  using T = std::underlying_type<EDyOptionCollections>::type;
  lhs = static_cast<EDyOptionCollections>(static_cast<T>(lhs) | static_cast<T>(rhs));
  return lhs;
}

///
/// @brief  Check flag collection enum type is having specified flags.
/// @return If succeeded, return true or false.
///
template <
    typename TEnum,
    typename = std::enable_if_t<
        std::is_enum_v<TEnum> &&
        (std::is_same_v<TEnum, EDyCharmapCollections>
      || std::is_same_v<TEnum, EDyOptionCollections>)
    >
>
inline bool IsHavingFlags(const TEnum& lhs, TEnum rhs)
{
  const auto lhsValue = static_cast<std::underlying_type_t<TEnum>>(lhs);
  const auto rhsValue = static_cast<std::underlying_type_t<TEnum>>(rhs);

  return (lhsValue & rhsValue) > 0;
}

///
/// @brief  Check given flag type is not specified anything.
/// @return If empty, return true or false.
///
template <
    typename TEnum,
    typename = std::enable_if_t<
        std::is_enum_v<TEnum> &&
        (std::is_same_v<TEnum, EDyCharmapCollections>
      || std::is_same_v<TEnum, EDyOptionCollections>)
    >
>
inline bool IsFlagNotCheckAll(const TEnum& lhs)
{
  const auto lhsValue = static_cast<std::underlying_type_t<TEnum>>(lhs);
  return (lhsValue ^ 0x00) == 0x00;
}

} /// ::dy namespace

#endif /// GUARD_DY_GLOBALENUMS_H