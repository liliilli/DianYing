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
#include <Dy/Builtin/Script/BtScActorTest2.h>
#include "Dy/Element/Actor.h"

void dy::BtScActorTest2::Update(TF32 dt)
{
  static TF32 f = 0.0f;

  f += dt;

  auto& refActor = this->GetActorReference();
  const auto& p  = refActor.GetTransform()->GetLocalPosition();
  refActor.GetTransform()->SetLocalPosition(DDyVector3{std::sin(f * 0.02f) * 10, p.Y, p.Z});
}
