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
#include <Dy/Component/Abstract/AActorCppScript.h>
#include <Dy/Component/Internal/Actor/CActorScriptCpp.h>
#include <Dy/Management/MGameTimer.h>
#include <Dy/Component/CPhysicsRigidbody.h>
#include <Dy/Helper/Library/HelperContainer.h>

namespace dy
{

FActor& AActorCppScript::GetActorReference()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mOutside), "Unexpected error occurred.");
  return this->mOutside->GetActorReference();
}

MGameTimer& AActorCppScript::GetGameTimerManager() noexcept
{
  return MGameTimer::GetInstance();
}

void AActorCppScript::MDY_PRIVATE(BindPtrTimerHandle)(_MIN_ FTimerHandle& iRefTimerHandler)
{
  this->mPtrTimerHandleList.emplace_back(&iRefTimerHandler);
}

void AActorCppScript::MDY_PRIVATE(DetachPtrTimerHandle)(_MIN_ FTimerHandle& iRefTimerHandler)
{
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mPtrTimerHandleList), 
      [ptr = &iRefTimerHandler](_MIN_ const decltype(mPtrTimerHandleList)::value_type& iPtrValidHandler)
  {
    return iPtrValidHandler == ptr;
  });
  if (it == this->mPtrTimerHandleList.end()) { return; }

  FaseErase(this->mPtrTimerHandleList, it);
}

void AActorCppScript::MDY_PRIVATE(AbortAllValidTimerHandler)()
{
  auto& timerManager = this->GetGameTimerManager();
  while (this->mPtrTimerHandleList.empty() == false)
  { // timer handle will be removed from list automatically.
    timerManager.StopActorTimer(*this->mPtrTimerHandleList.front());
  }
}

void AActorCppScript::MDY_PRIVATE(BindCollisionCbHandle)(
    _MIN_ CPhysicsRigidbody& iRefRigidbody, 
    _MIN_ ECollisionCallbackType iType,
    _MIN_ const void* iUniqueId)
{
  this->mCollisionCbHandleList.emplace_back(&iRefRigidbody, iType, iUniqueId);
}

EDySuccess AActorCppScript::MDY_PRIVATE(DetachCollisionCbHandle)(_MIN_ const void* iSpecificationId)
{
  auto lambda = [iSpecificationId](const auto& handle) { auto& [_1, _2, id] = handle; return id == iSpecificationId; };
  
  // Contains.
  if (ContainsIf(this->mCollisionCbHandleList, lambda) == false) { return DY_FAILURE; }

  // Find and remove.
  const auto it = std::find_if(MDY_BIND_BEGIN_END(this->mCollisionCbHandleList), lambda);
  MDY_ASSERT_MSG(it != this->mCollisionCbHandleList.end(), "Unexpected error occurred.");

  FaseErase(this->mCollisionCbHandleList, it);
  return DY_SUCCESS;
}

void AActorCppScript::MDY_PRIVATE(AbortAllCollisionCallback)()
{
  while (this->mCollisionCbHandleList.empty() == false)
  {
    // Get last element of list because it is fast.
    auto& [rigidbody, type, id] = this->mCollisionCbHandleList.back();
    // 
    const auto flag = rigidbody->RemoveCollisionCallback(type, id);
    MDY_ASSERT_MSG_FORCE(flag == DY_SUCCESS, "");

    this->mCollisionCbHandleList.erase(this->mCollisionCbHandleList.cbegin() + (this->mCollisionCbHandleList.size() - 1));
  }
}

void AActorCppScript::pfSetOutsideReference(_MIN_ CActorScriptCpp& outsideReference) noexcept
{
  this->mOutside = &outsideReference;
}

} /// ::dy namespace