#ifndef GUARD_DY_HELPER_TYPE_COLOR_H
#define GUARD_DY_HELPER_TYPE_COLOR_H
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

#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/Type/Vector4.h>

namespace dy {

///
/// @class DDyColorRGBA
/// @brief Float color type which stores 4 components (R, G, B, A)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
///
struct DDyColorRGBA final
{
  float R = 0.0f;
  float G = 0.0f;
  float B = 0.0f;
  float A = 0.0f;

  constexpr DDyColorRGBA() = default;

  constexpr explicit DDyColorRGBA(float r, float g, float b) noexcept : R{r}, G{g}, B{b}, A{1.0f} {
    if (this->R < 0.0f) this->R = 0.0f; else if (this->R > 1.0f) this->R = 1.0f;
    if (this->G < 0.0f) this->G = 0.0f; else if (this->G > 1.0f) this->G = 1.0f;
    if (this->B < 0.0f) this->B = 0.0f; else if (this->B > 1.0f) this->B = 1.0f;
  };

  constexpr explicit DDyColorRGBA(float r, float g, float b, float a) noexcept : R{r}, G{g}, B{b}, A{a} {
    if (this->R < 0.0f) this->R = 0.0f; else if (this->R > 1.0f) this->R = 1.0f;
    if (this->G < 0.0f) this->G = 0.0f; else if (this->G > 1.0f) this->G = 1.0f;
    if (this->B < 0.0f) this->B = 0.0f; else if (this->B > 1.0f) this->B = 1.0f;
    if (this->A < 0.0f) this->A = 0.0f; else if (this->A > 1.0f) this->A = 1.0f;
  };

  //!
  //! Methods
  //!

  ///
  /// @brief Check it is opaque (A == 1.0f) or not (otherwise).
  ///
  [[nodiscard]] constexpr bool IsOpaque() const noexcept {
    return this->A == 1.0f;
  }

  ///
  /// @brief Set this color to be opaque.
  ///
  constexpr void SetOpaque() noexcept {
    this->A = 1.0f;
  }

  ///
  /// @brief Get color's grayscale value following sRGB.
  ///
  [[nodiscard]] constexpr float GetGrayScale() const noexcept {
    return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
  }

  ///
  /// @brief  Data pointer
  /// @return Data pointer sequence.
  ///
  MDY_NODISCARD const float* Data() const noexcept
  {
    return &this->R;
  }

  //!
  //! Operator overloading
  //!


  //!
  //! Conversion operators
  //!

  ///
  /// @brief Can be convert DDyVector3 explicitly, but alpha information passed.
  ///
  explicit operator DDyVector3() const noexcept
  {
    return DDyVector3{this->R, this->G, this->B};
  }

  explicit operator glm::vec3() const noexcept
  {
    return glm::vec3{this->R, this->G, this->B};
  }

  operator glm::vec4() const noexcept
  {
    return glm::vec4{this->R, this->G, this->B, this->A};
  }

  operator DDyVector4() const noexcept
  {
    return DDyVector4{this->R, this->G, this->B, this->A};
  }

  //!
  //! Statics
  //!

  static const DDyColorRGBA Aqua;
  static const DDyColorRGBA Black;
  static const DDyColorRGBA Blue;
  static const DDyColorRGBA DarkRed;
  static const DDyColorRGBA DarkGray;
  static const DDyColorRGBA DarkGreen;
  static const DDyColorRGBA Gold;
  static const DDyColorRGBA Gray;
  static const DDyColorRGBA Green;
  static const DDyColorRGBA Magenta;
  static const DDyColorRGBA Orange;
  static const DDyColorRGBA Purple;
  static const DDyColorRGBA Red;
  static const DDyColorRGBA White;
  static const DDyColorRGBA Yellow;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLOR_H