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
#include <Dy/Component/CDyPhysicsCollider.h>
#include <Dy/Element/Actor.h>

namespace dy
{

bool CDyPhysicsCollider::IsNotifyHitEvent() const noexcept
{
  return this->mNotifyHitEvent;
}

bool CDyPhysicsCollider::IsNotifyOverlapEvent() const noexcept
{
  return this->mNotifyOverlapEvent;
}

bool CDyPhysicsCollider::IsRegistered() const noexcept
{
  return this->mIsRegistered;
}

EDyColliderType CDyPhysicsCollider::GetColliderType() const noexcept
{
  return this->mColliderType;
}

void CDyPhysicsCollider::MDY_PRIVATE_SPECIFIER(SetRegisterFlag)(_MIN_ bool iFlag) noexcept
{
  this->mIsRegistered = iFlag;
}

void CDyPhysicsCollider::TryActivateInstance()
{
  auto& bindedActor   = *this->GetBindedActor();
  auto* ptrRigidbody  =  bindedActor.GetRigidbody();
  if (MDY_CHECK_ISNULL(ptrRigidbody) || ptrRigidbody->IsComponentActivated() == false)
  {
    // Look forward to catch signals from rigidbody... (Activated & Created)
    return;
  }

  ptrRigidbody->RegisterCollider(*this);
}

void CDyPhysicsCollider::TryDeactivateInstance()
{
  auto& bindedActor   = *this->GetBindedActor();
  auto* ptrRigidbody  =  bindedActor.GetRigidbody();
  if (MDY_CHECK_ISNULL(ptrRigidbody) || ptrRigidbody->IsComponentActivated() == false)
  {
    // Look forward to catch signals from rigidbody... (Activated & Created)
    return;
  }

  ptrRigidbody->UnregisterCollider(*this);
}

} /// ::dy namespace