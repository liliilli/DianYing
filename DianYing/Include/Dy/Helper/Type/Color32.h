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
#include <Dy/Helper/Type/Color.h>

namespace dy
{

///
/// @class DDyColor32
/// @brief 8-bit color type which stores 4 components (R, G, B, A)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
///
struct alignas(4) DDyColor32 final
{
  TU08 R = MDY_INITIALIZE_DEFUINT;
  TU08 G = MDY_INITIALIZE_DEFUINT;
  TU08 B = MDY_INITIALIZE_DEFUINT;
  TU08 A = MDY_INITIALIZE_DEFUINT;

  constexpr DDyColor32() = default;
  constexpr explicit DDyColor32(_MIN_ const TU08 r, _MIN_ const TU08 g, _MIN_ const TU08 b) noexcept :
      DDyColor32{r, g, b, 0b11111111} {}
  constexpr explicit DDyColor32(_MIN_ const TU08 r, _MIN_ const TU08 g, _MIN_ const TU08 b, _MIN_ const TU08 a) noexcept
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
  constexpr operator DDyColor() const noexcept
  {
    return DDyColor
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

  static const DDyColor32 Aqua;
  static const DDyColor32 Black;
  static const DDyColor32 Blue;
  static const DDyColor32 DarkRed;
  static const DDyColor32 DarkGray;
  static const DDyColor32 DarkGreen;
  static const DDyColor32 Gold;
  static const DDyColor32 Gray;
  static const DDyColor32 Green;
  static const DDyColor32 Magenta;
  static const DDyColor32 Orange;
  static const DDyColor32 Purple;
  static const DDyColor32 Red;
  static const DDyColor32 White;
  static const DDyColor32 Yellow;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLOR32_H