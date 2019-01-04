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
#include <Dy/Builtin/Script/DebugActor/BtScActorTest3.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/WorldManager.h>

namespace dy
{

void BtScActorTest3::Initiate() 
{
  MDyWorld::GetInstance().CreateActor("CreatedActor", "TestPrefab", {});
}

void BtScActorTest3::Update(_MIN_ TF32 dt)
{
  auto& mInput = MDyInput::GetInstance();
  if (mInput.IsJoystickConnected() == true)
  {
    CDyTransform* refActor = this->GetActorReference().GetTransform();
    refActor->AddWorldEulerAngle(EDyAxis3D::Y, dt * 90.f * mInput.GetJoystickStickValue(2));

    const auto xAngleOld = refActor->GetRelativeWorldEulerAngle(EDyAxis3D::X);
    const DDyClamp<float, 0, 89> xAngleNew = refActor->GetRelativeWorldEulerAngle(EDyAxis3D::X) + 60.f * dt * mInput.GetJoystickStickValue(5);
    refActor->AddWorldEulerAngle(EDyAxis3D::X, xAngleNew - xAngleOld);
  }

  const auto list = MDyWorld::GetInstance().GetAllActorsWithTagRecursive("");
  for (const auto& ptrObj : list)
  {
    MDY_LOG_INFO("Object : {}", ptrObj->GetActorName());
  }
}

} /// ::dy namespace
