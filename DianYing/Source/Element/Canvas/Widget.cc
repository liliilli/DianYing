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
#include <Dy/Element/Canvas/Widget.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>

#include <Dy/Component/UI/CDyWidgetScriptCpp.h>
#include <Dy/Component/UI/CDyWidgetScriptLua.h>

//!
//! Forward declaration
//!

namespace
{

std::unique_ptr<dy::FDyText> testWidget = nullptr;

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess FDyUiWidget::Initialize(_MIN_ const PDyMetaWidgetRootDescriptor& widgetMetaDesc)
{
  ///
  /// @brief Make script component (lua or cpp)
  ///
  static auto MakeScriptComponent = [this](const PDyScriptComponentMetaInfo& info)
  {
    const auto& instanceInfo = MDyMetaInfo::GetInstance().GetScriptMetaInformation(info.mDetails.mSpecifierName);
    MDY_ASSERT(instanceInfo.mScriptType != EDyScriptType::NoneError, "");

    std::unique_ptr<CDyWidgetScript> component = nullptr;
    if (instanceInfo.mScriptType == EDyScriptType::Cpp)
    {
      auto componentPtr = std::make_unique<CDyWidgetScriptCpp>(std::ref(*this));
      MDY_CALL_ASSERT_SUCCESS(componentPtr->Initialize(info));
      component = std::move(componentPtr);
    }
    else
    {
      auto componentPtr = std::make_unique<CDyWidgetScriptLua>(std::ref(*this));
      MDY_CALL_ASSERT_SUCCESS(componentPtr->Initialize(info));
      component = std::move(componentPtr);
    }

    return component;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // (1) Make script instance following meta information.
  this->pSetObjectName(widgetMetaDesc.mWidgetSpecifierName);
  if (const auto& scriptName = widgetMetaDesc.mScriptReference.mDetails.mSpecifierName;
      scriptName.empty() == false)
  {
    this->mWidgetScript = MakeScriptComponent(widgetMetaDesc.mScriptReference);
    MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mWidgetScript), "Widget script must be valid in this case.");
  }

  // (2) Create UI objects.
  for (const auto& [specifier, objectMetaInfoPair] : widgetMetaDesc.mChildComponentList)
  {
    const auto& [objectType, objectMetaInfoPtr] = objectMetaInfoPair;
    switch (objectType)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyWidgetComponentType::Text:
      testWidget = std::make_unique<FDyText>();
      MDY_CALL_ASSERT_SUCCESS(testWidget->Initialize(
          *static_cast<PDyMetaWidgetTextDescriptor*>(objectMetaInfoPtr.get()))
      );
      break;
    }
  }

  return DY_SUCCESS;
}

void FDyUiWidget::Release()
{
  this->mWidgetScript = nullptr;
}

void FDyUiWidget::Render()
{
  if (testWidget)
  {
    testWidget->SetText(std::to_string(MDyWindow::GetInstance().GetCpuUsage()));
    testWidget->Render();
  };
}

} /// ::dy namespace