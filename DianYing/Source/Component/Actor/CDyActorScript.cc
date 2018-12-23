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
#include <Dy/Component/Actor/CDyActorScript.h>
#include <Dy/Management/ScriptManager.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Component/Internal/Actor/CDyActorScriptCpp.h>

namespace dy
{

CDyActorScript::CDyActorScript(_MIN_ FDyActor& iActor, _MIN_ const std::string& iScriptSpecifier) :
  ADyBaseComponent{ iActor },
  mPtrScriptStatus{ MDyScript::GetInstance().CreateActorScript(iScriptSpecifier, *ADyBaseComponent::GetBindedActor(), true) }
{ }

CDyActorScript::~CDyActorScript()
{
  switch (mPtrScriptStatus->GetScriptType())
  {
  case EDyScriptType::Cpp: 
  { // If Widget type is `Cpp`, do the thing.
    auto& i = MDyInput::GetInstance();

    // intentional.
    auto& ref = static_cast<CDyActorScriptCpp&>(*mPtrScriptStatus->MDY_PRIVATE_SPECIFIER(GetPtrInternalActorScript)());
    MDY_NOTUSED const auto _ = i.MDY_PRIVATE_SPECIFIER(TryDetachContollerActor)(*ref.MDY_PRIVATE_SPECIFIER(GetScriptInstance()));
  }
  case EDyScriptType::Lua: 
  { // If Widget type is `Lua`, do the thing. but not supported yet.
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  };
}

} /// ::dy namespace