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
#include <Dy/Helper/Type/DColorRGBA32.h>
#include <Dy/Helper/Type/DColorRGBA.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DColorRGB;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class DColorRGB24
/// @brief 8-bit color type which stores 3 components (R, G, B).
/// (h, s, v) does not support but can be converted calling HsvToRgb().
struct alignas(4) DColorRGB24 final
{
  TU08 R = 0, G = 0, B = 0;

  DColorRGB24() = default;
  DColorRGB24(TU08 r, TU08 g, TU08 b) noexcept;

  /// @brief Get color's grayscale value following sRGB.
  MDY_NODISCARD TF32 GetGrayScale() const noexcept;

   /// @brief Can be convert DColorRGB. 
  /// When converted to RGB type, Alpha will be 1.0. (opaque)
  operator DColorRGB() const noexcept;

  /// @brief Can be convert DColorRGBA. 
  /// When converted to RGBA type, Alpha will be 1.0. (opaque)
  operator DColorRGBA() const noexcept;

  /// @brief Can be convert DColorRGBA.
  /// When converted to RGBA type, Alpha will be 255. (opaque)
  operator DColorRGBA32() const noexcept;

  static const DColorRGB24 Aqua;
  static const DColorRGB24 Black;
  static const DColorRGB24 Blue;
  static const DColorRGB24 DarkRed;
  static const DColorRGB24 DarkGray;
  static const DColorRGB24 DarkGreen;
  static const DColorRGB24 Gold;
  static const DColorRGB24 Gray;
  static const DColorRGB24 Green;
  static const DColorRGB24 Magenta;
  static const DColorRGB24 Orange;
  static const DColorRGB24 Purple;
  static const DColorRGB24 Red;
  static const DColorRGB24 White;
  static const DColorRGB24 Yellow;
};

void to_json  (nlohmann::json& oJson, const DColorRGB24& iItem);
void from_json(const nlohmann::json& iJson, DColorRGB24& oJson);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLORRGB24_H