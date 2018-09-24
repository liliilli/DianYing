#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Management/LoggingManager.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace
{

inline constexpr const char* sChannelEffect     = "opEffect";
inline constexpr const char* sChannelBackground = "opBack";

MDY_SET_IMMUTABLE_STRING(sErrorSystemCreationFailed,
R"dy(Failed to create fmod sound system.)dy");

///
/// @function DyFmodInternalSystemCallback
/// @brief Internal callback function of fmod library.
///
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
  const auto hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  if (hr != S_OK)
  {
    CoUninitialize();
    return DY_FAILURE;
  }

  if (FMOD::System_Create(&this->mSoundSystem) != FMOD_OK)
  {
    this->mSoundSystem = nullptr;
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->mSoundSystem->setCallback(DyFmodInternalSystemCallback) != FMOD_OK)
  {
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->mSoundSystem->getVersion(&this->mVersion); this->mVersion < FMOD_VERSION)
  {
    this->mSoundSystem->release();        this->mSoundSystem = nullptr;
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->mSoundSystem->getNumDrivers(&this->mSoundDriverCount); this->mSoundDriverCount <= 0)
  {
    this->mSoundSystem->release();        this->mSoundSystem = nullptr;
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->mSoundSystem->init(32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
  {
    this->mSoundSystem->release();        this->mSoundSystem = nullptr;
    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->mSoundSystem->createChannelGroup(sChannelEffect, &this->sEffectChannel) != FMOD_OK)
  {
    this->sEffectChannel->release();      this->sEffectChannel = nullptr;
    this->mSoundSystem->release();        this->mSoundSystem   = nullptr;

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

  if (this->mSoundSystem->getMasterChannelGroup(&sMasterChannel) != FMOD_OK)
  {
    this->sMasterChannel->release();      this->sMasterChannel = nullptr;
    this->sBackgroundChannel->release();  this->sBackgroundChannel = nullptr;
    this->sEffectChannel->release();      this->sEffectChannel = nullptr;
    this->mSoundSystem->release();        this->mSoundSystem   = nullptr;

    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->sMasterChannel->addGroup(sEffectChannel) != FMOD_OK)
  {
    this->sMasterChannel->release();      this->sMasterChannel = nullptr;
    this->sBackgroundChannel->release();  this->sBackgroundChannel = nullptr;
    this->sEffectChannel->release();      this->sEffectChannel = nullptr;
    this->mSoundSystem->release();        this->mSoundSystem   = nullptr;

    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;
  }

  if (this->sMasterChannel->addGroup(sBackgroundChannel) != FMOD_OK)
  {
    this->sMasterChannel->release();      this->sMasterChannel = nullptr;
    this->sBackgroundChannel->release();  this->sBackgroundChannel = nullptr;
    this->sEffectChannel->release();      this->sEffectChannel = nullptr;
    this->mSoundSystem->release();        this->mSoundSystem   = nullptr;

    MDY_LOG_CRITICAL(sErrorSystemCreationFailed.data());
    return DY_FAILURE;;
  }

  this->sIssEnabledSoundSystem = true;
  return DY_SUCCESS;
}

EDySuccess MDySound::pfRelease()
{
  if (this->sMasterChannel)
  {
    this->sMasterChannel->release();
    this->sMasterChannel = nullptr;
  }
  if (this->sBackgroundChannel)
  {
    this->sBackgroundChannel->release();
    this->sBackgroundChannel = nullptr;
  }
  if (this->sEffectChannel)
  {
    this->sEffectChannel->release();
    this->sEffectChannel = nullptr;
  }
  if (this->mSoundSystem)
  {
    this->mSoundSystem->release();
    this->mSoundSystem = nullptr;
  }

  this->sIssEnabledSoundSystem = false;
  return DY_SUCCESS;
}

void MDySound::Update(float dt)
{
  if (this->mSoundSystem) { this->mSoundSystem->update(); }
}

} /// ::dy namespace