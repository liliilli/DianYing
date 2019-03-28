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
#include <Dy/Element/Interface/IUpdatable.h>
#include <Dy/Helper/Type/DClamp.h>
#include <Dy/Management/Type/Sound/FInstantSound2D.h>
#include <Dy/Management/Type/Sound/FInstantSound3D.h>
#include <Dy/Management/Type/Sound/FSound2DBinder.h>
#include <Dy/Management/Type/Sound/FSound3DBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class CCamera;
struct PDySoundSourceComponentMetaInfo;
class FActor;
class CTransform;
class FSoundChannel;
class FSoundGroup;
class FSoundInstance;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class MSound
/// @brief Sound management class
class MSound final : public ISingleton<MSound>, public IUpdatable
{
  MDY_SINGLETON_PROPERTIES(MSound);
  MDY_SINGLETON_DERIVED(MSound);
public:
  /// @brief Sound system should be ticked once per game update. 
  void Update(float dt) override final;

  /// @brief Check sound clip is exist on Dy resource system.
  MDY_NODISCARD bool IsSoundClipExist(const std::string& iSoundSpecifier) const noexcept;

  /// @brief Create a sound directly with no attenuation, perfect for UI Sounds.
  /// If failed to create, just return nullptr.
  std::unique_ptr<FInstantSound2D> CreateSound2D(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      const TF32 iDelay = 0.0f);
  
  /// @brief Play a sound directly with no attenuation, perfect for UI Sounds.
  /// Fire and forget!
  void PlaySound2D(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      const TF32 iDelay = 0.0f);

  /// @brief Play a sound directly with no attenuation, with looped.
  /// Return `FSound2DBinder` to control sound2d instance.
  MDY_NODISCARD std::optional<FSound2DBinder> PlaySound2DLooped(
      const std::string& iSoundSpecifier,
      const std::string& iSoundChannel,
      const DClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f);
    
  inline static constexpr TF32 s3DMinDistance = 0.5f;
  inline static constexpr TF32 s3DMaxDistance = 500.f;

  /// @brief Play a sound directly with attenuation. Fire and forget!
  void PlaySound3D(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DVec3& iWorldPosition,
      const DClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      const TF32 iDelay = 0.0f,
      const TF32 iMinDistance = s3DMinDistance,
      const TF32 iMaxDistance = s3DMaxDistance);
  
  /// @brief Play a sound directly with attenuation, with looped.
  /// Return `FSound3DBinder` to control sound 3d instance.
  MDY_NODISCARD std::optional<FSound3DBinder> PlaySound3DLooped(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DVec3& iWorldPosition,
      const DClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      const TF32 iMinDistance = s3DMinDistance,
      const TF32 iMaxDistance = s3DMaxDistance);

  /// @brief Get channel pointer instance.
  MDY_NODISCARD FSoundChannel* GetPtrChannel(const std::string& iSpecifier);
 
  /// @brief Get reference of group channel which have given `iSpecifier` name.
  MDY_NODISCARD FSoundGroup&    MDY_PRIVATE(GetGroupChannel)(const std::string& iSpecifier);
  /// @brief Get reference of internal sound library entry.
  MDY_NODISCARD FMOD::System&     MDY_PRIVATE(GetSystem)();
  /// @brief Create sound instance for `CSoundSource`.
  MDY_NODISCARD FSoundInstance* MDY_PRIVATE(CreateSoundInstance)(
      const PDySoundSourceComponentMetaInfo& iMetaInfo,
      FActor& iRefActor);

  /// @brief Check sound system is available.
  MDY_NODISCARD bool IsSoundSystemAvailable() const noexcept;

private:
  class Impl; Impl* mPimpl = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SOUND_MANAGER_H