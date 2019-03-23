#ifndef GUARD_DY_HELPER_TYPE_COLORRGB_H
#define GUARD_DY_HELPER_TYPE_COLORRGB_H
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

#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/Type/DClamp.h>
#include <Dy/Helper/Type/DVector3.h>
#include <Dy/Helper/Type/DColorRGBA.h>

namespace dy
{

/// @class DColorRGB
/// @brief Float color type which stores 3 components (R, G, B)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
struct DColorRGB final
{
  DClamp<TF32, 0, 1> R = 0.0f;
  DClamp<TF32, 0, 1> G = 0.0f;
  DClamp<TF32, 0, 1> B = 0.0f;

  constexpr DColorRGB() = default;

  constexpr explicit DColorRGB(TF32 r, TF32 g, TF32 b) noexcept :
      R{r}, G{g}, B{b}
  {};

  /// @brief Get color's grayscale value following sRGB.
  MDY_NODISCARD TF32 GetGrayScale() const noexcept;

  /// @brief  Get Data pointer
  MDY_NODISCARD const TF32* Data() const noexcept;

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
    return DVector3{this->R, this->G, this->B};
  }

  explicit operator glm::vec3() const noexcept
  {
    return glm::vec3{this->R, this->G, this->B};
  }

  operator DColorRGBA() const noexcept
  {
    return DColorRGBA{this->R, this->G, this->B, 1.0f};
  }

  operator glm::vec4() const noexcept
  {
    return glm::vec4{this->R, this->G, this->B, 1.0f};
  }

  //!
  //! Statics
  //!

  static const DColorRGB Aqua;
  static const DColorRGB Black;
  static const DColorRGB Blue;
  static const DColorRGB DarkRed;
  static const DColorRGB DarkGray;
  static const DColorRGB DarkGreen;
  static const DColorRGB Gold;
  static const DColorRGB Gray;
  static const DColorRGB Green;
  static const DColorRGB Magenta;
  static const DColorRGB Orange;
  static const DColorRGB Purple;
  static const DColorRGB Red;
  static const DColorRGB White;
  static const DColorRGB Yellow;
};

void to_json(nlohmann::json& oJson, const DColorRGB& iItem);
void from_json(const nlohmann::json& iJson, DColorRGB& oItem);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_COLORRGB_H