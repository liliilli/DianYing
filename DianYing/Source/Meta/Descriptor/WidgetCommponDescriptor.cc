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
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Helper/Library/HelperJson.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sType_Root, "Root");
MDY_SET_IMMUTABLE_STRING(sType_Text, "Text");
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
  else                                            { MDY_UNEXPECTED_BRANCH(); }
}

std::unique_ptr<PDyMetaWidgetRootDescriptor>
PDyMetaWidgetRootDescriptor::CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas)
{
  // Common
  auto instance = std::make_unique<PDyMetaWidgetRootDescriptor>();
  instance->mWidgetSpecifierName = DyJsonGetValueFrom<std::string>(itemAtlas, PDyMetaWidgetRootDescriptor::sHeader_Name);
  instance->mScriptReference     = DyJsonGetValueFrom<PDyScriptReferenceMetaInfo>(itemAtlas, PDyMetaWidgetRootDescriptor::sHeader_Script);

  return instance;
}

} /// ::dy namespace