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

#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/IOResourceManager.h>
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
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>

namespace dy
{

DyEngine* gEngine = nullptr;

EDySuccess DyEngine::pfInitialize()
{
  ///
  /// @brief Forward runtime arguments to setting manager.
  ///
  static auto InsertExecuteRuntimeArguments = []()
  {
    auto& settingManager = MDySetting::GetInstance();
    settingManager.pSetupExecutableArgumentSettings();
  };

  ///
  /// @brief Initialize threads.
  ///
  static auto InitializeThread = [this]
  {
    this->mIOThreadInstance = std::make_unique<TDyIO>();
    this->mIOThreadThread   = std::thread(&TDyIO::operator(), std::ref(*this->mIOThreadInstance));
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  InsertExecuteRuntimeArguments();
  // `MDyLog` must be initialized first because of logging message from each managers.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Initialize());
#endif

  // IO Manager (THREAD + TASK QUEUE)
  InitializeThread();

  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyPhysics::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyFont::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScript::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyWorld::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySynchronization::Initialize());
  MDY_LOG_WARNING_D("========== DIANYING MANAGER INITIALIZED ==========");

  gEngine = this;
  return DY_SUCCESS;
}

EDySuccess DyEngine::pfRelease()
{
  static auto ReleaseThread = [this]
  {
    SDyIOConnectionHelper::TryStop();

    this->mIOThreadThread.join();
    this->mIOThreadInstance = nullptr;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ReleaseThread();

  MDY_LOG_WARNING_D("========== DIANYING MANAGER RELEASED ==========");
  MDY_CALL_ASSERT_SUCCESS(dy::MDySynchronization::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWorld::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyScript::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyFont::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyPhysics::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Release());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Release());

  return DY_SUCCESS;
}

void DyEngine::operator()()
{
  static auto& window         = MDyWindow::GetInstance();
  static auto& timeManager    = MDyTime::GetInstance();
  static auto& settingManager = MDySetting::GetInstance();
  static auto& sceneManager   = MDyWorld::GetInstance();
  static auto& soundManager   = MDySound::GetInstance();

  sceneManager.OpenLevel(settingManager.GetInitialSceneInformationName());
  sceneManager.Update(-1);

  while (window.IsWindowShouldClose() == false)
  {
    timeManager.pUpdate();
    soundManager.Update(MDY_INITIALIZE_DEFINT);

    if (timeManager.IsGameFrameTicked() == DY_SUCCESS)
    {
      const auto dt = timeManager.GetGameScaledTickedDeltaTimeValue();

      this->pUpdate(dt);
      this->pRender();
    }
  };
}

NotNull<TDyIO*> DyEngine::pfGetIOThread()
{
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mIOThreadInstance), "IOThread Instance must not be null except for initialization and destruction.");
  return DyMakeNotNull(this->mIOThreadInstance.get());
}

void DyEngine::pUpdate(_MIN_ TF32 dt)
{
  #if defined(MDY_FLAG_IN_EDITOR)
    editor::MDyEditorGui::GetInstance().Update(dt);
  #endif // MDY_FLAG_IN_EDITOR

  //
  MDyPhysics::GetInstance().Update(dt);
  //
  MDyWorld::GetInstance().Update(dt);
  //
  MDyInput::GetInstance().pfUpdate(dt);
  MDyWorld::GetInstance().UpdateObjects(dt);
  //
  MDyWorld::GetInstance().RequestDrawCall(dt);
}

void DyEngine::pRender()
{
  glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef false
  gRenderer.CallDraw();
#endif

  glEnable(GL_DEPTH_TEST);
  MDyRendering::GetInstance().RenderDrawCallQueue();
  glDisable(GL_DEPTH_TEST);

  #if defined(MDY_FLAG_IN_EDITOR)
    editor::MDyEditorGui::GetInstance().DrawWindow(0);
  #endif // MDY_FLAG_IN_EDITOR

  this->GetWindowManager().TempSwapBuffers();
}

MDyTime& DyEngine::GetTimeManager()     { return MDyTime::GetInstance(); }
MDyWindow& DyEngine::GetWindowManager() { return MDyWindow::GetInstance(); }

} /// ::dy namespace