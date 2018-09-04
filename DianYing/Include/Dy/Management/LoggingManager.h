#ifndef GUARD_DY_MANAGEMENT_LOGGING_MANAGER_H
#define GUARD_DY_MANAGEMENT_LOGGING_MANAGER_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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

#include <spdlog/logger.h>
#if defined(_WIN32) && defined(MDY_PLATFORM_FLAG_WINDOWS)
#include <spdlog/sinks/wincolor_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

namespace dy
{

///
/// @enum EDyLogLevel
/// @brief log level enumeration
///
enum class EDyLogLevel
{
  Trace,
  Information,
  Debug,
  Warning,
  Critical,
  Error,
};

///
/// @class MDyLog
/// @brief
///
class MDyLog final : public ISingleton<MDyLog>
{
  MDY_SINGLETON_DERIVED(MDyLog);
  MDY_SINGLETON_PROPERTIES(MDyLog);
public:
  ///
  /// @brief Set logging output visibility level.
  ///
  void SetVisibleLevel(EDyLogLevel newLogLevel);

  ///
  /// @brief This function can push the log manually.
  /// In case of being intentialiy, recommend use MACRO version (MDY_LOG_INFORMATION)
  ///
  template <typename... TArgs>
  void PushLog(EDyLogLevel logLevel, const std::string_view& name, const TArgs&... args)
  {
    if (mLogger)
    {
      switch (logLevel)
      {
      case EDyLogLevel::Trace:        this->mLogger->trace(name, args...);     break;
      case EDyLogLevel::Information:  this->mLogger->info(name, args...);      break;
      case EDyLogLevel::Debug:        this->mLogger->debug(name, args...);     break;
      case EDyLogLevel::Warning:      this->mLogger->warn(name, args...);      break;
      case EDyLogLevel::Critical:     this->mLogger->critical(name, args...);  break;
      case EDyLogLevel::Error:        this->mLogger->error(name, args...);     break;;
      }
    }
  }

private:
  ///
  /// @brief Allocate and bind logger resource. (Enable)
  /// This function must be called when only MDySetting::mIsEnabledLogging value is being on.
  ///
  EDySuccess pfTurnOn();

  ///
  /// @brief Release logger resource. (Disable)
  /// This function must be called when only MDySetting::mIsEnabledLogging value is being off.
  ///
  EDySuccess pfTurnOff();

  std::vector<spdlog::sink_ptr>   mSinks        = {};
  std::shared_ptr<spdlog::logger> mLogger       = nullptr;
  EDyLogLevel                     mLogLevel     = EDyLogLevel::Information;

  friend class MDySetting;
};

} /// ::dy namespace

#if !defined(NDEBUG)
  #define MDY_LOG_INFORMATION(__MAString__, ...) \
  ::dy::MDyLog::GetInstance().PushLog(dy::EDyLogLevel::Information, __MAString__, __VA_ARGS__)
  #define MDY_LOG_DEBUG(__MAString__, ...) \
  ::dy::MDyLog::GetInstance().PushLog(dy::EDyLogLevel::Debug, __MAString__, __VA_ARGS__)
  #define MDY_LOG_WARNING(__MAString__, ...) \
  ::dy::MDyLog::GetInstance().PushLog(dy::EDyLogLevel::Warning, __MAString__, __VA_ARGS__)
  #define MDY_LOG_CRITICAL(__MAString__, ...) \
  ::dy::MDyLog::GetInstance().PushLog(dy::EDyLogLevel::Critical, __MAString__, __VA_ARGS__)
  #define MDY_LOG_ERROR(__MAString__, ...) \
  ::dy::MDyLog::GetInstance().PushLog(dy::EDyLogLevel::Error, __MAString__, __VA_ARGS__)
#else
  #define MDY_LOG_INFORMATION(__MAString__, ...)   ((void*)0)
  #define MDY_LOG_DEBUG(__MAString__, ...)         ((void*)0)
  #define MDY_LOG_WARNING(__MAString__, ...)       ((void*)0)
  #define MDY_LOG_CRITICAL(__MAString__, ...)      ((void*)0)
  #define MDY_LOG_ERROR(__MAString__, ...)         ((void*)0)
#endif /// opgs16::debug::PushLog only on _DEBUG

#endif /// GUARD_DY_MANAGEMENT_LOGGING_MANAGER_H