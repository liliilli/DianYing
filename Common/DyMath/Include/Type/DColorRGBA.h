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

#include <Dy/Helper/Type/DVector3.h>
#include <Dy/Helper/Type/DVector4.h>
#include <nlohmann/json_fwd.hpp>

namespace dy {

/// @class DColorRGBA
/// @brief Float color [0, 1) type which stores 4 components (R, G, B, A)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
struct DColorRGBA final
{
  TF32 R = 0.0f, G = 0.0f, B = 0.0f, A = 0.0f; 

  DColorRGBA() = default;
  explicit DColorRGBA(TF32 r, TF32 g, TF32 b) noexcept;
  explicit DColorRGBA(TF32 r, TF32 g, TF32 b, TF32 a) noexcept;
  explicit DColorRGBA(const std::array<TF32, 4>& iGlRgbaColor) noexcept;

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

  /// @brief Get array version of RGBA.
  std::array<TF32, 4> ToArray() const noexcept;

  //!
  //! Conversion operators
  //!

  /// @brief Can be convert DVector3 explicitly, but alpha information passed.
  explicit operator DVector3() const noexcept;
  explicit operator glm::vec3() const noexcept;

  operator glm::vec4() const noexcept;
  operator DVector4() const noexcept;

  //!
  //! Statics
  //!

  static const DColorRGBA Aqua;
  static const DColorRGBA Black;
  static const DColorRGBA Blue;
  static const DColorRGBA DarkRed;
  static const DColorRGBA DarkGray;
  static const DColorRGBA DarkGreen;
  static const DColorRGBA Gold;
  static const DColorRGBA Gray;
  static const DColorRGBA Green;
  static const DColorRGBA Magenta;
  static const DColorRGBA Orange;
  static const DColorRGBA Purple;
  static const DColorRGBA Red;
  static const DColorRGBA White;
  static const DColorRGBA Yellow;
};

void to_json(nlohmann::json& oJson, const DColorRGBA& iItem);
void from_json(const nlohmann::json& iJson, DColorRGBA& oItem);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLOR_H