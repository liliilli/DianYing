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
#include <Dy/Component/Internal/Script/FDyWidgetScriptStatus.h>
#include <Dy/Component/Internal/Widget/CDyWidgetScriptBase.h>
#include <Dy/Component/Internal/Widget/CDyWidgetScriptCpp.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>

namespace dy
{

FDyWidgetScriptState::FDyWidgetScriptState(
    _MIN_ FDyUiWidget& widgetReference, 
    _MIN_ const PDyScriptInstanceMetaInfo& descriptor) :
    mStatus{EDyScriptState::CalledNothing},
    mType{descriptor.mScriptType}
{
  switch (this->mType)
  {
  case EDyScriptType::Cpp: 
  { // Cpp
    this->mScriptInstance = std::make_unique<CDyWidgetScriptCpp>(widgetReference, descriptor);
  } break;
  case EDyScriptType::Lua: 
  { // Lua
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: MDY_NOT_IMPLEMENTED_ASSERT();
  }
}

void FDyWidgetScriptState::CallScriptFunction(_MIN_ float dt) noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance),"Script instace must be activated!");
  MDY_ASSERT_MSG(this->mStatus != EDyScriptState::NoneError, "FDyScriptState must be initialized!");

  switch (this->mStatus)
  {
  case EDyScriptState::CalledNothing:
    this->mScriptInstance->Initiate();
    this->mStatus = EDyScriptState::CalledInitiate;
    break;
  case EDyScriptState::CalledInitiate:
    this->mScriptInstance->Start();
    this->mScriptInstance->Update(dt);
    this->mStatus = EDyScriptState::CalledUpdate;
    break;
  case EDyScriptState::CalledUpdate:
    this->mScriptInstance->Update(dt);
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void FDyWidgetScriptState::MDY_PRIVATE(CallDestroyFunctionAnyway)() noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance),"Script instace must be activated!");
  this->mScriptInstance->Destroy();
}

EDyScriptType FDyWidgetScriptState::GetScriptType() const noexcept
{
  MDY_ASSERT_MSG(this->mType != decltype(this->mType)::NoneError, "Script type must be specified properly.");
  return this->mType;
}

EDyScriptState FDyWidgetScriptState::GetScriptStatus() const noexcept
{
  return this->mStatus;
}

CDyWidgetScriptBase* FDyWidgetScriptState::MDY_PRIVATE(GetPtrInternalWidgetScript)() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Internal script instance must be valid.");
  return this->mScriptInstance.get();
}

} /// ::dy namespace