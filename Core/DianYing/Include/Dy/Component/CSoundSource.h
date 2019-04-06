#ifndef GUARD_DY_COMPONENT_CDyAudioSource_H
#define GUARD_DY_COMPONENT_CDyAudioSource_H
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

#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Meta/Information/PrefabMetaInformation.h>

#ifdef PlaySound
#undef PlaySound
#endif

//!
//! Forward declaration
//!

namespace dy
{
class FSoundInstance;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CSoundSource
/// @brief Sound source component. This can control sound more detail.
class CSoundSource final : public AGeneralBaseComponent, public IInitializeHelper<PDySoundSourceComponentMetaInfo>
{
public:
  CSoundSource(_MIN_ FActor& actorReference) : AGeneralBaseComponent(actorReference) {};
  virtual ~CSoundSource() = default;
  
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CSoundSource);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CSoundSource);
  MDY_SET_CRC32_HASH_WITH_TYPE(CSoundSource);

  /// @brief  Initialize with descriptor.
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDySoundSourceComponentMetaInfo& descriptor) override final;
  /// @brief Release component.
  void Release() override final;

  /// @brief Play sound. If sound is not specified, do nothing but output log.
  void PlaySound();
  /// @brief Pause sound. If sound is not specified, already paused or stopped, do nothing.
  void PauseSound();
  /// @brief Stop sound. If sound is not specified, or stopped, do nothing.
  void StopSound();

  /// @brief Check this sound instance is playable. If resource is attached to internal instance, return true.
  MDY_NODISCARD bool IsPlayable() const noexcept;
  /// @brief Check this sound instance is paused now.
  MDY_NODISCARD bool IsPaused() const noexcept;
  /// @brief Check this sound instance is stopped.
  MDY_NODISCARD bool IsStopped() const noexcept;
  /// @brief Check this sound is playing now.
  MDY_NODISCARD bool IsPlaying() const noexcept;
  /// @brief Check this sound is muted.
  MDY_NODISCARD bool IsMuted() const noexcept;
  /// @brief Check this sound is looped.
  MDY_NODISCARD bool IsLooped() const noexcept;

  /// @brief Set new sound specifier into component. 
  /// If status is not `Stop` and `NotValid`, this function does nothing.
  void SetNewSound(_MIN_ const std::string& iSoundSpecifier);
  /// @brief Get sound specifier.
  MDY_NODISCARD const std::string& GetSound() const noexcept;
  /// @brief Set new channel specifier into component.
  /// If staus is not `Stop` and `NotValid`, this function do nothing.
  void SetNewChannel(_MIN_ const std::string& iChannelSpecifier);
  /// @brief Get channel specifier.
  MDY_NODISCARD const std::string& GetSpecifier() const noexcept;
  /// @brief Set new loop activation.
  /// If sound is playing when this function is called, and new value is false, play sound to end and update value.
  void SetLoop(_MIN_ bool iLooped);
  /// @brief Set mute.
  void SetMute(_MIN_ bool iMuted);

  /// @brief Set volume. Negative volume will be assigned as 0. The value bigger than 5 is assigned as 5.
  void SetVolume(_MIN_ TF32 iVolume);
  /// @brief Get volume.
  MDY_NODISCARD TF32 GetVolume() const noexcept;
  /// @brief Set pitch. Negative pitch will be assigned as 0. The pitch bigger than 5 is assigned as 5.
  void SetPitch(_MIN_ TF32 iPitch);
  /// @brief Get pitch.
  MDY_NODISCARD TF32 GetPitch() const noexcept;

  /// @brief Set 2D activation or not. If disabled, this sound will be a 3D sound.
  void Set2D(_MIN_ bool iActivated);
  /// @brief Check this sound is 2D sound.
  MDY_NODISCARD bool Is2DSound() const noexcept;

  /// @brief Set attenuation flag.
  void SetAttenuation(_MIN_ bool iActivated);
  /// @brief Check this sound is using attenuation anyway (nonetheless of 2D, 3D)
  MDY_NODISCARD bool IsUsingAttenuation() const noexcept;
  /// @brief Set attenuation minumum distance.
  void SetAttenuationMinDistance(_MIN_ TF32 iDistance);
  /// @brief Set attenuation maximum distance.
  void SetAttenuationMaxDistance(_MIN_ TF32 iDistance);
  /// @brief Get attenuation minimum distance.
  MDY_NODISCARD TF32 GetAttenuationMinDistance() const noexcept;
  /// @brief Get attenuation maximum distance.
  MDY_NODISCARD TF32 GetAttenuationMaxDistance() const noexcept;

  /// @brief Update component.
  void Update(_MIN_ TF32 dt) override;

  /// @brief Get information of component.
  std::string ToString() override final;

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  FSoundInstance* mPtrInternalSoundInstance = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDyAudioSource_H