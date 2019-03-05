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
#include <nlohmann/json_fwd.hpp>

namespace dy {

/// @class DDyColorRGBA
/// @brief Float color [0, 1) type which stores 4 components (R, G, B, A)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
struct DDyColorRGBA final
{
  TF32 R = 0.0f, G = 0.0f, B = 0.0f, A = 0.0f; 

  DDyColorRGBA() = default;
  explicit DDyColorRGBA(TF32 r, TF32 g, TF32 b) noexcept;
  explicit DDyColorRGBA(TF32 r, TF32 g, TF32 b, TF32 a) noexcept;

  //!
  //! Methods
  //!

  /// @brief Check it is opaque (A == 1.0f) or not (otherwise).
  MDY_NODISCARD bool IsOpaque() const noexcept;

  /// @brief Set this color to be opaque.
  void SetOpaque() noexcept;

  /// @brief Get color's grayscale value following sRGB.
  MDY_NODISCARD TF32 GetGrayScale() const noexcept;

  /// @brief  Data pointer
  /// @return Data pointer sequence.
  MDY_NODISCARD const TF32* Data() const noexcept;

  //!
  //! Conversion operators
  //!

  /// @brief Can be convert DDyVector3 explicitly, but alpha information passed.
  explicit operator DDyVector3() const noexcept;
  explicit operator glm::vec3() const noexcept;

  operator glm::vec4() const noexcept;
  operator DDyVector4() const noexcept;

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

void to_json(nlohmann::json& oJson, const DDyColorRGBA& iItem);
void from_json(const nlohmann::json& iJson, DDyColorRGBA& oItem);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLOR_H