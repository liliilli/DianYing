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
#include <Dy/Management/LoggingManager.h>
#include <Dy/Helper/Pointer.h>
#include "Dy/Management/SettingManager.h"

//!
//! Forward declaration
//!

namespace
{

inline constexpr const char* sChannelEffect     = "opEffect";
inline constexpr const char* sChannelBackground = "opBack";

MDY_SET_IMMUTABLE_STRING(sErrorSystemCreationFailed,
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

EDySuccess MDySound::pfInitialize()
{
  this->InitializeSoundSystem();
  return DY_SUCCESS;
}

EDySuccess MDySound::InitializeSoundSystem()
{
  // Check sound system is available.
  if (this->mIsSoundSystemAvailable == false)
  {
    MDY_LOG_CRITICAL("Sound system is not available.");
    return DY_FAILURE;
  }

  // Before calling any FMOD functions it is important to ensure COM is initialized. 
  // You can achieve this by calling CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED) on each thread 
  // that will interact with the FMOD API. 
  // This is balanced with a call to CoUninitialize() when you are completely finished with all calls to FMOD.
#if defined(_WIN32)
  const auto hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  if (hr != S_OK)
  {
    CoUninitialize();
    return DY_FAILURE;
  }
#endif

  //!
  //! Check logics.
  //!

  if (FMOD::System_Create(&this->mSoundSystem) != FMOD_OK
  ||  this->mSoundSystem->setCallback(DyFmodInternalSystemCallback) != FMOD_OK
  ||  this->mSoundSystem->getVersion(&this->mVersion) != FMOD_OK 
  ||  this->mSoundSystem->getNumDrivers(&this->mSoundDriverCount) != FMOD_OK)
  {
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    // Release.
    if (this->mSoundSystem != nullptr) { this->mSoundSystem->release(); this->mSoundSystem = nullptr; }
    this->mIsSoundSystemAvailable = false;
    return DY_FAILURE;
  }
  if (this->mVersion < FMOD_VERSION || this->mSoundDriverCount < 0)
  {
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    // Release.
    if (this->mSoundSystem != nullptr) { this->mSoundSystem->release(); this->mSoundSystem = nullptr; }
    this->mIsSoundSystemAvailable = false;
    return DY_FAILURE;
  }

  //!
  //! Sound system initialization logic.
  //! We need to create automatical sound channel 128 channels. This channel is used in internal engine.
  //!

  if (this->mSoundSystem->init(128, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
  {
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    if (this->mSoundSystem != nullptr) { this->mSoundSystem->release(); this->mSoundSystem = nullptr; }
    this->mIsSoundSystemAvailable = false;
    return DY_FAILURE;
  }
  
  // Set master channel.
  if (this->mSoundSystem->getMasterChannelGroup(&this->mMasterChannel) != FMOD_OK)
  {
    this->mMasterChannel->release();      this->mMasterChannel = nullptr;
    this->sBackgroundChannel->release();  this->sBackgroundChannel = nullptr;
    this->sEffectChannel->release();      this->sEffectChannel = nullptr;
    this->mSoundSystem->release();        this->mSoundSystem   = nullptr;

    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    this->mIsSoundSystemAvailable = false;
    return DY_FAILURE;
  }

  // Make channel group (not `Dy`'s group).
  // We need separate sub-channel and a channel as a group of `Dy` sound system,
  // to handling more flexibly and do volume ducking.
  const auto& settingManager = MDySetting::GetInstance();
  const auto& soundInstance  = settingManager.GetSoundSetting();

  for (const auto& [specifier, detail] : soundInstance.mGroup)
  {
    // 
    auto [it, isSucceeded] = this->mGroupContainer.try_emplace(specifier, FDySoundGroup{});
    MDY_ASSERT(isSucceeded == true, "Unexpected error.");
    // 
    auto& [_, instance] = *it;
    MDY_CALL_ASSERT_SUCCESS(instance.Initialize(*this->mSoundSystem, specifier, detail));
  }

  for (const auto& [specifier, detail] : soundInstance.mChannel)
  {
    //this->mSoundSystem->createChannelGroup(specifier.c_str(), )
  }

  //this->mSoundSystem->createChannelGroup("A", &this->sEffectChannel);
  //this->mSoundSystem->createChannelGroup("B", &this->sBackgroundChannel);
  //this->sBackgroundChannel->addGroup(this->sEffectChannel);

#ifdef false
  if (this->mSoundSystem->createChannelGroup(sChannelEffect, &this->sEffectChannel) != FMOD_OK)
  {
    if (this->mSoundSystem != nullptr) { this->mSoundSystem->release(); this->mSoundSystem = nullptr; }
    this->mIsSoundSystemAvailable = false;
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->mSoundSystem->createChannelGroup(sChannelBackground, &this->sBackgroundChannel) != FMOD_OK)
  {
    this->sBackgroundChannel->release();  this->sBackgroundChannel = nullptr;
    this->sEffectChannel->release();      this->sEffectChannel = nullptr;
    this->mSoundSystem->release();        this->mSoundSystem   = nullptr;

    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }
#endif

#ifdef false
  if (this->mMasterChannel->addGroup(sEffectChannel) != FMOD_OK)
  {
    this->mMasterChannel->release();      this->mMasterChannel = nullptr;
    this->sBackgroundChannel->release();  this->sBackgroundChannel = nullptr;
    this->sEffectChannel->release();      this->sEffectChannel = nullptr;
    this->mSoundSystem->release();        this->mSoundSystem   = nullptr;

    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->mMasterChannel->addGroup(sBackgroundChannel) != FMOD_OK)
  {
    this->mMasterChannel->release();      this->mMasterChannel = nullptr;
    this->sBackgroundChannel->release();  this->sBackgroundChannel = nullptr;
    this->sEffectChannel->release();      this->sEffectChannel = nullptr;
    this->mSoundSystem->release();        this->mSoundSystem   = nullptr;

    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;;
  }
#endif

  this->mIsSoundSystemAvailable = true;
  return DY_SUCCESS;
}

EDySuccess MDySound::pfRelease()
{
  this->ReleaseSoundSystem();
  return DY_SUCCESS;
}

EDySuccess MDySound::ReleaseSoundSystem()
{
  // Check
  if (this->mIsSoundSystemAvailable == false)
  {
    MDY_LOG_CRITICAL("Sound system is already not available, so does not have to release system.");
    return DY_FAILURE;
  }

  for (auto& [specifier, group] : this->mGroupContainer) { MDY_CALL_ASSERT_SUCCESS(group.Release()); }
  this->mGroupContainer.clear();

#ifdef false
  if (MDY_CHECK_ISNOTNULL(this->sBackgroundChannel))
  {
    this->sBackgroundChannel->release(); this->sBackgroundChannel = nullptr;
  }

  if (this->sEffectChannel)
  {
    this->sEffectChannel->release();
    this->sEffectChannel = nullptr;
  }
#endif
  if (MDY_CHECK_ISNOTNULL(this->mMasterChannel)) { this->mMasterChannel = nullptr; } 

  // Release sound.
  if (MDY_CHECK_ISNOTNULL(this->mSoundSystem))
  { this->mSoundSystem->release(); this->mSoundSystem = nullptr; }

  return DY_SUCCESS;
}

void MDySound::Update(MDY_NOTUSED float dt)
{
  // When using FMOD Studio, 
  // call Studio::System::update, which internally will also update the Low Level system. 
  // If using Low Level directly, instead call System::update.
  if (MDY_CHECK_ISNOTNULL(this->mSoundSystem)) 
  { 
    this->mSoundSystem->update(); 
  }
}

EDySuccess MDySound::PlaySoundElement(const std::string& soundName) const noexcept
{
  if (!this->mIsSoundSystemAvailable)
  {
    MDY_LOG_ERROR("Can not use sound system because of initialization error.");
    return DY_FAILURE;
  }

  MDY_NOT_IMPLEMENTED_ASSERT();
#ifdef false
  auto soundResource = MDyIOResource_Deprecated::GetInstance().GetSoundResource(soundName);
  if (soundResource == nullptr)
  {
    MDY_LOG_ERROR("Not found sound resource {}", soundName);
    return DY_FAILURE;
  }

  if (soundResource->mSoundStatus == EDySoundStatus::Stopped ||
      soundResource->mSoundStatus == EDySoundStatus::Paused)
  {
    switch (soundResource->mSoundStatus)
    {
    case EDySoundStatus::Stopped:
    {
      const auto result = this->mSoundSystem->playSound(soundResource->mSoundResourcePtr, this->mMasterChannel, false, &soundResource->mSoundChannel);
      if (result != FMOD_OK)
      {
        MDY_LOG_ERROR("Failed to play sound. Something error happened. | {} : {}", "Sound name", soundName);
        return DY_FAILURE;
      }
    } break;
    case EDySoundStatus::Paused:
    {
      soundResource->mSoundChannel->setPaused(false);
    } break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }

    soundResource->mSoundStatus = EDySoundStatus::Playing;
  }
  else
  {
    MDY_LOG_ERROR("Failed to play sound. Because sound resource is not stopped yet.");
    return DY_FAILURE;
  }
#endif

  return DY_SUCCESS;
}

EDySuccess MDySound::PauseSoundElement(const std::string& soundName) const noexcept
{
  if (!this->mIsSoundSystemAvailable)
  {
    MDY_LOG_ERROR("Can not use sound system because of initialization error.");
    return DY_FAILURE;
  }

  MDY_NOT_IMPLEMENTED_ASSERT();
#ifdef false
  auto soundResource = MDyIOResource_Deprecated::GetInstance().GetSoundResource(soundName);
  if (soundResource == nullptr)
  {
    MDY_LOG_ERROR("Not found sound resource {}", soundName);
    return DY_FAILURE;
  }

  if (soundResource->mSoundStatus == EDySoundStatus::Playing)
  {
    soundResource->mSoundChannel->setPaused(true);
    soundResource->mSoundStatus = EDySoundStatus::Paused;
  }
  else
  {
    MDY_LOG_ERROR("Failed to play sound. Because sound resource is not being played yet.");
    return DY_FAILURE;
  }
#endif

  return DY_SUCCESS;
}

EDySuccess MDySound::StopSoundElement(const std::string& soundName) const noexcept
{
  if (!this->mIsSoundSystemAvailable)
  {
    MDY_LOG_ERROR("Can not use sound system because of initialization error.");
    return DY_FAILURE;
  }

  MDY_NOT_IMPLEMENTED_ASSERT();
#ifdef false
  auto soundResource = MDyIOResource_Deprecated::GetInstance().GetSoundResource(soundName);
  if (soundResource == nullptr)
  {
    MDY_LOG_ERROR("Not found sound resource {}", soundName);
    return DY_FAILURE;
  }

  if (soundResource->mSoundStatus == EDySoundStatus::Playing)
  {
    soundResource->mSoundChannel->stop();
    soundResource->mSoundStatus = EDySoundStatus::Stopped;
  }
#endif

  return DY_SUCCESS;
}

EDySuccess MDySound::pfCreateSoundResource(const std::string& filePath, FMOD::Sound** soundResourcePtr)
{
  if (this->mSoundSystem)
  {
    const auto res = this->mSoundSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, soundResourcePtr);
    if (res != FMOD_OK)
    {
      // @TODO OUTPUT UNEXPECTED ERROR OCCURRED


      *soundResourcePtr = nullptr;
      return DY_FAILURE;
    }
  }
  else
  {
    // @TODO OUTPUT SOUND SYSTEM IS NOT INITILAIZED

    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

} /// ::dy namespace