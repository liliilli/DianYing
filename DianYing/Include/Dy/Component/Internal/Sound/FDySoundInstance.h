#ifndef GUARD_DY_COMPONENT_INTENRAL_SOUND_FDySoundInstance_H
#define GUARD_DY_COMPONENT_INTENRAL_SOUND_FDySoundInstance_H
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
#include <Dy/Management/Type/Sound/ADySoundInstance.h>
#include <Dy/Helper/Type/Clamp.h>
#include <Dy/Core/Resource/Type/TDyInformationBinder.h>

#ifdef PlaySound
#undef PlaySound
#endif

//!
//! Forward declaration
//!

namespace dy
{
class FDyActor;
struct PDySoundSourceComponentMetaInfo;
} /// ::dy namespace

//!
//! Implemetation
//!

namespace dy
{

/// @class FDySoundInstance
/// @brief Internal sound instance
class FDySoundInstance final : public ADySoundInstance
{
public:
  FDySoundInstance(
      _MIN_ const PDySoundSourceComponentMetaInfo& iInputInfo, 
      _MIN_ FDyActor& iRefActor);
  virtual ~FDySoundInstance();

  /// @brief Set new sound. If sound instance is playing or paused, sound status will be moved to stop.
  /// If sound is not exist on meta system, do nothing.
  void SetSound(_MIN_ const std::string& iSpecifier);
  /// @brief Set new channel. If sound instance is playing or paused, sound status will be moved to stop.
  /// If channel is not exist on meta system, do nothing.
  void SetChannel(_MIN_ const std::string& iChannelSpecifier);
  /// @brief Set sound setting newly to 2D or 3D.
  /// If sound instance is playing or paused, sound status will be moved to stop.
  void Set2DSound(_MIN_ bool i2DActivated);
  /// @brief Set new loop activation.
  /// If sound is playing when this function is called, and new value is false, play sound to end and update value.
  void SetLoop(_MIN_ bool iLooped);

  /// @brief Update instance.
  void Update(_MIN_ TF32 dt);

  /// @brief Play sound. If sound is not specified, do nothing but output log.
  void PlaySound();
  /// @brief Pause sound. If sound is not specified, already paused or stopped, do nothing.
  void PauseSound();
  /// @brief Stop sound. If sound is not specified, or stopped, do nothing.
  void StopSound();

  /// @brief Get Sound specifier name.
  MDY_NODISCARD const std::string& GetSoundSpecifier() const noexcept { return this->mSoundSpecifier; }
  /// @brief Get Channel specifier name.
  MDY_NODISCARD const std::string& GetChannelSpecifier() const noexcept { return this->mChannelSpecifier; }

  /// @brief Check this sound is muted.
  MDY_NODISCARD bool IsMuted() const noexcept;
  /// @brief Check this sound 
  void SetMute(_MIN_ bool iMuted) noexcept;

  /// @brief Check this sound is looped.
  MDY_NODISCARD bool IsLooped() const noexcept { return this->mLooped; }
  /// @brief CHeck this sound is 2D sound or 3D.
  MDY_NODISCARD bool Is2DSound() const noexcept { return this->m2DSound; }

private:
  /// @brief Try initialize sound resource, 
  /// Succeeded if only sound resource is bound and status is `NotValid` yet.
  EDySuccess TryInitialize();

  /// @brief Update internal atteunation property. 
  /// mPtrInternalSound and mPtrInternalChannel must be valid, when call this.
  void UpdateInternalAttenuationProperty();
  /// @brief Update internal world-relative position & velocity from actor.
  /// This function only process logic when 
  void UpdateInternal3DPositionVelocity();
  /// @brief Update internal mute function.
  void UpdateInternalMute();
  /// @brief Update internal loop function.
  void UpdateInternalLoop();

  /// @brief 
  struct DAttenuation final
  {
    bool mActivated = false;
    DDyClamp<TF32, 0, 1'000'000> mNearDistance  = 0.5f;
    DDyClamp<TF32, 0, 1'000'000> mFarDistance   = 5000.f;
  };

  std::string mSoundSpecifier; // Sound specifier.

  bool m2DSound = false; // Check this component is 2d sound or 3d.
  bool mMuted   = false; // Check this component is muted.
  bool mLooped  = false; // Check this component's sound will be played looped.

  DDyClamp<TF32, 0, 5> mVolumeMultiplier  = 1.0f; // Sound channel's volume multiplication.
  DDyClamp<TF32, 0, 5> mPitchMultiplier   = 1.0f; // Sound channel's pitch multiplication.
  std::string mChannelSpecifier; // If blank, this sound just use master channel.

  DAttenuation mAttenuation; // 3D Atteunation properties.
  TDyLInformationBinderSound mBinderClipResource;
  
  FMOD::Sound*    mPtrInternalSound   = nullptr;
  FMOD::Channel*  mPtrInternalChannel = nullptr;
  NotNull<FDyActor*> mPtrActor;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTENRAL_SOUND_FDySoundInstance_H