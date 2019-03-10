#ifndef GUARD_DY_HELPER_TYPE_COLORRGB24_H
#define GUARD_DY_HELPER_TYPE_COLORRGB24_H
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

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/Type/ColorRGBA32.h>
#include <Dy/Helper/Type/ColorRGBA.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DDyColorRGB;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class DDyColorRGB24
/// @brief 8-bit color type which stores 3 components (R, G, B).
/// (h, s, v) does not support but can be converted calling HsvToRgb().
struct alignas(4) DDyColorRGB24 final
{
  TU08 R = 0, G = 0, B = 0;

  DDyColorRGB24() = default;
  DDyColorRGB24(TU08 r, TU08 g, TU08 b) noexcept;

  /// @brief Get color's grayscale value following sRGB.
  MDY_NODISCARD TF32 GetGrayScale() const noexcept;

   /// @brief Can be convert DDyColorRGB. 
  /// When converted to RGB type, Alpha will be 1.0. (opaque)
  operator DDyColorRGB() const noexcept;

  /// @brief Can be convert DDyColorRGBA. 
  /// When converted to RGBA type, Alpha will be 1.0. (opaque)
  operator DDyColorRGBA() const noexcept;

  /// @brief Can be convert DDyColorRGBA.
  /// When converted to RGBA type, Alpha will be 255. (opaque)
  operator DDyColorRGBA32() const noexcept;

  static const DDyColorRGB24 Aqua;
  static const DDyColorRGB24 Black;
  static const DDyColorRGB24 Blue;
  static const DDyColorRGB24 DarkRed;
  static const DDyColorRGB24 DarkGray;
  static const DDyColorRGB24 DarkGreen;
  static const DDyColorRGB24 Gold;
  static const DDyColorRGB24 Gray;
  static const DDyColorRGB24 Green;
  static const DDyColorRGB24 Magenta;
  static const DDyColorRGB24 Orange;
  static const DDyColorRGB24 Purple;
  static const DDyColorRGB24 Red;
  static const DDyColorRGB24 White;
  static const DDyColorRGB24 Yellow;
};

void to_json  (nlohmann::json& oJson, const DDyColorRGB24& iItem);
void from_json(const nlohmann::json& iJson, DDyColorRGB24& oJson);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLORRGB24_H