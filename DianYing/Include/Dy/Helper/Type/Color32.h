#ifndef GUARD_DY_HELPER_TYPE_COLOR32_H
#define GUARD_DY_HELPER_TYPE_COLOR32_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Dy/Helper/Type/color32.h
/// @brief 8-bit color type which stores 4 components (r, g, b, a)
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Create file.
///

#include <cstdint>
#include <Dy/Helper/Type/Color.h>

namespace dy {

///
/// @class DColor32
/// @brief 8-bit color type which stores 4 components (r, g, b, a)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
///
struct DColor32 final {
  uint8_t R = 0;
  uint8_t G = 0;
  uint8_t B = 0;
  uint8_t A = 0;

  constexpr DColor32() = default;
  explicit DColor32(uint8_t r, uint8_t g, uint8_t b) noexcept : DColor32{r, g, b, 0xFF} {}
  explicit DColor32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept {
    this->R = r;
    this->G = g;
    this->B = b;
    this->A = a;
  }

  //!
  //! Methods
  //!

  constexpr bool IsOpaque() const noexcept {
    return this->A == std::numeric_limits<uint8_t>::max();
  }

  constexpr void SetOpqaue() noexcept {
    this->A = 0xFF;
  }

  constexpr float GetGrayScale() const noexcept {
    return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
  }

  //!
  //! Operator overloading
  //!



  //!
  //! Conversion operators
  //!

  ///
  /// @brief Can be convert DColor explicitly.
  ///
  constexpr operator DColor() const noexcept {
    return DColor
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

  static const DColor32 Aqua;
  static const DColor32 Black;
  static const DColor32 Blue;
  static const DColor32 DarkRed;
  static const DColor32 DarkGray;
  static const DColor32 DarkGreen;
  static const DColor32 Gold;
  static const DColor32 Gray;
  static const DColor32 Green;
  static const DColor32 Magenta;
  static const DColor32 Orange;
  static const DColor32 Purple;
  static const DColor32 Red;
  static const DColor32 White;
  static const DColor32 Yellow;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLOR32_H