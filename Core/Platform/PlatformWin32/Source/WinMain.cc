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

#include <FWindowsPlatform.h>
#include <AEngineBase.h>
#include <csignal>
#include <cassert>

using namespace dy;

namespace
{

/// @brief Signal handler callback function.
void OnSignalHandler(int signal)  
{  
  if (signal == SIGABRT) 
  { 
    DebugBreak();
    exit(3); 
  }
}  

/// @brief Turn on memory leak detection feature and console window for logging.
void InitializeWin32Debug()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

  typedef void (*SignalHandlerPointer)(int);  
  signal(SIGABRT, OnSignalHandler); 
}

} /// ::unnamed namespace

#if defined(_DEBUG) == true

#define WIN32_CRT_BREAKPOINT(Id)    _crtBreakAlloc(Id) 
#define WIN32_TRY_TURN_ON_DEBUG()   InitializeWin32Debug()
#define WIN32_TRY_TURN_OFF_DEBUG()  (void)0

#else

#define WIN32_CRT_BREAKPOINT(Id)    (void)0
#define WIN32_TRY_TURN_ON_DEBUG()   (void)0
#define WIN32_TRY_TURN_OFF_DEBUG()  (void)0

#endif

/// @brief Main function of win32 / win64 platform.
int APIENTRY WinMain(
  [[maybe_unused]] HINSTANCE hInstance, 
  [[maybe_unused]] HINSTANCE hPrevInstance, 
  [[maybe_unused]] LPSTR pCmdLine, 
  [[maybe_unused]] int nCmdShow)
{
  // Boot strapping.
  WIN32_TRY_TURN_ON_DEBUG();
  gEngine.reset(CreateEngine());

  // Store command lines.
  assert(gEngine != nullptr);
  gEngine->StoreCommmandLines(__argc, __argv);

  // Create platform information and insert it to gEngine.
  gEngine->SetPlatformInfo(std::make_unique<FWindowsPlatform>());

  // Initialize engine.
  {
    const auto flag = gEngine->GetPlatformInfo().InitPlatform();
    assert(flag == true);
  }
  {
    const auto flag = gEngine->Initialize();
    assert(flag == expr::DY_SUCCESS);
  }

  // Run engine.
  gEngine->Run();

  // Shutdown engine.
  {
    const auto flag = gEngine->Shutdown();
    assert(flag == expr::DY_SUCCESS);
  }

  // Release engine.
  {
    const auto flag = gEngine->GetPlatformInfo().ReleasePlatform();
    assert(flag == true);
  }
  gEngine = nullptr;
  WIN32_TRY_TURN_OFF_DEBUG();

  return 0;
}
