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
#include "Dy/Helper/Type/Vector2.h"

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
  MDY_NODISCARD bool IsVSyncEnabled() const noexcept;

  ///
  /// @brief Get enable/disable flag of feature, logging as true/false.
  ///
  MDY_NODISCARD FORCEINLINE bool IsEnabledFeatureLogging() const noexcept
  {
    return this->mIsEnabledLogging;
  }

  ///
  /// @brief Get enable/disable flag of subfeature, logging to console as true/false.
  /// If changed, logging feature must be restarted manually.
  ///
  MDY_NODISCARD FORCEINLINE bool IsEnabledSubFeatureLoggingToConsole() const noexcept
  {
    return this->mIsEnabledLoggingToConsole;
  }

  ///
  /// @brief Get enable/disable flag of subfeature, logging to file as true/false.
  /// If changed, logging feature must be restarted manually.
  ///
  MDY_NODISCARD FORCEINLINE bool IsEnableSubFeatureLoggingToFile() const noexcept
  {
    return this->mIsEnabledLoggingToFile;
  }

  ///
  /// @brief Get the path of log file.
  ///
  MDY_NODISCARD FORCEINLINE const std::string& GetLogFilePath() const noexcept
  {
    return this->mLogFilePath;
  }

  ///
  /// @brief Get rendering type enum of present rendering api.
  ///
  MDY_NODISCARD EDyRenderingApiType GetRenderingType() const noexcept;

  ///
  /// @brief Get overall window width size.
  ///
  MDY_NODISCARD FORCEINLINE int32_t GetWindowSizeWidth() const noexcept
  {
    return this->mWindowSizeWidth;
  }

  ///
  /// @brief Get overall window height size.
  ///
  MDY_NODISCARD FORCEINLINE int32_t GetWindowSizeHeight() const noexcept
  {
    return this->mWindowSizeHeight;
  }

  ///
  /// @brief  Get initial scene name to create initial scene instance.
  /// @return PDyLevelConstructDescriptor name. Name is same to populated actual scene instance.
  ///
  MDY_NODISCARD FORCEINLINE const std::string& GetInitialSceneInformationName() const noexcept
  {
    return this->mInitialSceneName;
  }

  ///
  /// @brief  Get global default shadow map resolution as DDyVector2
  /// @return DDyVector2 size of default shadow map resolution.
  ///
  FORCEINLINE MDY_NODISCARD const DDyVector2& GetGlobalDefaultShadowMapResolution() const noexcept
  {
    return this->mShadowGlobalDefaultMap;
  }

  ///
  /// @brief Enable or disable logging feature.
  /// before enable logging feature, must set console or file sink.
  /// @param isEnabled
  ///
  void SetFeatureLogging(_MIN_ bool isEnabled) noexcept;

  ///
  /// @brief Enable or disable log output sink console.
  /// @param isEnabled
  ///
  void SetSubFeatureLoggingToConsole(_MIN_ bool isEnabled) noexcept;

  ///
  /// @brief Enable or disable log output sink file.
  /// @param isEnabled
  ///
  void SetSubFeatureLoggingToFile(_MIN_ bool isEnabled) noexcept;

  ///
  /// @brief Set file path for logging. Must be restarted when updated.
  /// @param path
  ///
  void SetLogFilePath(_MIN_ const std::string& path) noexcept;

  ///
  /// @brief Set global default shadow map resolution.
  /// @param size
  ///
  void SetGlobalDefaultShadowMapResolution(_MIN_ const DDyVector2& size) noexcept;

  ///
  /// @brief Set vsync mode.
  /// If vsync mode is off, application will be running by more 60 fps but unstable.
  /// If vsync mode is on, it will be running by your specified fps or 60 fps.
  /// @param enableVsync
  ///
  void SetVSyncMode(_MIN_ bool enableVsync) noexcept;

  ///
  ///
  ///

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

  std::string mProjectName            = MDY_INITILAIZE_EMPTYSTR;
  std::string mWindowName             = MDY_INITILAIZE_EMPTYSTR;

  TI32 mVersionHigh                   = MDY_INITIALIZE_DEFINT;
  TI32 mVersionMid                    = MDY_INITIALIZE_DEFINT;
  TI32 mVersionLow                    = MDY_INITIALIZE_DEFINT;

  std::string mCompanyName            = MDY_INITILAIZE_EMPTYSTR;
  std::string mHomepage               = MDY_INITILAIZE_EMPTYSTR;
  std::string mSupportContact         = MDY_INITILAIZE_EMPTYSTR;

  std::string mInitialSceneName       = MDY_INITILAIZE_EMPTYSTR;
  TI32 mWindowSizeWidth               = MDY_INITIALIZE_DEFINT;
  TI32 mWindowSizeHeight              = MDY_INITIALIZE_DEFINT;

  /// Global default shadow map size.
  DDyVector2 mShadowGlobalDefaultMap  = {};

  std::vector<const char*> mApplicationArgs;
  bool mIsInitialized                 = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SETTING_MANAGER_H