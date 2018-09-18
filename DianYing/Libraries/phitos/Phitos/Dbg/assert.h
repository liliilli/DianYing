#ifndef INCLUDE_PHITOS_DBG_ASSERT_H
#define INCLUDE_PHITOS_DBG_ASSERT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Dbg/assert.h
///
/// @brief
/// Enhanced assertion implementation header file.
///
/// These assertions are used to replace with generic assert(expression)
/// providing more information and features such as logging.
///
/// The user can use assertions with printf.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-20 Create file.
/// 2018-05-27 Move file.
///
/// https://stackoverflow.com/questions/12062365/is-using-assert-in-c-bad-practice
///
/// @todo Check if console window is opened or not.
/// @todo Implement third logger library message printing.
/// @todo Show stack trace in debug mode.
///

#include <cstdio>
#include <cstdlib>

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Macroes
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

#if defined(_DEBUG)

#define PHITOS_SET_RELEASE_FUNCTION(__MAFunc__) \
    ::phitos::debug::__ReleaseFunction(__MAFunc__, true)

///
/// @def PHITOS_ASSERT(__MAExpr__, __MAMessage__)
/// Do runtime check for __MAExpr__ (Macro Argument Expression) in debug mode,
/// If expression is false, Assert application with __MAMessage__.
///
#define PHITOS_ASSERT(__MAExpr__, __MAMessage__) \
  ::phitos::debug::__EnhancedAssert( \
      #__MAExpr__, __MAExpr__, __FILE__, __LINE__, __MAMessage__)

///
/// @def PHITOS_NOT_IMPLEMENTED_ASSERT()
/// Assert program when statement is called in runtime debug mode.
/// Programmer which uses this function must use this macro only in not implemen
/// ted function completely properly.
///
#define PHITOS_NOT_IMPLEMENTED_ASSERT() \
  ::phitos::debug::__NotImplementedAssert(__FILE__, __LINE__)

///
/// @def PHITOS_UNEXPECTED_BRANCH()
///
#define PHITOS_UNEXPECTED_BRANCH() \
  ::phitos::debug::__UnexpectedBranch(__FILE__, __LINE__)

#else

#define PHITOS_SET_RELEASE_FUNCTION(__MAFunc__) \
  (void(0))

#define PHITOS_ASSERT(__MAExpr__, __MAMessage__) \
  (void(0));

#define PHITOS_NOT_IMPLEMENTED_ASSERT() \
  (void(0));

#define PHITOS_UNEXPECTED_BRANCH() \
  (void(0));

#endif /// defined(_DEBUG)

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Implementation
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

namespace phitos::debug {

///
/// @brief
///
inline void __ReleaseFunction(void (*function)(void), bool update) {
  static void (*s_function)(void) = nullptr;

  if (update)
    s_function = function;
  else {
    if (s_function) s_function();
  }
}

///
/// @brief
/// Test expression on runtime.
/// If expression return false, abort() with failed_message unlike assert().
///
/// https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert
///
/// @todo Implement third logger library message printing.
///
inline void __EnhancedAssert(
    const char* expression_string,
    bool expression_result,
    const char* file_path,
    int line,
    const char* failed_message)
{
  if (!expression_result) {
    printf("Assert failed : %s\n"
           "Expected : \t%s\n"
           "Source : \t%s, line : %d\n",
           failed_message, expression_string, file_path, line);
    __ReleaseFunction(nullptr, false);
    abort();
  }
}

///
/// @brief
/// Implement error message on function is defined but not implemented yet.
///
/// https://stackoverflow.com/questions/3314314/ways-to-show-your-co-programmers-that-some-methods-are-not-yet-implemented-in-a-c/3316954#3316954
///
/// @todo Implement third logger library message printing.
///
inline void __NotImplementedAssert(
    const char* file_path,
    int line)
{
  printf("This function is not implemented yet.\n"
         "Source : \t%s, line : %d\n",
         file_path, line);
  __ReleaseFunction(nullptr, false);
  abort();
}

///
/// @brief
/// Unexpected branch entering.
///
inline void __UnexpectedBranch(
    const char* file_path,
    int line)
{
  printf("Unexpected branch not intended.\n"
         "Source : \t%s, line : %d\n",
         file_path, line);
  __ReleaseFunction(nullptr, false);
  abort();
}

} /// ::phitos::debug namespace

#endif /// INCLUDE_PHITOS_DBG_ASSERT_H
