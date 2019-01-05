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
#include <Dy/Core/DyEngine.h>

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
#include <Dy/Management/Editor/GuiManager.h>
#include <Dy/Management/Internal/MDySynchronization.h>
#include <Dy/Management/Internal/MDyProfiling.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include "Dy/Management/GameTimerManager.h"

//!
//! Implementation
//!

namespace dy
{

DyEngine* gEngine = nullptr;

EDySuccess DyEngine::pfInitialize()
{
  gEngine = this;
  auto& settingManager = MDySetting::GetInstance();
  settingManager.pSetupExecutableArgumentSettings();

  this->pfInitializeIndependentManager();
  this->mSynchronization = &MDySynchronization::GetInstance();
  return DY_SUCCESS;
}

EDySuccess DyEngine::pfRelease()
{
  this->mSynchronization = nullptr;
  this->pfReleaseIndependentManager();
  return DY_SUCCESS;
}

void DyEngine::operator()()
{
  static auto& window         = MDyWindow::GetInstance();
  static auto& timeManager    = MDyTime::GetInstance();
  static auto& soundManager   = MDySound::GetInstance();
  //soundManager.Update(MDY_INITIALIZE_DEFINT);

  while (window.IsWindowShouldClose() == false)
  {
    // Try game status transition and pre-housesholds.
    this->TryUpdateStatus();
    if (this->mIsStatusTransitionDone == false)
    {
      this->MDY_PRIVATE_SPECIFIER(ReflectGameStatusTransition)();
      this->mIsStatusTransitionDone = true;
    }

    if (this->mIsGameEndCalled == false)
    { // Real-time update sequence when `GameEnd call sign` not checked.
      timeManager.pUpdate();
      if (timeManager.IsGameFrameTicked() == DY_FAILURE) { continue; }
    }
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
      this->mSynchronization->TrySynchronization();
      MDyGameTimer::GetInstance().MDY_PRIVATE_SPECIFIER(TryGcRemoveAbortedTimerInstance)();

      this->MDY_PRIVATE_SPECIFIER(Update)(this->mStatus, timeManager.GetGameScaledTickedDeltaTimeValue());
      if (this->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) 
      { // If game must be stopped, return but change status to Shutdown (GameRuntime => Shutdown);
        this->SetNextGameStatus(EDyGlobalGameStatus::Shutdown);
      }
      else { this->MDY_PRIVATE_SPECIFIER(Render)(this->mStatus); }
    } break;
    case EDyGlobalGameStatus::Shutdown: 
    { // Just wait I/O Worker thread is slept.
      this->mSynchronization->TrySynchronization(); 
    } break;
    case EDyGlobalGameStatus::Ended: break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  };

  MDY_LOG_INFO("Game End. bye!");
}

