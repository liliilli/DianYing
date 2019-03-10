#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Type/ColorRGB24.h>
#include <Dy/Helper/Type/ColorRGB.h>

#include <bitset>
#include <nlohmann/json.hpp>

//!
//! Forward declaration & local translation unit.
//!

namespace
{

/// @brief Get DDyColor32 RGB color from TU32 24 bit value.
/// @param bit24Value 24bit value, [23...16] R [15...8] G [7...0] B
/// @return RGB Color with alpha 1.0 (always alpha 1.0)
MDY_NODISCARD dy::DDyColorRGB24 DyGetRGBColorFromTU32(_MIN_ const TU32 bit24Value) noexcept
{
  std::bitset<24>   backgroundColorBit = bit24Value;
  dy::DDyColorRGB24 background8BitColor;
  {
    TU08 r = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = 16 + i;
      r += backgroundColorBit[index] << i;
    }
    background8BitColor.R = r;

    TU08 g = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = 8 + i;
      g += backgroundColorBit[index] << i;
    }
    background8BitColor.G = g;

    TU08 b = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = i;
      b += backgroundColorBit[index] << i;
    }
    background8BitColor.B = b;
  }

  return background8BitColor;
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

DDyColorRGB24::DDyColorRGB24(TU08 r, TU08 g, TU08 b) noexcept
  : R{r}, G{g}, B{b} {}

TF32 DDyColorRGB24::GetGrayScale() const noexcept
{
  return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
}

DDyColorRGB24::operator DDyColorRGB() const noexcept
{
  return DDyColorRGB
  {
    static_cast<TF32>(this->R) / 0xFF,
    static_cast<TF32>(this->G) / 0xFF,
    static_cast<TF32>(this->B) / 0xFF,
  };
}

DDyColorRGB24::operator DDyColorRGBA() const noexcept
{
  return DDyColorRGBA
  {
    static_cast<TF32>(this->R) / 0xFF,
    static_cast<TF32>(this->G) / 0xFF,
    static_cast<TF32>(this->B) / 0xFF,
    1.0f
  };
}

DDyColorRGB24::operator DDyColorRGBA32() const noexcept
{
  return DDyColorRGBA32{this->R, this->G, this->B, 0xFF};
}

void to_json(nlohmann::json& oJson, const DDyColorRGB24& iItem)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& iJson, DDyColorRGB24& oItem)
{
  oItem = DyGetRGBColorFromTU32(iJson.get<TU32>());
}

} /// ::dy namespace