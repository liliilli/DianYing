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

namespace dy
{

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
    this->mIOThreadInstance = new TDyIO;
    this->mIOThread = std::thread(&TDyIO::operator(), std::ref(*this->mIOThreadInstance));
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
  MDY_LOG_WARNING_D("========== DIANYING MANAGER INITIALIZED ==========");

  gEngine = this;
  return DY_SUCCESS;
}

EDySuccess DyEngine::pfRelease()
{
  static auto ReleaseThread = [this]
  {
    this->mIOThread.join();
    delete this->mIOThreadInstance;
    this->mIOThreadInstance = nullptr;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ReleaseThread();

  MDY_LOG_WARNING_D("========== DIANYING MANAGER RELEASED ==========");
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
  dy::MDyWindow::GetInstance().Run();
}

MDyTime& DyEngine::GetTimeManager() { return MDyTime::GetInstance(); }
MDyWindow& DyEngine::GetWindowManager() { return MDyWindow::GetInstance(); }


} /// ::dy namespace