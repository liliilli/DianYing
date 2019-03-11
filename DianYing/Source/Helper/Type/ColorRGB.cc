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
#include <Dy/Helper/Type/ColorRGB.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/ColorRGB24.h>

namespace dy
{
  
const DDyColorRGB DDyColorRGB::Aqua       = DDyColorRGB24{0x00, 0xAA, 0xFF};
const DDyColorRGB DDyColorRGB::Black      = DDyColorRGB24{0x00, 0x00, 0x00};
const DDyColorRGB DDyColorRGB::Blue       = DDyColorRGB24{0x00, 0x00, 0xFF};
const DDyColorRGB DDyColorRGB::DarkRed    = DDyColorRGB24{0xAA, 0x00, 0x00};
const DDyColorRGB DDyColorRGB::DarkGray   = DDyColorRGB24{0x55, 0x55, 0x55};
const DDyColorRGB DDyColorRGB::DarkGreen  = DDyColorRGB24{0x00, 0xAA, 0x00};
const DDyColorRGB DDyColorRGB::Gold       = DDyColorRGB24{0xFF, 0xAA, 0x00};
const DDyColorRGB DDyColorRGB::Gray       = DDyColorRGB24{0xAA, 0xAA, 0xAA};
const DDyColorRGB DDyColorRGB::Green      = DDyColorRGB24{0x00, 0xFF, 0x00};
const DDyColorRGB DDyColorRGB::Magenta    = DDyColorRGB24{0xAA, 0x00, 0xAA};
const DDyColorRGB DDyColorRGB::Orange     = DDyColorRGB24{0xFF, 0x55, 0x00};
const DDyColorRGB DDyColorRGB::Purple     = DDyColorRGB24{0xAA, 0x00, 0xFF};
const DDyColorRGB DDyColorRGB::Red        = DDyColorRGB24{0xFF, 0x00, 0x00};
const DDyColorRGB DDyColorRGB::White      = DDyColorRGB24{0xFF, 0xFF, 0xFF};
const DDyColorRGB DDyColorRGB::Yellow     = DDyColorRGB24{0xFF, 0xFF, 0x00};

TF32 DDyColorRGB::GetGrayScale() const noexcept
{
  return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
}

const TF32* DDyColorRGB::Data() const noexcept
{
  return this->R.Data();
}

void to_json(nlohmann::json& oJson, const DDyColorRGB& iItem)
{
  oJson = nlohmann::json { {"R", iItem.R}, {"G", iItem.G}, {"B", iItem.B} }; 
}

void from_json(const nlohmann::json& iJson, DDyColorRGB& oItem)
{
  DyJsonGetValueFromTo(iJson, "R", oItem.R);
  DyJsonGetValueFromTo(iJson, "G", oItem.G);
  DyJsonGetValueFromTo(iJson, "B", oItem.B);
}

} /// ::dy namespace