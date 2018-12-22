#ifndef GUARD_DY_HELPER_TYPE_COLORRGB24_H
#define GUARD_DY_HELPER_TYPE_COLORRGB24_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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

namespace dy
{

///
/// @class DDyColorRGB24
/// @brief 8-bit color type which stores 3 components (R, G, B)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
///
struct alignas(4) DDyColorRGB24 final
{
  TU08 R = MDY_INITIALIZE_DEFUINT;
  TU08 G = MDY_INITIALIZE_DEFUINT;
  TU08 B = MDY_INITIALIZE_DEFUINT;

  constexpr DDyColorRGB24() = default;

  constexpr DDyColorRGB24(_MIN_ const TU08 r, _MIN_ const TU08 g, _MIN_ const TU08 b) noexcept
  {
    this->R = r;
    this->G = g;
    this->B = b;
  }

  ///
  /// @brief Get color's grayscale value following sRGB.
  ///
  MDY_NODISCARD constexpr TF32 GetGrayScale() const noexcept
  {
    return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
  }

  ///
  /// @brief Can be convert DDyColorRGBA.
  ///
  constexpr operator DDyColorRGBA() const noexcept
  {
    return DDyColorRGBA
    {
      static_cast<float>(this->R) / 0xFF,
      static_cast<float>(this->G) / 0xFF,
      static_cast<float>(this->B) / 0xFF,
      1.0f
    };
  }

  ///
  /// @brief Can be convert DDyColorRGBA.
  ///
  constexpr operator DDyColorRGBA32() const noexcept
  {
    return DDyColorRGBA32 { this->R, this->G, this->B, 0xFF };
  }

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

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const DDyColorRGB24& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDyColorRGB24& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLORRGB24_H