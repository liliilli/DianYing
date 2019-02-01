#ifndef GUARD_DY_MANAGEMENT_TYPE_SOUND_FDySoundGroup_H
#define GUARD_DY_MANAGEMENT_TYPE_SOUND_FDySoundGroup_H
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

/// @class FDySoundGroup
/// @brief Sound group channel for managing sound sequence and sub-channel (`Dy`'s channel) in sound system.
class FDySoundGroup final
{
public:
  /// @brief Initialize sound group.
  /// @param iGroupName key for specifying each channel in internal sound system.
  MDY_NODISCARD EDySuccess Initialize(
      _MIN_ FMOD::System& iPtrSystem, 
      _MIN_ const std::string& iGroupName, 
      _MIN_ const DDySettingSound::DDetail& iDetail);
  /// @brief Release sound group.
  /// Prior to release sound group, dependent channels must be released.
  MDY_NODISCARD EDySuccess Release();

  /// @brief Set volume.
  void SetVolume(_MIN_ const DDyClamp<TF32, 0, 1>& iVolume);
  /// @brief Set mute flag.
  void SetMute(_MIN_ bool iMuted);
  
private:
  std::string         mInternalSpecifier = "";
  FMOD::ChannelGroup* mInternalGroup = nullptr;
  bool mIsInitilaized = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_FDySoundGroup_H