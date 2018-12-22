#include <precompiled.h>
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

/// Header file
#include <Dy/Management/LoggingManager.h>

#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <Dy/Management/SettingManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/Helper/GuiLogStreamSink.h>

namespace {

///
/// @brief spdlog internal error callback function.
///
void DyCallbackLoggerError(_MIN_ const std::string& message)
{
  std::printf("DianYing internal error : %s\n", message.c_str());
}

///
/// @brief Get spdlog::level enum from abstracted log level EDyLogLevel.
/// @param level `Dy` Log level value
/// @return `spdlog` dependent log level.
///
MDY_NODISCARD spdlog::level::level_enum DyGetLogLevel(_MIN_ dy::EDyLogLevel level) noexcept
{
  switch (level)
  {
  case dy::EDyLogLevel::Trace:        return spdlog::level::trace;
  case dy::EDyLogLevel::Debug:        return spdlog::level::debug;
  case dy::EDyLogLevel::Information:  return spdlog::level::info;
  case dy::EDyLogLevel::Warning:      return spdlog::level::warn;
  case dy::EDyLogLevel::Critical:     return spdlog::level::critical;
  case dy::EDyLogLevel::Error:        return spdlog::level::err;
  default:
    MDY_UNEXPECTED_BRANCH();
    return spdlog::level::off;
  }
}

} /// unnamed namespace

namespace dy
{

EDySuccess MDyLog::pfInitialize()
{
  MDY_LOG_DEBUG_D("{} | MDyLog::pfInitialize().", "FunctionCall");
  this->pfTurnOn();

  return DY_SUCCESS;
}

EDySuccess MDyLog::pfRelease()
{
  MDY_LOG_DEBUG_D("{} | MDyLog::pfRelease().", "FunctionCall");
  this->pfTurnOff();
  spdlog::shutdown();

  return DY_SUCCESS;
}

EDySuccess MDyLog::pfTurnOn()
{
  MDY_LOG_DEBUG_D("{} | MDyLog::pfTurnOn()", "Function call");

  if (MDY_CHECK_ISNOTEMPTY(this->mLogger))
  {
    MDY_LOG_INFO_D("MDyLog::mLogger already allocated.");
    return DY_SUCCESS;
  }

  // Create sinks for logging.
  const auto& settingManager = MDySetting::GetInstance();
  if (settingManager.IsEnabledFeatureLogging() == false) { return DY_FAILURE; }

  if ( settingManager.IsEnableSubFeatureLoggingToFile()     == false
    && settingManager.IsEnabledSubFeatureLoggingToConsole() == false)
  {
  #if defined(_WIN32) && defined(MDY_PLATFORM_FLAG_WINDOWS)
    this->mSinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
  #else
    this->mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  #endif
  }
  else
  {
    if (settingManager.IsEnabledSubFeatureLoggingToConsole() == true)
    {
    #if defined(_WIN32) && defined(MDY_PLATFORM_FLAG_WINDOWS)
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

  MDY_LOG_DEBUG_D("MDyLog::mLogger level : {}.", DyGetLogLevel(this->mLogLevel));
  MDY_LOG_DEBUG_D("MDyLog::mLogger resource allocated.");
  spdlog::register_logger(this->mLogger);
  MDY_LOG_DEBUG_D("MDyLog::mLogger resource registered.");
  spdlog::set_error_handler(DyCallbackLoggerError);

  return DY_SUCCESS;
}

EDySuccess MDyLog::pfTurnOff()
{
  MDY_LOG_INFO_D("{} | MDyLog::pfTurnOff().", "FunctionCall");

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

void MDyLog::SetVisibleLevel(_MIN_ EDyLogLevel newLogLevel)
{
  this->mLogLevel = newLogLevel;

  if (const auto& settingManager = MDySetting::GetInstance();
      settingManager.IsEnabledFeatureLogging() == true)
  {
    const auto logLevelValue = DyGetLogLevel(this->mLogLevel);

    this->mLogger->set_level(logLevelValue);
    MDY_LOG_DEBUG_D("MDyLog::mLogger level : {}.", logLevelValue);
  }
}

} /// ::dy namespace