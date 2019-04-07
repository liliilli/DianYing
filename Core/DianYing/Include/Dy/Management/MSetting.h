#ifndef GUARD_DY_MANAGEMENT_SETTING_MANAGER_H
#define GUARD_DY_MANAGEMENT_SETTING_MANAGER_H
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
/// @todo IMPLEMENT SET OVERALL WINDOW WIDTH, HEIGHT AS CHANGING VIEWPORT OF EACH API FRAMEBUFFER.
///

#include <Dy/Core/EDyAppMode.h>
#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Meta/Type/EDyRenderingApi.h>
#include <Dy/Management/Type/SettingContainer.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/Render/ERenderingModelMode.h>

namespace dy
{

/// @class MSetting
/// @brief manages global settings of DianYing renderer application.
class MSetting final : public ISingleton<MSetting>
{
  MDY_SINGLETON_DERIVED(MSetting);
  MDY_SINGLETON_PROPERTIES(MSetting);
public:
  /// @brief Check if rendering vsync is enabled. \n
  /// If Vsync enabled, max frame per seconds are fixed by 60 fps. \n
  /// If not, frame will be beyond 60 fps along your hardware but jittering may be occured sometimes.
  MDY_NODISCARD bool IsEnabledVSync() const noexcept;

  /// @brief Get enable/disable flag of feature, logging as true/false.
  MDY_NODISCARD bool IsEnabledFeatureLogging() const noexcept;
  /// @brief Get enable/disable flag of subfeature, logging to console as true/false. \n
  /// If changed, logging feature must be restarted manually.
  MDY_NODISCARD bool IsEnabledSubFeatureLoggingToConsole() const noexcept;
  /// @brief Get enable/disable flag of subfeature, logging to file as true/false. \n
  /// If changed, logging feature must be restarted manually.
  MDY_NODISCARD bool IsEnableSubFeatureLoggingToFile() const noexcept;
  /// @brief Get the path of log file.
  MDY_NODISCARD const std::string& GetLogFilePath() const noexcept;

  /// @brief Get application mode.
  MDY_NODISCARD EDyAppMode GetApplicationMode() const noexcept;
  /// @brief Get rendering type enum of present rendering api.
  MDY_NODISCARD EDyRenderingApi GetRenderingType() const noexcept;
  /// @brief Set default shadow option if iFlag is same to previous value, do nothing.
  void SetDefaultShadowOption(bool iFlag) noexcept;
  /// @brief Get flag of `default shadow option`.
  MDY_NODISCARD bool IsDefaultShadowOptionActivated() const noexcept;
  /// @brief Set default ssao (screen space ambient occlusion) option if iFlag is same to previous value, do nothing.
  void SetDefaultSsaoOption(bool iFlag) noexcept;
  /// @brief Get flag of `default ssao option`.
  MDY_NODISCARD bool IsDefaultSsaoOptionActivated() const noexcept;

  /// @brief Check app is debug mode.
  MDY_NODISCARD bool IsDebugMode() const noexcept;

  /// @brief Get overall window width size.
  MDY_NODISCARD TI32 GetWindowSizeWidth() const noexcept;
  /// @brief Get overall window height size.
  MDY_NODISCARD TI32 GetWindowSizeHeight() const noexcept;

  /// @brief  Get initial scene name to create initial scene instance.
  /// @return PDyLevelConstructDescriptor name. Name is same to populated actual scene instance.
  MDY_NODISCARD const std::string& GetInitialSceneInformationName() const noexcept;

  /// @brief  Get global default shadow map resolution as DVec2
  /// @return DVec2 size of default shadow map resolution.
  MDY_NODISCARD const DIVec2& GetGlobalDefaultShadowMapResolution() const noexcept;

  /// @brief Get input setting information, not modifiable.
  MDY_NODISCARD const DDySettingInput& GetInputSettingInformation() const noexcept;

  /// @brief Get gameplay (graphics etc) setting information, not modifiable.
  MDY_NODISCARD const DDySettingGameplay& GetGameplaySettingInformation() const noexcept;

  /// @brief Get meta file path setting information, not modifiable.
  MDY_NODISCARD const DDySettingMetaPath& GetMetaPathSettingInformation() const noexcept;

  /// @brief Enable or disable logging feature.
  /// before enable logging feature, must set console or file sink.
  void SetFeatureLogging(bool isEnabled) noexcept;

  /// @brief Enable or disable log output sink console.
  void SetSubFeatureLoggingToConsole(bool isEnabled) noexcept;

