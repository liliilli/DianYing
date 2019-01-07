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
#include <Dy/Element/Canvas/Widget.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetBarMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetImageMetaInformation.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Element/Canvas/FDyBasicGaugeBar.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>

#include <Dy/Component/Internal/Widget/CDyWidgetScriptCpp.h>
#include <Dy/Component/Internal/Widget/CDyWidgetScriptLua.h>
#include <Dy/Element/Canvas/FDyImage.h>
#include <Dy/Management/ScriptManager.h>

namespace dy
{

FDyUiWidget::FDyUiWidget(_MIN_ const PDyMetaWidgetRootDescriptor& widgetMetaDesc)
{
  // (1) Get position and frame size from meta.
  this->SetFrameSize      ({ 1280, 720 });
  this->SetRelativePosition({ 640, 360 });

  // (2) Create UI objects.
  for (const auto& [specifier, objectMetaInfoPair] : widgetMetaDesc.mChildComponentList)
  {
    const auto& [objectType, objectMetaInfoPtr] = objectMetaInfoPair;
    switch (objectType)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyWidgetComponentType::BasicGaugeBar:
    {
      this->AddUiObject<FDyBasicGaugeBar>(*static_cast<PDyMetaWidgetBarDescriptor*>(objectMetaInfoPtr.get()));
    } break;
    case EDyWidgetComponentType::Text:
    {
      this->AddUiObject<FDyText>(*static_cast<PDyMetaWidgetTextDescriptor*>(objectMetaInfoPtr.get()));
    } break;
    case EDyWidgetComponentType::Image:
    {
      this->AddUiObject<FDyImage>(*static_cast<PDyMetaWidgetImageDescriptor*>(objectMetaInfoPtr.get()));
    } break;
    }
  }

  // (3) Make script instance following meta information.
  this->pSetObjectName(widgetMetaDesc.mWidgetSpecifierName);
  if (const auto& scriptName = widgetMetaDesc.mScriptReference.mDetails.mSpecifierName;
      scriptName.empty() == false)
  {
    this->mWidgetScript = std::make_unique<CDyWidgetScript>(scriptName, *this);
  }
}

void FDyUiWidget::Render() { FDyUiObjectChildrenable::Render(); }

} /// ::dy namespace