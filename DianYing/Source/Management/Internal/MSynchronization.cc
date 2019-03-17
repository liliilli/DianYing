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
#include <Dy/Management/Internal/MSynchronization.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/GDyEngine.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/MScript.h>

namespace dy
{

EDySuccess MSynchronization::pfInitialize()
{
  // Initialize threads.
  this->mIOThreadInstance = std::make_unique<TDyIO>();
  this->mIOThreadThread   = std::thread(&TDyIO::operator(), std::ref(*this->mIOThreadInstance));
  return DY_SUCCESS;
}

EDySuccess MSynchronization::pfRelease()
{
  SDyIOConnectionHelper::TryStop();
  this->mIOThreadThread.join();
  this->mIOThreadInstance = nullptr;
  return DY_SUCCESS;
}

NotNull<TDyIO*> MSynchronization::pfGetIOThread()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mIOThreadInstance), "IOThread Instance must not be null except for initialization and destruction.");
  return DyMakeNotNull(this->mIOThreadInstance.get());
}

void MSynchronization::TrySynchronization()
{
  switch (gEngine->GetGlobalGameStatus())
  {
  case EGlobalGameState::Booted:       { this->pRunFrameBooted(); }        break;
  case EGlobalGameState::FirstLoading: { this->pRunFrameFirstLoading(); }  break; // Syncrhonization
  case EGlobalGameState::Loading:      { this->pRunFrameLoading(); }       break;
  case EGlobalGameState::GameRuntime:  { this->pRunFrameGameRuntime(); }   break;
  case EGlobalGameState::Shutdown:     { this->PRunFrameShutdown(); }      break;
  default: /* Do nothing */ break;
  }
}

void MSynchronization::pRunFrameBooted()
{
  if (SDyIOConnectionHelper::IsMainTaskListIsEmpty() == false)
  {
    SDyIOConnectionHelper::ForceProcessDeferredMainTaskList();
  }

  if (SDyIOConnectionHelper::CheckIOResultInCondition() == true)
  {
    SDyIOConnectionHelper::ForceProcessIOInsertPhase();
  }

  SDyIOConnectionHelper::TryCallSleptCallbackFunction();
}

void MSynchronization::pRunFrameFirstLoading()
{
  using TSyncHelper = SDyIOConnectionHelper;
  if (TSyncHelper::IsMainTaskListIsEmpty() == false)    { TSyncHelper::ForceProcessDeferredMainTaskList(); }
  if (TSyncHelper::CheckIOResultInCondition() == true)  { TSyncHelper::ForceProcessIOInsertPhase(); }

  // Check whether IO thread working is done, if so change status to `Loading`. 
  SDyIOConnectionHelper::TryCallSleptCallbackFunction();
}

void MSynchronization::pRunFrameLoading()
{
  using TSyncHelper = SDyIOConnectionHelper;
  if (TSyncHelper::IsMainTaskListIsEmpty() == false)    { TSyncHelper::ForceProcessDeferredMainTaskList(); }
  if (TSyncHelper::CheckIOResultInCondition() == true)  { TSyncHelper::ForceProcessIOInsertPhase(); }

  // Check whether IO thread working is done, if so change status to `Loading`. 
  SDyIOConnectionHelper::TryCallSleptCallbackFunction();
}

void MSynchronization::pRunFrameGameRuntime()
{
  // Try call script, destroy function when level is alive.
  {
    auto& manager = MScript::GetInstance();
    if (manager.IsGcedActorScriptExist() == true)
    {
      manager.CallDestroyFuncActorScriptGCList();
      manager.ClearActorScriptGCList();
    }

    if (manager.IsGcedWidgetScriptExist() == true)
    {
      manager.CallDestroyFuncWidgetScriptGCList();
      manager.ClearWidgetScriptGCList();
    }
  }

  {
    // Level Try create actors.
    auto& world = MWorld::GetInstance(); 
    if (world.CheckCreationActorExist() == true)
    {
      world.TryCreateActorsOfCreationActorList();
      world.CleanCreationActorList();
    }
    
    // Remove GC actor list.
    if (world.CheckIsGcActorExist() == true)
    {
      world.MDY_PRIVATE(TryRemoveActorGCList)();
      // Check Resource GC to IO Thread,...
    }
  }

  // Synchronization 
  using TSyncHelper = SDyIOConnectionHelper;
  if (TSyncHelper::IsMainTaskListIsEmpty() == false)  { TSyncHelper::ForceProcessDeferredMainTaskList(); }
  if (TSyncHelper::CheckIOResultInCondition() == true){ TSyncHelper::ForceProcessIOInsertPhase(); }
}

void MSynchronization::PRunFrameShutdown()
{
  // Check whether IO thread working is done, if so change status to `Loading`. 
  SDyIOConnectionHelper::TryCallSleptCallbackFunction();
}

} /// ::dy namespace