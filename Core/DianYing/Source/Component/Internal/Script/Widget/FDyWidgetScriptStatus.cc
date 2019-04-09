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
#include <Dy/Component/Internal/Script/FWidgetScriptState.h>
#include <Dy/Component/Internal/Widget/CBaseWidgetScript.h>
#include <Dy/Component/Internal/Widget/CWidgetScriptCpp.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy
{

FWidgetScriptState::FWidgetScriptState(FWidget& widgetReference, const PDyScriptInstanceMetaInfo& descriptor) 
  : mStatus{EScriptState::CalledNothing},
    mType{descriptor.mScriptType}
{
  switch (this->mType)
  {
  case EDyScriptType::Cpp: 
  { // Cpp
    this->mScriptInstance = std::make_unique<CWidgetScriptCpp>(widgetReference, descriptor);
  } break;
  case EDyScriptType::Lua: 
  { // Lua
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: MDY_UNEXPECTED_BRANCH();
  }
}

void FWidgetScriptState::CallScriptFunction(TF32 dt) noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance),"Script instace must be activated!");
  MDY_ASSERT_MSG(this->mStatus != EScriptState::NoneError, "FDyScriptState must be initialized!");

  switch (this->mStatus)
  {
  case EScriptState::CalledNothing:
    this->mScriptInstance->Initiate();
    this->mStatus = EScriptState::CalledInitiate;
    break;
  case EScriptState::CalledInitiate:
    this->mScriptInstance->Start();
    this->mScriptInstance->Update(dt);
    this->mStatus = EScriptState::CalledUpdate;
    break;
  case EScriptState::CalledUpdate:
    this->mScriptInstance->Update(dt);
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void FWidgetScriptState::MDY_PRIVATE(CallDestroyFunctionAnyway)() noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance),"Script instace must be activated!");
  this->mScriptInstance->Destroy();
}

EDyScriptType FWidgetScriptState::GetScriptType() const noexcept
{
  MDY_ASSERT_MSG(this->mType != decltype(this->mType)::NoneError, "Script type must be specified properly.");
  return this->mType;
}

EScriptState FWidgetScriptState::GetScriptStatus() const noexcept
{
  return this->mStatus;
}

CBaseWidgetScript* FWidgetScriptState::MDY_PRIVATE(GetPtrInternalWidgetScript)() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Internal script instance must be valid.");
  return this->mScriptInstance.get();
}

} /// ::dy namespace