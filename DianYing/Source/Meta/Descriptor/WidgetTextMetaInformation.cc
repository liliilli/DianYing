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
#include <Dy/Element/Helper/DescriptorHelperFunctions.h>

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
      "EdgeSize": 1,
      "FontSpecifierName": "Arial",
      "IsBold": true,
      "IsItalic": false,
      "IsUsingEdge": false,
      "IsUsingDefaultFont": true
    }
  }
  */

  static MDY_SET_IMMUTABLE_STRING(sHeader_InitialString,     "InitialString");
  static MDY_SET_IMMUTABLE_STRING(sHeader_InitialPosition,   "InitialPosition");
  static MDY_SET_IMMUTABLE_STRING(sHeader_InitialColor,      "InitialColor");
  static MDY_SET_IMMUTABLE_STRING(sHeader_InitialAlpha,      "InitialAlpha");
  static MDY_SET_IMMUTABLE_STRING(sHeader_FontSize,          "FontSize");
  static MDY_SET_IMMUTABLE_STRING(sHeader_EdgeSize,          "EdgeSize");
  static MDY_SET_IMMUTABLE_STRING(sHeader_FontSpecifierName, "FontSpecifierName");
  static MDY_SET_IMMUTABLE_STRING(sHeader_IsBold,            "IsBold");
  static MDY_SET_IMMUTABLE_STRING(sHeader_IsItalic,          "IsItalic");
  static MDY_SET_IMMUTABLE_STRING(sHeader_IsUsingEdge,       "IsUsingEdge");
  static MDY_SET_IMMUTABLE_STRING(sHeader_IsUsingDefaultFont,"IsUsingDefaultFont");

  using TPDyMWCBD = PDyMetaWidgetCommonBaseDesc;

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Common
  auto instance = std::make_unique<PDyMetaWidgetTextDescriptor>();
  instance->mComponentSpecifierName = DyGetValue<std::string>(itemAtlas, TPDyMWCBD::sHeader_Name);
  instance->mComponentType          = EDyWidgetComponentType::Text;
  instance->mParentSpecifierName    = DyGetValue<std::string>(itemAtlas, TPDyMWCBD::sHeader_Parent);

  // Detail (TEXT)
  const auto& detailAtlas = itemAtlas[MSVSTR(TPDyMWCBD::sHeader_Details)];
  instance->mInitialString    = DDyString{DyGetValue<std::string>(detailAtlas, sHeader_InitialString)};
  instance->mInitialPosition  = DDyVectorInt2{DyGetDDyVector2FromJson(detailAtlas[MSVSTR(sHeader_InitialPosition)])};

  DDyColor colorRgb = DyGetRGBColorFromTU32(DyGetValue<TU32>(detailAtlas, sHeader_InitialColor));
  colorRgb.A        = DyGetValue<float>(detailAtlas, sHeader_InitialAlpha);
  instance->mInitialColor       = colorRgb;
  instance->mFontSize           = DyGetValue<TU32>(detailAtlas, sHeader_FontSize);
  instance->mEdgeSize           = DyGetValue<TU32>(detailAtlas, sHeader_EdgeSize);
  instance->mFontSpecifierName  = DyGetValue<std::string>(detailAtlas, sHeader_FontSpecifierName);
  instance->mIsBold             = DyGetValue<bool>(detailAtlas, sHeader_IsBold);
  instance->mIsItalic           = DyGetValue<bool>(detailAtlas, sHeader_IsItalic);
  instance->mIsUsingEdge        = DyGetValue<bool>(detailAtlas, sHeader_IsUsingEdge);
  instance->mIsUsingDefaultFont = DyGetValue<bool>(detailAtlas, sHeader_IsUsingDefaultFont);
  return instance;
}

} /// ::dy namespace