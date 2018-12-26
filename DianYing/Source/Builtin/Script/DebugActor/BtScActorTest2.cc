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
  f += dt;

  auto& refActor = this->GetActorReference();
  const auto& p  = refActor.GetTransform()->GetWorldPosition();
  refActor.GetTransform()->SetWorldPosition(DDyVector3{std::sin(f) * 10, p.Y, std::cos(f) * 10});

#ifdef false
  auto& mInput = MDyInput::GetInstance();
  if (mInput.IsJoystickConnected() == true)
  {
    auto& refActor = this->GetActorReference();
    const auto& p  = 
        refActor.GetTransform()->GetWorldPosition() 
      + DDyVector3{
          5 * mInput.GetJoystickStickValue(0),
          0,
          5 * mInput.GetJoystickStickValue(1)} * dt;
    refActor.GetTransform()->SetWorldPosition(p);
  }
#endif
}

void dy::BtScActorTest2::Test()
{
  MDY_LOG_CRITICAL("Game end Test!");
  MDY_CALL_ASSERT_SUCCESS(gEngine->TryEndGame());
}
