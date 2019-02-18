#ifndef GUARD_DY_MANAGEMENT_TYPE_SOUND_FDySoundChannel_H
#define GUARD_DY_MANAGEMENT_TYPE_SOUND_FDySoundChannel_H
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

#include <fmod.hpp>
#include <Dy/Management/Type/SettingContainer.h>

namespace dy
{

/// @class FDySoundChannel
/// @brief Sound sub-channel for managing sound sequence and sub-channel (`Dy`'s channel) in sound system.
class FDySoundChannel final
{
public:
  /// @brief Initialize sound sub-channel.
  /// @param iChannelName key for specifying each channel in internal sound system.
  MDY_NODISCARD EDySuccess Initialize(
      _MIN_ FMOD::System& iPtrSystem, 
      _MIN_ const std::string& iChannelName, 
      _MIN_ const DDySettingSound::DChannelDetail& iDetail);
  /// @brief Release sound channel.
  /// Prior to release sound channel, dependent channels must be released.
  MDY_NODISCARD EDySuccess Release();

  /// @brief Set volume.
  void SetVolume(_MIN_ const DDyClamp<TF32, 0, 1>& iVolume);
  /// @brief Set mute flag.
  void SetMute(_MIN_ bool iMuted);

  /// @brief Get the pointer of internal channel group.
  MDY_NODISCARD FMOD::ChannelGroup* MDY_PRIVATE(GetPtrChannel)() noexcept;
  
private:
  std::string         mInternalSpecifier  = "";
  FMOD::ChannelGroup* mInternalGroup      = nullptr;
  bool                mIsInitilaized      = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_FDySoundChannel_H