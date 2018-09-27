#ifndef GUARD_DY_MANAGEMENT_SETTING_MANAGER_H
#define GUARD_DY_MANAGEMENT_SETTING_MANAGER_H
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
/// @todo IMPLEMENT SET OVERALL WINDOW WIDTH, HEIGHT AS CHANGING VIEWPORT OF EACH API FRAMEBUFFER.
///

#include <Dy/Management/Interface/ISingletonCrtp.h>

namespace dy
{

///
/// @enum EDyRenderingApiType
/// @brief Rendering API type for rendering scene. Must not be EDyRenderingApiType::NoneError when using.
///
enum class EDyRenderingApiType
{
  NoneError,
  Vulkan,
#if defined(_WIN32)
  DirectX11,
  DirectX12,
#endif
#if defined(__APPLE__)
  Metal,
#else
  OpenGL,
#endif
};

///
/// @class MDySetting
/// @brief manages global settings of DianYing renderer application.
///
class MDySetting final : public ISingleton<MDySetting>
{
  MDY_SINGLETON_DERIVED(MDySetting);
  MDY_SINGLETON_PROPERTIES(MDySetting);
public:
  ///
  /// @brief Check if rendering vsync is enabled.
  /// If Vsync enabled, max frame per seconds are fixed by 60 fps.
  /// If not, frame will be beyond 60 fps along your hardware but jittering may be occured sometimes.
  ///
  [[nodiscard]] bool IsVSyncEnabled() const noexcept;

  ///
  /// @brief Get enable/disable flag of feature, logging as true/false.
  ///
  [[nodiscard]] FORCEINLINE bool IsEnabledFeatureLogging() const noexcept
  {
    return this->mIsEnabledLogging;
  }

  ///
  /// @brief Get enable/disable flag of subfeature, logging to console as true/false.
  /// If changed, logging feature must be restarted manually.
  ///
  [[nodiscard]] FORCEINLINE bool IsEnabledSubFeatureLoggingToConsole() const noexcept
  {
    return this->mIsEnabledLoggingToConsole;
  }

  ///
  /// @brief Get enable/disable flag of subfeature, logging to file as true/false.
  /// If changed, logging feature must be restarted manually.
  ///
  [[nodiscard]] FORCEINLINE bool IsEnableSubFeatureLoggingToFile() const noexcept
  {
    return this->mIsEnabledLoggingToFile;
  }

  ///
  /// @brief Get the path of log file.
  ///
  [[nodiscard]] FORCEINLINE const std::string& GetLogFilePath() const noexcept
  {
    return this->mLogFilePath;
  }

  ///
  /// @brief Get rendering type enum of present rendering api.
  ///
  [[nodiscard]] EDyRenderingApiType GetRenderingType() const noexcept;

  ///
  /// @brief Get overall window width size.
  ///
  [[nodiscard]] FORCEINLINE int32_t GetWindowSizeWidth() const noexcept
  {
    return this->mWindowSizeWidth;
  }

  ///
  /// @brief Get overall window height size.
  ///
  [[nodiscard]] FORCEINLINE int32_t GetWindowSizeHeight() const noexcept
  {
    return this->mWindowSizeHeight;
  }

  ///
  /// @brief Enable or disable logging feature.
  /// before enable logging feature, must set console or file sink.
  ///
  void SetFeatureLogging(bool isEnabled) noexcept;

  ///
  /// @brief Enable or disable log output sink console.
  ///
  void SetSubFeatureLoggingToConsole(bool isEnabled) noexcept;

  ///
  /// @brief Enable or disable log output sink file.
  ///
  void SetSubFeatureLoggingToFile(bool isEnabled) noexcept;

  ///
  /// @brief Set file path for logging. Must be restarted when updated.
  ///
  void SetLogFilePath(const std::string& path) noexcept;

  ///
  /// @brief Set vsync mode.
  /// If vsync mode is off, application will be running by more 60 fps but unstable.
  /// If vsync mode is on, it will be running by your specified fps or 60 fps.
  ///
  void SetVSyncMode(bool enableVsync) noexcept;

  ///
  /// @brief Push arguments when executing program.
  /// This function will not operate after manager initialized.
  ///
  void pArgsPushback(const char* argsString);

private:
  EDyRenderingApiType mRenderingType  = EDyRenderingApiType::NoneError;
  bool mIsRenderingTypeSet            = false;
  bool mIsEnabledVsync                = true;

  bool mIsEnabledLogging              = false;
  bool mIsEnabledLoggingToConsole     = false;
  bool mIsEnabledLoggingToFile        = false;
  std::string mLogFilePath            = "./log.txt";

  std::string mProjectName            = MDY_NOT_INITILAIZED_STR;
  std::string mWindowName             = MDY_NOT_INITILAIZED_STR;

  TI32 mVersionHigh                   = MDY_NOT_INITIALIZED_M1;
  TI32 mVersionMid                    = MDY_NOT_INITIALIZED_M1;
  TI32 mVersionLow                    = MDY_NOT_INITIALIZED_M1;

  std::string mCompanyName            = MDY_NOT_INITILAIZED_STR;
  std::string mHomepage               = MDY_NOT_INITILAIZED_STR;
  std::string mSupportContact         = MDY_NOT_INITILAIZED_STR;

  std::string mInitialSceneName       = MDY_NOT_INITILAIZED_STR;
  TI32 mWindowSizeWidth               = MDY_NOT_INITIALIZED_M1;
  TI32 mWindowSizeHeight              = MDY_NOT_INITIALIZED_M1;

  std::vector<const char*> mApplicationArgs;
  bool mIsInitialized                 = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SETTING_MANAGER_H