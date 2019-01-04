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
#include <Dy/Management/Internal/MDyProfiling.h>
#include <Dy/Helper/Math/Math.h>

namespace dy
{

void FDyBuiltinDebugUiScript::Initiate()
{
#ifdef false
  const auto i = MDY_ACQUIRE_CONTROLLER_UI(); // Use this for acquire controller exclusive right for UI.
  MDY_ASSERT(i == DY_SUCCESS, "Unexpected error occurred.");

  MDY_BIND_INPUT_AXIS("XAxis", &FDyBuiltinDebugUiScript::Bar_MoveLeft);
  MDY_BIND_INPUT_AXIS("YAxis", &FDyBuiltinDebugUiScript::Bar_MoveUp);
  MDY_BIND_INPUT_ACTION("Enter", EDyInputActionStatus::Pressed, &FDyBuiltinDebugUiScript::EndApplication);
#endif

  this->mTimeManager      = &MDyTime::GetInstance();
  this->mProfilingManger  = &MDyProfiling::GetInstance();
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

  FDyText* infoText     = widgetRef.GetUiObject<FDyText>("DebugTestText");
  FDyText* joystickText = widgetRef.GetUiObject<FDyText>("JoystickTest");
  FDyBasicGaugeBar* bar = widgetRef.GetUiObject<FDyBasicGaugeBar>("BasicBarTest");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(infoText), "Unexpected error occurred.");
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

  auto t = this->mTimeManager->GetCalendarTime();
  infoText->SetText(fmt::format(
      "{:05.2f} %, {:0d} fps | Time : {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n"
      "| Obj : 000 | Tex : {:03} | Shd : {:03} | Vtx : {:03} |\n"
      "Ram Usage : {} Bytes", 
      usageCpu, this->mTimeManager->GetPresentFpsCountValue(),
      t.Year(), t.Month(), t.Day(), t.Hour(), t.Minute(), t.Second(),
      this->mProfilingManger->GetOnBindTextureCount(),
      this->mProfilingManger->GetOnBindShaderCount(),
      this->mProfilingManger->GetOnBindVertexCount(),
      usageRam
  ));
  //bar->SetRelativePosition(bar->GetRelativePosition(EDyOrigin::Center_Center) + DDyVector2{0, -dt * 16.0f});
  bar->SetPresentValue(usageCpu);

  const auto& inputManager = MDyInput::GetInstance();
  joystickText->SetText(fmt::format(
      "Analog 01 : X {:05.2f} Y {:05.2f}\n"
      "Analog 02 : X {:05.2f} Y {:05.2f}",
      inputManager.GetJoystickStickValue(1), inputManager.GetJoystickStickValue(0),
      inputManager.GetJoystickStickValue(5), inputManager.GetJoystickStickValue(2)
  ));
}

void FDyBuiltinDebugUiScript::Bar_MoveLeft(_MIN_ TF32 iXAxis) noexcept
{
  if (math::IsNearlyZero(iXAxis) == false) { MDY_LOG_CRITICAL("iXAxis : {:.4f}", iXAxis); }
}

void FDyBuiltinDebugUiScript::Bar_MoveUp(_MIN_ TF32 iYAxis) noexcept
{
  if (math::IsNearlyZero(iYAxis) == false) { MDY_LOG_CRITICAL("iYAxis : {:.4f}", iYAxis); }
}

void FDyBuiltinDebugUiScript::EndApplication() noexcept
{
  MDY_LOG_CRITICAL("Action!");
  MDY_NOTUSED const auto _ = MDY_DETACH_CONTROLLER_UI(); // Use this for give controller exclusive right to Actor or nothing.
}

} /// ::dy namespace