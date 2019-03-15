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
#include <Dy/Core/DyEngine.h>

#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Management/MInput.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/MPhysics.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/MSound.h>
#include <Dy/Management/MTime.h>
#include <Dy/Management/MWindow.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/MFont.h>
#include <Dy/Management/MScript.h>
#include <Dy/Management/Internal/MSynchronization.h>
#include <Dy/Management/Internal/MProfiling.h>
#include <Dy/Management/MGameTimer.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>
#include <Dy/Management/Internal/MDebug.h>

//!
//! Implementation
//!

namespace dy
{

DyEngine* gEngine = nullptr;

EDySuccess DyEngine::pfInitialize()
{
  gEngine = this;
  MSetting::GetInstance().pSetupExecutableArgumentSettings();

  switch (MSetting::GetInstance().GetApplicationMode())
  {
  case EDyAppMode::ModeRuntime: 
  {
    this->pfInitializeIndependentManager();
    this->mSynchronization = &MSynchronization::GetInstance();
  } break;
  }

  return DY_SUCCESS;
}

EDySuccess DyEngine::pfRelease()
{
  switch (MSetting::GetInstance().GetApplicationMode())
  {
  case EDyAppMode::ModeRuntime: 
  {
    this->mSynchronization = nullptr;
    this->pfReleaseIndependentManager();
  } break;
  }
  return DY_SUCCESS;
}

void DyEngine::operator()()
{
  static auto& timeManager = MTime::GetInstance();
  while (MWindow::GetInstance().IsWindowShouldClose() == false)
  {
    // Try game status transition and pre-housesholds.
    this->TryUpdateStatus();
    if (this->mIsStatusTransitionDone == false)
    {
      this->MDY_PRIVATE(ReflectGameStatusTransition)();
      this->mIsStatusTransitionDone = true;
    }

    // Real-time update sequence when `GameEnd call sign` not checked.
    if (this->mIsGameEndCalled == false)
    { 
      timeManager.pUpdate();
      if (timeManager.IsGameFrameTicked() == DY_FAILURE) { continue; }
    }

    // one frame sync, update, render.
    switch (this->GetGlobalGameStatus())
    {
    case EDyGlobalGameStatus::Booted: 
    { 
      this->mSynchronization->TrySynchronization(); 
    } break;
    case EDyGlobalGameStatus::FirstLoading: 
    case EDyGlobalGameStatus::Loading: 
    case EDyGlobalGameStatus::GameRuntime: 
    {
      if (this->mIsInGameUpdatePaused == false)
      {
        this->mSynchronization->TrySynchronization();
        MGameTimer::GetInstance().MDY_PRIVATE(TryGcRemoveAbortedTimerInstance)();
      }

      // Get delta-time.
      const auto dt = timeManager.GetGameScaledTickedDeltaTimeValue();
      // Update
      this->MDY_PRIVATE(Update)(this->mStatus, dt);

      // `IsGameNeedToBeTransitted` can only be triggered when in `Update` function.
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true)
      { // If something need to be changed.
        // If game must be stopped, return but change status to Shutdown (GameRuntime => Shutdown);
        if (this->MDY_PRIVATE(IsGameEndCalled)() == true) 
        { this->SetNextGameStatus(EDyGlobalGameStatus::Shutdown); }
        // If game level should be changed to new one.
        if (this->MDY_PRIVATE(IsGameNeedTransitLevel)() == true) 
        { this->SetNextGameStatus(EDyGlobalGameStatus::Loading); }
        break;
      }

      // This function is internal update function for Dy Engine before rendering.
      this->MDY_PRIVATE(PreRender)(this->mStatus, dt);
      this->MDY_PRIVATE(Render)(this->mStatus); 
      this->MDY_PRIVATE(PostRender)(this->mStatus, dt);
    } break;
    case EDyGlobalGameStatus::Shutdown: 
    { // Just wait I/O Worker thread is slept.
      this->mSynchronization->TrySynchronization(); 
    } break;
    case EDyGlobalGameStatus::Ended: break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  };

  DyPushLogInfo("Game End. bye!");
}

void DyEngine::MDY_PRIVATE(ReflectGameStatusTransition)()
{
  switch (this->mPrevStatus)
  {
  case EDyGlobalGameStatus::None: 
  {
    switch (this->mStatus)
    {
    case EDyGlobalGameStatus::Booted: // None => Booted.
      MIOMeta::GetInstance().MDY_PRIVATE(PopulateBootResourceSpecifierList)();
      break;
    default: MDY_UNEXPECTED_BRANCH();
    } 
  } break;
  case EDyGlobalGameStatus::Booted: 
  { 
    switch (this->mStatus)
    {
    case EDyGlobalGameStatus::FirstLoading: 
    { // Booted => FirstLoading.
      this->pfInitializeDependentManager();
      if (MSetting::GetInstance().IsDebugMode() == true)
      {
        MDY_CALL_ASSERT_SUCCESS(MDebug::Initialize());
        MDY_CALL_ASSERT_SUCCESS(MWorld::GetInstance().TryCreateDebugUi());
      }

      MDY_CALL_BUT_NOUSE_RESULT (MWorld::GetInstance().TryCreateLoadingUi());
      MIOMeta::GetInstance().MDY_PRIVATE(PopulateGlobalResourceSpecifierList)();
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  } break;
  case EDyGlobalGameStatus::FirstLoading: 
  { 
    switch (this->mStatus)
    {
    default: MDY_UNEXPECTED_BRANCH();
    case EDyGlobalGameStatus::Loading: 
    { // FirstLoading => Loading.
      MRendering::GetInstance().ActivateEntryRenderPipeline("dyBtDefaultLoading", true);

      MDY_CALL_ASSERT_SUCCESS(MWorld::GetInstance().MDY_PRIVATE(OpenFirstLevel)());
    } break;
    }
  } break;
  case EDyGlobalGameStatus::Loading: 
  {
    switch (this->mStatus)
    {
    default: MDY_UNEXPECTED_BRANCH();
    case EDyGlobalGameStatus::GameRuntime: 
    { // Loading => GameRuntime.
      // Remove Loading UI, and detach resource with RAII & Script.
      MDY_CALL_BUT_NOUSE_RESULT(MWorld::GetInstance().TryRemoveLoadingUi());
      auto& scriptManager = MScript::GetInstance(); 
      MRendering::GetInstance().ActivateEntryRenderPipeline("dyBtDefaultLoading", false);
      MRendering::GetInstance().ActivateEntryRenderPipeline("dyBtDefault", true);

      if (scriptManager.IsGcedWidgetScriptExist() == true)
      { // Remove ui script manually.
        scriptManager.CallDestroyFuncWidgetScriptGCList();
        scriptManager.ClearWidgetScriptGCList();
        scriptManager.RemoveEmptyOnWidgetScriptList();
      }
    } break;
    }
  } break;
  case EDyGlobalGameStatus::GameRuntime: 
  {
    switch (this->mStatus)
    {
    default: MDY_UNEXPECTED_BRANCH();
    case EDyGlobalGameStatus::Loading: 
    { // GameRuntime => Loading.
      // If level change from one to another, do that.
      MWorld::GetInstance().MDY_PRIVATE(RemoveLevel)();
      MDY_CALL_BUT_NOUSE_RESULT(MWorld::GetInstance().TryCreateLoadingUi());

      MRendering::GetInstance().ActivateEntryRenderPipeline("dyBtDefault", false);
      MRendering::GetInstance().ActivateEntryRenderPipeline("dyBtDefaultLoading", true);

      SDyIOConnectionHelper::PopulateResourceList(
          std::vector<DDyResourceName>{}, EDyScope::Global, 
          []() { 
            auto& worldManager = MWorld::GetInstance();
            worldManager.MDY_PRIVATE(PopulateNextLevelResources)();
          }
      );
    } break;
    case EDyGlobalGameStatus::Shutdown: 
    { // GameRuntime => Shutdown. Just wait IO Thread is slept.
      MRendering::GetInstance().ActivateEntryRenderPipeline("dyBtDefault", false);

      SDyIOConnectionHelper::PopulateResourceList(
          std::vector<DDyResourceName>{}, EDyScope::Global, 
          []() { DyEngine::GetInstance().SetNextGameStatus(EDyGlobalGameStatus::Ended); }
      );
    } break;
    }
  } break;
  case EDyGlobalGameStatus::Shutdown:
  {
    switch (this->mStatus)
    {
    case EDyGlobalGameStatus::Ended: 
    { // Shutdown => Ended.
      MWorld::GetInstance().MDY_PRIVATE(RemoveLevel)();

      auto& scriptManager = MScript::GetInstance();
      scriptManager.CallonEndGlobalScriptList();
      scriptManager.RemoveGlobalScriptInstances();

      // If debug mode, try to release debug manager and relevent ui.
      if (MSetting::GetInstance().IsDebugMode() == true)
      {
        // If debug ui exist, remove.
        if (MWorld::GetInstance().IsDebugUiExist() == true)
        { MDY_CALL_BUT_NOUSE_RESULT(MWorld::GetInstance().TryRemoveDebugUi()); }
        // 
        MDY_CALL_ASSERT_SUCCESS(MDebug::Release());
      }

      this->pfReleaseDependentManager();
      MDY_CALL_ASSERT_SUCCESS(MWindow::GetInstance().MDY_PRIVATE(TerminateWindow)());
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  } break;
  case EDyGlobalGameStatus::Ended: break;
  default: ;
  }
}

void DyEngine::MDY_PRIVATE(Update)(_MIN_ EDyGlobalGameStatus iEngineStatus, _MIN_ TF32 dt)
{
  switch (iEngineStatus)
  {
  case EDyGlobalGameStatus::FirstLoading: 
  case EDyGlobalGameStatus::Loading: 
  {
    MScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();
    MScript::GetInstance().UpdateWidgetScript(dt);
  } break;
  case EDyGlobalGameStatus::GameRuntime: 
  {
    // Check mode is debug mode, if true, poll input of debug first.
    // and, if return value is DY_FAILURE, try to global update.
    if (MSetting::GetInstance().IsDebugMode() == true)
    {
      if (MDebug::GetInstance().CheckInput(dt) == DY_FAILURE)
      {
        MInput::GetInstance().pfGlobalUpdate(dt);
      }
      if (this->mIsInGameUpdatePaused == false)
      {
        // Physics pre-update time.
        // Must get transform from PhysX and rebind transform to render transform.
        MPhysics::GetInstance().UpdateRenderObjectTransform(dt);
        // And call callback collision functions once.
        MPhysics::GetInstance().CallCallbackIssueOnce();
      }
    }
    else
    { // If not debug mode, just poll input key of global.
      MInput::GetInstance().pfGlobalUpdate(dt);
      // Physics pre-update time.
      // Must get transform from PhysX and rebind transform to render transform.
      MPhysics::GetInstance().UpdateRenderObjectTransform(dt);
      // And call callback collision functions once.
      MPhysics::GetInstance().CallCallbackIssueOnce();
    }

    // If in-game update should not be passed, just update in-game. Otherwise, neglect.
    if (this->mIsInGameUpdatePaused == false)
    {
      MScript::GetInstance().UpdateActorScript(0.0f, EScriptState::CalledNothing);
      MScript::GetInstance().TryMoveInsertActorScriptToMainContainer();
      MScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

      MInput::GetInstance().pfInGameUpdate(dt);
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true) { return; }
      MGameTimer::GetInstance().Update(dt);
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true) { return; }
      MScript::GetInstance().UpdateActorScript(dt);
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true) { return; }
      MScript::GetInstance().UpdateWidgetScript(dt);
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true) { return; }
      
      MWorld::GetInstance().Update(dt);
      MWorld::GetInstance().UpdateObjects(dt);

      auto& soundManager = MSound::GetInstance();
      if (soundManager.IsSoundSystemAvailable() == true) { soundManager.Update(dt); }
    }
    else
    {
    }
  } break;
  case EDyGlobalGameStatus::Shutdown: break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void DyEngine::MDY_PRIVATE(PreRender)(_MIN_ EDyGlobalGameStatus iEngineStatus, _MIN_ TF32 dt)
{
  switch (iEngineStatus)
  {
  case EDyGlobalGameStatus::FirstLoading: 
  case EDyGlobalGameStatus::Loading:
  { MRendering::GetInstance().PreRender(dt);
  } break;
  case EDyGlobalGameStatus::GameRuntime: 
  {
    // Reset frame dependent profiling count.
    SDyProfilingHelper::ResetFrameDependentCounts();
    // Update physics `PxScene` parameter. This function must be called before MPhysics::Update(dt).
    MPhysics::GetInstance().UpdateInternalPxSceneParameter();

    if (this->mIsInGameUpdatePaused == false)
    {
      MWorld::GetInstance().UpdateAnimator(dt);
      // Update physics collision simulation.
      MPhysics::GetInstance().Update(dt);
    }

    // Debug render queue requisition.
    if (MSetting::GetInstance().IsRenderPhysicsCollisionShape() == true)
    {
      MPhysics::GetInstance().TryEnqueueDebugDrawCall();
    }

    // Pre-render update of rendering manager.
    MRendering::GetInstance().SetupDrawModelTaskQueue();
    MRendering::GetInstance().PreRender(dt);
  } break;
  default: break;
  }
}

void DyEngine::MDY_PRIVATE(Render)(_MIN_ EDyGlobalGameStatus iEngineStatus)
{
  switch (iEngineStatus)
  {
  case EDyGlobalGameStatus::FirstLoading: 
  case EDyGlobalGameStatus::Loading: 
  {
    auto& render = MRendering::GetInstance();
    render.RenderPipelines();
  } break;
  case EDyGlobalGameStatus::GameRuntime: 
  {
    // Request render call.
    auto& render = MRendering::GetInstance();
    render.RenderPipelines();

    // If debug mode is enabled, update and render ui item.
    // imgui has unified update & render architecture, so can not separate update and render routine.
    if (MSetting::GetInstance().IsDebugMode() == true)
    {
      MDebug::GetInstance().UpdateAndRender();
    }
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MRendering::GetInstance().SwapBuffers();
}

void DyEngine::MDY_PRIVATE(PostRender)(_MIN_ EDyGlobalGameStatus iEngineStatus, _MIN_ TF32 dt)
{
  switch (iEngineStatus)
  {
  case EDyGlobalGameStatus::GameRuntime: 
  {
    // Object picking routine.
    {
      static bool isPicked = false;
      auto& refInput = MInput::GetInstance();
      if (const bool value = refInput.IsKeyPressed(EDyInputButton::Mouse0Lmb);
          isPicked == false
      &&  refInput.GetMouseMode() == EDyMouseMode::Picking 
      &&  value == true)
      {
        refInput.TryPickObject(refInput.GetPresentMousePosition());
        isPicked = true;
      }
      else if (value == false) 
      { 
        isPicked = false; 
      }
    }

    // Do other things...

  } break;
  default: break;
  }
}

void DyEngine::pfInitializeIndependentManager()
{
  // `MLog` must be initialized first because of logging message from each managers.
  MDY_CALL_ASSERT_SUCCESS(dy::MLog::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MProfiling::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MIOMeta::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MSetting::Initialize());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Initialize());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MWindow::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MIORescInfo::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MIOResource::Initialize());

  MDY_CALL_ASSERT_SUCCESS(MSynchronization::Initialize());
}

void DyEngine::pfInitializeDependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MRendering::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MInput::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MSound::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MPhysics::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MFont::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MGameTimer::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MScript::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MWorld::Initialize());
}

void DyEngine::pfReleaseDependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MWorld::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MScript::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MGameTimer::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MFont::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MPhysics::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MSound::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MInput::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MRendering::Release());

  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Information);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Information);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Information);

  SDyIOConnectionHelper::TryGC(EDyScope::Global, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Global, EDyResourceStyle::Information);
  SDyIOConnectionHelper::TryGC(EDyScope::Global, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Global, EDyResourceStyle::Information);
  SDyIOConnectionHelper::TryGC(EDyScope::Global, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Global, EDyResourceStyle::Information);

  MDY_CALL_ASSERT_SUCCESS(dy::MSynchronization::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MIOResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MIORescInfo::Release());
}

