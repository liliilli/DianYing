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

#ifdef false
#include <Dy/VkInterface.h>
#endif

#include <d3dx11effect.h>
#include <sol2/sol.hpp>

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Management/SceneManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Management/RenderingManager.h>

#include <Dy/Management/Editor/GuiManager.h>

namespace
{

#ifdef MDY_FLAG_IN_EDITOR
///
/// @brief Initialize all gui editor managers related to editing easily.
///
void DyInitializeEditorManagers()
{

}

void DyReleaseEditorManagers()
{

}
#endif

///
/// @brief Initialize all managers related to DianYing rendering application.
/// Initialzation order must be ended with MDyWindow and started with MDySetting by getting argv
/// as option.
///
void DyInitiailzeAllManagers()
{
  { // Setting manager
    auto& settingManager = dy::MDySetting::GetInstance();
    #if defined(MDY_PLATFORM_FLAG_WINDOWS) && defined(_WIN32)
      const int32_t size = __argc;
      for (int32_t i = 0; i < size; ++i)
      {
        settingManager.pArgsPushback(__argv[i]);
      }
    #elif defined(MDY_PLATFORM_FLAG_LINUX) && defined(__linux__)
      static_assert(false, "Linux does not support now.");
    #elif defined(MDY_PLATFORM_FLAG_MACOS)
      static_assert(false, "Macos does not support now.");
    #endif
    MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());
  }

  {
    auto& logManager = dy::MDySetting::GetInstance();
    logManager.SetSubFeatureLoggingToConsole(true);
    logManager.SetSubFeatureLoggingToFile(true);
  }
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Initialize());
#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Initialize());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyHeapResource::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Initialize());

  // MDyWindow must be initialized at last.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Initialize());

  MDY_LOG_WARNING_D("========== DIANYING MANAGER INITIALIZED ==========");
}

///
/// @brief Release all managers related to DianYing rendering application.
/// Order must be revered with DyInitializeAllManagers().
///
void DyReleaseAllManagers()
{
  MDY_LOG_WARNING_D("========== DIANYING MANAGER RELEASED ==========");

  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Release());

  // Release other management instance.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyHeapResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());
#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Release());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
}

} /// unnamed namespace

///
/// @brief Main entry function of WIN32 platforms.
///
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{

FILE*     gFp             = nullptr;
HINSTANCE ghInstance      = nullptr;
HINSTANCE ghPrevInstance  = nullptr;
LPSTR     gpCmdLine;
int       gnCmdShow;

///
/// @brief Turn on memory leak detection feature and console window for logging.
///
EDySuccess __DyInitializeWin32Debug()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

  if (!AllocConsole()) {
    MessageBox(nullptr, L"The console window was not created.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }

  freopen_s(&gFp, "CONOUT$", "w", stdout);
  return DY_SUCCESS;
}

///
/// @brief Turn off memory leak detection feature and console window for logging.
///
EDySuccess __DyReleaseWin32Debug()
{
  fclose(gFp);

  if (!FreeConsole()) {
    MessageBox(nullptr, L"Failed to free console resource.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }
  return DY_SUCCESS;
}

} /// unname namespace

#if defined(_DEBUG) || !defined(NDEBUG)
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   MDY_CALL_ASSERT_SUCCESS(__DyInitializeWin32Debug())
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  MDY_CALL_ASSERT_SUCCESS(__DyReleaseWin32Debug())
#else
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   (void)0
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  (void)0
#endif

///
/// @brief Main function of win32 / win64 platform.
///
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
  ghInstance      = hInstance;
  ghPrevInstance  = hPrevInstance;
  gpCmdLine       = pCmdLine;
  gnCmdShow       = nCmdShow;

  MDY_WIN32_TRY_TURN_ON_DEBUG();

#ifdef MDY_FLAG_IN_EDITOR
  DyInitializeEditorManagers();
#endif
  DyInitiailzeAllManagers();

  MDY_LOG_INFO_D("Platform : Windows");
  MDY_LOG_INFO_D("Running application routine.");

  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);

  int value = lua.script("return 54");
  MDY_LOG_CRITICAL_D("Hello world Lua! : {}", value);

  dy::MDyWindow::GetInstance().Run();

  MDY_LOG_INFO_D("Release all managers and resources.");

  DyReleaseAllManagers();
#ifdef MDY_FLAG_IN_EDITOR
  DyReleaseEditorManagers();
#endif
  MDY_WIN32_TRY_TURN_OFF_DEBUG();
  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
int main()
{

  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_MACOS)
int main()
{

  return 0;
}
#endif