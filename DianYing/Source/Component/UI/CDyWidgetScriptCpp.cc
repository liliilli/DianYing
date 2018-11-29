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
#include <Dy/Component/UI/CDyWidgetScriptCpp.h>

#include <Dy/Management/IO/MetaInfoManager.h>
#include "Dy/Helper/Pointer.h"

namespace dy
{

EDySuccess CDyWidgetScriptCpp::Initialize(const PDyScriptComponentMetaInfo& descriptor)
{
  const auto& metaInfo = MDyMetaInfo::GetInstance().GetScriptMetaInformation(descriptor.mDetails.mSpecifierName);
  MDY_ASSERT(metaInfo.mScriptType == EDyScriptType::Cpp,    "Script type is not matched to CDyWidgetScriptCpp.");
  MDY_ASSERT(metaInfo.mScriptMode == EDyScriptMode::Widget, "Given script must be widget type.");

  MDY_ASSERT(MDY_CHECK_ISNOTNULL(metaInfo.mBtInstantiationFunction), "Cpp script instantiation function must be not null.");
  this->mScriptInstance = DyConvertUniquePtrTo<ADyWidgetCppScript>(metaInfo.mBtInstantiationFunction());
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mScriptInstance),    "Script instance could not bound to system.");

  this->mScriptInstance->pfSetOutsideReference(*this);
  return DY_SUCCESS;
}

void CDyWidgetScriptCpp::Release()
{
  this->mScriptInstance = nullptr;
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