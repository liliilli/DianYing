#ifndef GUARD_DY_HELPER_TYPE_COLOR32_H
#define GUARD_DY_HELPER_TYPE_COLOR32_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Dy/Helper/Type/color32.h
/// @brief 8-bit color type which stores 4 components (R, G, B, A)
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Create file.
///

#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/Type/ColorRGBA.h>

namespace dy
{

///
/// @class DDyColorRGBA32
/// @brief 8-bit color type which stores 4 components (R, G, B, A)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
///
struct alignas(4) DDyColorRGBA32 final
{
  TU08 R = MDY_INITIALIZE_DEFUINT;
  TU08 G = MDY_INITIALIZE_DEFUINT;
  TU08 B = MDY_INITIALIZE_DEFUINT;
  TU08 A = MDY_INITIALIZE_DEFUINT;

  constexpr DDyColorRGBA32() = default;
  constexpr explicit DDyColorRGBA32(_MIN_ const TU08 r, _MIN_ const TU08 g, _MIN_ const TU08 b) noexcept :
      DDyColorRGBA32{r, g, b, 0b11111111} {}
  constexpr explicit DDyColorRGBA32(_MIN_ const TU08 r, _MIN_ const TU08 g, _MIN_ const TU08 b, _MIN_ const TU08 a) noexcept
  {
    this->R = r;
    this->G = g;
    this->B = b;
    this->A = a;
  }

  //!
  //! Methods
  //!

  ///
  /// @brief Check it is opaque (A == 0xFF) or not (otherwise).
  ///
  MDY_NODISCARD constexpr bool IsOpaque() const noexcept
  {
    return this->A == std::numeric_limits<uint8_t>::max();
  }

  ///
  /// @brief Set this color to be opaque.
  ///
  constexpr void SetOpqaue() noexcept
  {
    this->A = 0xFF;
  }

  ///
  /// @brief Get color's grayscale value following sRGB.
  ///
  MDY_NODISCARD constexpr float GetGrayScale() const noexcept
  {
    return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
  }

  //!
  //! Operator overloading
  //!

  //!
  //! Conversion operators
  //!

  ///
  /// @brief Can be convert DDyColor explicitly.
  ///
  constexpr operator DDyColorRGBA() const noexcept
  {
    return DDyColorRGBA
    {
      static_cast<float>(R) / 0xFF,
      static_cast<float>(G) / 0xFF,
      static_cast<float>(B) / 0xFF,
      static_cast<float>(A) / 0xFF
    };
  }

  //!
  //! Statics
  //!

  static const DDyColorRGBA32 Aqua;
  static const DDyColorRGBA32 Black;
  static const DDyColorRGBA32 Blue;
  static const DDyColorRGBA32 DarkRed;
  static const DDyColorRGBA32 DarkGray;
  static const DDyColorRGBA32 DarkGreen;
  static const DDyColorRGBA32 Gold;
  static const DDyColorRGBA32 Gray;
  static const DDyColorRGBA32 Green;
  static const DDyColorRGBA32 Magenta;
  static const DDyColorRGBA32 Orange;
  static const DDyColorRGBA32 Purple;
  static const DDyColorRGBA32 Red;
  static const DDyColorRGBA32 White;
  static const DDyColorRGBA32 Yellow;
};

/// @brief Serialization function.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyColorRGBA32& p);
/// @brief Deerialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDyColorRGBA32& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLOR32_H