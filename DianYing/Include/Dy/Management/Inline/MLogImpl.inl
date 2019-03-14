#ifndef GUARD_DY_MANAGEMENT_MLOG_IMPL_INL
#define GUARD_DY_MANAGEMENT_MLOG_IMPL_INL
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

namespace dy
{

inline MLog::Impl::~Impl()
{
  spdlog::shutdown();
}

inline void MLog::Impl::SetVisibleLevel(ELevel iLogLevel)
{
  this->mLogLevel = iLogLevel;

  if (MDySetting::GetInstance().IsEnabledFeatureLogging() == true)
  {
    const auto logLevelValue = DyGetLogLevel(this->mLogLevel);

    this->mLogger->set_level(logLevelValue);
    DyPushLogDebugDebug("MLog::mLogger level : {}.", logLevelValue);
  }
}

inline void MLog::Impl::PushLog(ELevel logLevel, const std::string& iLogString)
{
  if (this->mLogger == nullptr) { return; }

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

inline EDySuccess MLog::Impl::pfTurnOn()
{
  DyPushLogDebugDebug("{} | MLog::pfTurnOn()", "Function call");

  if (this->mLogger != nullptr)
  {
    DyPushLogDebugInfo("MLog::mLogger already allocated.");
    return DY_SUCCESS;
  }

  // Create sinks for logging.
  const auto& settingManager = MDySetting::GetInstance();
  if (settingManager.IsEnabledFeatureLogging() == false) { return DY_FAILURE; }

  if (settingManager.IsEnableSubFeatureLoggingToFile() == false
  &&  settingManager.IsEnabledSubFeatureLoggingToConsole() == false)
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
      if (auto& managerWindow = MDyWindow::GetInstance();
          managerWindow.IsInitialized() == true)
      {
        MDY_CALL_ASSERT_SUCCESS(managerWindow.CreateConsoleWindow());
      }
      #if defined(_WIN32)
      this->mSinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
      #else
      this->mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
      #endif
    }

    if (settingManager.IsEnableSubFeatureLoggingToFile() == true)
    { // Caution :: To let spdlog create log file to specific path, directories must be created in advance.
      this->mSinks.push_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(settingManager.GetLogFilePath(), false));
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

  DyPushLogDebugDebug("MLog::mLogger level : {}.", DyGetLogLevel(this->mLogLevel));
  DyPushLogDebugDebug("MLog::mLogger resource allocated.");
  spdlog::register_logger(this->mLogger);
  DyPushLogDebugDebug("MLog::mLogger resource registered.");
  spdlog::set_error_handler(DyCallbackLoggerError);

  return DY_SUCCESS;
}

inline EDySuccess MLog::Impl::pfTurnOff()
{
  DyPushLogDebugInfo("{} | MLog::pfTurnOff().", "FunctionCall");

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

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_MLOG_IMPL_INL