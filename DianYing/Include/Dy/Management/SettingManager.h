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
/// @log
/// 2018-08-21 Create file.
///

#include <Dy/Management/Interface/ISingletonCrtp.h>

namespace dy
{

///
/// @brief
///
enum class ERenderingType
{
  None,
  DirectX11,
  DirectX12,
  OpenGL,
  Vulkan
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
  /// @brief
  ///
  [[nodiscard]]
  bool IsVSyncEnabled() const noexcept;

  ///
  /// @brief
  ///
  [[nodiscard]]
  FORCEINLINE bool IsEnabledFeatureLogging() const noexcept
  {
    return this->mIsEnabledLogging;
  }

  ///
  /// @brief
  ///
  [[nodiscard]]
  FORCEINLINE bool IsEnabledSubFeatureLoggingToConsole() const noexcept
  {
    return this->mIsEnabledLoggingToConsole;
  }

  ///
  /// @brief
  ///
  [[nodiscard]]
  FORCEINLINE bool IsEnableSubFeatureLoggingToFile() const noexcept
  {
    return this->mIsEnabledLoggingToFile;
  }

  ///
  /// @brief
  ///
  [[nodiscard]]
  ERenderingType GetRenderingType() const noexcept;

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
  /// @brief Set vsync mode.
  /// If vsync mode is off, application will be running by more 60 fps but unstable.
  /// If vsync mode is on, it will be running by your specified fps or 60 fps.
  ///
  void SetVSyncMode(bool enableVsync) noexcept;



  ///
  /// @brief
  ///
  void ArgsPushback(const char* argsString);

private:
  ERenderingType mRenderingType   = ERenderingType::None;
  bool mIsRenderingTypeSet        = false;
  bool mIsEnabledVsync            = true;

  bool mIsEnabledLogging          = false;
  bool mIsEnabledLoggingToConsole = false;
  bool mIsEnabledLoggingToFile    = false;

  std::vector<const char*> mApplicationArgs;
  bool mIsInitialized           = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SETTING_MANAGER_H