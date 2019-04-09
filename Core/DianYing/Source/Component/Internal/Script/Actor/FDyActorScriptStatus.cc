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
#include <Dy/Component/Internal/Actor/FActorScriptState.h>
#include <Dy/Component/Internal/Actor/CBaseActorScript.h>
#include <Dy/Component/Internal/Actor/CActorScriptCpp.h>
#include <Dy/Component/Internal/Actor/CActorScriptLua.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy
{

FActorScriptState::FActorScriptState(FActor& iRefActor, const PDyScriptInstanceMetaInfo& descriptor) 
  : mStatus {EScriptState::CalledNothing},
    mType   {descriptor.mScriptType}
{
  switch (this->mType)
  {
  case EDyScriptType::Cpp: 
  { 
    this->mScriptInstance = std::make_unique<CActorScriptCpp>(iRefActor, descriptor);
  } break;
  case EDyScriptType::Lua: 
  { 
    this->mScriptInstance = std::make_unique<CActorScriptLua>(iRefActor, descriptor);
  } break;
  default: MDY_NOT_IMPLEMENTED_ASSERT();
  }
}

void FActorScriptState::CallScriptFunction(_MIN_ TF32 dt) noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance),"Script instace must be activated!");
  MDY_ASSERT_MSG(this->mStatus != EScriptState::NoneError, "FActorScriptState must be initialized!");

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

void FActorScriptState::MDY_PRIVATE(CallDestroyFunctionAnyway)() noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Script instace must be activated!");
  this->mScriptInstance->Destroy();
}

EDyScriptType FActorScriptState::GetScriptType() const noexcept
{
  MDY_ASSERT_MSG(this->mType != decltype(this->mType)::NoneError, "Script type must be specified properly.");
  return this->mType;
}

EScriptState FActorScriptState::GetScriptStatus() const noexcept
{
  return this->mStatus;
}

CBaseActorScript* FActorScriptState::MDY_PRIVATE(GetPtrInternalActorScript)() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mScriptInstance), "Internal script instance must be valid.");
  return this->mScriptInstance.get();
}

} /// ::dy namespace