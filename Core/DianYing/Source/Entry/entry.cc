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

#include <Dy/Include/Entry.h>

/// @brief Main entry function of WIN32 platforms.
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
#include <csignal>

namespace
{

HINSTANCE ghInstance      = nullptr;
HINSTANCE ghPrevInstance  = nullptr;
LPSTR     gpCmdLine;
int       gnCmdShow;

void __SignalHandler(int signal)  
{  
  if (signal == SIGABRT) { exit(3); }
}  

///
/// @brief Turn on memory leak detection feature and console window for logging.
///
EDySuccess MDY_PRIVATE(DyInitializeWin32Debug)()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );


  typedef void (*SignalHandlerPointer)(int);  
  signal(SIGABRT, __SignalHandler); 
  return DY_SUCCESS;
}

} /// unname namespace

#if (defined(_DEBUG) == true) || (defined(NDEBUG) == false)
#define MDY_WIN32_TRY_TURN_ON_DEBUG() MDY_CALL_ASSERT_SUCCESS(MDY_PRIVATE(DyInitializeWin32Debug)())
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  (void)0
#else
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   (void)0
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  (void)0
#endif

/// @brief Main function of win32 / win64 platform.
int APIENTRY WinMain(_MIN_ HINSTANCE hInstance, _MIN_ HINSTANCE hPrevInstance, _MIN_ LPSTR pCmdLine, _MIN_ int nCmdShow)
{
  ghInstance      = hInstance;
  ghPrevInstance  = hPrevInstance;
  gpCmdLine       = pCmdLine;
  gnCmdShow       = nCmdShow;

  //_crtBreakAlloc = 681;

  MDY_WIN32_TRY_TURN_ON_DEBUG();
  MDY_CALL_ASSERT_SUCCESS(dy::GEngine::Initialize());
  dy::GEngine::GetInstance()();
  MDY_CALL_ASSERT_SUCCESS(dy::GEngine::Release());
  MDY_WIN32_TRY_TURN_OFF_DEBUG();
  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
int main() { return 0; }
#elif defined(MDY_PLATFORM_FLAG_MACOS)
int main() { return 0; }
#endif