#include <precompiled.h>
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

/// Header file
#include "AstroidConstructor.h"
#include <Dy/Management/MInput.h>
#include <Dy/Element/FActor.h>
#include <Dy/Management/MWorld.h>

#include <Dy/Core/GEngine.h>
#include <Dy/Component/CTransform.h>
#include <Dy/Component/CSkybox.h>
#include <Math/Utility/XLinearMath.h>
#include <Math/Utility/XMath.h>

void AstroidConstructor::Initiate() 
{
  using namespace dy;
  MDY_CALL_BUT_NOUSE_RESULT(MDY_ACQUIRE_CONTROLLER());
  MDY_BIND_INPUT_ACTION("Escape", EDyInputActionStatus::Pressed, &AstroidConstructor::InGameEnd);
  //MDY_BIND_INPUT_ACTION("Enter", EDyInputActionStatus::Pressed, &AstroidConstructor::InNewScene);
  MDY_BIND_INPUT_ACTION("Enter", EDyInputActionStatus::Pressed, &AstroidConstructor::InUi);

  //this->GetGameTimerManager().SetTimer(this->mTimerHandle, *this, &AstroidConstructor::CbCreateActorRandomly, 6.0f, true, 0.0f);
}

void AstroidConstructor::Start()
{
  using namespace dy;
}

void AstroidConstructor::Update(TF32 dt)
{
  using namespace dy;

  auto& mInput = MInput::GetInstance();
  if (mInput.IsJoystickConnected() == true)
  {
    CTransform* refActor = this->GetActorReference().GetTransform();
    refActor->AddWorldEulerAngle(EAxis::Y, dt * 90.f * mInput.GetJoystickStickValue(0));

    const auto xAngleOld = refActor->GetRelativeWorldEulerAngle(EAxis::X);
    const DClamp<float, 0, 89> xAngleNew = 
        refActor->GetRelativeWorldEulerAngle(EAxis::X) + 60.f * dt * mInput.GetJoystickStickValue(5);
    refActor->AddWorldEulerAngle(EAxis::X, xAngleNew - xAngleOld);

    const auto yAngle = refActor->GetRelativeWorldEulerAngle(EAxis::Y) * math::kToRadian<TF32>;
    auto fwd = DVec3{std::sin(yAngle), 0, std::cos(yAngle)};

    static const auto upDir = DVec3{0, 1, 0};
    auto right = math::Cross(upDir, fwd);
    right *= -mInput.GetJoystickStickValue(0);
    fwd   *= -mInput.GetJoystickStickValue(1);

    refActor->AddRelativeWorldPosition((fwd + right) * 60.f * dt);
  }

  auto ptrSkybox = this->GetActorReference().GetGeneralComponent<CSkybox>();
  if (ptrSkybox.has_value() == true)
  {
    auto& skybox = (*ptrSkybox);
    skybox->SetRotationDegree(skybox->GetRotationDegree() + (1.f * dt));
  }
}

void AstroidConstructor::InGameEnd()
{
  DyPushLogCritical("InGameEnd Called!");
  dy::gEngine->TryEndGame();
}

void AstroidConstructor::InNewScene()
{
  DyPushLogCritical("InNewScene Called.");
  dy::MWorld::GetInstance().OpenLevel("TestScene");
}

void AstroidConstructor::InUi()
{
  DyPushLogCritical("InSetPause Called.");
  static bool flag = false;

  if (flag == false)
  {
    const auto _ = dy::MWorld::GetInstance().TryCreateDebugUi();
    flag = true;
  }
  else
  {
    const auto _ = dy::MWorld::GetInstance().TryRemoveDebugUi();
    flag = false;
  }
}
