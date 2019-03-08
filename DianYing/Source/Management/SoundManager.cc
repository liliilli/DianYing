#include <precompiled.h>
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

/// Header file
#include <Dy/Management/SoundManager.h>

#include <cstdio>
#include <fmod.hpp>

#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/Internal/Sound/FDySoundInstance.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/Type/Sound/FDySoundGroup.h>
#include <Dy/Management/Type/Sound/FDySoundChannel.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(
  sErrorSystemCreationFailed,
  R"dy(Failed to create fmod sound system.)dy");

/// @function DyFmodInternalSystemCallback
/// @brief Internal callback function of fmod library.
FMOD_RESULT F_CALLBACK DyFmodInternalSystemCallback(
    FMOD_SYSTEM* system,
    FMOD_SYSTEM_CALLBACK_TYPE type,
    void* commanddata1,
    void* commanddata2,
    void* userdata)
{
  dy::NotNull<FMOD::System*> sys = dy::DyMakeNotNull(reinterpret_cast<FMOD::System*>(system));

  switch (type)
  {
  case FMOD_SYSTEM_CALLBACK_DEVICELISTCHANGED:
  {
    int numdrivers;

    printf("NOTE : FMOD_SYSTEM_CALLBACK_DEVICELISTCHANGED occurred.\n");
    sys->getNumDrivers(&numdrivers);
    printf("Numdevices = %d\n", numdrivers);
    break;
  }
  case FMOD_SYSTEM_CALLBACK_MEMORYALLOCATIONFAILED:
  {
    printf("ERROR : FMOD_SYSTEM_CALLBACK_MEMORYALLOCATIONFAILED occurred.\n");
#ifdef false
    printf("%s.\n",       (char*)commanddata1);
    printf("%d bytes.\n", *(int*)commanddata2);
#endif
    break;
  }
  case FMOD_SYSTEM_CALLBACK_THREADCREATED:
  {
    printf("NOTE : FMOD_SYSTEM_CALLBACK_THREADCREATED occurred.\n");
#ifdef false
    printf("Thread ID = %d\n",    *(int*)commanddata1);
    printf("Thread Name = %s\n",  (char *)commanddata2);
#endif
    break;
  }
  case FMOD_SYSTEM_CALLBACK_THREADDESTROYED:
  {
    printf("NOTE : FMOD_SYSTEM_CALLBACK_THREADDESTROYED occurred.\n");
#ifdef false
    printf("Thread ID = %d\n",    *(int*)commanddata1);
    printf("Thread Name = %s\n",  (char *)commanddata2);
#endif
    break;
  }
  case FMOD_SYSTEM_CALLBACK_BADDSPCONNECTION:
  {
    FMOD::DSP* source = static_cast<FMOD::DSP *>(commanddata1);
    FMOD::DSP* dest   = static_cast<FMOD::DSP *>(commanddata2);
    printf("ERROR : FMOD_SYSTEM_CALLBACK_BADDSPCONNECTION occurred.\n");

    if (source)
    {
      char name[256]; source->getInfo(name, 0, 0, 0, 0);
      printf("SOURCE = %s\n", name);
    }
    if (dest)
    {
      char name[256]; dest->getInfo(name, 0, 0, 0, 0);
      printf("DEST = %s\n", name);
    }
    break;
  }
#ifdef false
  case FMOD_SYSTEM_CALLBACK_PREMIX:
  {
    printf("NOTE : FMOD_SYSTEM_CALLBACK_PREMIX occurred.\n");
    break;
  }
  case FMOD_SYSTEM_CALLBACK_MIDMIX:
  {
    printf("NOTE : FMOD_SYSTEM_CALLBACK_MIDMIX occurred.\n");
    break;
  }
  case FMOD_SYSTEM_CALLBACK_POSTMIX:
  {
    printf("NOTE : FMOD_SYSTEM_CALLBACK_POSTMIX occurred.\n");
    break;
  }
#endif
  case FMOD_SYSTEM_CALLBACK_ERROR:
  {
    FMOD_ERRORCALLBACK_INFO* info = static_cast<FMOD_ERRORCALLBACK_INFO*>(commanddata1);
    printf("NOTE : FMOD_SYSTEM_CALLBACK_ERROR occurred.\n");
    printf("  ERROR (%d) from %s(%s) with instance %p (type %d)\n", info->result, info->functionname, info->functionparams, info->instance, info->instancetype);
    break;
  }
  default: break;
  }

  return FMOD_OK;
}

} /// unnamed namespace

