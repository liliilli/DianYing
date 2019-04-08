#ifndef GUARD_DY_BUILTINSCRIPT_FDYBTSDCTESTLOADINGUI_H
#define GUARD_DY_BUILTINSCRIPT_FDYBTSDCTESTLOADINGUI_H
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
#include <Dy/Builtin/Abstract/AScriptResource.h>
#include <Dy/Component/Abstract/AActorCppScript.h>
#include <Dy/Component/Abstract/AGlobalCppScript.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>

namespace dy::builtin
{

class FDyBtScTestLoadingUi final : public AScriptResource, public AWidgetCppScript
{
  MDY_REGISTER_RESOURCE_SCRIPT(FDyBtScTestLoadingUi, "FDyBtScTestLoadingUi");
public:
  void Initiate() override final;

  void Start() override final;

  void Update(_MIN_ TF32 dt) override final;

  void Destroy() override final;
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTINSCRIPT_FDYBTSDCTESTLOADINGUI_H