void DyEngine::MDY_PRIVATE_SPECIFIER(ReflectGameStatusTransition)()
{
  switch (this->mPrevStatus)
  {
  case EDyGlobalGameStatus::None: 
  {
    switch (this->mStatus)
    {
    case EDyGlobalGameStatus::Booted: // None => Booted.
      MDyMetaInfo::GetInstance().MDY_PRIVATE_SPECIFIER(PopulateBootResourceSpecifierList)();
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
      gEngine->pfInitializeDependentManager();
      MDY_CALL_ASSERT_SUCCESS   (MDyWorld::GetInstance().TryCreateDebugUi());
      MDY_CALL_BUT_NOUSE_RESULT (MDyWorld::GetInstance().TryCreateLoadingUi());
#ifdef false
      auto& refSettingManager = MDySetting::GetInstance();
      if (refSettingManager.IsDebugUiVisible() == true)
      { // If debug ui need to be visible, create debug ui.
        MDY_CALL_ASSERT_SUCCESS(MDyWorld::GetInstance().TryCreateDebugUi());
      }
#endif
      MDyMetaInfo::GetInstance().MDY_PRIVATE_SPECIFIER(PopulateGlobalResourceSpecifierList)();
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  } break;
  case EDyGlobalGameStatus::FirstLoading: 
  { 
    switch (this->mStatus)
    {
    case EDyGlobalGameStatus::Loading: 
    { // FirstLoading => Loading.
      MDY_CALL_ASSERT_SUCCESS(MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(OpenFirstLevel)());
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  } break;
  case EDyGlobalGameStatus::Loading: 
  {
    switch (this->mStatus)
    {
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
    default: MDY_UNEXPECTED_BRANCH();
    }
  } break;
  case EDyGlobalGameStatus::GameRuntime: 
  {
    switch (this->mStatus)
    {
    case EDyGlobalGameStatus::Loading: 
    { // GameRuntime => Loading.
      MDY_NOT_IMPLEMENTED_ASSERT();
      // If level change from one to another, do that.
      MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(RemoveLevel)();
      MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(PopulateNextLevelResources)();
      MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(BuildNextLevel)();
      MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(TransitionToNextLevel)();
      //MDyWorld::GetInstance().OpenLevel(MDySetting::GetInstance().GetInitialSceneInformationName());
    } break;
    case EDyGlobalGameStatus::Shutdown: 
    { // GameRuntime => Shutdown. Just wait IO Thread is slept.
      if (MDyWorld::GetInstance().IsDebugUiExist() == true)
      { // If debug ui exist, remove.
        MDY_CALL_BUT_NOUSE_RESULT(MDyWorld::GetInstance().TryRemoveDebugUi());
      }
      SDyIOConnectionHelper::PopulateResourceList(
          std::vector<DDyResourceName>{}, EDyScope::Global, 
          []() { DyEngine::GetInstance().SetNextGameStatus(EDyGlobalGameStatus::Ended); }
      );
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  } break;
  case EDyGlobalGameStatus::Shutdown:
  {
    switch (this->mStatus)
    {
    case EDyGlobalGameStatus::Ended: 
    { // Shutdown => Ended.
      MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(RemoveLevel)();
      this->pfReleaseDependentManager();
      MDY_CALL_ASSERT_SUCCESS(MDyWindow::GetInstance().MDY_PRIVATE_SPECIFIER(TerminateWindow)());
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  } break;
  case EDyGlobalGameStatus::Ended: break;
  default: ;
  }
}

void DyEngine::MDY_PRIVATE_SPECIFIER(Update)(_MIN_ EDyGlobalGameStatus iEngineStatus, _MIN_ TF32 dt)
{
  #if defined(MDY_FLAG_IN_EDITOR)
    editor::MDyEditorGui::GetInstance().Update(dt);
  #endif // MDY_FLAG_IN_EDITOR

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
    MDyScript::GetInstance().UpdateActorScript(0.0f, EDyScriptState::CalledNothing);
    MDyScript::GetInstance().TryMoveInsertActorScriptToMainContainer();
    MDyScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

    MDyInput::GetInstance().pfUpdate(dt);
    if (this->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
    MDyGameTimer::GetInstance().Update(dt);
    if (this->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }
    MDyScript::GetInstance().UpdateActorScript(dt);
    if (this->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }

    MDyScript::GetInstance().UpdateWidgetScript(dt);
    if (this->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) { return; }

    MDyPhysics::GetInstance().Update(dt);
    MDyWorld::GetInstance().Update(dt);
    MDyWorld::GetInstance().UpdateObjects(dt);
  } break;
  case EDyGlobalGameStatus::Shutdown: 
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void DyEngine::MDY_PRIVATE_SPECIFIER(Render)(_MIN_ EDyGlobalGameStatus iEngineStatus)
{
  switch (iEngineStatus)
  {
  case EDyGlobalGameStatus::FirstLoading: 
  case EDyGlobalGameStatus::Loading: 
    MDyRendering::GetInstance().MDY_PRIVATE_SPECIFIER(RenderLoading)();
    break;
  case EDyGlobalGameStatus::GameRuntime: 
    MDyWorld::GetInstance().RequestDrawCall();
    MDyRendering::GetInstance().RenderDrawCallQueue();
    break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  #if defined(MDY_FLAG_IN_EDITOR)
    editor::MDyEditorGui::GetInstance().DrawWindow(0);
  #endif // MDY_FLAG_IN_EDITOR

  this->GetWindowManager().TempSwapBuffers();
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

EDySuccess DyEngine::TryEndGame() noexcept
{
  if (this->MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() == true) 
  { 
    MDY_LOG_CRITICAL("Quering game ending is already done.");
    return DY_FAILURE; 
  }

  this->mIsGameEndCalled = true;
  return DY_SUCCESS;
}

bool DyEngine::MDY_PRIVATE_SPECIFIER(IsGameEndCalled)() const noexcept
{
  return this->mIsGameEndCalled;
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