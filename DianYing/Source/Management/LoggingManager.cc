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
#include <Dy/Management/LoggingManager.h>

#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <Dy/Management/SettingManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/Helper/GuiLogStreamSink.h>

#include <spdlog/logger.h>
#if defined(_WIN32) && defined(MDY_PLATFORM_FLAG_WINDOWS)
#include <spdlog/sinks/wincolor_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

namespace {

using ELevel = ::dy::MDyLog::ELevel;

/// @brief spdlog internal error callback function.
void DyCallbackLoggerError(_MIN_ const std::string& message)
{
  std::printf("DianYing internal error : %s\n", message.c_str());
}

/// @brief Get spdlog::level enum from abstracted log level EDyLogLevel.
/// @param level `Dy` Log level value
/// @return `spdlog` dependent log level.
MDY_NODISCARD spdlog::level::level_enum DyGetLogLevel(_MIN_ ELevel level) noexcept
{
  switch (level)
  {
  case ELevel::Trace:        return spdlog::level::trace;
  case ELevel::Debug:        return spdlog::level::debug;
  case ELevel::Information:  return spdlog::level::info;
  case ELevel::Warning:      return spdlog::level::warn;
  case ELevel::Critical:     return spdlog::level::critical;
  case ELevel::Error:        return spdlog::level::err;
  default:
    MDY_UNEXPECTED_BRANCH();
    return spdlog::level::off;
  }
}

} /// unnamed namespace

namespace dy
{

/// @class MDyLog::Impl 
/// @brief Implementation of logging management type.
class MDyLog::Impl final
{
public:
  Impl() = default;
  ~Impl()
  {
    spdlog::shutdown();
  }

  /// @brief Set logging output visibility level.
  void SetVisibleLevel(ELevel newLogLevel)
  {
    this->mLogLevel = newLogLevel;

    if (const auto& settingManager = MDySetting::GetInstance();
        settingManager.IsEnabledFeatureLogging() == true)
    {
      const auto logLevelValue = DyGetLogLevel(this->mLogLevel);

      this->mLogger->set_level(logLevelValue);
      DyPushLogDebugDebug("MDyLog::mLogger level : {}.", logLevelValue);
    }
  }
 
  /// @brief This function can push the log manually.
  /// In case of being intentialiy, recommend use MACRO version (DyPushLogDebugInfo)
  void PushLog(ELevel logLevel, const std::string& iLogString)
  {
    if (mLogger == nullptr) { return; }

    switch (logLevel)
    {
    case ELevel::Trace:        this->mLogger->trace(iLogString);     break;
    case ELevel::Information:  this->mLogger->info(iLogString);      break;
    case ELevel::Debug:        this->mLogger->debug(iLogString);     break;
    case ELevel::Warning:      this->mLogger->warn(iLogString);      break;
    case ELevel::Error:        this->mLogger->error(iLogString);     break;;
    case ELevel::Critical:     this->mLogger->critical(iLogString);  break;
    }
  }

  /// @brief Allocate and bind logger resource. (Enable)
  /// This function must be called when only MDySetting::mIsEnabledLogging value is being on.
  EDySuccess pfTurnOn()
  {
    DyPushLogDebugDebug("{} | MDyLog::pfTurnOn()", "Function call");

    if (MDY_CHECK_ISNOTEMPTY(this->mLogger))
    {
      DyPushLogDebugInfo("MDyLog::mLogger already allocated.");
      return DY_SUCCESS;
    }

    // Create sinks for logging.
    const auto& settingManager = MDySetting::GetInstance();
    if (settingManager.IsEnabledFeatureLogging() == false) { return DY_FAILURE; }

    if (settingManager.IsEnableSubFeatureLoggingToFile() == false
      && settingManager.IsEnabledSubFeatureLoggingToConsole() == false)
    {
#if defined(_WIN32)
      this->mSinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
#else
      this->mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#endif
    }
    else
    {
      if (settingManager.IsEnabledSubFeatureLoggingToConsole() == true)
      {
#if defined(_WIN32)
        MDY_CALL_ASSERT_SUCCESS(MDyWindow::GetInstance().CreateConsoleWindow());
        this->mSinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
#else
        this->mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#endif
      }

      if (settingManager.IsEnableSubFeatureLoggingToFile() == true)
      { // Caution :: To let spdlog create log file to specific path, directories must be created in advance.
        this->mSinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(settingManager.GetLogFilePath(), false));
      }
    }

    // Also, create gui logging sink.
    this->mSinks.push_back(std::make_shared<TGuiLogStreamSinkMt>());

    // Create logger instance and thread pool for logging.
    spdlog::init_thread_pool(8192, 1);
    this->mLogger = std::make_shared<spdlog::async_logger>(
      "DianYing",
      this->mSinks.begin(), this->mSinks.end(),
      spdlog::thread_pool(),
      spdlog::async_overflow_policy::overrun_oldest);
    this->mLogger->set_level(DyGetLogLevel(this->mLogLevel));

    DyPushLogDebugDebug("MDyLog::mLogger level : {}.", DyGetLogLevel(this->mLogLevel));
    DyPushLogDebugDebug("MDyLog::mLogger resource allocated.");
    spdlog::register_logger(this->mLogger);
    DyPushLogDebugDebug("MDyLog::mLogger resource registered.");
    spdlog::set_error_handler(DyCallbackLoggerError);

    return DY_SUCCESS;
  }

  /// @brief Release logger resource. (Disable)
  /// This function must be called when only MDySetting::mIsEnabledLogging value is being off.
  EDySuccess pfTurnOff()
  {
    DyPushLogDebugInfo("{} | MDyLog::pfTurnOff().", "FunctionCall");

    // Drop all registration of logging sink, instance.
    spdlog::drop_all();
    this->mLogger .reset();
    this->mSinks  .clear();

    auto& windowManager = MDyWindow::GetInstance();
    if (windowManager.IsCreatedConsoleWindow() == true)
    {
      MDY_CALL_ASSERT_SUCCESS(windowManager.RemoveConsoleWindow());
    }
    return DY_SUCCESS;
  }

private:
  std::vector<spdlog::sink_ptr>   mSinks        = {};
  std::shared_ptr<spdlog::logger> mLogger       = nullptr;
  ELevel                          mLogLevel     = ELevel::Debug;
};

} /// ::dy namespace

namespace dy
{

EDySuccess MDyLog::pfInitialize()
{
  this->mPimpl =  new (std::nothrow) Impl();
  this->pfTurnOn();

  return DY_SUCCESS;
}

EDySuccess MDyLog::pfRelease()
{
  this->pfTurnOff();
  delete this->mPimpl;

  return DY_SUCCESS;
}

EDySuccess MDyLog::pfTurnOn()
{
  return this->mPimpl->pfTurnOn();
}

EDySuccess MDyLog::pfTurnOff()
{
  return this->mPimpl->pfTurnOff();
}

void MDyLog::SetVisibleLevel(ELevel newLogLevel)
{
  this->mPimpl->SetVisibleLevel(newLogLevel);
}

void MDyLog::PushLog(ELevel logLevel, const std::string& iLogString)
{
  this->mPimpl->PushLog(logLevel, iLogString);
}

} /// ::dy namespace