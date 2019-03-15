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

#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/logger.h>

#include <Dy/Management/MSetting.h>
#include <Dy/Management/MWindow.h>
#include <Dy/Management/Helper/FGuiLogStreamSink.h>

#if defined(_WIN32) == true
#include <spdlog/sinks/wincolor_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

namespace 
{

using ELevel = ::dy::MLog::ELevel;

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

/// @class MLog::Impl 
/// @brief Implementation of logging management type.
class MLog::Impl final
{
public:
  Impl() = default;
  ~Impl();

  /// @brief Set logging output visibility level.
  void SetVisibleLevel(ELevel newLogLevel);
 
  /// @brief This function can push the log manually.
  /// In case of being intentialiy, recommend use MACRO version (DyPushLogDebugInfo)
  void PushLog(ELevel logLevel, const std::string& iLogString);

  /// @brief Allocate and bind logger resource. (Enable)
  /// This function must be called when only MSetting::mIsEnabledLogging value is being on.
  EDySuccess pfTurnOn();
  /// @brief Release logger resource. (Disable)
  /// This function must be called when only MSetting::mIsEnabledLogging value is being off.
  EDySuccess pfTurnOff();
 
private:
  std::vector<spdlog::sink_ptr>   mSinks        = {};
  std::shared_ptr<spdlog::logger> mLogger       = nullptr;
  ELevel                          mLogLevel     = ELevel::Debug;
};

} /// ::dy namespace
#include <Dy/Management/Inline/MLogImpl.inl>

namespace dy
{

EDySuccess MLog::pfInitialize()
{
  this->mPimpl = new (std::nothrow) Impl();
  return DY_SUCCESS;
}

EDySuccess MLog::pfRelease()
{
  delete this->mPimpl;
  return DY_SUCCESS;
}

EDySuccess MLog::pfTurnOn()
{
  return this->mPimpl->pfTurnOn();
}

EDySuccess MLog::pfTurnOff()
{
  return this->mPimpl->pfTurnOff();
}

void MLog::SetVisibleLevel(ELevel newLogLevel)
{
  this->mPimpl->SetVisibleLevel(newLogLevel);
}

void MLog::PushLog(ELevel logLevel, const std::string& iLogString)
{
  this->mPimpl->PushLog(logLevel, iLogString);
}

} /// ::dy namespace