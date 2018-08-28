#ifndef GUARD_DY_HELPER_TYPE_COLOR_H
#define GUARD_DY_HELPER_TYPE_COLOR_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Dy/Helper/Type/color.h
/// @brief Float color type which stores 4 components (r, g, b, a)
/// @author Jongmin Yun
///
/// @log
/// 2018-08-23 Create file.
///

#include <Dy/Helper/Type/Vector3.h>

namespace dy {

///
/// @class DColor
/// @brief Float color type which stores 4 components (r, g, b, a)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
///
struct DColor final {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 0.0f;

  constexpr DColor() = default;

  constexpr explicit DColor(float r, float g, float b) noexcept : r{r}, g{g}, b{b}, a{1.0f} {
    if (this->r < 0.0f) this->r = 0.0f; else if (this->r > 1.0f) this->r = 1.0f;
    if (this->g < 0.0f) this->g = 0.0f; else if (this->g > 1.0f) this->g = 1.0f;
    if (this->b < 0.0f) this->b = 0.0f; else if (this->b > 1.0f) this->b = 1.0f;
  };

  constexpr explicit DColor(float r, float g, float b, float a) noexcept : r{r}, g{g}, b{b}, a{a} {
    if (this->r < 0.0f) this->r = 0.0f; else if (this->r > 1.0f) this->r = 1.0f;
    if (this->g < 0.0f) this->g = 0.0f; else if (this->g > 1.0f) this->g = 1.0f;
    if (this->b < 0.0f) this->b = 0.0f; else if (this->b > 1.0f) this->b = 1.0f;
    if (this->a < 0.0f) this->a = 0.0f; else if (this->a > 1.0f) this->a = 1.0f;
  };

  //!
  //! Methods
  //!

  ///
  /// @brief Check it is opaque (a == 1.0f) or not (otherwise).
  ///
  constexpr bool IsOpaque() const noexcept {
    return this->a == 1.0f;
  }

  ///
  /// @brief Set this color to be opaque.
  ///
  constexpr void SetOpaque() noexcept {
    this->a = 1.0f;
  }

  ///
  /// @brief Get color's grayscale value following sRGB.
  ///
  constexpr float GetGrayScale() const noexcept {
    return 0.2126f * this->r + 0.7152f * this->g + 0.0722f * this->b;
  }

  //!
  //! Operator overloading
  //!


  //!
  //! Conversion operators
  //!

  ///
  /// @brief Can be convert DVector3 explicitly, but alpha information passed.
  ///
  explicit operator DVector3() const noexcept
  {
    return DVector3{r, g, b};
  }

  explicit operator glm::vec3() const noexcept
  {
    return glm::vec3{r, g, b};
  }

  operator glm::vec4() const noexcept
  {
    return glm::vec4{r, g, b, a};
  }

  //!
  //! Statics
  //!

  static const DColor Aqua;
  static const DColor Black;
  static const DColor Blue;
  static const DColor DarkRed;
  static const DColor DarkGray;
  static const DColor DarkGreen;
  static const DColor Gold;
  static const DColor Gray;
  static const DColor Green;
  static const DColor Magenta;
  static const DColor Orange;
  static const DColor Purple;
  static const DColor Red;
  static const DColor White;
  static const DColor Yellow;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLOR_H