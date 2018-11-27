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
#include <Dy/Builtin/Script/DebugUiScriptCpp.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/LoggingManager.h>

#include <Dy/Element/Canvas/Widget.h>
#include <Dy/Element/Canvas/Text.h>

namespace dy
{

void FDyBuiltinDebugUiScript::Initiate()
{

}

void FDyBuiltinDebugUiScript::Start()
{

}

void FDyBuiltinDebugUiScript::Update(_MIN_ TF32 dt)
{
  const TF32 usage = MDyWindow::GetInstance().GetCpuUsage();
  auto& widgetRef = this->GetWidgetReference();

  FDyText* text = widgetRef.GetUiObject<FDyText>("DebugTestText");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(text), "Unexpected error occurred.");

  text->SetText(fmt::format("{:.2f} %", usage));
  MDY_LOG_CRITICAL("CPU USAGE : {}%", usage);
}

} /// ::dy namespace