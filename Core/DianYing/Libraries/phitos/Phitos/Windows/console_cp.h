#ifndef INCLUDE_PHITOS_DEPENDENT_CONSOLE_UTF8_H
#define INCLUDE_PHITOS_DEPENDENT_CONSOLE_UTF8_H
#if defined(_WIN32)

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Phitos/Windows/console_cp.h
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-27 Create file.
///

#include <cassert>

#include <Windows.h>

///
/// @namespace phitos::windows
/// @brief
///
///
namespace phitos::windows {

///
/// @class ConsoleCodePage
/// @brief
/// This class manages codepage of Windows console using RAII idiom.
/// To use language-specific or not os depedent console codepage to display
/// region independent string, This class must be instantiated by using
/// helper function.
///
class ConsoleCodePage final
{
public:
  ConsoleCodePage(unsigned int code_page)
  {
    assert(s_is_instantiated == EIsInstantiated::No);

    // To support displaying of unicode string (UTF-8, UTF-16, UTF-32)
    // We need to change console encoding to 65001 temporarilly.
    // When the application exits,
    // console encoding must be returned to previous encoding codepage.
    m_old_code_page = GetConsoleOutputCP();

    if (SetConsoleOutputCP(code_page))
      s_is_instantiated = EIsInstantiated::Yes;
    else {
      assert(false);
    }
  }

  ~ConsoleCodePage()
  {
    if (SetConsoleOutputCP(m_old_code_page)) {

    }
    else {
      assert(false);
    }
  }

  ConsoleCodePage(const ConsoleCodePage&) = delete;
  ConsoleCodePage& operator=(const ConsoleCodePage&) = delete;
  ConsoleCodePage(ConsoleCodePage&&) = delete;
  ConsoleCodePage& operator=(ConsoleCodePage&&) = delete;

private:
  enum class EIsInstantiated : bool {
    No = false,
    Yes = true,
  };

  inline static EIsInstantiated s_is_instantiated = EIsInstantiated::No;
  unsigned int m_old_code_page = 0;
};

}

#endif /// defined(_WIN32)
#endif /// INCLUDE_PHITOS_DEPENDENT_CONSOLE_UTF8_H
