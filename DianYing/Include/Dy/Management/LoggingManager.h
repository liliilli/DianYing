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

/// @brief Use this when logging even though build mode is release.
template <typename... TArgs> void MDY_LOG_INFO(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void MDY_LOG_WARNING(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void MDY_LOG_ERROR(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void MDY_LOG_CRITICAL(const std::string& iFormatString, TArgs&&... iArgs);

/// @brief Use this when logging only if build mdoe is debug.
template <typename... TArgs> void MDY_LOG_TRACE_D(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void MDY_LOG_DEBUG_D(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void MDY_LOG_INFO_D(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void MDY_LOG_WARNING_D(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void MDY_LOG_ERROR_D(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void MDY_LOG_CRITICAL_D(const std::string& iFormatString, TArgs&&... iArgs);

#endif /// GUARD_DY_MANAGEMENT_LOGGING_MANAGER_H
#include <Dy/Management/Inline/MLog.inl>