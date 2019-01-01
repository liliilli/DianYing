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
#include <Dy/Builtin/Script/DebugActor/BtScActorTest2.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Core/DyEngine.h>

void dy::BtScActorTest2::Initiate()
{
  MDY_CALL_BUT_NOUSE_RESULT(MDY_ACQUIRE_CONTROLLER());
  MDY_BIND_INPUT_ACTION("Enter", EDyInputActionStatus::Pressed, &BtScActorTest2::Test);
}

void dy::BtScActorTest2::Update(TF32 dt)
{
  static TF32 f = 0.0f;
  const auto old = DDyVector3{std::cos(f) * 10, 0, std::sin(f) * 10};
  f += dt;
  auto _new = DDyVector3{std::cos(f) * 10, 0, std::sin(f) * 10};
  _new -= old;

  auto& refActor = this->GetActorReference();
  const auto& pos = refActor.GetTransform()->GetRelativeWorldPosition();
  refActor.GetTransform()->SetRelativeWorldPosition(DDyVector3{std::cos(f) * 10, pos.Y, std::sin(f) * 10});
  refActor.GetTransform()->AddWorldEulerAngle(EDyAxis3D::Z, dt * 90.f);

#ifdef false
  auto& mInput = MDyInput::GetInstance();
  if (mInput.IsJoystickConnected() == true)
  {
    auto& refActor = this->GetActorReference();
    const auto& p  = 
        refActor.GetTransform()->GetRelativeWorldPosition() 
      + DDyVector3{
          5 * mInput.GetJoystickStickValue(0),
          0,
          5 * mInput.GetJoystickStickValue(1)} * dt;
    refActor.GetTransform()->SetRelativeWorldPosition(p);
  }
#endif
}

void dy::BtScActorTest2::Test()
{
  MDY_LOG_CRITICAL("Game end Test!");
  MDY_CALL_ASSERT_SUCCESS(gEngine->TryEndGame());
}