//!
//! Implementation
//!

namespace dy
{
  
class MDySound::Impl final : public IDyUpdatable
{
public:
  /// @brief Initialize sound system. If already initialized, (without release) just do nothing and return DY_FAILURE.
  EDySuccess InitializeSoundSystem();
  /// @brief Release sound system. If already released, (without initialization) just do nothing and return DY_FAILURE.
  /// This function is only called in release phase of Dy engine.
  EDySuccess ReleaseSoundSystem();

  /// @brief Sound system should be ticked once per game update. 
  void Update(TF32 iDt) override final;

  /// @brief Check sound clip is exist on Dy resource system.
  MDY_NODISCARD bool IsSoundClipExist(const std::string& iSoundSpecifier) const noexcept
  {
    const auto& manager = MDyMetaInfo::GetInstance();
    return manager.IsSoundMetaInfoExist(iSoundSpecifier);
  }

  /// @brief Create a sound directly with no attenuation, perfect for UI Sounds.
  /// If failed to create, just return nullptr.
  std::unique_ptr<FDyInstantSound2D> CreateSound2D(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      TF32 iDelay = 0.0f);
  
  /// @brief Play a sound directly with no attenuation, perfect for UI Sounds.
  /// Fire and forget!
  void PlaySound2D(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      TF32 iDelay = 0.0f);

  /// @brief Play a sound directly with no attenuation, with looped.
  /// Return `TDyBinderSound2D` to control sound2d instance.
  MDY_NODISCARD std::optional<TDyBinderSound2D> PlaySound2DLooped(
      const std::string& iSoundSpecifier,
      const std::string& iSoundChannel,
      const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f);
    
  inline static constexpr TF32 s3DMinDistance = 0.5f;
  inline static constexpr TF32 s3DMaxDistance = 500.f;

  /// @brief Play a sound directly with attenuation. Fire and forget!
  void PlaySound3D(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DDyVector3& iWorldPosition,
      const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      TF32 iDelay = 0.0f,
      TF32 iMinDistance = s3DMinDistance,
      TF32 iMaxDistance = s3DMaxDistance);
  
  /// @brief Play a sound directly with attenuation, with looped.
  /// Return `TDyBinderSound3D` to control sound 3d instance.
  MDY_NODISCARD std::optional<TDyBinderSound3D> PlaySound3DLooped(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DDyVector3& iWorldPosition,
      const DDyClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DDyClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      TF32 iMinDistance = s3DMinDistance,
      TF32 iMaxDistance = s3DMaxDistance);

  /// @brief Get channel pointer instance.
  MDY_NODISCARD FDySoundChannel* GetPtrChannel(const std::string& iSpecifier);

  /// @brief Get reference of group channel which have given `iSpecifier` name.
  MDY_NODISCARD FDySoundGroup& MDY_PRIVATE(GetGroupChannel)(const std::string& iSpecifier);
  /// @brief Get reference of internal sound library entry.
  MDY_NODISCARD FMOD::System& MDY_PRIVATE(GetSystem)();
  /// @brief Create sound instance for `CDySoundSource`.
  MDY_NODISCARD FDySoundInstance* MDY_PRIVATE(CreateSoundInstance)(
      const PDySoundSourceComponentMetaInfo& iMetaInfo,
      FDyActor& iRefActor);

  /// @brief Check sound system is available.
  bool mIsSoundSystemAvailable = true;

private:
  /// @brief Set setting of 3D listener.
  void Set3DListenerSetting(bool iActivated);
  /// @brief Set actor setting.
  void Set3DListenerActorSetting(TU32 iId, CDyCamera& iCamera);

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

  /// @brief Specifies sound system can sound 3d sound because 3d listener activated.
  bool mIsUsing3DListener = true;
};

} /// ::dy namespace
#include <Dy/Management/Inline/MSoundImpl.inl>

//!
//! Proxy
//!

