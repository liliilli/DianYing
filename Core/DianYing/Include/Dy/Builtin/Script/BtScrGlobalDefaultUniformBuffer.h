#ifndef GUARD_DY_BUILTIN_SCRIPT_FGLOBALDEFAULTUNIFORMBUFFER_H
#define GUARD_DY_BUILTIN_SCRIPT_FGLOBALDEFAULTUNIFORMBUFFER_H
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
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Component/Abstract/AGlobalCppScript.h>
#include <Dy/Component/Abstract/AActorCppScript.h>

namespace dy
{
   
class BtScrGlobalDefaultUniformBuffer final : public AScriptResource, public AGlobalCppScript
{
  MDY_REGISTER_RESOURCE_SCRIPT(BtScrGlobalDefaultUniformBuffer, "BtGblDefaultUniformBuffer")
public:
  void OnStart() override;
  void OnEnd() override;
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_SCRIPT_FGLOBALDEFAULTUNIFORMBUFFER_H