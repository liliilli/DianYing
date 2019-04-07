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
#include <Dy/Management/MSound.h>

#include <cstdio>
#include <fmod.hpp>

#include <Dy/Component/CCamera.h>
#include <Dy/Component/Internal/Sound/FSoundInstance.h>
#include <Dy/Helper/System/Pointer.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/Type/Sound/FSoundGroup.h>
#include <Dy/Management/Type/Sound/FSoundChannel.h>

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

struct MSound::Impl final
{
  DY_DEFAULT_PIMPL_CTOR(MSound);

  /// @brief Initialize sound system. If already initialized, (without release) just do nothing and return EDySuccess::DY_FAILURE.
  EDySuccess InitializeSoundSystem();
  /// @brief Release sound system. If already released, (without initialization) just do nothing and return EDySuccess::DY_FAILURE.
  /// This function is only called in release phase of Dy engine.
  EDySuccess ReleaseSoundSystem();

  /// @brief Check sound clip is exist on Dy resource system.
  MDY_NODISCARD bool IsSoundClipExist(const std::string& iSoundSpecifier) const noexcept
  {
    const auto& manager = MIOMeta::GetInstance();
    return manager.IsSoundMetaInfoExist(iSoundSpecifier);
  }

  /// @brief Create a sound directly with no attenuation, perfect for UI Sounds.
  /// If failed to create, just return nullptr.
  std::unique_ptr<FInstantSound2D> CreateSound2D(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      TF32 iDelay = 0.0f);
  
  /// @brief Play a sound directly with no attenuation, perfect for UI Sounds.
  /// Fire and forget!
  void PlaySound2D(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      TF32 iDelay = 0.0f);

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
      TF32 iDelay = 0.0f,
      TF32 iMinDistance = s3DMinDistance,
      TF32 iMaxDistance = s3DMaxDistance);
  
  /// @brief Play a sound directly with attenuation, with looped.
  /// Return `FSound3DBinder` to control sound 3d instance.
  MDY_NODISCARD std::optional<FSound3DBinder> PlaySound3DLooped(
      const std::string& iSoundSpecifier, 
      const std::string& iSoundChannel,
      const DVec3& iWorldPosition,
      const DClamp<TF32, 0, 5>& iVolumeMultiplier = 1.0f,
      const DClamp<TF32, 0, 5>& iPitchMultiplier = 1.0f,
      TF32 iMinDistance = s3DMinDistance,
      TF32 iMaxDistance = s3DMaxDistance);

  /// @brief Get channel pointer instance.
  MDY_NODISCARD FSoundChannel* GetPtrChannel(const std::string& iSpecifier);

  /// @brief Get reference of group channel which have given `iSpecifier` name.
  MDY_NODISCARD FSoundGroup& MDY_PRIVATE(GetGroupChannel)(const std::string& iSpecifier);
  /// @brief Get reference of internal sound library entry.
  MDY_NODISCARD FMOD::System& MDY_PRIVATE(GetSystem)();
  /// @brief Create sound instance for `CSoundSource`.
  MDY_NODISCARD FSoundInstance* MDY_PRIVATE(CreateSoundInstance)(
      const PDySoundSourceComponentMetaInfo& iMetaInfo,
      FActor& iRefActor);

  /// @brief Check sound system is available.
  bool mIsSoundSystemAvailable = true;

  /// @brief Set setting of 3D listener.
  void Set3DListenerSetting(bool iActivated);
  /// @brief Set actor setting.
  void Set3DListenerActorSetting(TU32 iId, CCamera& iCamera);

  FMOD::System*       mSoundSystem      = nullptr;
  MDY_TRANSIENT TU32  mVersion          = MDY_INITIALIZE_DEFUINT;
  MDY_TRANSIENT TI32  mSoundDriverCount = MDY_INITIALIZE_DEFINT;

  TStringHashMap<FSoundGroup>   mGroupContainer;
  TStringHashMap<FSoundChannel> mChannelContainer;

  /// @brief Instant 2d sound instance list.
  std::forward_list<std::unique_ptr<FInstantSound2D>> mInstantSound2DList;
  /// @brief Instant 3d sound instance list.
  std::forward_list<std::unique_ptr<FInstantSound3D>> mInstantSound3DList;
  /// @brief General (CSoundSource) sound instance list.
  std::vector<std::unique_ptr<FSoundInstance>> mGeneralSoundInstanceList;

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

