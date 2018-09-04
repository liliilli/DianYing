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

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <Dy/Management/SettingManager.h>

namespace {

///
/// @brief spdlog internal error callback function.
///
void DyCallbackLoggerError(const std::string& message)
{
  std::printf("DianYing internal error : %s\n", message.c_str());
}

///
/// @brief Get spdlog::level enum from abstracted log level EDyLogLevel.
///
[[nodiscard]]
spdlog::level::level_enum DyGetLogLevel(dy::EDyLogLevel level) noexcept
{
  switch (level)
  {
  case dy::EDyLogLevel::Trace:        return spdlog::level::trace;
  case dy::EDyLogLevel::Information:  return spdlog::level::info;
  case dy::EDyLogLevel::Debug:        return spdlog::level::debug;
  case dy::EDyLogLevel::Warning:      return spdlog::level::warn;
  case dy::EDyLogLevel::Critical:     return spdlog::level::critical;
  case dy::EDyLogLevel::Error:        return spdlog::level::err;
  default:                            return spdlog::level::trace;
  }
}

} /// unnamed namespace

namespace dy
{

EDySuccess MDyLog::pfInitialize()
{
  //this->pfTurnOn();
  return DY_SUCCESS;
}

EDySuccess MDyLog::pfRelease()
{
  this->pfTurnOff();
  return DY_SUCCESS;
}

void MDyLog::SetVisibleLevel(EDyLogLevel newLogLevel)
{
  this->mLogLevel = newLogLevel;

  const auto& settingManager = MDySetting::GetInstance();
  if (settingManager.IsEnabledFeatureLogging())
  {
    this->mLogger->set_level(DyGetLogLevel(this->mLogLevel));
  }
}

EDySuccess MDyLog::pfTurnOn()
{
  if (this->mLogger) return DY_SUCCESS;

  const auto& settingManager = MDySetting::GetInstance();
  if (!settingManager.IsEnableSubFeatureLoggingToFile() &&
      !settingManager.IsEnabledSubFeatureLoggingToConsole())
  {
    #if defined(_WIN32) && defined(MDY_PLATFORM_FLAG_WINDOWS)
      this->mSinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
    #else
      this->mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    #endif
  }
  else
  {
    if (settingManager.IsEnabledSubFeatureLoggingToConsole()) {
      #if defined(_WIN32) && defined(MDY_PLATFORM_FLAG_WINDOWS)
        this->mSinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
      #else
        this->mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
      #endif
    }

    // Caution :: To let spdlog create log file to specific path, directories must be created in advance.
    if (settingManager.IsEnableSubFeatureLoggingToFile()) {
      this->mSinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("./logs/debug_log.txt", false));
    }
  }

  this->mLogger = std::make_shared<spdlog::logger>("DianYing", this->mSinks.begin(), this->mSinks.end());
  this->mLogger->set_level(DyGetLogLevel(this->mLogLevel));

  spdlog::register_logger(this->mLogger);
  spdlog::set_error_handler(DyCallbackLoggerError);
  return DY_SUCCESS;
}

EDySuccess MDyLog::pfTurnOff()
{
  spdlog::drop_all();
  this->mLogger.reset();
  this->mSinks.clear();
  return DY_SUCCESS;
}

} /// ::dy namespace