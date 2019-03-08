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
#include <Dy/Component/Internal/Actor/FDyActorScriptStatus.h>
#include <Dy/Component/Internal/Actor/CDyActorScriptBase.h>
#include <Dy/Component/Internal/Actor/CDyActorScriptCpp.h>
#include <Dy/Component/Internal/Actor/CDyActorScriptLua.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>

namespace dy
{

FDyActorScriptState::FDyActorScriptState(_MIN_ FDyActor& iRefActor, _MIN_ const PDyScriptInstanceMetaInfo& descriptor) :
    mStatus {EDyScriptState::CalledNothing},
    mType   {descriptor.mScriptType}
{
  switch (this->mType)
  {
  case EDyScriptType::Cpp: 
  { 
    this->mScriptInstance = std::make_unique<CDyActorScriptCpp>(iRefActor, descriptor);
  } break;
  case EDyScriptType::Lua: 
  { 
    this->mScriptInstance = std::make_unique<CDyActorScriptLua>(iRefActor, descriptor);
  } break;
  default: MDY_NOT_IMPLEMENTED_ASSERT();
  }
}

void FDyActorScriptState::CallScriptFunction(_MIN_ TF32 dt) noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance),"Script instace must be activated!");
  MDY_ASSERT_MSG(this->mStatus != EDyScriptState::NoneError, "FDyActorScriptState must be initialized!");

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

void FDyActorScriptState::MDY_PRIVATE(CallDestroyFunctionAnyway)() noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Script instace must be activated!");
  this->mScriptInstance->Destroy();
}

EDyScriptType FDyActorScriptState::GetScriptType() const noexcept
{
  MDY_ASSERT_MSG(this->mType != decltype(this->mType)::NoneError, "Script type must be specified properly.");
  return this->mType;
}

EDyScriptState FDyActorScriptState::GetScriptStatus() const noexcept
{
  return this->mStatus;
}

CDyActorScriptBase* FDyActorScriptState::MDY_PRIVATE(GetPtrInternalActorScript)() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Internal script instance must be valid.");
  return this->mScriptInstance.get();
}

} /// ::dy namespace