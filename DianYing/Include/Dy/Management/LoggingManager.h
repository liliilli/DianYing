#ifndef GUARD_DY_MANAGEMENT_LOGGING_MANAGER_H
#define GUARD_DY_MANAGEMENT_LOGGING_MANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Helper/Library/HelperString.h>

namespace dy
{

/// @class MDyLog
/// @brief Manages logging.
class MDyLog final : public IDySingleton<MDyLog>
{
  MDY_SINGLETON_DERIVED(MDyLog);
  MDY_SINGLETON_PROPERTIES(MDyLog);
public:
  /// @enum ELevel
  /// @brief Log level enumeration
  enum class ELevel { Trace, Debug, Information, Warning, Error, Critical, };

  /// @brief Set logging output visibility level.
  void SetVisibleLevel(ELevel newLogLevel);
 
  /// @brief This function can push the log manually.
  /// In case of being intentialiy, recommend use MACRO version (MDY_LOG_INFO_D)
  void PushLog(ELevel logLevel, const std::string& iLogString);

private:
  /// @brief Allocate and bind logger resource. (Enable)
  /// This function must be called when only MDySetting::mIsEnabledLogging value is being on.
  EDySuccess pfTurnOn();

  /// @brief Release logger resource. (Disable)
  /// This function must be called when only MDySetting::mIsEnabledLogging value is being off.
  EDySuccess pfTurnOff();

  class Impl;
  Impl* mPimpl = nullptr;

  friend class MDySetting;
};

} /// ::dy namespace

//!
//! Global log macro
//!

#define MDY_LOG_INFO(__MAString__, ...) \
{ \
  using __ELevel = ::dy::MDyLog::ELevel; \
  const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
  ::dy::MDyLog::GetInstance().PushLog(__ELevel::Information, __String); \
}

#define MDY_LOG_WARNING(__MAString__, ...) \
{ \
  using __ELevel = ::dy::MDyLog::ELevel; \
  const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
  ::dy::MDyLog::GetInstance().PushLog(__ELevel::Warning, __String); \
}

#define MDY_LOG_ERROR(__MAString__, ...) \
{ \
  using __ELevel = ::dy::MDyLog::ELevel; \
  const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
  ::dy::MDyLog::GetInstance().PushLog(__ELevel::Error, __String); \
}

#define MDY_LOG_CRITICAL(__MAString__, ...) \
{ \
  using __ELevel = ::dy::MDyLog::ELevel; \
  const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
  ::dy::MDyLog::GetInstance().PushLog(__ELevel::Critical, __String); \
}

//!
//! Debug log macro for just only debug mode
//!

#if (defined(NDEBUG) == false)
  #define MDY_LOG_TRACE_D(__MAString__, ...) \
  { \
    using __ELevel = ::dy::MDyLog::ELevel; \
    const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
    ::dy::MDyLog::GetInstance().PushLog(__ELevel::Trace, __String); \
  }

  #define MDY_LOG_DEBUG_D(__MAString__, ...) \
  { \
    using __ELevel = ::dy::MDyLog::ELevel; \
    const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
    ::dy::MDyLog::GetInstance().PushLog(__ELevel::Debug, __String); \
  }

  #define MDY_LOG_INFO_D(__MAString__, ...) \
  { \
    using __ELevel = ::dy::MDyLog::ELevel; \
    const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
    ::dy::MDyLog::GetInstance().PushLog(__ELevel::Information, __String); \
  }

  #define MDY_LOG_WARNING_D(__MAString__, ...) \
  { \
    using __ELevel = ::dy::MDyLog::ELevel; \
    const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
    ::dy::MDyLog::GetInstance().PushLog(__ELevel::Warning, __String); \
  }

  #define MDY_LOG_ERROR_D(__MAString__, ...) \
  { \
    using __ELevel = ::dy::MDyLog::ELevel; \
    const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
    ::dy::MDyLog::GetInstance().PushLog(__ELevel::Error, __String); \
  }

  #define MDY_LOG_CRITICAL_D(__MAString__, ...) \
  { \
    using __ELevel = ::dy::MDyLog::ELevel; \
    const auto __String = ::dy::MakeStringU8(__MAString__, __VA_ARGS__); \
    ::dy::MDyLog::GetInstance().PushLog(__ELevel::Critical, __String); \
  }

#else
  #define MDY_LOG_TRACE_D(__MAString__, ...)    ((void*)0)
  #define MDY_LOG_INFO_D(__MAString__, ...)     ((void*)0)
  #define MDY_LOG_DEBUG_D(__MAString__, ...)    ((void*)0)
  #define MDY_LOG_WARNING_D(__MAString__, ...)  ((void*)0)
  #define MDY_LOG_CRITICAL_D(__MAString__, ...) ((void*)0)
  #define MDY_LOG_ERROR_D(__MAString__, ...)    ((void*)0)
#endif /// opgs16::debug::PushLog only on _DEBUG

#endif /// GUARD_DY_MANAGEMENT_LOGGING_MANAGER_H