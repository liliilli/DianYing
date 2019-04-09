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
#include <Dy/Component/Widget/CWidgetScript.h>
#include <Dy/Helper/System/Assertion.h>
#include <Dy/Component/Internal/Widget/CWidgetScriptCpp.h>
#include <Dy/Component/Internal/Script/FWidgetScriptState.h>
#include <Dy/Management/MScript.h>
#include <Dy/Management/MInput.h>

namespace dy
{

CWidgetScript::CWidgetScript(const std::string& iScriptSpecifier, FWidget& iWidget) 
  : mPtrWidget{&iWidget},
    mPtrWidgetStatus{ MScript::GetInstance().CreateWidgetScript(iScriptSpecifier, *mPtrWidget, true) }
{ }

CWidgetScript::~CWidgetScript()
{
  switch (mPtrWidgetStatus->GetScriptType())
  {
  case EDyScriptType::Cpp: 
  { // If Widget type is `Cpp`, do the thing.
    auto& i = MInput::GetInstance();

    // intentional.
    auto& ref = static_cast<CWidgetScriptCpp&>(*mPtrWidgetStatus->MDY_PRIVATE(GetPtrInternalWidgetScript)());
    MDY_NOTUSED const auto _ = i.MDY_PRIVATE(TryDetachContollerUi(*ref.MDY_PRIVATE(GetScriptInstance())));
    // Change related Actor timer status to abort for avoiding callback function call.
    auto& script = *ref.__GetScriptInstance();
    script.MDY_PRIVATE(AbortAllValidTimerHandler)();
    MScript::GetInstance().TryForwardWidgetScriptToGCList(mPtrWidgetStatus);
  } break;
  case EDyScriptType::Lua: 
  { // If Widget type is `Lua`, do the thing. but not supported yet.
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  };
}

} /// ::dy namespace