EDySuccess MSound::pfInitialize()
{
  DY_INITIALIZE_PIMPL();
  const auto flag = DY_PIMPL->InitializeSoundSystem();
  if (flag == EDySuccess::DY_FAILURE)
  {
    DyPushLogError("Failed to initialize sound system. Could not create sound instance.");
  }

  return EDySuccess::DY_SUCCESS;
}

std::unique_ptr<FInstantSound2D> MSound::CreateSound2D(
  const std::string& iSoundSpecifier,
  const std::string& iSoundChannel, 
  const DClamp<TF32, 0, 5>& iVolumeMultiplier,
  const DClamp<TF32, 0, 5>& iPitchMultiplier, 
  const TF32 iDelay)
{
  return DY_PIMPL->CreateSound2D(iSoundSpecifier, iSoundChannel, iVolumeMultiplier, iPitchMultiplier, iDelay);
} 

void MSound::PlaySound2D(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel,
  const DClamp<TF32, 0, 5>& iVolumeMultiplier, 
  const DClamp<TF32, 0, 5>& iPitchMultiplier, 
  const TF32 iDelay)
{
  DY_PIMPL->PlaySound2D(iSoundSpecifier, iSoundChannel, iVolumeMultiplier, iPitchMultiplier, iDelay);
}

std::optional<FSound2DBinder> MSound::PlaySound2DLooped(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel,
  const DClamp<TF32, 0, 5>& iVolumeMultiplier, 
  const DClamp<TF32, 0, 5>& iPitchMultiplier)
{
  return DY_PIMPL->PlaySound2DLooped(iSoundSpecifier, iSoundChannel, iVolumeMultiplier, iPitchMultiplier);
}

void MSound::PlaySound3D(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel,
  const DVec3& iWorldPosition,
  const DClamp<TF32, 0, 5>& iVolumeMultiplier,
  const DClamp<TF32, 0, 5>& iPitchMultiplier,
  const TF32 iDelay,
  const TF32 iMinDistance,
  const TF32 iMaxDistance)
{
  return DY_PIMPL->PlaySound3D(
    iSoundSpecifier, 
    iSoundChannel, 
    iWorldPosition, 
    iVolumeMultiplier, 
    iPitchMultiplier,
    iDelay,
    iMinDistance,
    iMaxDistance);
}

std::optional<FSound3DBinder> MSound::PlaySound3DLooped(
     const std::string& iSoundSpecifier, 
     const std::string& iSoundChannel,
     const DVec3& iWorldPosition,
     const DClamp<TF32, 0, 5>& iVolumeMultiplier,
     const DClamp<TF32, 0, 5>& iPitchMultiplier,
     const TF32 iMinDistance,
     const TF32 iMaxDistance)
{
  return DY_PIMPL->PlaySound3DLooped(
    iSoundSpecifier, iSoundChannel, 
    iWorldPosition, 
    iVolumeMultiplier, iPitchMultiplier,
    iMinDistance, iMaxDistance);
}

bool MSound::IsSoundClipExist(const std::string& iSoundSpecifier) const noexcept
{
  return DY_PIMPL->IsSoundClipExist(iSoundSpecifier);
}

FSoundChannel* MSound::GetPtrChannel(const std::string& iSpecifier)
{
  return DY_PIMPL->GetPtrChannel(iSpecifier);
}

EDySuccess MSound::pfRelease()
{
  DY_PIMPL->ReleaseSoundSystem();
  DY_RESET_PIMPL();
  return EDySuccess::DY_SUCCESS;
}

