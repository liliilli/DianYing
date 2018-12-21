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
#include <Dy/Component/UI/CDyWidgetScript.h>
#include <Dy/Management/ScriptManager.h>

namespace dy
{

CDyWidgetScript::CDyWidgetScript(_MIN_ const std::string& iScriptSpecifier, _MINOUT_ FDyUiWidget& iWidget) :
  mPtrWidget{&iWidget},
  mPtrWidgetStatus{ MDyScript::GetInstance().CreateWidgetScript(iScriptSpecifier, *mPtrWidget, true) }
{ }

CDyWidgetScript::~CDyWidgetScript()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

} /// ::dy namespace