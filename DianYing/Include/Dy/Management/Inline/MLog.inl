#ifndef GUARD_DY_MANAGEMENT_INLINE_MLOG_INL
#define GUARD_DY_MANAGEMENT_INLINE_MLOG_INL
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

#include <Dy/Management/LoggingManager.h>

template <typename... TArgs>
void MDY_LOG_INFO(const std::string& iFormatString, TArgs&&... iArgs)
{
  ::dy::MDyLog::GetInstance().PushLog(
      ::dy::MDyLog::ELevel::Information, 
      ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
  );
}

template <typename... TArgs>
void MDY_LOG_WARNING(const std::string& iFormatString, TArgs&&... iArgs)
{
  ::dy::MDyLog::GetInstance().PushLog(
      ::dy::MDyLog::ELevel::Warning, 
      ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
  );
}

template <typename... TArgs>
void MDY_LOG_ERROR(const std::string& iFormatString, TArgs&&... iArgs)
{
  ::dy::MDyLog::GetInstance().PushLog(
      ::dy::MDyLog::ELevel::Error, 
      ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
  );
}

template <typename... TArgs>
void MDY_LOG_CRITICAL(const std::string& iFormatString, TArgs&&... iArgs)
{
  ::dy::MDyLog::GetInstance().PushLog(
      ::dy::MDyLog::ELevel::Critical, 
      ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
  );
}

#if (defined(NDEBUG) == false)
  template <typename... TArgs>
  void MDY_LOG_TRACE_D(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MDyLog::GetInstance().PushLog(
        ::dy::MDyLog::ELevel::Trace, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void MDY_LOG_DEBUG_D(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MDyLog::GetInstance().PushLog(
        ::dy::MDyLog::ELevel::Debug, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void MDY_LOG_INFO_D(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MDyLog::GetInstance().PushLog(
        ::dy::MDyLog::ELevel::Information, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void MDY_LOG_WARNING_D(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MDyLog::GetInstance().PushLog(
        ::dy::MDyLog::ELevel::Warning, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void MDY_LOG_ERROR_D(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MDyLog::GetInstance().PushLog(
        ::dy::MDyLog::ELevel::Error, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

  template <typename... TArgs>
  void MDY_LOG_CRITICAL_D(const std::string& iFormatString, TArgs&&... iArgs)
  {
    ::dy::MDyLog::GetInstance().PushLog(
        ::dy::MDyLog::ELevel::Critical, 
        ::dy::MakeStringU8(iFormatString, std::forward<TArgs>(iArgs)...)
    );
  }

#else
  #define MDY_LOG_TRACE_D(__MAString__, ...)    ((void*)0)
  #define MDY_LOG_INFO_D(__MAString__, ...)     ((void*)0)
  #define MDY_LOG_DEBUG_D(__MAString__, ...)    ((void*)0)
  #define MDY_LOG_WARNING_D(__MAString__, ...)  ((void*)0)
  #define MDY_LOG_CRITICAL_D(__MAString__, ...) ((void*)0)
  #define MDY_LOG_ERROR_D(__MAString__, ...)    ((void*)0)
#endif /// opgs16::debug::PushLog only on _DEBUG

#endif /// GUARD_DY_MANAGEMENT_INLINE_MLOG_INL