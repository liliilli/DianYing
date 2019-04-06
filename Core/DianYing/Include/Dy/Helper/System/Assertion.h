#ifndef GUARD_DY_HELPER_ASSERTION_H
#define GUARD_DY_HELPER_ASSERTION_H
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

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Macroes
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

#if defined(_DEBUG)

/// @def MDY_ASSERT_MSG(__MAExpr__, __MAMessage__)
/// @brief Do runtime check for __MAExpr__ (Macro Argument Expression) in debug mode,
/// If expression is false, Assert application with __MAMessage__.
#define MDY_ASSERT_MSG(__MAExpr__, __MAMessage__) \
  ::dy::assert::AssertEnhanced(#__MAExpr__, __MAExpr__, __FILE__, __LINE__, __MAMessage__)

/// @def MDY_ASSERT(__MAExpr__)
/// @brief Do runtime check for __MAExpr__ (Macro Argument Expression) in debug mode,
/// If expression is false, Assert application with __MAExpr__.
#define MDY_ASSERT(__MAExpr__) \
  ::dy::assert::AssertEnhanced(#__MAExpr__, __MAExpr__, __FILE__, __LINE__, "Failed to assert " #__MAExpr__)

/// @def MDY_NOT_IMPLEMENTED_ASSERT()
/// @brief Assert program when statement is called in runtime debug mode.
/// Programmer which uses this function must use this macro only in not implemen
/// ted function completely properly.
#define MDY_NOT_IMPLEMENTED_ASSERT() \
  ::dy::assert::AssertNotImplemented(__FILE__, __LINE__)

/// @def MDY_UNEXPECTED_BRANCH()
/// @brief
#define MDY_UNEXPECTED_BRANCH() \
  ::dy::assert::AssertUnexpectedBranch(__FILE__, __LINE__)

#else

#define PHITOS_SET_RELEASE_FUNCTION(__MAFunc__) (void(0))
#define MDY_ASSERT_MSG(__MAExpr__, __MAMessage__) (void(0))
#define MDY_ASSERT(__MAExpr__) (void(0))
#define MDY_NOT_IMPLEMENTED_ASSERT() (void(0))
#define MDY_UNEXPECTED_BRANCH() (void(0))

#endif /// defined(_DEBUG)
  
/// @def MDY_ASSERT_MSG_FORCE(__MAExpr__, __MAMessage__)
/// @brief Do runtime check for __MAExpr__ (Macro Argument Expression) in anymode,
/// If expression is false, Assert application with __MAMessage__.
#define MDY_ASSERT_MSG_FORCE(__MAExpr__, __MAMessage__) \
  ::dy::assert::AssertEnhanced(#__MAExpr__, __MAExpr__, __FILE__, __LINE__, __MAMessage__)

/// @def MDY_ASSERT_MSG(__MAExpr__)
/// @brief Do runtime check for __MAExpr__ (Macro Argument Expression) in anymode,
/// If expression is false, Assert application with __MAExpr__.
#define MDY_ASSERT_FORCE(__MAExpr__) \
  ::dy::assert::AssertEnhanced(#__MAExpr__, __MAExpr__, __FILE__, __LINE__, "Failed to assert " #__MAExpr__)

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Implementation
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

/// @brief
/// @param function
/// @param update
inline void ReleaseFunction(void (*function)(), bool update) 
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

/// @brief
/// Test expression on runtime.
/// If expression return false, abort() with failedMessage unlike assert().
///
/// https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert
inline void AssertEnhanced(
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

/// @brief
/// Implement error message on function is defined but not implemented yet.
///
/// https://stackoverflow.com/questions/3314314/ways-to-show-your-co-programmers-that-some-methods-are-not-yet-implemented-in-a-c/3316954#3316954
inline void AssertNotImplemented(const char* filePath, int line)
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

/// @brief Unexpected branch entering.
inline void AssertUnexpectedBranch(const char* filePath, int line)
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

#endif /// GUARD_DY_HELPER_ASSERTION_H