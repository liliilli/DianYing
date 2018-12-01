#ifndef GUARD_DY_BUILTINSCRIPT_DEBUGUISCRIPTCPP_H
#define GUARD_DY_BUILTINSCRIPT_DEBUGUISCRIPTCPP_H
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

#include <Dy/Component/Abstract/ADyWidgetCppScript.h>
#include <Dy/Builtin/Abstract/ADyScriptResource.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>

namespace dy
{

class FDyBuiltinDebugUiScript final : public ADyScriptResource, public ADyWidgetCppScript
{
  MDY_REGISTER_RESOURCE_SCRIPT(FDyBuiltinDebugUiScript);
public:
  void Initiate() override final;

  void Start() override final;

  void Update(_MIN_ TF32 dt) override final;

private:

};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTINSCRIPT_DEBUGUISCRIPTCPP_H