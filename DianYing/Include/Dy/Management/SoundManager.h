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

  /// @brief Player a sound directly with no attenuation, perfect for UI Sounds.
  void PlaySound2D(
      _MIN_ const std::string& iSoundSpecifier, 
      _MIN_ const std::string& iSoundChannel,
      _MIN_ const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      _MIN_ const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      _MIN_ const TF32 iDelay = 0.0f);

  /// @brief Get channel pointer instance.
  MDY_NODISCARD FDySoundChannel* GetPtrChannel(_MIN_ const std::string& iSpecifier);

  /// @brief Check sound system is available.
  bool mIsSoundSystemAvailable = true;
  
  /// @brief Get reference of group channel which have given `iSpecifier` name.
  FDySoundGroup& MDY_PRIVATE_SPECIFIER(GetGroupChannel)(_MIN_ const std::string& iSpecifier);
  /// @brief Get reference of internal sound library entry.
  MDY_NODISCARD FMOD::System& MDY_PRIVATE_SPECIFIER(GetSystem)();

private:
  /// @brief Initialize sound system. If already initialized, (without release) just do nothing and return DY_FAILURE.
  EDySuccess InitializeSoundSystem();
  /// @brief Release sound system. If already released, (without initialization) just do nothing and return DY_FAILURE.
  /// This function is only called in release phase of Dy engine.
  EDySuccess ReleaseSoundSystem();

#ifdef false
  /// @brief Play sound element
  /// @TODO THIS FUNCTION IS TEMPORARY FUNCTION, SO WHEN IN PRODUCTION CODE HAVE TO PAUSE INDIVIDUAL 'SOUND CUE' INSTNACE AS PARAMETER.
  EDySuccess PlaySoundElement(const std::string& soundName) const noexcept;

  /// @brief Pause sound element
  /// @TODO THIS FUNCTION IS TEMPORARY FUNCTION, SO WHEN IN PRODUCTION CODE HAVE TO PAUSE INDIVIDUAL 'SOUND CUE' INSTNACE AS PARAMETER.
  EDySuccess PauseSoundElement(const std::string& soundName) const noexcept;

  /// @brief Stop sound element
  /// @TODO THIS FUNCTION IS TEMPORARY FUNCTION, SO WHEN IN PRODUCTION CODE HAVE TO PAUSE INDIVIDUAL 'SOUND CUE' INSTNACE AS PARAMETER.
  EDySuccess StopSoundElement(const std::string& soundName) const noexcept;

  /// @brief
  [[nodiscard]]
  EDySuccess pfCreateSoundResource(const std::string& filePath, FMOD::Sound** soundResourcePtr);
#endif

  FMOD::System*       mSoundSystem      = nullptr;
  MDY_TRANSIENT TU32  mVersion          = MDY_INITIALIZE_DEFUINT;
  MDY_TRANSIENT TI32  mSoundDriverCount = MDY_INITIALIZE_DEFINT;

  TStringHashMap<FDySoundGroup>   mGroupContainer;
  TStringHashMap<FDySoundChannel> mChannelContainer;

  /// @brief Instant 2d sound instance list.
  std::forward_list<std::unique_ptr<FDyInstantSound2D>> mInstantSound2DList;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SOUND_MANAGER_H