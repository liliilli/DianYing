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

/// @class MLog
/// @brief Manages logging.
class MLog final : public IDySingleton<MLog>
{
  MDY_SINGLETON_DERIVED(MLog);
  MDY_SINGLETON_PROPERTIES(MLog);
public:
  /// @enum ELevel
  /// @brief Log level enumeration
  enum class ELevel { Trace, Debug, Information, Warning, Error, Critical, };

  /// @brief Set logging output visibility level.
  void SetVisibleLevel(ELevel newLogLevel);
 
  /// @brief This function can push the log manually.
  /// In case of being intentialiy, recommend use MACRO version (DyPushLogDebugInfo)
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
template <typename... TArgs> void DyPushLogInfo(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void DyPushLogWarning(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void DyPushLogError(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void DyPushLogCritical(const std::string& iFormatString, TArgs&&... iArgs);

/// @brief Use this when logging only if build mdoe is debug.
template <typename... TArgs> void DyPushLogDebugTrace(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void DyPushLogDebugDebug(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void DyPushLogDebugInfo(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void DyPushLogDebugWarning(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void DyPushLogDebugError(const std::string& iFormatString, TArgs&&... iArgs);
template <typename... TArgs> void DyPushLogDebugCritical(const std::string& iFormatString, TArgs&&... iArgs);

#endif /// GUARD_DY_MANAGEMENT_LOGGING_MANAGER_H
#include <Dy/Management/Inline/MLogHelper.inl>