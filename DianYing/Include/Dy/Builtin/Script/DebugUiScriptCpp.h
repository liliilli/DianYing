#ifndef GUARD_DY_BUILTINSCRIPT_DEBUGUISCRIPTCPP_H
#define GUARD_DY_BUILTINSCRIPT_DEBUGUISCRIPTCPP_H
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

#include <Dy/Component/Abstract/AWidgetCppScript.h>
#include <Dy/Component/Abstract/AActorCppScript.h>
#include <Dy/Component/Abstract/AGlobalCppScript.h>
#include <Dy/Builtin/Abstract/AScriptResource.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Element/Type/Timer/FDyTimerHandle.h>

namespace dy
{
class MTime;
class MProfiling;

class FDyBuiltinDebugUiScript final : public AScriptResource, public AWidgetCppScript
{
  MDY_REGISTER_RESOURCE_SCRIPT(FDyBuiltinDebugUiScript, "FDyBuiltinDebugUiScript");
public:
  void Initiate() override final;

  void Start() override final;

  void Update(_MIN_ TF32 dt) override final;

  void Bar_MoveLeft(_MIN_ TF32 iXAxis) noexcept;
  void Bar_MoveUp(_MIN_ TF32 iYAxis) noexcept;
  void EndApplication() noexcept;

private:
  MTime*      mTimeManager      = nullptr;
  MProfiling* mProfilingManger  = nullptr;

  FDyTimerHandle mTimerHandle;
  FDyTimerHandle mTimerHandle2;

  void CbMoveBar();
  void CbChangeImageTexture();

  bool flag = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTINSCRIPT_DEBUGUISCRIPTCPP_H