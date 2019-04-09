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
#include <Dy/Management/MLog.h>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/logger.h>

#include <Dy/Management/MSetting.h>
#include <Dy/Management/MWindow.h>
#include <Dy/Management/Helper/FGuiLogStreamSink.h>
#include <Dy/Helper/System/Assertion.h>

#if defined(_WIN32) == true
#include <spdlog/sinks/wincolor_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

namespace 
{

using ELevel = ::dy::MLog::ELevel;

/// @brief spdlog internal error callback function.
void DyCallbackLoggerError(const std::string& message)
{
  std::printf("DianYing internal error : %s\n", message.c_str());
}

/// @brief Get spdlog::level enum from abstracted log level EDyLogLevel.
/// @param level `Dy` Log level value
/// @return `spdlog` dependent log level.
MDY_NODISCARD spdlog::level::level_enum DyGetLogLevel(ELevel level) noexcept
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

/// @class MLog::Impl 
/// @brief Implementation of logging management type.
struct MLog::Impl final
{
  DY_DEFAULT_PIMPL_CTOR(MLog);

  std::vector<spdlog::sink_ptr>   mSinks        = {};
  std::shared_ptr<spdlog::logger> mLogger       = nullptr;
  ELevel                          mLogLevel     = ELevel::Debug;
};

} /// ::dy namespace

namespace dy
{

EDySuccess MLog::pfInitialize()
{
  DY_INITIALIZE_PIMPL();
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MLog::pfRelease()
{
  spdlog::shutdown();

  DY_RESET_PIMPL();
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MLog::pfTurnOn()
{
  // If logger is alreay turned on,
  if (DY_PIMPL->mLogger != nullptr)
  {
    DyPushLogDebugInfo("MLog::mLogger already allocated.");
    return EDySuccess::DY_SUCCESS;
  }

  // Create sinks for logging.
  const auto& settingManager = MSetting::GetInstance();
  if (settingManager.IsEnabledFeatureLogging() == false) { return EDySuccess::DY_FAILURE; }

  if (settingManager.IsEnableSubFeatureLoggingToFile() == false
  &&  settingManager.IsEnabledSubFeatureLoggingToConsole() == false)
  {
    #if defined(_WIN32)
    DY_PIMPL->mSinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
    #else
    DY_PIMPL->mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    #endif
  }
  else
  {
    if (settingManager.IsEnabledSubFeatureLoggingToConsole() == true)
    {
      auto& managerWindow = gEngine->GetPlatformInfo();
      {
        const auto flag = managerWindow.CreateConsoleWindow();
        MDY_ASSERT_FORCE(flag == true);
      }

      #if defined(_WIN32)
      DY_PIMPL->mSinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
      #else
      DY_PIMPL->mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
      #endif
    }

    if (settingManager.IsEnableSubFeatureLoggingToFile() == true)
    { // Caution :: To let spdlog create log file to specific path, directories must be created in advance.
      DY_PIMPL->mSinks.push_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(settingManager.GetLogFilePath(), false));
    }
  }

  // Also, create gui logging sink.
  DY_PIMPL->mSinks.push_back(std::make_shared<TGuiLogStreamSinkMt>());

  // Create logger instance and thread pool for logging.
  spdlog::init_thread_pool(8192, 1);
  DY_PIMPL->mLogger = std::make_shared<spdlog::async_logger>(
    "DianYing",
    DY_PIMPL->mSinks.begin(), DY_PIMPL->mSinks.end(),
    spdlog::thread_pool(),
    spdlog::async_overflow_policy::overrun_oldest);
  DY_PIMPL->mLogger->set_level(DyGetLogLevel(DY_PIMPL->mLogLevel));

  // Register logger.
  DyPushLogDebugDebug("MLog::mLogger level : {}.", DyGetLogLevel(DY_PIMPL->mLogLevel));
  DyPushLogDebugDebug("MLog::mLogger resource allocated.");
  spdlog::register_logger(DY_PIMPL->mLogger);

  DyPushLogDebugDebug("MLog::mLogger resource registered.");
  spdlog::set_error_handler(DyCallbackLoggerError);

  return EDySuccess::DY_SUCCESS;
}

EDySuccess MLog::pfTurnOff()
{
  // Drop all registration of logging sink, instance.
  spdlog::drop_all();
  DY_PIMPL->mLogger.reset();
  DY_PIMPL->mSinks.clear();

  if (auto& windowManager = gEngine->GetPlatformInfo();
      windowManager.IsConsoleWindowCreated() == true)
  {
    const auto flag = windowManager.RemoveConsoleWindow();
    MDY_ASSERT(flag == true);
  }
  return EDySuccess::DY_SUCCESS;
}

void MLog::SetVisibleLevel(ELevel newLogLevel)
{
  DY_PIMPL->mLogLevel = newLogLevel;

  if (MSetting::GetInstance().IsEnabledFeatureLogging() == true)
  {
    const auto logLevelValue = DyGetLogLevel(DY_PIMPL->mLogLevel);

    DY_PIMPL->mLogger->set_level(logLevelValue);
    DyPushLogDebugDebug("MLog::mLogger level : {}.", logLevelValue);
  }
}

void MLog::PushLog(ELevel logLevel, const std::string& iLogString)
{
  if (DY_PIMPL->mLogger == nullptr) { return; }

  switch (logLevel)
  {
  case ELevel::Trace:        DY_PIMPL->mLogger->trace(iLogString);     break;
  case ELevel::Information:  DY_PIMPL->mLogger->info(iLogString);      break;
  case ELevel::Debug:        DY_PIMPL->mLogger->debug(iLogString);     break;
  case ELevel::Warning:      DY_PIMPL->mLogger->warn(iLogString);      break;
  case ELevel::Error:        DY_PIMPL->mLogger->error(iLogString);     break;;
  case ELevel::Critical:     DY_PIMPL->mLogger->critical(iLogString);  break;
  }
}

DY_DEFINE_PIMPL(MLog);
DY_DEFINE_DEFAULT_DESTRUCTOR(MLog);

} /// ::dy namespace