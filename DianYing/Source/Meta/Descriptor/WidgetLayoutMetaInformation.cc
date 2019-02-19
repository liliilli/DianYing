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
#include <Dy/Meta/Descriptor/WidgetLayoutMetaInformation.h>
#include <Dy/Helper/Library/HelperJson.h>

//!
//! Forward declaration & local translation unit data
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_Area,     "Area");
MDY_SET_IMMUTABLE_STRING(sHeader_Alignment,"Alignment");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

std::unique_ptr<PDyMetaWidgetHorizontalLayout>
PDyMetaWidgetHorizontalLayout::CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas)
{
  /* Template
   *  {
        "Name": "__HorizontalBox",
        "Type": "HorizontalLayout",
        "Parent": "Root",
        "Details": {
          "Area": {
            "LeftDown": { "X": 0.0, "Y": 0.0 },
            "RightUp" : { "X": 1.0, "Y": 1.0 }
          },
          "Alignment": "Left"
        }
      },
  */

  using TPDyMWCBD = PDyMetaWidgetCommonBaseDesc;

  auto instance = std::make_unique<PDyMetaWidgetHorizontalLayout>();
  instance->mUiObjectSpecifierName = DyJsonGetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Name);
  instance->mComponentType          = EDyWidgetComponentType::HorizontalLayout;
  instance->mParentSpecifierName    = DyJsonGetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Parent);
  DyJsonGetValueFromTo(itemAtlas, "ZOrder", instance->mZOrder);

  // Detail (PDyMetaWidgetHorizontalLayout)
  const auto& detailAtlas     = itemAtlas[MSVSTR(TPDyMWCBD::sHeader_Details)];
  instance->mAlignment        = DyJsonGetValueFrom<EDyHorizontalAlignment>(detailAtlas, sHeader_Alignment);
  instance->mArea             = DyJsonGetValueFrom<DDyArea2D>             (detailAtlas, sHeader_Area);

  return instance;
}

std::unique_ptr<PDyMetaWidgetVerticalLayout>
PDyMetaWidgetVerticalLayout::CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas)
{
  /* Template
   *  {
        "Name": "Vert",
        "Type": "VerticalLayout",
        "Parent": "Root",
        "Details": {
          "Area": {
            "LeftDown": { "X": 0.0, "Y": 0.0 },
            "RightUp" : { "X": 1.0, "Y": 1.0 }
          },
          "Alignment": "Bottom"
        }
      }
  */

  using TPDyMWCBD = PDyMetaWidgetCommonBaseDesc;

  auto instance = std::make_unique<PDyMetaWidgetVerticalLayout>();
  instance->mUiObjectSpecifierName = DyJsonGetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Name);
  instance->mComponentType          = EDyWidgetComponentType::VerticalLayout;
  instance->mParentSpecifierName    = DyJsonGetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Parent);
  DyJsonGetValueFromTo(itemAtlas, "ZOrder", instance->mZOrder);

  // Detail (PDyMetaWidgetHorizontalLayout)
  const auto& detailAtlas     = itemAtlas[MSVSTR(TPDyMWCBD::sHeader_Details)];
  instance->mAlignment        = DyJsonGetValueFrom<EDyVerticalAlignment>(detailAtlas, sHeader_Alignment);
  instance->mArea             = DyJsonGetValueFrom<DDyArea2D>           (detailAtlas, sHeader_Area);

  return instance;
}

} /// ::dy namespace