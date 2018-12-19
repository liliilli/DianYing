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
#include <Dy/Management/TimeManager.h>

#include <Dy/Element/Canvas/Widget.h>
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Element/Canvas/FDyBasicGaugeBar.h>
#include <Dy/Management/InputManager.h>

namespace dy
{

void FDyBuiltinDebugUiScript::Initiate()
{
  FDyText* text = this->GetWidgetReference().GetUiObject<FDyText>("DebugTestText");
  auto position = text->GetWidgetPosition(EDyOrigin::Center_Center);
  position.Y += 96;
  text->SetWidgetCentralPosition(position);
  //text->SetFontSize(12);
}

void FDyBuiltinDebugUiScript::Start()
{

}

void FDyBuiltinDebugUiScript::Update(_MIN_ TF32 dt)
{
  auto& windowManager = MDyWindow::GetInstance(); 
  const TF32 usageCpu = windowManager.GetCpuUsage();
  const auto usageRam = windowManager.GetRamUsage();
  auto& widgetRef = this->GetWidgetReference();

  FDyText* text = widgetRef.GetUiObject<FDyText>("DebugTestText");
  FDyBasicGaugeBar* bar = widgetRef.GetUiObject<FDyBasicGaugeBar>("BasicBarTest");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(text), "Unexpected error occurred.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(bar),  "Unexpected error occurred.");

#ifdef false
  static auto SetTemporaryInitialSetting = [&] {
    desc.mInitialString = R"dy(
[Task]        A     B     C   DRAW  TOTAL
Process : 1 0.123 0.456 0.789 0.123 0.123
ChrPre  : 1 0.123 0.456 0.789 0.456 0.123
Char    : 0 0.123 0.456 0.789 0.789 0.123
Post    : 2
BG      : 1
Canvas0 : 1
Canvas1 : 0
Camera0 : 2
)dy";
  };
#endif

  auto& time = MDyTime::GetInstance();
  auto& inputManager = MDyInput::GetInstance();
  text->SetText(fmt::format(
      "A0:{:+03.2f}, A1:{:+03.2f}, A2:{:+03.2f}, A3:{:+03.2f}, A4:{:+03.2f}, A5:{:+03.2f}\n"
      "{:05.2f} %, {:0d} fps\n"
      "Ram Usage : {} Bytes", 
      inputManager.GetJoystickStickValue(0),
      inputManager.GetJoystickStickValue(1),
      inputManager.GetJoystickStickValue(2),
      inputManager.GetJoystickStickValue(3),
      inputManager.GetJoystickStickValue(4),
      inputManager.GetJoystickStickValue(5),
      usageCpu, time.GetPresentFpsCountValue(),
      usageRam
  ));
  bar->SetPresentValue(usageCpu);
}

} /// ::dy namespace