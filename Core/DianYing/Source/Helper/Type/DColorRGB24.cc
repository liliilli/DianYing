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
#include <Dy/Helper/Type/DColorRGB24.h>
#include <Dy/Helper/Type/DColorRGB.h>

#include <bitset>
#include <nlohmann/json.hpp>
#include <Dy/Helper/System/Assertion.h>

//!
//! Forward declaration & local translation unit.
//!

namespace
{

/// @brief Get DDyColor32 RGB color from TU32 24 bit value.
/// @param bit24Value 24bit value, [23...16] R [15...8] G [7...0] B
/// @return RGB Color with alpha 1.0 (always alpha 1.0)
MDY_NODISCARD dy::DColorRGB24 DyGetRGBColorFromTU32(_MIN_ const TU32 bit24Value) noexcept
{
  std::bitset<24>   backgroundColorBit = bit24Value;
  dy::DColorRGB24 background8BitColor;
  {
    TU8 r = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = 16 + i;
      r += backgroundColorBit[index] << i;
    }
    background8BitColor.R = r;

    TU8 g = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = 8 + i;
      g += backgroundColorBit[index] << i;
    }
    background8BitColor.G = g;

    TU8 b = 0;
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

DColorRGB24::DColorRGB24(TU8 r, TU8 g, TU8 b) noexcept
  : R{r}, G{g}, B{b} {}

TF32 DColorRGB24::GetGrayScale() const noexcept
{
  return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
}

DColorRGB24::operator DColorRGB() const noexcept
{
  return DColorRGB
  {
    static_cast<TF32>(this->R) / 0xFF,
    static_cast<TF32>(this->G) / 0xFF,
    static_cast<TF32>(this->B) / 0xFF,
  };
}

DColorRGB24::operator DColorRGBA() const noexcept
{
  return DColorRGBA
  {
    static_cast<TF32>(this->R) / 0xFF,
    static_cast<TF32>(this->G) / 0xFF,
    static_cast<TF32>(this->B) / 0xFF,
    1.0f
  };
}

DColorRGB24::operator DColorRGBA32() const noexcept
{
  return DColorRGBA32{this->R, this->G, this->B, 0xFF};
}

void to_json(nlohmann::json& oJson, const DColorRGB24& iItem)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& iJson, DColorRGB24& oItem)
{
  oItem = DyGetRGBColorFromTU32(iJson.get<TU32>());
}

} /// ::dy namespace