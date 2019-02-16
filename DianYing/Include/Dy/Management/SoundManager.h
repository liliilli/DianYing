#ifndef GUARD_DY_MANAGEMENT_SOUND_MANAGER_H
#define GUARD_DY_MANAGEMENT_SOUND_MANAGER_H
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
#include <fmod.hpp>
#include <Dy/Element/Interface/IDyUpdatable.h>
#include <Dy/Management/Type/Sound/FDySoundGroup.h>
#include <Dy/Management/Type/Sound/FDySoundChannel.h>
#include <Dy/Management/Type/Sound/FDyInstantSound2D.h>
#include <Dy/Management/Type/Sound/FDyInstantSound3D.h>
#include <Dy/Management/Type/Sound/TDyBinderSound2D.h>
#include <Dy/Management/Type/Sound/TDyBinderSound3D.h>
#include <Dy/Component/Internal/Sound/FDySoundInstance.h>

//!
//! Forward declaration
//!

namespace dy
{
  class CDyCamera;
  struct PDySoundSourceComponentMetaInfo;
class FDyActor;
class CDyTransform;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class MDySound
/// @brief Sound management class
///
class MDySound final : public IDySingleton<MDySound>, public IDyUpdatable
{
  MDY_SINGLETON_PROPERTIES(MDySound);
  MDY_SINGLETON_DERIVED(MDySound);
public:
  /// @brief Sound system should be ticked once per game update. 
  void Update(float dt) override final;

  /// @brief Check sound clip is exist on Dy resource system.
  MDY_NODISCARD bool IsSoundClipExist(_MIN_ const std::string& iSoundSpecifier) const noexcept;

  /// @brief Create a sound directly with no attenuation, perfect for UI Sounds.
  /// If failed to create, just return nullptr.
  std::unique_ptr<FDyInstantSound2D> CreateSound2D(
      _MIN_ const std::string& iSoundSpecifier, 
      _MIN_ const std::string& iSoundChannel,
      _MIN_ const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      _MIN_ const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      _MIN_ const TF32 iDelay = 0.0f);
  
  /// @brief Play a sound directly with no attenuation, perfect for UI Sounds.
  /// Fire and forget!
  void PlaySound2D(
      _MIN_ const std::string& iSoundSpecifier, 
      _MIN_ const std::string& iSoundChannel,
      _MIN_ const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      _MIN_ const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      _MIN_ const TF32 iDelay = 0.0f);

  /// @brief Play a sound directly with no attenuation, with looped.
  /// Return `TDyBinderSound2D` to control sound2d instance.
  MDY_NODISCARD std::optional<TDyBinderSound2D> PlaySound2DLooped(
      _MIN_ const std::string& iSoundSpecifier,
      _MIN_ const std::string& iSoundChannel,
      _MIN_ const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      _MIN_ const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f);
    
  inline static constexpr TF32 s3DMinDistance = 0.5f;
  inline static constexpr TF32 s3DMaxDistance = 500.f;

  /// @brief Play a sound directly with attenuation. Fire and forget!
  void PlaySound3D(
      _MIN_ const std::string& iSoundSpecifier, 
      _MIN_ const std::string& iSoundChannel,
      _MIN_ const DDyVector3& iWorldPosition,
      _MIN_ const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      _MIN_ const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      _MIN_ const TF32 iDelay = 0.0f,
      _MIN_ const TF32 iMinDistance = s3DMinDistance,
      _MIN_ const TF32 iMaxDistance = s3DMaxDistance);
  
  /// @brief Play a sound directly with attenuation, with looped.
  /// Return `TDyBinderSound3D` to control sound 3d instance.
  MDY_NODISCARD std::optional<TDyBinderSound3D> PlaySound3DLooped(
      _MIN_ const std::string& iSoundSpecifier, 
      _MIN_ const std::string& iSoundChannel,
      _MIN_ const DDyVector3& iWorldPosition,
      _MIN_ const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      _MIN_ const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      _MIN_ const TF32 iMinDistance = s3DMinDistance,
      _MIN_ const TF32 iMaxDistance = s3DMaxDistance);

  /// @brief Get channel pointer instance.
  MDY_NODISCARD FDySoundChannel* GetPtrChannel(_MIN_ const std::string& iSpecifier);

  /// @brief Check sound system is available.
  bool mIsSoundSystemAvailable = true;
  
  /// @brief Get reference of group channel which have given `iSpecifier` name.
  MDY_NODISCARD FDySoundGroup& MDY_PRIVATE(GetGroupChannel)(_MIN_ const std::string& iSpecifier);
  /// @brief Get reference of internal sound library entry.
  MDY_NODISCARD FMOD::System& MDY_PRIVATE(GetSystem)();
  /// @brief Create sound instance for `CDySoundSource`.
  MDY_NODISCARD FDySoundInstance* MDY_PRIVATE(CreateSoundInstance)(
      _MIN_ const PDySoundSourceComponentMetaInfo& iMetaInfo,
      _MIN_ FDyActor& iRefActor);

private:
  /// @brief Initialize sound system. If already initialized, (without release) just do nothing and return DY_FAILURE.
  EDySuccess InitializeSoundSystem();
  /// @brief Release sound system. If already released, (without initialization) just do nothing and return DY_FAILURE.
  /// This function is only called in release phase of Dy engine.
  EDySuccess ReleaseSoundSystem();

  /// @brief Set setting of 3D listener.
  void Set3DListenerSetting(_MIN_ bool iActivated);
  /// @brief Set actor setting.
  void Set3DListenerActorSetting(_MIN_ TU32 iId, _MIN_ CDyCamera& iTransform);

  FMOD::System*       mSoundSystem      = nullptr;
  MDY_TRANSIENT TU32  mVersion          = MDY_INITIALIZE_DEFUINT;
  MDY_TRANSIENT TI32  mSoundDriverCount = MDY_INITIALIZE_DEFINT;

  TStringHashMap<FDySoundGroup>   mGroupContainer;
  TStringHashMap<FDySoundChannel> mChannelContainer;

  /// @brief Instant 2d sound instance list.
  std::forward_list<std::unique_ptr<FDyInstantSound2D>> mInstantSound2DList;
  /// @brief Instant 3d sound instance list.
  std::forward_list<std::unique_ptr<FDyInstantSound3D>> mInstantSound3DList;
  /// @brief General (CDySoundSource) sound instance list.
  std::vector<std::unique_ptr<FDySoundInstance>> mGeneralSoundInstanceList;

  /// @brief 
  bool mIsUsing3DListener = true;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SOUND_MANAGER_H