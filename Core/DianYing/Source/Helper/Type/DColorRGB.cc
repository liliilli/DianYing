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
#include <Dy/Helper/Type/DColorRGB.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/DColorRGB24.h>
#include <nlohmann/json.hpp>

namespace dy
{
  
const DColorRGB DColorRGB::Aqua       = DColorRGB24{0x00, 0xAA, 0xFF};
const DColorRGB DColorRGB::Black      = DColorRGB24{0x00, 0x00, 0x00};
const DColorRGB DColorRGB::Blue       = DColorRGB24{0x00, 0x00, 0xFF};
const DColorRGB DColorRGB::DarkRed    = DColorRGB24{0xAA, 0x00, 0x00};
const DColorRGB DColorRGB::DarkGray   = DColorRGB24{0x55, 0x55, 0x55};
const DColorRGB DColorRGB::DarkGreen  = DColorRGB24{0x00, 0xAA, 0x00};
const DColorRGB DColorRGB::Gold       = DColorRGB24{0xFF, 0xAA, 0x00};
const DColorRGB DColorRGB::Gray       = DColorRGB24{0xAA, 0xAA, 0xAA};
const DColorRGB DColorRGB::Green      = DColorRGB24{0x00, 0xFF, 0x00};
const DColorRGB DColorRGB::Magenta    = DColorRGB24{0xAA, 0x00, 0xAA};
const DColorRGB DColorRGB::Orange     = DColorRGB24{0xFF, 0x55, 0x00};
const DColorRGB DColorRGB::Purple     = DColorRGB24{0xAA, 0x00, 0xFF};
const DColorRGB DColorRGB::Red        = DColorRGB24{0xFF, 0x00, 0x00};
const DColorRGB DColorRGB::White      = DColorRGB24{0xFF, 0xFF, 0xFF};
const DColorRGB DColorRGB::Yellow     = DColorRGB24{0xFF, 0xFF, 0x00};

TF32 DColorRGB::GetGrayScale() const noexcept
{
  return 0.2126f * this->R + 0.7152f * this->G + 0.0722f * this->B;
}

const TF32* DColorRGB::Data() const noexcept
{
  return this->R.Data();
}

void to_json(nlohmann::json& oJson, const DColorRGB& iItem)
{
  oJson = nlohmann::json { {"R", iItem.R}, {"G", iItem.G}, {"B", iItem.B} }; 
}

void from_json(const nlohmann::json& iJson, DColorRGB& oItem)
{
  json::GetValueFromTo(iJson, "R", oItem.R);
  json::GetValueFromTo(iJson, "G", oItem.G);
  json::GetValueFromTo(iJson, "B", oItem.B);
}

} /// ::dy namespace