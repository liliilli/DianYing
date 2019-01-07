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
#include <Dy/Component/Abstract/ADyActorCppScript.h>
#include <Dy/Component/Internal/Actor/CDyActorScriptCpp.h>
#include <Dy/Management/GameTimerManager.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FDyActor& ADyActorCppScript::GetActorReference()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mOutside), "Unexpected error occurred.");
  return this->mOutside->GetActorReference();
}

MDyGameTimer& ADyActorCppScript::GetGameTimerManager() noexcept
{
  return MDyGameTimer::GetInstance();
}

void ADyActorCppScript::MDY_PRIVATE_SPECIFIER(BindPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler)
{
  this->mPtrTimerHandleList.emplace_back(&iRefTimerHandler);
}

void ADyActorCppScript::MDY_PRIVATE_SPECIFIER(DetachPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler)
{
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mPtrTimerHandleList), 
      [ptr = &iRefTimerHandler](_MIN_ const decltype(mPtrTimerHandleList)::value_type& iPtrValidHandler)
  {
    return iPtrValidHandler == ptr;
  });
  if (it == this->mPtrTimerHandleList.end()) { return; }

  DyFastErase(this->mPtrTimerHandleList, it);
}

void ADyActorCppScript::MDY_PRIVATE_SPECIFIER(AbortAllValidTimerHandler)()
{
  auto& timerManager = this->GetGameTimerManager();
  for (auto& ptrTimerHandler : this->mPtrTimerHandleList)
  {
    timerManager.StopTimer(*ptrTimerHandler);
  }
  this->mPtrTimerHandleList.clear();
}

} /// ::dy namespace