void DyEngine::pfReleaseIndependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MWindow::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MTime::Release());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Release());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MSetting::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MIOMeta::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MProfiling::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MLog::Release());
}

NotNull<TDyIO*> DyEngine::pfGetIOThread()
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mSynchronization), "Synchronization manager must be valid.");
  return this->mSynchronization->pfGetIOThread();
}

MTime& DyEngine::GetTimeManager()     { return MTime::GetInstance(); }
MWindow& DyEngine::GetWindowManager() { return MWindow::GetInstance(); }

EDyGlobalGameStatus DyEngine::GetGlobalGameStatus() const noexcept
{
  return this->mStatus;
}

void DyEngine::SetNextGameStatus(_MIN_ EDyGlobalGameStatus iNextStatus) noexcept
{
  this->mNextStatus = iNextStatus;
}

void DyEngine::SetInGameUpdatePause(_MIN_ bool iActivated) noexcept
{
  this->mIsInGameUpdatePaused = iActivated;
}

bool DyEngine::IsInGameUpdatePaused() const noexcept
{
  return this->mIsInGameUpdatePaused;
}

EDySuccess DyEngine::TryEndGame() noexcept
{
  if (this->MDY_PRIVATE(IsGameEndCalled)() == true) 
  { 
    DyPushLogCritical("Quering game ending is already done.");
    return DY_FAILURE; 
  }

  this->mIsGameEndCalled = true;
  return DY_SUCCESS;
}

bool DyEngine::MDY_PRIVATE(IsGameEndCalled)() const noexcept
{
  return this->mIsGameEndCalled;
}

bool DyEngine::MDY_PRIVATE(IsGameNeedTransitLevel)() const noexcept
{
  return MWorld::GetInstance().IsNeedTransitNextLevel();
}

bool DyEngine::MDY_PRIVATE(IsGameNeedToBeTransitted)() const noexcept
{
  return this->MDY_PRIVATE(IsGameNeedTransitLevel)()
      || this->MDY_PRIVATE(IsGameEndCalled)();
}

void DyEngine::TryUpdateStatus()
{
  if (this->mNextStatus != this->mStatus)
  {
    this->mPrevStatus = this->mStatus;
    this->mStatus = this->mNextStatus;
    this->mIsStatusTransitionDone = false;
  }
}

} /// ::dy namespace