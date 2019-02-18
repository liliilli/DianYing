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
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Helper/Library/HelperJson.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sType_Root, "Root");
MDY_SET_IMMUTABLE_STRING(sType_Text, "Text");
MDY_SET_IMMUTABLE_STRING(sType_Image, "Image");
MDY_SET_IMMUTABLE_STRING(sType_BasicGaugeBar, "BasicBar");
MDY_SET_IMMUTABLE_STRING(sType_HorizontalLayout, "HorizontalLayout");
MDY_SET_IMMUTABLE_STRING(sType_VerticalLayout,   "VerticalLayout");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyWidgetComponentType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyWidgetComponentType& p)
{
  const std::string typeString = j.get<std::string>();
  if      (typeString == sType_Root)              { p = EDyWidgetComponentType::Root; }
  else if (typeString == sType_Text)              { p = EDyWidgetComponentType::Text; }
  else if (typeString == sType_HorizontalLayout)  { p = EDyWidgetComponentType::HorizontalLayout; }
  else if (typeString == sType_VerticalLayout)    { p = EDyWidgetComponentType::VerticalLayout;   }
  else if (typeString == sType_BasicGaugeBar)     { p = EDyWidgetComponentType::BasicGaugeBar; }
  else if (typeString == sType_Image)             { p = EDyWidgetComponentType::Image; }
  else                                            { MDY_UNEXPECTED_BRANCH(); }
}

std::unique_ptr<PDyMetaWidgetRootDescriptor>
PDyMetaWidgetRootDescriptor::CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas)
{
  auto instance = std::make_unique<PDyMetaWidgetRootDescriptor>();

  DyJsonGetValueFromTo(itemAtlas, sHeader_Name,   instance->mWidgetSpecifierName);
  DyJsonGetValueFromTo(itemAtlas, "ZOrder",       instance->mZOrder);
  DyJsonGetValueFromTo(itemAtlas, sHeader_Script, instance->mScriptReference);

  return instance;
}

} /// ::dy namespace