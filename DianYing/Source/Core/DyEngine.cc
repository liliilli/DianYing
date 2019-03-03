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
#include <Dy/Management/InputManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/PhysicsManager.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/FontManager.h>
#include <Dy/Management/ScriptManager.h>
#include <Dy/Management/Internal/MDySynchronization.h>
#include <Dy/Management/Internal/MDyProfiling.h>
#include <Dy/Management/GameTimerManager.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>
#include <Dy/Management/Internal/MDyDebug.h>

//!
//! Implementation
//!

namespace dy
{

DyEngine* gEngine = nullptr;

EDySuccess DyEngine::pfInitialize()
{
  gEngine = this;
  MDySetting::GetInstance().pSetupExecutableArgumentSettings();

  switch (MDySetting::GetInstance().GetApplicationMode())
  {
  case EDyAppMode::ModeRuntime: 
  {
    this->pfInitializeIndependentManager();
    this->mSynchronization = &MDySynchronization::GetInstance();
  } break;
  }

  return DY_SUCCESS;
}

EDySuccess DyEngine::pfRelease()
{
  switch (MDySetting::GetInstance().GetApplicationMode())
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
  static auto& timeManager = MDyTime::GetInstance();
  while (MDyWindow::GetInstance().IsWindowShouldClose() == false)
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
        MDyGameTimer::GetInstance().MDY_PRIVATE(TryGcRemoveAbortedTimerInstance)();
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
      MDyMetaInfo::GetInstance().MDY_PRIVATE(PopulateBootResourceSpecifierList)();
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
      if (MDySetting::GetInstance().IsDebugMode() == true)
      {
        MDY_CALL_ASSERT_SUCCESS(MDyDebug::Initialize());
        MDY_CALL_ASSERT_SUCCESS(MDyWorld::GetInstance().TryCreateDebugUi());
      }

      MDY_CALL_BUT_NOUSE_RESULT (MDyWorld::GetInstance().TryCreateLoadingUi());
      MDyMetaInfo::GetInstance().MDY_PRIVATE(PopulateGlobalResourceSpecifierList)();
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
      MDY_CALL_ASSERT_SUCCESS(MDyWorld::GetInstance().MDY_PRIVATE(OpenFirstLevel)());
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
      MDY_CALL_BUT_NOUSE_RESULT(MDyWorld::GetInstance().TryRemoveLoadingUi());
      auto& scriptManager = MDyScript::GetInstance(); 
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
      MDyWorld::GetInstance().MDY_PRIVATE(RemoveLevel)();
      MDY_CALL_BUT_NOUSE_RESULT(MDyWorld::GetInstance().TryCreateLoadingUi());

      SDyIOConnectionHelper::PopulateResourceList(
          std::vector<DDyResourceName>{}, EDyScope::Global, 
          []() { 
            auto& worldManager = MDyWorld::GetInstance();
            worldManager.MDY_PRIVATE(PopulateNextLevelResources)();
          }
      );
    } break;
    case EDyGlobalGameStatus::Shutdown: 
    { // GameRuntime => Shutdown. Just wait IO Thread is slept.
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
      MDyWorld::GetInstance().MDY_PRIVATE(RemoveLevel)();

      auto& scriptManager = MDyScript::GetInstance();
      scriptManager.CallonEndGlobalScriptList();
      scriptManager.RemoveGlobalScriptInstances();

      // If debug mode, try to release debug manager and relevent ui.
      if (MDySetting::GetInstance().IsDebugMode() == true)
      {
        // If debug ui exist, remove.
        if (MDyWorld::GetInstance().IsDebugUiExist() == true)
        { MDY_CALL_BUT_NOUSE_RESULT(MDyWorld::GetInstance().TryRemoveDebugUi()); }
        // 
        MDY_CALL_ASSERT_SUCCESS(MDyDebug::Release());
      }

      this->pfReleaseDependentManager();
      MDY_CALL_ASSERT_SUCCESS(MDyWindow::GetInstance().MDY_PRIVATE(TerminateWindow)());
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
    MDyScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();
    MDyScript::GetInstance().UpdateWidgetScript(dt);
  } break;
  case EDyGlobalGameStatus::GameRuntime: 
  {
    // Check mode is debug mode, if true, poll input of debug first.
    // and, if return value is DY_FAILURE, try to global update.
    if (MDySetting::GetInstance().IsDebugMode() == true)
    {
      if (MDyDebug::GetInstance().CheckInput(dt) == DY_FAILURE)
      {
        MDyInput::GetInstance().pfGlobalUpdate(dt);
      }
      if (this->mIsInGameUpdatePaused == false)
      {
        // Physics pre-update time.
        // Must get transform from PhysX and rebind transform to render transform.
        MDyPhysics::GetInstance().UpdateRenderObjectTransform(dt);
        // And call callback collision functions once.
        MDyPhysics::GetInstance().CallCallbackIssueOnce();
      }
    }
    else
    { // If not debug mode, just poll input key of global.
      MDyInput::GetInstance().pfGlobalUpdate(dt);
      // Physics pre-update time.
      // Must get transform from PhysX and rebind transform to render transform.
      MDyPhysics::GetInstance().UpdateRenderObjectTransform(dt);
      // And call callback collision functions once.
      MDyPhysics::GetInstance().CallCallbackIssueOnce();
    }

    // If in-game update should not be passed, just update in-game. Otherwise, neglect.
    if (this->mIsInGameUpdatePaused == false)
    {
      MDyScript::GetInstance().UpdateActorScript(0.0f, EDyScriptState::CalledNothing);
      MDyScript::GetInstance().TryMoveInsertActorScriptToMainContainer();
      MDyScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

      MDyInput::GetInstance().pfInGameUpdate(dt);
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true) { return; }
      MDyGameTimer::GetInstance().Update(dt);
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true) { return; }
      MDyScript::GetInstance().UpdateActorScript(dt);
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true) { return; }
      MDyScript::GetInstance().UpdateWidgetScript(dt);
      if (this->MDY_PRIVATE(IsGameNeedToBeTransitted)() == true) { return; }
      
      MDyWorld::GetInstance().Update(dt);
      MDyWorld::GetInstance().UpdateObjects(dt);

      auto& soundManager = MDySound::GetInstance();
      if (soundManager.IsSoundSystemAvailable() == true) { soundManager.Update(dt); }
    }
    else
    {
    }
  } break;
  case EDyGlobalGameStatus::Shutdown: 
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void DyEngine::MDY_PRIVATE(PreRender)(_MIN_ EDyGlobalGameStatus iEngineStatus, _MIN_ TF32 dt)
{
  switch (iEngineStatus)
  {
  case EDyGlobalGameStatus::FirstLoading: 
  case EDyGlobalGameStatus::Loading:
  { MDyRendering::GetInstance().PreRender(dt);
  } break;
  case EDyGlobalGameStatus::GameRuntime: 
  {
    // Reset frame dependent profiling count.
    SDyProfilingHelper::ResetFrameDependentCounts();
    // Update physics `PxScene` parameter. This function must be called before MDyPhysics::Update(dt).
    MDyPhysics::GetInstance().UpdateInternalPxSceneParameter();

    if (this->mIsInGameUpdatePaused == false)
    {
      MDyWorld::GetInstance().UpdateAnimator(dt);
      // Update physics collision simulation.
      MDyPhysics::GetInstance().Update(dt);
    }

    // Debug render queue requisition.
    if (MDySetting::GetInstance().IsRenderPhysicsCollisionShape() == true)
    {
      MDyPhysics::GetInstance().TryEnqueueDebugDrawCall();
    }

    // Pre-render update of rendering manager.
    MDyRendering::GetInstance().SetupDrawModelTaskQueue();
    MDyRendering::GetInstance().PreRender(dt);
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
    MDyRendering::GetInstance().MDY_PRIVATE(RenderLoading)();
  } break;
  case EDyGlobalGameStatus::GameRuntime: 
  {
    // Request render call.
    auto& render = MDyRendering::GetInstance();
    render.RenderLevelInformation();
    render.RenderUIInformation();
    render.RenderDebugInformation();
    render.Integrate();

    // If debug mode is enabled, update and render ui item.
    // imgui has unified update & render architecture, so can not separate update and render routine.
    if (MDySetting::GetInstance().IsDebugMode() == true)
    {
      MDyDebug::GetInstance().UpdateAndRender();
    }
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  this->GetWindowManager().TempSwapBuffers();
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
      auto& refInput = MDyInput::GetInstance();
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
  // `MDyLog` must be initialized first because of logging message from each managers.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyProfiling::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyMetaInfo::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Initialize());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOData::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOResource::Initialize());

  MDY_CALL_ASSERT_SUCCESS(MDySynchronization::Initialize());
}

void DyEngine::pfInitializeDependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyPhysics::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyFont::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyGameTimer::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScript::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyWorld::Initialize());
}

void DyEngine::pfReleaseDependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWorld::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyScript::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyGameTimer::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyFont::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyPhysics::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Release());

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

  MDY_CALL_ASSERT_SUCCESS(dy::MDySynchronization::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyIOData::Release());
}

void DyEngine::pfReleaseIndependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Release());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyMetaInfo::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyProfiling::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Release());
}

NotNull<TDyIO*> DyEngine::pfGetIOThread()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mSynchronization), "Synchronization manager must be valid.");
  return this->mSynchronization->pfGetIOThread();
}

MDyTime& DyEngine::GetTimeManager()     { return MDyTime::GetInstance(); }
MDyWindow& DyEngine::GetWindowManager() { return MDyWindow::GetInstance(); }

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
  return MDyWorld::GetInstance().IsNeedTransitNextLevel();
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