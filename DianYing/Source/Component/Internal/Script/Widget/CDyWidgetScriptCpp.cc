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
#include <Dy/Component/Internal/Widget/CDyWidgetScriptCpp.h>

#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Helper/Pointer.h>

namespace dy
{

CDyWidgetScriptCpp::CDyWidgetScriptCpp(_MIN_ FDyUiWidget& widgetReference, _MIN_ const PDyScriptInstanceMetaInfo& metaInfo) :
    CDyWidgetScriptBase{widgetReference}
{
  MDY_ASSERT_MSG(metaInfo.mScriptType == EDyScriptType::Cpp,    "Script type is not matched to CDyWidgetScriptCpp.");
  MDY_ASSERT_MSG(metaInfo.mScriptMode == EDyScriptMode::Widget, "Given script must be widget type.");

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(metaInfo.mBtInstantiationFunction), "Cpp script instantiation function must be not null.");
  this->mScriptInstance = DyConvertUniquePtrTo<ADyWidgetCppScript>(metaInfo.mBtInstantiationFunction());
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mScriptInstance),    "Script instance could not bound to system.");

  this->mScriptName = metaInfo.mSpecifierName;
  this->mScriptInstance->pfSetOutsideReference(*this);
  this->mIsScriptInstanceBinded = true;
}

ADyWidgetCppScript* CDyWidgetScriptCpp::MDY_PRIVATE(GetScriptInstance)() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Script instance must be valid, not empty.");
  return this->mScriptInstance.get();
}

void CDyWidgetScriptCpp::Initiate()
{
  this->mScriptInstance->Initiate();
}

void CDyWidgetScriptCpp::Start()
{
  this->mScriptInstance->Start();
}

void CDyWidgetScriptCpp::Update(_MIN_ float dt)
{
  this->mScriptInstance->Update(dt);
}

void CDyWidgetScriptCpp::OnEnabled()
{
  this->mScriptInstance->OnEnabled();
}

void CDyWidgetScriptCpp::OnDisabled()
{
  this->mScriptInstance->OnDisabled();
}

void CDyWidgetScriptCpp::Destroy()
{
  this->mScriptInstance->Destroy();
}

} /// ::dy namespace