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
#include <Dy/Component/Internal/Widget/CWidgetScriptCpp.h>

#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Helper/System/Pointer.h>

namespace dy
{

CWidgetScriptCpp::CWidgetScriptCpp(FWidget& widgetReference, const PDyScriptInstanceMetaInfo& metaInfo) 
  : CBaseWidgetScript{widgetReference}
{
  MDY_ASSERT_MSG(metaInfo.mScriptType == EDyScriptType::Cpp,    "Script type is not matched to CWidgetScriptCpp.");
  MDY_ASSERT_MSG(metaInfo.mScriptMode == EDyScriptMode::Widget, "Given script must be widget type.");

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(metaInfo.mBtInstantiationFunction), "Cpp script instantiation function must be not null.");
  this->mScriptInstance = DyConvertUniquePtrTo<AWidgetCppScript>(metaInfo.mBtInstantiationFunction());
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mScriptInstance),    "Script instance could not bound to system.");

  this->mScriptName = metaInfo.mSpecifierName;
  this->mScriptInstance->pfSetOutsideReference(*this);
  this->mIsScriptInstanceBinded = true;
}

AWidgetCppScript* CWidgetScriptCpp::MDY_PRIVATE(GetScriptInstance)() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Script instance must be valid, not empty.");
  return this->mScriptInstance.get();
}

void CWidgetScriptCpp::Initiate()
{
  this->mScriptInstance->Initiate();
}

void CWidgetScriptCpp::Start()
{
  this->mScriptInstance->Start();
}

void CWidgetScriptCpp::Update(TF32 dt)
{
  this->mScriptInstance->Update(dt);
}

void CWidgetScriptCpp::OnEnabled()
{
  this->mScriptInstance->OnEnabled();
}

void CWidgetScriptCpp::OnDisabled()
{
  this->mScriptInstance->OnDisabled();
}

void CWidgetScriptCpp::Destroy()
{
  this->mScriptInstance->Destroy();
}

} /// ::dy namespace