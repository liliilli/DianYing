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

#include <Dy/Helper/Pointer.h>
#include <Dy/Core/Etc/StaticCoreTrigger.h>
#include <Dy/Management/WindowManager.h>
#include "Dy/Test/TestCxxopt.h"

namespace
{

///
/// @brief Initialize all managers related to DianYing rendering application.
/// Initialzation order must be ended with MDyWindow and started with MDySetting by getting argv
/// as option.
///

///
/// @brief Release all managers related to DianYing rendering application.
/// Order must be revered with DyInitializeAllManagers().
///

} /// unnamed namespace

///
/// @brief Main entry function of WIN32 platforms.
///
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{

dy::Owner<FILE*> gFp      = nullptr;
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
  using dy::SDyCoreTrigger;

  ghInstance      = hInstance;
  ghPrevInstance  = hPrevInstance;
  gpCmdLine       = pCmdLine;
  gnCmdShow       = nCmdShow;

  MDY_WIN32_TRY_TURN_ON_DEBUG();

  SDyCoreTrigger::InitiailzeAllManagers();

  //MDY_LOG_INFO_D("Running application routine.");
  dy::MDyWindow::GetInstance().Run();

  //MDY_LOG_INFO_D("Release all managers and resources.");

  SDyCoreTrigger::ReleaseAllManagers();
  MDY_WIN32_TRY_TURN_OFF_DEBUG();
  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
int main() { return 0; }
#elif defined(MDY_PLATFORM_FLAG_MACOS)
int main() { return 0; }
#endif