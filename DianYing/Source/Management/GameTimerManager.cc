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
#include <Dy/Management/GameTimerManager.h>
#include <Dy/Element/Type/Timer/FDyTimerHandle.h>
#include <Dy/Core/DyEngine.h>

namespace dy
{

EDySuccess MDyGameTimer::pfInitialize()
{
  mTimerList.reserve(32);
  return DY_SUCCESS;
}

EDySuccess MDyGameTimer::pfRelease()
{
  return DY_SUCCESS;
}

void MDyGameTimer::StopTimer(_MIN_ FDyTimerHandle& iRefHandler)
{
  if (iRefHandler.IsBound() == false) { return; }

  const auto validId = iRefHandler.MDY_PRIVATE_SPECIFIER(GetIndex)();
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mTimerList), 
      [validId](const decltype(mTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == validId; });
  MDY_ASSERT(it != this->mTimerList.end(), "Unexpected error occurred.");
  
  it->MDY_PRIVATE_SPECIFIER(Abort)();
}

void MDyGameTimer::MDY_PRIVATE_SPECIFIER(pSetTimer)(
    _MIN_ FDyTimerHandle& iRefHandler, 
    _MIN_ ADyActorCppScript& iRefScript,
    _MIN_ std::function<void(void)> iFunction,
    _MIN_ TF32 iTickTime, 
    _MIN_ bool iIsLooped, 
    _MIN_ TF32 iDelayTime)
{
  if (iRefHandler.IsBound() == false)
  { // Create Item, and bind `FDyTimerHandle` handler to `iRefScript`.
    this->mTimerList.emplace_back(std::ref(iRefHandler), std::ref(iRefScript), iDelayTime, iTickTime, iIsLooped, iFunction);
  }
  else
  {
    const auto oldId = iRefHandler.MDY_PRIVATE_SPECIFIER(GetIndex)();
    auto it = std::find_if(
        MDY_BIND_BEGIN_END(this->mTimerList), 
        [oldId](const decltype(mTimerList)::value_type& iTimerItem) { return iTimerItem.GetIndex() == oldId; });
    MDY_ASSERT(it != this->mTimerList.end(), "Unexpected error occurred.");

    it->MDY_PRIVATE_SPECIFIER(ResetTimerProperties)(iDelayTime, iTickTime, iIsLooped, iFunction);
  }
}

void MDyGameTimer::Update(_MIN_ TF32 iDt)
{
  for (auto& timerItem : this->mTimerList)
  {
    if (timerItem.GetTimerStatus() == FDyTimerItem::EStatus::Play)
    {
      timerItem.Update(iDt);
      if (timerItem.Checked() == true) { timerItem.CallFunction(false); }
      // Check game end signal is called.
      if (gEngine->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
    }
  }
}

} /// ::dy namespace