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
void ReleaseFunction(void (*function)(), bool update);

/// @brief
/// Test expression on runtime.
/// If expression return false, abort() with failedMessage unlike assert().
/// https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert
void AssertEnhanced(
  const char* expression, 
  bool result, 
  const char* filePath, 
  int line,
  const char* failedMessage);

/// @brief
/// Implement error message on function is defined but not implemented yet.
/// https://stackoverflow.com/questions/3314314/ways-to-show-your-co-programmers-that-some-methods-are-not-yet-implemented-in-a-c/3316954#3316954
void AssertNotImplemented(const char* filePath, int line);

/// @brief Unexpected branch entering.
void AssertUnexpectedBranch(const char* filePath, int line);

} /// ::dy::assert namespace

#endif /// GUARD_DY_HELPER_ASSERTION_H