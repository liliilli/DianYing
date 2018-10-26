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
#include <Dy/Helper/JsonHelper.h>

namespace dy
{

EDyWidgetComponentType DyGetWidgetComponentTypeFrom(_MIN_ const std::string& typeString)
{
  MDY_SET_IMMUTABLE_STRING(sType_Root, "Root");
  MDY_SET_IMMUTABLE_STRING(sType_Text, "Text");
  MDY_SET_IMMUTABLE_STRING(sType_HorizontalBox, "HorizontalBox");
  MDY_SET_IMMUTABLE_STRING(sType_VerticalBox,   "VerticalBox");

  if      (typeString == sType_Root) { return EDyWidgetComponentType::Root; }
  else if (typeString == sType_Text) { return EDyWidgetComponentType::Text; }
  else if (typeString == sType_HorizontalBox) { return EDyWidgetComponentType::HorizontalBox; }
  else if (typeString == sType_VerticalBox)   { return EDyWidgetComponentType::VerticalBox;   }
  else return EDyWidgetComponentType::NoneError;
}

std::unique_ptr<PDyMetaWidgetRootDescriptor>
PDyMetaWidgetRootDescriptor::CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas)
{
  // Common
  auto instance = std::make_unique<PDyMetaWidgetRootDescriptor>();
  instance->mWidgetSpecifierName = DyGetValue<std::string>(itemAtlas, PDyMetaWidgetCommonBaseDesc::sHeader_Name);
  instance->mScriptSpecifierName = DyGetValue<std::string>(itemAtlas, PDyMetaWidgetCommonBaseDesc::sHeader_Script);

  return instance;
}

} /// ::dy namespace