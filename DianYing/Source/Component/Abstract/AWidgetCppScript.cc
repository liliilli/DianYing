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
#include <Dy/Component/Abstract/AWidgetCppScript.h>
#include <Dy/Component/Internal/Widget/CWidgetScriptCpp.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/MGameTimer.h>

namespace dy
{

FDyUiWidget& AWidgetCppScript::GetWidgetReference()
{
  MDY_ASSERT_MSG(this->mOutside != nullptr, "Unexpected error occurred.");
  return this->mOutside->GetWidgetReference();
}

MGameTimer& AWidgetCppScript::GetGameTimerManager() noexcept
{
  return MGameTimer::GetInstance();
}

void AWidgetCppScript::MDY_PRIVATE(BindPtrTimerHandle)(FDyTimerHandle& iRefTimerHandler)
{
  this->mPtrTimerHandleList.emplace_back(&iRefTimerHandler);
}

void AWidgetCppScript::MDY_PRIVATE(DetachPtrTimerHandle)(FDyTimerHandle& iRefTimerHandler)
{
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mPtrTimerHandleList), 
      [ptr = &iRefTimerHandler](const decltype(mPtrTimerHandleList)::value_type& iPtrValidHandler)
  {
    return iPtrValidHandler == ptr;
  });
  if (it == this->mPtrTimerHandleList.end()) { return; }

  FaseErase(this->mPtrTimerHandleList, it);
}

void AWidgetCppScript::MDY_PRIVATE(AbortAllValidTimerHandler)()
{
  if (this->mPtrTimerHandleList.empty() == true) { return; }

  auto& timerManager = this->GetGameTimerManager();
  while (this->mPtrTimerHandleList.empty() == false)
  { // timer handle will be removed from list automatically.
    timerManager.StopWidgetTimer(*this->mPtrTimerHandleList.front());
  }
}

void AWidgetCppScript::pfSetOutsideReference(CWidgetScriptCpp& outsideReference) noexcept
{
  this->mOutside = &outsideReference;
}

} /// ::dy namespace