namespace dy
{

EDySuccess MDySound::pfInitialize()
{
  this->mPimpl = new (std::nothrow) Impl();
  const auto flag = this->mPimpl->InitializeSoundSystem();
  if (flag == DY_FAILURE)
  {
    DyPushLogError("Failed to initialize sound system. Could not create sound instance.");
  }

  return DY_SUCCESS;
}

std::unique_ptr<FDyInstantSound2D> MDySound::CreateSound2D(
  const std::string& iSoundSpecifier,
  const std::string& iSoundChannel, 
  const DDyClamp<TF32, 0, 5>& iVolumeMultiplier,
  const DDyClamp<TF32, 0, 5>& iPitchMultiplier, 
  const TF32 iDelay)
{
  return this->mPimpl->CreateSound2D(iSoundSpecifier, iSoundChannel, iVolumeMultiplier, iPitchMultiplier, iDelay);
} 

void MDySound::PlaySound2D(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel,
  const DDyClamp<TF32, 0, 5>& iVolumeMultiplier, 
  const DDyClamp<TF32, 0, 5>& iPitchMultiplier, 
  const TF32 iDelay)
{
  this->mPimpl->PlaySound2D(iSoundSpecifier, iSoundChannel, iVolumeMultiplier, iPitchMultiplier, iDelay);
}

std::optional<TDyBinderSound2D> MDySound::PlaySound2DLooped(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel,
  const DDyClamp<TF32, 0, 5>& iVolumeMultiplier, 
  const DDyClamp<TF32, 0, 5>& iPitchMultiplier)
{
  return this->mPimpl->PlaySound2DLooped(iSoundSpecifier, iSoundChannel, iVolumeMultiplier, iPitchMultiplier);
}

void MDySound::PlaySound3D(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel,
  const DDyVector3& iWorldPosition,
  const DDyClamp<TF32, 0, 5>& iVolumeMultiplier,
  const DDyClamp<TF32, 0, 5>& iPitchMultiplier,
  const TF32 iDelay,
  const TF32 iMinDistance,
  const TF32 iMaxDistance)
{
  return this->mPimpl->PlaySound3D(
    iSoundSpecifier, 
    iSoundChannel, 
    iWorldPosition, 
    iVolumeMultiplier, 
    iPitchMultiplier,
    iDelay,
    iMinDistance,
    iMaxDistance);
}

std::optional<TDyBinderSound3D> MDySound::PlaySound3DLooped(
     const std::string& iSoundSpecifier, 
     const std::string& iSoundChannel,
     const DDyVector3& iWorldPosition,
     const DDyClamp<TF32, 0, 5>& iVolumeMultiplier,
     const DDyClamp<TF32, 0, 5>& iPitchMultiplier,
     const TF32 iMinDistance,
     const TF32 iMaxDistance)
{
  return this->mPimpl->PlaySound3DLooped(
    iSoundSpecifier, iSoundChannel, 
    iWorldPosition, 
    iVolumeMultiplier, iPitchMultiplier,
    iMinDistance, iMaxDistance);
}

bool MDySound::IsSoundClipExist(const std::string& iSoundSpecifier) const noexcept
{
  return this->mPimpl->IsSoundClipExist(iSoundSpecifier);
}

FDySoundChannel* MDySound::GetPtrChannel(const std::string& iSpecifier)
{
  return this->mPimpl->GetPtrChannel(iSpecifier);
}

EDySuccess MDySound::pfRelease()
{
  this->mPimpl->ReleaseSoundSystem();
  delete this->mPimpl;
  return DY_SUCCESS;
}

void MDySound::Update(TF32 iDt) { this->mPimpl->Update(iDt); }

FDySoundGroup& MDySound::MDY_PRIVATE(GetGroupChannel)(const std::string& iSpecifier)
{
  return this->mPimpl->__GetGroupChannel(iSpecifier);
}

FMOD::System& MDySound::MDY_PRIVATE(GetSystem)()
{
  return this->mPimpl->__GetSystem();
}

FDySoundInstance* MDySound::MDY_PRIVATE(CreateSoundInstance)(
     const PDySoundSourceComponentMetaInfo& iMetaInfo,
     FDyActor& iRefActor)
{
  return this->mPimpl->__CreateSoundInstance(iMetaInfo, iRefActor);
}

bool MDySound::IsSoundSystemAvailable() const noexcept
{
  return this->mPimpl->mIsSoundSystemAvailable;
}

} /// ::dy namespace