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
#include <FWindowsDebug.h>
#include <cstdio>
#include <csignal>
#include <iostream>
#include <Windows.h>

namespace dy
{

void FWindowsDebug::OnAssertionFailed(
  const char* failedMessage, const char* function, const char* file, int line)
{
  // Make final messsage.
  char message[512] = {0};
  sprintf_s(message, 
    "Assert %s, in %s of %s at %d.", 
    failedMessage, function, file, line);

  // Output message into IDE?
  OutputDebugStringA(message);

  // Ouptut message into console if exist.
  std::cerr << message;
 
  // Display message box.
  int nCode = MessageBoxA(NULL, message, "Runtime assertion failed",
    MB_TASKMODAL|MB_ICONHAND|MB_ABORTRETRYIGNORE|MB_SETFOREGROUND);

  // Abort: abort the program
  if (nCode == IDABORT)
  {
    // raise abort signal
    raise(SIGABRT);
    this->TryCallReleaseFunction();

    // We usually won't get here, but it's possible that
    //  SIGABRT was ignored.  So exit the program anyway.
    exit(3);
  }

  // Retry: call the debugger
  if (nCode == IDRETRY)
  {
    DebugBreak();
    this->TryCallReleaseFunction();
    return;
  }

  // Ignore: continue execution
  if (nCode == IDIGNORE) { return; }
}

} /// ::dy namespace