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
//#include <Dy/Builtin/Widget/DebugUiMeta.h>

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
  this->mSynchronization  = &MDySynchronization::GetInstance();
  return DY_SUCCESS;
}

EDySuccess DyEngine::pfRelease()
{
  this->pfReleaseDependentManager();
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

    // Real-time update sequence.
    timeManager.pUpdate();
    if (timeManager.IsGameFrameTicked() == DY_FAILURE) { continue; }
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
      this->MDY_PRIVATE_SPECIFIER(Update)(this->mStatus, timeManager.GetGameScaledTickedDeltaTimeValue());
      this->MDY_PRIVATE_SPECIFIER(Render)(this->mStatus);
    } break;
    case EDyGlobalGameStatus::Shutdown: break;
    case EDyGlobalGameStatus::Ended: break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  };
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
      {
        scriptManager.CallDestroyGcWidgetScriptAndClear();
        scriptManager.GcWidgetScriptList();
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
    { MDY_NOT_IMPLEMENTED_ASSERT();
      { // If level change from one to another, do that.
        MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(RemoveLevel)();
        MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(PopulateNextLevelResources)();
        MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(BuildNextLevel)();
        MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(TransitionToNextLevel)();
        //MDyWorld::GetInstance().OpenLevel(MDySetting::GetInstance().GetInitialSceneInformationName());
      }
    } break;
    case EDyGlobalGameStatus::Shutdown: 
    { MDY_NOT_IMPLEMENTED_ASSERT();
    } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  } break;
  case EDyGlobalGameStatus::Shutdown: break;
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
    MDyScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();
    MDyScript::GetInstance().TryMoveInsertActorScriptToMainContainer();
    MDyScript::GetInstance().UpdateWidgetScript(dt);
    MDyScript::GetInstance().UpdateActorScript(dt);

    MDyPhysics::GetInstance().Update(dt);
    MDyInput::GetInstance().pfUpdate(dt);
    MDyWorld::GetInstance().Update(dt);
    MDyWorld::GetInstance().UpdateObjects(dt);
    MDyWorld::GetInstance().RequestDrawCall(dt);
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
  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Initialize());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Initialize());
  MDY_CALL_ASSERT_SUCCESS(MDySynchronization::Initialize());
}

void DyEngine::pfInitializeDependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyPhysics::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyFont::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScript::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyWorld::Initialize());
}

void DyEngine::pfReleaseDependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWorld::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyScript::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyFont::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyPhysics::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySynchronization::Release());
}

void DyEngine::pfReleaseIndependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Release());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
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