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
#include <Dy/Component/Abstract/ADyWidgetCppScript.h>
#include <Dy/Component/Internal/Widget/CDyWidgetScriptCpp.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/GameTimerManager.h>

namespace dy
{

FDyUiWidget& ADyWidgetCppScript::GetWidgetReference()
{
  MDY_ASSERT(this->mOutside != nullptr, "Unexpected error occurred.");
  return this->mOutside->GetWidgetReference();
}

MDyGameTimer& ADyWidgetCppScript::GetGameTimerManager() noexcept
{
  return MDyGameTimer::GetInstance();
}

void ADyWidgetCppScript::MDY_PRIVATE(BindPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler)
{
  this->mPtrTimerHandleList.emplace_back(&iRefTimerHandler);
}

void ADyWidgetCppScript::MDY_PRIVATE(DetachPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler)
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

void ADyWidgetCppScript::MDY_PRIVATE(AbortAllValidTimerHandler)()
{
  if (this->mPtrTimerHandleList.empty() == true) { return; }

  auto& timerManager = this->GetGameTimerManager();
  while (this->mPtrTimerHandleList.empty() == false)
  { // timer handle will be removed from list automatically.
    timerManager.StopWidgetTimer(*this->mPtrTimerHandleList.front());
  }
}

void ADyWidgetCppScript::pfSetOutsideReference(_MIN_ CDyWidgetScriptCpp& outsideReference) noexcept
{
  this->mOutside = &outsideReference;
}

} /// ::dy namespace