  /// @brief Enable or disable log output sink file.
  void SetSubFeatureLoggingToFile(bool isEnabled) noexcept;

  /// @brief Set file path for logging. Must be restarted when updated.
  void SetLogFilePath(const std::string& path) noexcept;

  /// @brief Set global default shadow map resolution.
  void SetGlobalDefaultShadowMapResolution(const DVec2& size) noexcept;

  /// @brief Set vsync mode.
  /// If vsync mode is off, application will be running by more 60 fps but unstable. \n
  /// If vsync mode is on, it will be running by your specified fps or 60 fps. 
  /// @param enableVsync
  void SetVSyncMode(bool enableVsync) noexcept;

  /// @TODO TEMPORAL FUNCTION
  MDY_NODISCARD const DDySettingTag& tempGetTagList() const noexcept { return this->mTag; }

  /// @brief Check Specified tag is in object tag list.
  MDY_NODISCARD EDySuccess 
  MDY_PRIVATE(CheckObjectTagIsExist)(const std::string& iSpecifiedTag) const noexcept;

  /// @brief Get entry setting file path.
  MDY_NODISCARD const std::string& 
  MDY_PRIVATE(GetEntrySettingFile)() const noexcept;

  //!
  //! Sound
  //!

  /// @brief Get const invariable sound setting instance.
  MDY_NODISCARD const DDySettingSound& GetSoundSetting() const noexcept; 

  //!
  //! Physics
  //!

  /// @brief Get const invariant physics setting instance.
  MDY_NODISCARD const DDySettingPhysics& GetPhysicsSetting() const noexcept;

  //!
  //! Rendering
  //!
  
  /// @brief Set rendering mode.
  void SetRenderingMode(ERenderingModelMode iNewMode) noexcept; 
  /// @brief Get rendering mode of model.
  MDY_NODISCARD ERenderingModelMode GetRenderingMode() const noexcept;

  /// @brief Set rendering physics collision shape mode.
  void SetRenderingPhysicsCollisionShape(bool iIsEnabled) noexcept;
  /// @brief Check `Physics Collision Shape` rendering is enabled.
  MDY_NODISCARD bool IsRenderPhysicsCollisionShape() const noexcept;
  /// @brief Set rendering physcis collision AABB mode.
  void SetRenderingPhysicsCollisionAABB(bool iIsEnabled) noexcept;
  /// @brief Check `Physics Collision AABB` rendering is enabled.
  MDY_NODISCARD bool IsRenderPhysicsCollisionAABB() const noexcept;

private:
  /// @brief Setup executable argument settings.
  /// This function must be called before initialization.
  void pSetupExecutableArgumentSettings();

  EDyAppMode mApplicationMode         = EDyAppMode::ModeRuntime;
  EDyFileLoadingMode mFileLoadingMode = EDyFileLoadingMode::LoadCompressedFile;
  EDyRenderingApi mRenderingType      = EDyRenderingApi::NoneError;
  bool mIsEnabledLogging              = false;
  bool mIsEnabledLoggingToConsole     = false;
  bool mIsEnabledLoggingToFile        = false;
  std::string mLogFilePath            = "./log.txt";
  std::string mEntrySettingPath       = "./data/Data000.dydat";

  //!
  //! Serializable setting options.
  //!

  DDySettingDescription mDescription  = {};
  DDySettingGameplay    mGamePlay     = {};
  DDySettingInput       mInput        = {};
  DDySettingTag         mTag          = {};
  DDySettingSound       mSound        = {};
  /// @brief ONLY USED WHEN `MDY_FLAG_LOAD_COMPRESSED_DATAFILE` IS NOT DEFINED.
  DDySettingMetaPath    mDevMetaPath  = {};
  DDySettingPhysics     mPhysics      = {};

  bool mIsEnabledVsync  = true;
  bool mIsInitialized   = false;

  /// If -d setup, debug mode will be setup.
  /// Specified keyboard key will be setup (F1 ~ F12) and override given game runtime key. 
  /// and, imgui will also be initiated to see informations.
  /// when -d set up, all resources will be loaded like -r flag. and saved as saparated files
  bool mIsDebugMode     = false;

  /// @brief Set model rendering mode.
  ERenderingModelMode mModelRenderingMode = ERenderingModelMode::FillNormal;
  /// @brief If true, rendering manager should draw physics collision shape.
  bool mIsRenderPhysicsCollisionShape = false;
  /// @brief If true, rendering manager should draw physics collision AABB as fixed color.
  bool mIsRenderPhysicsCollisionAABB = false;

  friend class GEngine;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SETTING_MANAGER_H