#include <precompiled.h>
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

/// Header file
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Type/ColorRGB24.h>

namespace dy
{

std::unique_ptr<PDyMetaWidgetTextDescriptor>
PDyMetaWidgetTextDescriptor::CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas)
{
  /* Template
  {
    "Name": "WidgetName",
    "Type": "Text",
    "Parent": "Root",
    "Details":
    {
      "InitialString": "Hello world!",
      "InitialPosition": { "X":0, "Y":0 },
      "InitialColor": 16777215,
      "InitialAlpha": 1.0,
      "FontSize": 10,
      "EdgeColor": 0,
      "FontSpecifierName": "Arial",
      "IsUsingEdge": false,
    }
  }
  */

  static MDY_SET_IMMUTABLE_STRING(sHeader_InitialString,     "InitialString");
  static MDY_SET_IMMUTABLE_STRING(sHeader_InitialPosition,   "InitialPosition");
  static MDY_SET_IMMUTABLE_STRING(sHeader_InitialColor,      "InitialColor");
  static MDY_SET_IMMUTABLE_STRING(sHeader_InitialAlpha,      "InitialAlpha");
  static MDY_SET_IMMUTABLE_STRING(sHeader_FontSize,          "FontSize");
  static MDY_SET_IMMUTABLE_STRING(sHeader_FontSpecifierName, "FontSpecifierName");
  static MDY_SET_IMMUTABLE_STRING(sHeader_IsUsingEdge,       "IsUsingEdge");

  using TPDyMWCBD = PDyMetaWidgetCommonBaseDesc;

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Common
  auto instance = std::make_unique<PDyMetaWidgetTextDescriptor>();
  instance->mUiObjectSpecifierName = DyJsonGetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Name);
  instance->mComponentType          = EDyWidgetComponentType::Text;
  instance->mParentSpecifierName    = DyJsonGetValueFrom<std::string>(itemAtlas, TPDyMWCBD::sHeader_Parent);

  // Detail (TEXT)
  const auto& detailAtlas = itemAtlas[MSVSTR(TPDyMWCBD::sHeader_Details)];
  instance->mInitialString    = DDyString{DyJsonGetValueFrom<std::string>(detailAtlas, sHeader_InitialString)};
  instance->mInitialPosition  = DDyVectorInt2{DyJsonGetValueFrom<DDyVector2>(detailAtlas, sHeader_InitialPosition)};

  DDyColorRGBA colorRgb         = DyJsonGetValueFrom<DDyColorRGB24>(detailAtlas, sHeader_InitialColor);
  colorRgb.A                    = DyJsonGetValueFrom<float>(detailAtlas, sHeader_InitialAlpha);

  instance->mInitialColor       = colorRgb;
  instance->mFontSize           = DyJsonGetValueFrom<TU32>(detailAtlas, sHeader_FontSize);
  instance->mFontSpecifierName  = DyJsonGetValueFrom<std::string>(detailAtlas, sHeader_FontSpecifierName);
  instance->mIsUsingEdge        = DyJsonGetValueFrom<bool>(detailAtlas, sHeader_IsUsingEdge);
  instance->mEdgeColor          = DyJsonGetValueFrom<DDyColorRGB24>(detailAtlas, "EdgeColor");

  return instance;
}

} /// ::dy namespace