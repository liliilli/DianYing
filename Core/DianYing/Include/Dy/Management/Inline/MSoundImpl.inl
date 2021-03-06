#ifndef GUARD_DY_MANAGEMENT_SOUNDMANAGER_IMPL_INL
#define GUARD_DY_MANAGEMENT_SOUNDMANAGER_IMPL_INL
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

#include <Dy/Helper/Library/HelperContainer.h>

#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace dy
{

inline std::unique_ptr<FInstantSound2D> MSound::Impl::CreateSound2D(
  const std::string& iSoundSpecifier,
  const std::string& iSoundChannel, 
  const DClamp<TF32, 0, 5>& iVolumeMultiplier,
  const DClamp<TF32, 0, 5>& iPitchMultiplier, 
  TF32 iDelay)
{
  // Check error.
  if (this->IsSoundClipExist(iSoundSpecifier) == false) 
  { 
    DyPushLogError("Sound clip {} is not found, so Failed to play 2D sound.", iSoundSpecifier);
    return nullptr; 
  }

  // Create `FInstantSound2D`.
  return std::make_unique<FInstantSound2D>(
    iSoundSpecifier, 
    iSoundChannel, 
    iVolumeMultiplier, 
    iPitchMultiplier, 
    iDelay, 
    false);
}

inline void MSound::Impl::PlaySound2D(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel,
  const DClamp<TF32, 0, 5>& iVolumeMultiplier, 
  const DClamp<TF32, 0, 5>& iPitchMultiplier, 
  TF32 iDelay)
{
  // Check error.
  if (this->IsSoundClipExist(iSoundSpecifier) == false) 
  { 
    DyPushLogError("Sound clip {} is not found, so Failed to play 2D sound.", iSoundSpecifier);
    return; 
  }

  // Create `FInstantSound2D`.
  this->mInstantSound2DList.emplace_front(
    std::make_unique<FInstantSound2D>(
      iSoundSpecifier, 
      iSoundChannel, 
      iVolumeMultiplier, 
      iPitchMultiplier, 
      iDelay, 
      false)
  );
}

inline std::optional<FSound2DBinder> MSound::Impl::PlaySound2DLooped(
  const std::string& iSoundSpecifier,
  const std::string& iSoundChannel, 
  const DClamp<TF32, 0, 5>& iVolumeMultiplier,
  const DClamp<TF32, 0, 5>& iPitchMultiplier)
{
  // Check error.
  if (this->IsSoundClipExist(iSoundSpecifier) == false)
  {
    DyPushLogError("Sound clip {} is not found, so Failed to play 2D sound.", iSoundSpecifier);
    return std::nullopt;
  }

  // Create `FInstantSound2D`.
  auto& refInstance = this->mInstantSound2DList.emplace_front(
    std::make_unique<FInstantSound2D>(iSoundSpecifier, iSoundChannel, iVolumeMultiplier, iPitchMultiplier, 0.0f, true)
  );
  return FSound2DBinder{ *refInstance };
}

inline void MSound::Impl::PlaySound3D(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel,
  const DVec3& iWorldPosition, 
  const DClamp<TF32, 0, 5>& iVolumeMultiplier,
  const DClamp<TF32, 0, 5>& iPitchMultiplier, 
  TF32 iDelay, 
  TF32 iMinDistance, 
  TF32 iMaxDistance)
{
  // Check error.
  if (this->IsSoundClipExist(iSoundSpecifier) == false)
  {
    DyPushLogError("Sound clip {} is not found, so Failed to play 3D sound.", iSoundSpecifier);
    return;
  }

  // Create `FInstantSound3D`.
  this->mInstantSound3DList.emplace_front(
    std::make_unique<FInstantSound3D>(
      iSoundSpecifier,
      iSoundChannel,
      iWorldPosition, iVolumeMultiplier, iPitchMultiplier, iDelay,
      iMinDistance, iMaxDistance,
      false)
  );
}

inline std::optional<FSound3DBinder> MSound::Impl::PlaySound3DLooped(
  const std::string& iSoundSpecifier, 
  const std::string& iSoundChannel, 
  const DVec3& iWorldPosition, 
  const DClamp<TF32, 0, 5>& iVolumeMultiplier, 
  const DClamp<TF32, 0, 5>& iPitchMultiplier, 
  TF32 iMinDistance, 
  TF32 iMaxDistance)
{
  // Check error.
  if (this->IsSoundClipExist(iSoundSpecifier) == false) 
  { 
    DyPushLogError("Sound clip {} is not found, so Failed to play 2D sound.", iSoundSpecifier);
    return std::nullopt; 
  } 

  // Create `FInstantSound3D`.
  auto& refInstance = this->mInstantSound3DList.emplace_front(
      std::make_unique<FInstantSound3D>(
          iSoundSpecifier, 
          iSoundChannel, 
          iWorldPosition, iVolumeMultiplier, iPitchMultiplier, 0.0f, 
          iMinDistance, iMaxDistance,
          true)
  );
  return FSound3DBinder{*refInstance};
}

inline FSoundChannel* MSound::Impl::GetPtrChannel(const std::string& iSpecifier)
{
  // Check validity.
  if (Contains(this->mChannelContainer, iSpecifier) == false)
  {
    DyPushLogError("Failed to find sound channel, {}.", iSpecifier);
    return nullptr;
  }

  // Return pointer of channel.
  return &this->mChannelContainer.find(iSpecifier)->second;
}

inline FSoundGroup& MSound::Impl::MDY_PRIVATE(GetGroupChannel)(const std::string& iSpecifier)
{
  auto it = this->mGroupContainer.find(iSpecifier);
  MDY_ASSERT_MSG_FORCE(
    it != this->mGroupContainer.end(), 
    "Unexpected error occurred. Given group channel not found.");

  return it->second;
}

inline FMOD::System& MSound::Impl::MDY_PRIVATE(GetSystem)()
{
  MDY_ASSERT_MSG(
    this->mIsSoundSystemAvailable == true, 
    "Failed to getting sound system instance. Sound system must be initialized.");

  return *this->mSoundSystem;
}

inline FSoundInstance* MSound::Impl::__CreateSoundInstance(
  const PDySoundSourceComponentMetaInfo& iMetaInfo,
  FActor& iRefActor)
{
  PDySoundSourceComponentMetaInfo metaInfo = iMetaInfo;

  // Check validity
  if (metaInfo.mDetails.mSoundSpecifier.empty() == false)
  {
    // If not found given sound specifier, just leave it blank.
    if (const auto flag = MIOMeta::GetInstance().IsSoundMetaInfoExist(metaInfo.mDetails.mSoundSpecifier);
        flag == false)
    { metaInfo.mDetails.mSoundSpecifier.clear(); }
  }
  if (metaInfo.mDetails.mChannelSpecifier.empty() == false)
  {
    // If not found given channel, just leave it master channel.
    if (Contains(this->mChannelContainer, metaInfo.mDetails.mChannelSpecifier) == false)
    {
      metaInfo.mDetails.mChannelSpecifier.clear();
    }
  }

  // Insert and get.
  auto ptrsmtInstance = std::make_unique<FSoundInstance>(metaInfo, iRefActor);
  auto& ref = this->mGeneralSoundInstanceList.emplace_back(std::move(ptrsmtInstance));
  return ref.get();
}

inline void MSound::Impl::Set3DListenerSetting(bool iActivated)
{
  if (this->mIsUsing3DListener == iActivated) { return; }

  this->mIsUsing3DListener = iActivated;
  if (this->mIsUsing3DListener == true)
  {
    const auto& soundInstance = MSetting::GetInstance().GetSoundSetting();
    const auto& setting3D = soundInstance.m3DSetting;
    this->mSoundSystem->set3DSettings(
      setting3D.mDopplerOffset, 
      setting3D.mDistanceUnit, 
      setting3D.mAttenuationFactor);
  }
  else
  {
    const auto& soundInstance = MSetting::GetInstance().GetSoundSetting();
    const auto& setting3D = soundInstance.m3DSetting;
    this->mSoundSystem->set3DSettings(
      setting3D.mDopplerOffset, 
      setting3D.mDistanceUnit, 
      std::numeric_limits<float>::max());
  }
}

inline void MSound::Impl::Set3DListenerActorSetting(TU32 iId, CCamera& iCamera)
{
  const auto& worldPos = iCamera.GetPosition();
  const FMOD_VECTOR applyPos      = {worldPos.X, worldPos.Y, worldPos.Z};
  const FMOD_VECTOR applyVelocity = {0, 0, 0};

  const auto& viewMatrix = iCamera.GetViewMatrix();
  const auto& forward = viewMatrix[2];
  const FMOD_VECTOR applyForward  = {forward.X, forward.Y, forward.Z};

  const auto& up = viewMatrix[1];
  const FMOD_VECTOR applyUp = {up.X, up.Y, up.Z};

  const auto flag = this->mSoundSystem->set3DListenerAttributes(iId, &applyPos, &applyVelocity, &applyForward, &applyUp);
  MDY_ASSERT_MSG(flag == FMOD_OK, "Unexpected error occurred.");
}

inline EDySuccess MSound::Impl::InitializeSoundSystem()
{
  // Check sound system is available.
  if (this->mIsSoundSystemAvailable == false)
  {
    DyPushLogCritical("Sound system is not available.");
    return EDySuccess::DY_FAILURE;
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
    return EDySuccess::DY_FAILURE;
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
    DyPushLogCritical(sErrorSystemCreationFailed);
    // Release.
    if (this->mSoundSystem != nullptr) { this->mSoundSystem->release(); this->mSoundSystem = nullptr; }
    this->mIsSoundSystemAvailable = false;
    return EDySuccess::DY_FAILURE;
  }
  if (this->mVersion < FMOD_VERSION || this->mSoundDriverCount < 0)
  {
    DyPushLogCritical(sErrorSystemCreationFailed);
    // Release.
    if (this->mSoundSystem != nullptr) { this->mSoundSystem->release(); this->mSoundSystem = nullptr; }
    this->mIsSoundSystemAvailable = false;
    return EDySuccess::DY_FAILURE;
  }

  //!
  //! Sound system initialization logic.
  //! We need to create automatical sound channel 128 channels. This channel is used in internal engine.
  //!

  // We must use RIGHT_HANDED_COORDINATE
  if (this->mSoundSystem->init(128, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr) != FMOD_OK)
  {
    DyPushLogCritical(sErrorSystemCreationFailed);
    if (this->mSoundSystem != nullptr) { this->mSoundSystem->release(); this->mSoundSystem = nullptr; }
    this->mIsSoundSystemAvailable = false;
    return EDySuccess::DY_FAILURE;
  }
  
  const auto& settingManager = MSetting::GetInstance();
  const auto& soundInstance  = settingManager.GetSoundSetting();

  // Set 3D properties to system.
  this->Set3DListenerSetting(false);

  // Make channel group (not `Dy`'s group).
  // We need separate sub-channel and a channel as a group of `Dy` sound system,
  // to handling more flexibly and do volume ducking.
  for (const auto& [specifier, detail] : soundInstance.mGroup)
  {
    // 
    auto [it, isSucceeded] = this->mGroupContainer.try_emplace(specifier, FSoundGroup{});
    MDY_ASSERT_MSG(isSucceeded == true, "Unexpected error.");
    // 
    auto& [_, instance] = *it;
    MDY_CALL_ASSERT_SUCCESS(instance.Initialize(*this->mSoundSystem, specifier, detail));
  }

  // Make channel sub-channel. (`Dy`'s channel).
  for (const auto& [specifier, detail] : soundInstance.mChannel)
  {
    // 
    auto [it, isSucceeded] = this->mChannelContainer.try_emplace(specifier, FSoundChannel{});
    MDY_ASSERT_MSG(isSucceeded == true, "Unexpected error.");
    // 
    auto& [_, instance] = *it;
    MDY_CALL_ASSERT_SUCCESS(instance.Initialize(*this->mSoundSystem, specifier, detail));
  }

  this->mIsSoundSystemAvailable = true;
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MSound::Impl::ReleaseSoundSystem()
{
  // Check
  if (this->mIsSoundSystemAvailable == false)
  {
    DyPushLogCritical("Sound system is already not available, so does not have to release system.");
    return EDySuccess::DY_FAILURE;
  }

  // Clear all list.
  this->mInstantSound2DList.clear();
  this->mInstantSound3DList.clear();
  this->mGeneralSoundInstanceList.clear();

  // Release Channel instance.
  for (auto& [specifier, channel] : this->mChannelContainer) { MDY_CALL_ASSERT_SUCCESS(channel.Release()); }
  this->mChannelContainer.clear();

  // Release Group instance.
  for (auto& [specifier, group] : this->mGroupContainer) { MDY_CALL_ASSERT_SUCCESS(group.Release()); }
  this->mGroupContainer.clear();

  // Release sound.
  if (this->mSoundSystem != nullptr)
  { 
    this->mSoundSystem->release(); 
    this->mSoundSystem = nullptr; 
  }

  return EDySuccess::DY_SUCCESS;
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SOUNDMANAGER_IMPL_INL