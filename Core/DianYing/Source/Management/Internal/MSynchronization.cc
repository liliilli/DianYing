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
#include <Dy/Core/Thread/SIOConnectionHelper.h>
#include <Dy/Core/GEngine.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/MScript.h>
#include <AEngineBase.h>

namespace dy
{

EDySuccess MSynchronization::pfInitialize()
{
  // Initialize threads.
  this->mIOThreadInstance = std::make_unique<TRescIO>();
  this->mIOThreadThread   = std::thread(&TRescIO::operator(), std::ref(*this->mIOThreadInstance));
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MSynchronization::pfRelease()
{
  SIOConnectionHelper::TryStop();
  this->mIOThreadThread.join();
  this->mIOThreadInstance = nullptr;
  return EDySuccess::DY_SUCCESS;
}

NotNull<TRescIO*> MSynchronization::pfGetIOThread()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mIOThreadInstance), "IOThread Instance must not be null except for initialization and destruction.");
  return DyMakeNotNull(this->mIOThreadInstance.get());
}

void MSynchronization::TrySynchronization()
{
  switch (TEMP_CAST(gEngine)->GetGlobalGameStatus())
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
  if (SIOConnectionHelper::IsMainTaskListEmpty() == false)
  {
    SIOConnectionHelper::ForceProcessDeferredMainTaskList();
  }

  if (SIOConnectionHelper::IsWorkerResultExist() == true)
  {
    SIOConnectionHelper::ForceProcessIOInsertPhase();
  }

  SIOConnectionHelper::TryCallSleptCallbackFunction();
}

void MSynchronization::pRunFrameFirstLoading()
{
  using TSyncHelper = SIOConnectionHelper;
  if (TSyncHelper::IsMainTaskListEmpty() == false)    { TSyncHelper::ForceProcessDeferredMainTaskList(); }
  if (TSyncHelper::IsWorkerResultExist() == true)  { TSyncHelper::ForceProcessIOInsertPhase(); }

  // Check whether IO thread working is done, if so change status to `Loading`. 
  SIOConnectionHelper::TryCallSleptCallbackFunction();
}

void MSynchronization::pRunFrameLoading()
{
  using TSyncHelper = SIOConnectionHelper;
  if (TSyncHelper::IsMainTaskListEmpty() == false)    { TSyncHelper::ForceProcessDeferredMainTaskList(); }
  if (TSyncHelper::IsWorkerResultExist() == true)  { TSyncHelper::ForceProcessIOInsertPhase(); }

  // Check whether IO thread working is done, if so change status to `Loading`. 
  SIOConnectionHelper::TryCallSleptCallbackFunction();
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
    // Try create actors in level.
    auto& world = MWorld::GetInstance(); 
    if (world.IsActorCreationExist() == true)
    {
      world.TryCreateActorsFromCreationList();
      world.CleanActorCreationList();
    }
    
    // Remove GC actor list.
    if (world.IsActorGCCandidateExist() == true)
    {
      world.MDY_PRIVATE(TryRemoveActorGCList)();
    }
  }

  // Check Resource GC to IO Thread,...
  if (SIOConnectionHelper::IsGcCandidateExist() == true)
  {
    SIOConnectionHelper::TryGC();
  }

  // Synchronization 
  if (SIOConnectionHelper::IsMainTaskListEmpty() == false)    
  { 
    SIOConnectionHelper::ForceProcessDeferredMainTaskList(); 
  }
  if (SIOConnectionHelper::IsWorkerResultExist() == true)
  { 
    SIOConnectionHelper::ForceProcessIOInsertPhase(); 
  }
}

void MSynchronization::PRunFrameShutdown()
{
  // Check whether IO thread working is done, if so change status to `Loading`. 
  SIOConnectionHelper::TryCallSleptCallbackFunction();
}

} /// ::dy namespace