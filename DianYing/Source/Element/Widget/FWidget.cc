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
#include <Dy/Element/Widget/FWidget.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetBarMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetImageMetaInformation.h>
#include <Dy/Component/Internal/Widget/CWidgetScriptCpp.h>
#include <Dy/Component/Internal/Widget/CWidgetScriptLua.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Element/Widget/FWidgetText.h>
#include <Dy/Element/Widget/FWidgetBasicGaugeBar.h>
#include <Dy/Element/Widget/FWidgetImage.h>
#include <Dy/Element/Type/DWidgetBinder.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/MWindow.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/MScript.h>
#include <Dy/Management/MWorld.h>

namespace dy
{

FWidget::FWidget(const PDyMetaWidgetRootDescriptor& widgetMetaDesc)
{
  // (1) Get position and frame size from meta.
  // @TODO TEMPORARY CODE
  this->SetFrameSize      ({ 1280, 720 });
  this->SetRelativePosition({ 640, 360 });
  this->mZOrder = widgetMetaDesc.mZOrder;

  // (2) Create UI objects.
  for (const auto& [specifier, objectMetaInfoPair] : widgetMetaDesc.mChildComponentList)
  {
    const auto& [objectType, objectMetaInfoPtr] = objectMetaInfoPair;
    switch (objectType)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyWidgetComponentType::BasicGaugeBar:
    {
      this->CreateWidget<FWidgetBasicGaugeBar>(*static_cast<PDyMetaWidgetBarDescriptor*>(objectMetaInfoPtr.get()));
    } break;
    case EDyWidgetComponentType::Text:
    {
      this->CreateWidget<FWidgetText>(*static_cast<PDyMetaWidgetTextDescriptor*>(objectMetaInfoPtr.get()));
    } break;
    case EDyWidgetComponentType::Image:
    {
      this->CreateWidget<FWidgetImage>(*static_cast<PDyMetaWidgetImageDescriptor*>(objectMetaInfoPtr.get()));
    } break;
    }
  }

  // (3) Make script instance following meta information.
  if (widgetMetaDesc.mIsUsingScript == true)
  {
    this->pSetObjectName(widgetMetaDesc.mWidgetSpecifierName);
    if (const auto& scriptName = widgetMetaDesc.mScriptReference.mDetails.mSpecifierName;
        scriptName.empty() == false)
    {
      this->mWidgetScript = std::make_unique<CWidgetScript>(scriptName, *this);
    }
  }

  // (4) Activate when flag is true.
  if (widgetMetaDesc.mIsActivated == true) { this->Activate(); }
  this->SetupFlagAsParent(true);
}

FWidget::~FWidget()
{
  if (this->IsActivated() == true) { this->Deactivate(); }

  // If there are binder to being bound with this, detach them from this.
  if (this->mBoundUiBinderList.empty() == false)
  {
    for (auto& ptrBind : this->mBoundUiBinderList)
    {
      ptrBind->MDY_PRIVATE(DetachUiObjectFromUiObject)();
    }
    this->mBoundUiBinderList.clear();
  }
}

void FWidget::Render() 
{ 
  if (this->IsActivated() == false) { return; }

  AWidgetUiObjectChildrenable::Render(); 
}

void FWidget::__TryAttachBinderFromBinder(DWidgetBinder& iRefUiBinder)
{
  this->mBoundUiBinderList.emplace_back(DyMakeNotNull(&iRefUiBinder));
}

void FWidget::__TryDetachBinderFromBinder(DWidgetBinder& iRefUiBinder)
{
  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mBoundUiBinderList), 
      [ptrSource = &iRefUiBinder](const auto& ptrBinder) { return ptrBinder.Get() == ptrSource; });
  MDY_ASSERT_MSG_FORCE(it != this->mBoundUiBinderList.end(), "Failed guarantee UiBinder binding integrity.");

  // Fast erase.
  FaseErase(this->mBoundUiBinderList, it);
}

void FWidget::__SetName(_MIN_ const std::string& iNewName)
{
  this->pSetObjectName(iNewName);
}

void FWidget::TryActivateInstance()
{
  auto& refWorld = MWorld::GetInstance();
  refWorld.MDY_PRIVATE(BindActiveUiObject)(*this);

  AWidgetUiObjectChildrenable::TryActivateInstance();
}

void FWidget::TryDeactivateInstance()
{
  auto& refWorld = MWorld::GetInstance();
  refWorld.MDY_PRIVATE(UnbindActiveUiObject)(*this);

  AWidgetUiObjectChildrenable::TryDeactivateInstance();
}

} /// ::dy namespace