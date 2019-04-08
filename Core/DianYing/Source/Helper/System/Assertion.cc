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
#include <Dy/Helper/System/Assertion.h>

#include <cstdio>
#include <cstdlib>
#include <spdlog/fmt/fmt.h>

#if defined(_WIN32) == true
#include <Windows.h>
#endif

#if defined(max)
#undef max
#endif
#if defined(min)
#undef min
#endif

namespace dy::assert
{
 
void ReleaseFunction(void(* function)(), bool update)
{
  static void (*sFunction)() = nullptr;

  if (update == true) 
  { 
    sFunction = function; 
    return;
  }

  if (sFunction != nullptr) 
  {
    sFunction();
  }
}
 
void AssertEnhanced(
  const char* expression, 
  bool result, 
  const char* filePath, 
  int line, 
  const char* failedMessage)
{
  if (result == true)
  {
    return;
  }

  const auto msg = fmt::format(
    R"dy(Assert Failed : {0}, Expected : {1}, Sourtce : {2}, Line : {3})dy",
    failedMessage, expression, filePath, line);
  std::printf("%s\n", msg.c_str());

  #if defined(_WIN32) == true
  {
    std::wstring winMsg{msg.begin(), msg.end()};
    MessageBox(nullptr, winMsg.c_str(), L"Error!", MB_ICONWARNING | MB_OK);
  }
  #endif

  ReleaseFunction(nullptr, false);
  exit(1);
}

void AssertNotImplemented(const char* filePath, int line)
{
  const auto msg = fmt::format(
    R"dy(This function is not implemented yet. Source : {0}, Line : {1})dy", 
    filePath, line);
  std::printf("%s\n", msg.c_str());

  #if defined(_WIN32) == true
  {
    std::wstring winMsg{msg.begin(), msg.end()};
    MessageBox(nullptr, winMsg.c_str(), L"Error!", MB_ICONWARNING | MB_OK);
  }
  #endif

  ReleaseFunction(nullptr, false);
  exit(1);
}

void AssertUnexpectedBranch(const char* filePath, int line)
{
  const auto msg = fmt::format(
    R"dy(Unexpected branch not intended. Source : {0}, Line : {1})dy", 
    filePath, line);
  std::printf("%s\n", msg.c_str());

  #if defined(_WIN32) == true
  {
    std::wstring winMsg{msg.begin(), msg.end()};
    MessageBox(nullptr, winMsg.c_str(), L"Error!", MB_ICONWARNING | MB_OK);
  }
  #endif

  printf("Unexpected branch not intended.\n" "Source : \t%s, line : %d\n", filePath, line);

  ReleaseFunction(nullptr, false);
  exit(1);
}

} /// ::dy::assert namespace