void MSound::Update(TF32 iDt) 
{ 
  // When using FMOD Studio, 
  // call Studio::System::update, which internally will also update the Low Level system. 
  // If using Low Level directly, instead call System::update.
  if (DY_PIMPL->mSoundSystem == nullptr) { return; }

  // If any `new` `focused camera` is exist on system, 
  // and DY_PIMPL camera is using 3D Listener, activate or deactivate.
  // @TODO CODE SMELL, NEED TO REFACTOR CODE.
  auto& worldManager = MWorld::GetInstance();
  if (worldManager.GetFocusedCameraCount() > 0)
  {
    auto ptrCamera  = worldManager.GetFocusedCameraValidReference(0);
    if (ptrCamera.value()->IsUsing3DListener() == true)
    {
      if (DY_PIMPL->mIsUsing3DListener == false) { DY_PIMPL->Set3DListenerSetting(true); }
      DY_PIMPL->Set3DListenerActorSetting(0, *ptrCamera.value());
    }
    else
    {
      if (DY_PIMPL->mIsUsing3DListener == true) { DY_PIMPL->Set3DListenerSetting(false); }
    }
  }
  else 
  { 
    if (DY_PIMPL->mIsUsing3DListener == true) { DY_PIMPL->Set3DListenerSetting(false); } 
  }

  //
  // Check intant 2D sound instance is valid.
  for (auto& ptrsmtInstance : DY_PIMPL->mInstantSound2DList)
  {
    // If instance is not valid, we have to check sound is valid so able to initialize.
    if (const auto status = ptrsmtInstance->GetStatus(); status == ESoundState::NotValid) 
    { 
      ptrsmtInstance->TryInitialize(); 
    }
    // Otherwise, we have to check instance is stopped so have to release.
    else if (status == ESoundState::Stop) { ptrsmtInstance = nullptr; }
  }
  // Remove empty 2d instant sound instance item.
  DY_PIMPL->mInstantSound2DList.remove_if([](const auto& ptrsmtInstance) { return ptrsmtInstance == nullptr; });

  //
  // Check intant 3D sound instance is valid.
  for (auto& ptrsmtInstance : DY_PIMPL->mInstantSound3DList)
  {
    // If instance is not valid, we have to check sound is valid so able to initialize.
    if (const auto status = ptrsmtInstance->GetStatus(); status == ESoundState::NotValid) 
    { 
      ptrsmtInstance->TryInitialize(); 
    }
    // Otherwise, we have to check instance is stopped so have to release.
    else if (status == ESoundState::Stop) { ptrsmtInstance = nullptr; }
  }
  // Remove empty 3d instant sound instance item.
  DY_PIMPL->mInstantSound3DList.remove_if([](const auto& ptrsmtInstance) { return ptrsmtInstance == nullptr; });

  //
  // Check sound instance is valid.
  for (auto& ptrsmtInstance : DY_PIMPL->mGeneralSoundInstanceList)
  {
    // If instance is not valid, we have to check given sound is valid so able to initialize.
    if (ptrsmtInstance == nullptr) { continue; }

    ptrsmtInstance->Update(iDt);
    // If instance must be removed, remove.
    if (ptrsmtInstance->GetStatus() == ESoundState::Component_Vanished) { ptrsmtInstance = nullptr; }
  }
  EraseRemove(DY_PIMPL->mGeneralSoundInstanceList, nullptr);

  // Update system to make sound.
  DY_PIMPL->mSoundSystem->update(); 
}

FSoundGroup& MSound::MDY_PRIVATE(GetGroupChannel)(const std::string& iSpecifier)
{
  return DY_PIMPL->__GetGroupChannel(iSpecifier);
}

FMOD::System& MSound::MDY_PRIVATE(GetSystem)()
{
  return DY_PIMPL->__GetSystem();
}

FSoundInstance* MSound::MDY_PRIVATE(CreateSoundInstance)(
     const PDySoundSourceComponentMetaInfo& iMetaInfo,
     FActor& iRefActor)
{
  return DY_PIMPL->__CreateSoundInstance(iMetaInfo, iRefActor);
}

bool MSound::IsSoundSystemAvailable() const noexcept
{
  return DY_PIMPL->mIsSoundSystemAvailable;
}

DY_DEFINE_PIMPL(MSound);
DY_DEFINE_DEFAULT_DESTRUCTOR(MSound);

} /// ::dy namespace