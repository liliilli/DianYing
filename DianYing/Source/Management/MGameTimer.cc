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
#include <Dy/Management/MGameTimer.h>
#include <Dy/Element/Type/Timer/FTimerHandle.h>
#include <Dy/Core/GDyEngine.h>

namespace dy
{

EDySuccess MGameTimer::pfInitialize()
{
  mActorTimerList.reserve(32);
  mWidgetTimerList.reserve(32);
  return DY_SUCCESS;
}

EDySuccess MGameTimer::pfRelease()
{
  return DY_SUCCESS;
}

void MGameTimer::PauseActorTimer(FTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }
  
  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActorTimerList), 
      [validId](const decltype(mActorTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT_MSG(it != this->mActorTimerList.end(), "Unexpected error occurred.");
 
  if (it->GetTimerStatus() == ETimerState::Play) 
  { 
    it->SetTimerStatus(ETimerState::Paused); 
  }
}

void MGameTimer::ResumeActorTimer(FTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActorTimerList), 
      [validId](const decltype(mActorTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT_MSG(it != this->mActorTimerList.end(), "Unexpected error occurred.");
   
  if (it->GetTimerStatus() == ETimerState::Paused) 
  { 
    it->SetTimerStatus(ETimerState::Play); 
  }
}

void MGameTimer::StopActorTimer(FTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActorTimerList), 
      [validId](const decltype(mActorTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT_MSG(it != this->mActorTimerList.end(), "Unexpected error occurred.");
  
  it->MDY_PRIVATE(Abort)();
}

void MGameTimer::PauseWidgetTimer(FTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mWidgetTimerList), 
      [validId](const decltype(mWidgetTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT_MSG(it != this->mWidgetTimerList.end(), "Unexpected error occurred.");
   
  if (it->GetTimerStatus() == ETimerState::Paused) 
  { 
    it->SetTimerStatus(ETimerState::Play); 
  }
}

void MGameTimer::ResumeWidgetTimer(FTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mWidgetTimerList), 
      [validId](const decltype(mWidgetTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT_MSG(it != this->mWidgetTimerList.end(), "Unexpected error occurred.");
   
  if (it->GetTimerStatus() == ETimerState::Paused) 
  { 
    it->SetTimerStatus(ETimerState::Play); 
  }
}

void MGameTimer::MDY_PRIVATE(pSetTimer)(
    FTimerHandle& iRefHandler, 
    AActorCppScript& iRefScript,
    std::function<void(void)> iFunction,
    TF32 iTickTime, 
    bool iIsLooped, 
    TF32 iDelayTime)
{
  if (iRefHandler.IsBound() == false)
  { // Create Item, and bind `FTimerHandle` handler to `iRefScript`.
    this->mActorTimerList.emplace_back(std::ref(iRefHandler), std::ref(iRefScript), iDelayTime, iTickTime, iIsLooped, iFunction);
  }
  else
  { // If handler is bound and exist on list (paused or played)
    const auto oldId = iRefHandler.MDY_PRIVATE(GetIndex)();
    auto it = std::find_if(
        MDY_BIND_BEGIN_END(this->mActorTimerList), 
        [oldId](const decltype(mActorTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == oldId; });
    MDY_ASSERT_MSG(it != this->mActorTimerList.end(), "Unexpected error occurred.");

    it->MDY_PRIVATE(ResetTimerProperties)(iDelayTime, iTickTime, iIsLooped, iFunction);
  }
}

void MGameTimer::MDY_PRIVATE(pSetTimer)(
    FTimerHandle& iRefHandler, 
    AWidgetCppScript& iRefScript,
    std::function<void(void)> iFunction,
    TF32 iTickTime, 
    bool iIsLooped, 
    TF32 iDelayTime)
{
  if (iRefHandler.IsBound() == false)
  { // Create Item, and bind `FTimerHandle` handler to `iRefScript`.
    this->mWidgetTimerList.emplace_back(std::ref(iRefHandler), std::ref(iRefScript), iDelayTime, iTickTime, iIsLooped, iFunction);
  }
  else
  { // If handler is bound and exist on list (paused or played)
    const auto oldId = iRefHandler.MDY_PRIVATE(GetIndex)();
    auto it = std::find_if(
        MDY_BIND_BEGIN_END(this->mWidgetTimerList), 
        [oldId](const decltype(mWidgetTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == oldId; });
    MDY_ASSERT_MSG(it != this->mWidgetTimerList.end(), "Unexpected error occurred.");

    it->MDY_PRIVATE(ResetTimerProperties)(iDelayTime, iTickTime, iIsLooped, iFunction);
  }
}

void MGameTimer::Update(TF32 iDt)
{
  for (auto& timerItem : this->mActorTimerList)
  {
    if (timerItem.GetTimerStatus() == ETimerState::Play)
    {
      timerItem.Update(iDt);
      if (timerItem.Checked() == true) { timerItem.CallFunction(false); }
      // Check game end signal is called.
      if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
    }
  }

  for (auto& timerItem : this->mWidgetTimerList)
  {
    if (timerItem.GetTimerStatus() == ETimerState::Play)
    {
      timerItem.Update(iDt);
      if (timerItem.Checked() == true) { timerItem.CallFunction(false); }
      // Check game end signal is called.
      if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
    }
  }
}

void MGameTimer::StopWidgetTimer(FTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mWidgetTimerList), 
      [validId](const decltype(mWidgetTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT_MSG(it != this->mWidgetTimerList.end(), "Unexpected error occurred.");
  
  it->MDY_PRIVATE(Abort)();
}

void MGameTimer::__TryGcRemoveAbortedTimerInstance()
{
  this->pTryGcRemoveAbortedActorTimerInstance();
  this->pTryGcRemoveAbortedWidgetTimerInstance();
}

void MGameTimer::pTryGcRemoveAbortedActorTimerInstance()
{
  if (this->mActorTimerList.empty() == true) { return; }
  EraseRemoveIf(this->mActorTimerList, [](const decltype(mActorTimerList)::value_type& iInstance)
  {
    return iInstance.GetTimerStatus() == ETimerState::Aborted;
  });
}

void MGameTimer::pTryGcRemoveAbortedWidgetTimerInstance()
{
  if (this->mWidgetTimerList.empty() == true) { return; }
  EraseRemoveIf(this->mWidgetTimerList, [](const decltype(mWidgetTimerList)::value_type& iInstance)
  {
    return iInstance.GetTimerStatus() == ETimerState::Aborted;
  });
}

} /// ::dy namespace