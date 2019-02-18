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
#include <Dy/Management/GameTimerManager.h>
#include <Dy/Element/Type/Timer/FDyTimerHandle.h>
#include <Dy/Core/DyEngine.h>

namespace dy
{

EDySuccess MDyGameTimer::pfInitialize()
{
  mActorTimerList.reserve(32);
  mWidgetTimerList.reserve(32);
  return DY_SUCCESS;
}

EDySuccess MDyGameTimer::pfRelease()
{
  return DY_SUCCESS;
}

void MDyGameTimer::PauseActorTimer(_MIN_ FDyTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }
  
  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActorTimerList), 
      [validId](const decltype(mActorTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT(it != this->mActorTimerList.end(), "Unexpected error occurred.");
 
  if (it->GetTimerStatus() == EDyTimerStatus::Play) 
  { 
    it->SetTimerStatus(EDyTimerStatus::Paused); 
  }
}

void MDyGameTimer::ResumeActorTimer(_MIN_ FDyTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActorTimerList), 
      [validId](const decltype(mActorTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT(it != this->mActorTimerList.end(), "Unexpected error occurred.");
   
  if (it->GetTimerStatus() == EDyTimerStatus::Paused) 
  { 
    it->SetTimerStatus(EDyTimerStatus::Play); 
  }
}

void MDyGameTimer::StopActorTimer(_MIN_ FDyTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActorTimerList), 
      [validId](const decltype(mActorTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT(it != this->mActorTimerList.end(), "Unexpected error occurred.");
  
  it->MDY_PRIVATE(Abort)();
}

void MDyGameTimer::PauseWidgetTimer(FDyTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mWidgetTimerList), 
      [validId](const decltype(mWidgetTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT(it != this->mWidgetTimerList.end(), "Unexpected error occurred.");
   
  if (it->GetTimerStatus() == EDyTimerStatus::Paused) 
  { 
    it->SetTimerStatus(EDyTimerStatus::Play); 
  }
}

void MDyGameTimer::ResumeWidgetTimer(FDyTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mWidgetTimerList), 
      [validId](const decltype(mWidgetTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT(it != this->mWidgetTimerList.end(), "Unexpected error occurred.");
   
  if (it->GetTimerStatus() == EDyTimerStatus::Paused) 
  { 
    it->SetTimerStatus(EDyTimerStatus::Play); 
  }
}

void MDyGameTimer::MDY_PRIVATE(pSetTimer)(
    _MIN_ FDyTimerHandle& iRefHandler, 
    _MIN_ ADyActorCppScript& iRefScript,
    _MIN_ std::function<void(void)> iFunction,
    _MIN_ TF32 iTickTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ TF32 iDelayTime)
{
  if (iRefHandler.IsBound() == false)
  { // Create Item, and bind `FDyTimerHandle` handler to `iRefScript`.
    this->mActorTimerList.emplace_back(std::ref(iRefHandler), std::ref(iRefScript), iDelayTime, iTickTime, iIsLooped, iFunction);
  }
  else
  { // If handler is bound and exist on list (paused or played)
    const auto oldId = iRefHandler.MDY_PRIVATE(GetIndex)();
    auto it = std::find_if(
        MDY_BIND_BEGIN_END(this->mActorTimerList), 
        [oldId](const decltype(mActorTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == oldId; });
    MDY_ASSERT(it != this->mActorTimerList.end(), "Unexpected error occurred.");

    it->MDY_PRIVATE(ResetTimerProperties)(iDelayTime, iTickTime, iIsLooped, iFunction);
  }
}

void MDyGameTimer::MDY_PRIVATE(pSetTimer)(
    _MIN_ FDyTimerHandle& iRefHandler, 
    _MIN_ ADyWidgetCppScript& iRefScript,
    _MIN_ std::function<void(void)> iFunction,
    _MIN_ TF32 iTickTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ TF32 iDelayTime)
{
  if (iRefHandler.IsBound() == false)
  { // Create Item, and bind `FDyTimerHandle` handler to `iRefScript`.
    this->mWidgetTimerList.emplace_back(std::ref(iRefHandler), std::ref(iRefScript), iDelayTime, iTickTime, iIsLooped, iFunction);
  }
  else
  { // If handler is bound and exist on list (paused or played)
    const auto oldId = iRefHandler.MDY_PRIVATE(GetIndex)();
    auto it = std::find_if(
        MDY_BIND_BEGIN_END(this->mWidgetTimerList), 
        [oldId](const decltype(mWidgetTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == oldId; });
    MDY_ASSERT(it != this->mWidgetTimerList.end(), "Unexpected error occurred.");

    it->MDY_PRIVATE(ResetTimerProperties)(iDelayTime, iTickTime, iIsLooped, iFunction);
  }
}

void MDyGameTimer::Update(_MIN_ TF32 iDt)
{
  for (auto& timerItem : this->mActorTimerList)
  {
    if (timerItem.GetTimerStatus() == EDyTimerStatus::Play)
    {
      timerItem.Update(iDt);
      if (timerItem.Checked() == true) { timerItem.CallFunction(false); }
      // Check game end signal is called.
      if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
    }
  }

  for (auto& timerItem : this->mWidgetTimerList)
  {
    if (timerItem.GetTimerStatus() == EDyTimerStatus::Play)
    {
      timerItem.Update(iDt);
      if (timerItem.Checked() == true) { timerItem.CallFunction(false); }
      // Check game end signal is called.
      if (gEngine->MDY_PRIVATE(IsGameEndCalled)() == true) { return; }
    }
  }
}

void MDyGameTimer::StopWidgetTimer(_MIN_ FDyTimerHandle& iRefHandler)
{
  // Bound is false only when aborted or not exist on list.
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mWidgetTimerList), 
      [validId](const decltype(mWidgetTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT(it != this->mWidgetTimerList.end(), "Unexpected error occurred.");
  
  it->MDY_PRIVATE(Abort)();
}

void MDyGameTimer::__TryGcRemoveAbortedTimerInstance()
{
  this->pTryGcRemoveAbortedActorTimerInstance();
  this->pTryGcRemoveAbortedWidgetTimerInstance();
}

void MDyGameTimer::pTryGcRemoveAbortedActorTimerInstance()
{
  if (this->mActorTimerList.empty() == true) { return; }
  DyEraseRemoveIf(this->mActorTimerList, [](const decltype(mActorTimerList)::value_type& iInstance)
  {
    return iInstance.GetTimerStatus() == EDyTimerStatus::Aborted;
  });
}

void MDyGameTimer::pTryGcRemoveAbortedWidgetTimerInstance()
{
  if (this->mWidgetTimerList.empty() == true) { return; }
  DyEraseRemoveIf(this->mWidgetTimerList, [](const decltype(mWidgetTimerList)::value_type& iInstance)
  {
    return iInstance.GetTimerStatus() == EDyTimerStatus::Aborted;
  });
}

} /// ::dy namespace