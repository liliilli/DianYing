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
#include <Dy/Component/Actor/CActorScript.h>
#include <Dy/Management/MScript.h>
#include <Dy/Management/MInput.h>
#include <Dy/Component/Internal/Actor/CActorScriptCpp.h>

namespace dy
{

CActorScript::CActorScript(FActor& iActor, const std::string& iScriptSpecifier) 
  : ABaseComponent{ iActor },
    mPtrScriptStatus{ MScript::GetInstance().CreateActorScript(
      iScriptSpecifier, 
      *ABaseComponent::GetBindedActor(), true) 
    }
{ }

CActorScript::~CActorScript()
{
  switch (this->mPtrScriptStatus->GetScriptType())
  {
  case EDyScriptType::Cpp: 
  { // If Widget type is `Cpp`, do the thing.
    auto& i = MInput::GetInstance();

    // intentional.
    auto& ref = static_cast<CActorScriptCpp&>(
      *this->mPtrScriptStatus->MDY_PRIVATE(GetPtrInternalActorScript)());

    auto& script = *ref.__GetScriptInstance();
    // Detach controller from this.
    MDY_NOTUSED const auto _ = i.MDY_PRIVATE(TryDetachContollerActor)(script);
    // Change related Actor timer status to abort for avoiding callback function call.
    script.MDY_PRIVATE(AbortAllValidTimerHandler)();
    script.MDY_PRIVATE(AbortAllCollisionCallback)();

    MScript::GetInstance().TryForwardActorScriptToGCList(this->mPtrScriptStatus);
  } break;
  case EDyScriptType::Lua: 
  { // If Widget type is `Lua`, do the thing. but not supported yet.
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  };
}

std::string CActorScript::ToString()
{ 
  return ""; 
}

void CActorScript::TryActivateInstance()
{
  /* Do nothing */
}

void CActorScript::TryDeactivateInstance()
{
  /* Do nothing */
}

} /// ::dy namespace