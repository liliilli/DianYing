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
#include <Dy/Component/Internal/Sound/FSoundInstance.h>

#include <Dy/Management/MSound.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/Type/Sound/FSoundChannel.h>
#include <Dy/Core/Resource/Information/FInformationSound.h>
#include <Dy/Management/Internal/Sound/XCallbackChannel.h>
#include <Dy/Element/FActor.h>
#include <Dy/Component/CTransform.h>

namespace dy
{

FSoundInstance::FSoundInstance(
  const PDySoundSourceComponentMetaInfo& iInputInfo,
  FActor& iRefActor) 
  : mSoundSpecifier{iInputInfo.mDetails.mSoundSpecifier},
    m2DSound{iInputInfo.mDetails.m2DSound},
    mMuted{iInputInfo.mDetails.mMuted},
    mLooped{iInputInfo.mDetails.mLooped},
    mVolumeMultiplier{iInputInfo.mDetails.mVolumeMultiplier},
    mPitchMultiplier{iInputInfo.mDetails.mPitchMultiplier},
    mChannelSpecifier{iInputInfo.mDetails.mChannelSpecifier},
    mAttenuation{
      iInputInfo.mDetails.mAttenuation.mActivated, 
      iInputInfo.mDetails.mAttenuation.mNearDistance, 
      iInputInfo.mDetails.mAttenuation.mFarDistance},
    mPtrActor{&iRefActor}
{
  // Set sound resource if specifier is not empty.
  if (this->mSoundSpecifier.empty() == false) { this->SetSound(this->mSoundSpecifier); }

  // First try for initializing internal sound resource.
  this->TryInitialize();
}

FSoundInstance::~FSoundInstance()
{
  // If internal sound & channel resource is initialized, release them.
  if (this->mSoundStatus != ESoundState::NotValid)
  {
    if (this->mPtrInternalSound != nullptr)
    {
      this->mPtrInternalSound->release();
      this->mPtrInternalChannel = nullptr;
      this->mPtrInternalSound   = nullptr;
    }
  }
}

void FSoundInstance::SetSound(const std::string& iSpecifier)
{
  // If sound is paused or playing...
  if (const auto status = this->GetStatus(); 
      status == ESoundState::Paused || status == ESoundState::Play) { this->StopSound(); }

  // Check validity.
  const auto& infoManager = MIOMeta::GetInstance();
  if (infoManager.IsSoundMetaInfoExist(iSpecifier) == false)
  {
    DyPushLogError("Failed to set sound. Sound is not exist, `{}`.", iSpecifier);
    return;
  }

  // Release internal resource.
  {
    if (this->mPtrInternalSound != nullptr)
    {
      const auto flag = this->mPtrInternalSound->release();
      MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to play sound instance.");
      this->mPtrInternalSound = nullptr;
    }
    this->mPtrInternalChannel = nullptr;
    this->__SetStatus(ESoundState::NotValid);
  }

  // Set and try require resource.
  this->mSoundSpecifier = iSpecifier;
  this->mBinderClipResource.TryRequireResource(this->mSoundSpecifier);
}

void FSoundInstance::SetChannel(const std::string& iChannelSpecifier)
{
  // If sound is paused or playing...
  if (const auto status = this->GetStatus(); 
      status == ESoundState::Paused || status == ESoundState::Play) { this->StopSound(); }

  // Check validity.
  auto& soundManager = MSound::GetInstance();
  if (MDY_CHECK_ISNOTNULL(soundManager.GetPtrChannel(iChannelSpecifier)))
  { this->mChannelSpecifier = iChannelSpecifier; }
  else 
  { this->mChannelSpecifier.clear(); }

  // Release internal resource.
  {
    if (this->mPtrInternalSound != nullptr)
    {
      const auto flag = this->mPtrInternalSound->release();
      MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to play sound instance.");
      this->mPtrInternalSound = nullptr;
    }
    this->mPtrInternalChannel = nullptr;
    this->__SetStatus(ESoundState::NotValid);
  }
}

void FSoundInstance::Set2DSound(bool i2DActivated)
{
  // If sound is paused or playing...
  if (const auto status = this->GetStatus(); 
      status == ESoundState::Paused || status == ESoundState::Play) { this->StopSound(); }

  this->m2DSound = i2DActivated;

  // Release internal resource.
  {
    if (this->mPtrInternalSound != nullptr)
    {
      const auto flag = this->mPtrInternalSound->release();
      MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to play sound instance.");
      this->mPtrInternalSound = nullptr;
    }
    this->mPtrInternalChannel = nullptr;
    this->__SetStatus(ESoundState::NotValid);
  }
}

EDySuccess FSoundInstance::TryInitialize()
{
  // Failure check.
  if (this->mSoundSpecifier.empty() == true) { return EDySuccess::DY_FAILURE; }

  if (this->GetStatus() != ESoundState::NotValid 
  ||  this->mBinderClipResource.IsResourceExist() == false) { return EDySuccess::DY_FAILURE; }

  // Initiate (Initialize)
  auto& soundManager = MSound::GetInstance();
  auto& refSystem = soundManager.MDY_PRIVATE(GetSystem)();

  // Create sound
  {
    const auto& soundPath = this->mBinderClipResource->GetPath();
    // If this sound instance should be looped, set customized flag.
    decltype(FMOD_DEFAULT) soundFlag = FMOD_DEFAULT | FMOD_LOOP_NORMAL;
    if (this->m2DSound == true) 
    {
      soundFlag |= FMOD_2D; 
    } 
    else 
    { 
      soundFlag |= (FMOD_3D | FMOD_3D_WORLDRELATIVE | FMOD_3D_LINEARROLLOFF); 
    }

    const auto flag = refSystem.createSound(
      soundPath.string().c_str(), soundFlag, nullptr, &this->mPtrInternalSound);
    MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to create sound instance.");
  }

  this->__SetStatus(ESoundState::Stop);

  return EDySuccess::DY_SUCCESS;
}

void FSoundInstance::Update(TF32 dt)
{
  if (this->GetStatus() == ESoundState::NotValid) { this->TryInitialize(); }
  if (this->GetStatus() == ESoundState::Play)
  {
    if (this->IsMuted() == false && this->Is2DSound() == false)
    {
      this->UpdateInternal3DPositionVelocity();
    };
  }
}

void FSoundInstance::PlaySound()
{
  // `mPtrInternalSound` and `mPtrInternalChannel` is valid on now.
  if (this->mSoundStatus == ESoundState::Paused)
  {
    this->mPtrInternalChannel->setPaused(false);
  }
  // `mPtrInternalSound` is valid but `mPtrInternalChannel` is not valid.
  else if (this->mSoundStatus == ESoundState::Stop)
  {
    // Initiate (Initialize)
    auto& soundManager  = MSound::GetInstance();

    // Set sound instance properties.
    // play sound but let it be paused becasue we have to set up some properties. and Add instant channel to group.
    FMOD::ChannelGroup* ptrInternalChannel = nullptr;
    if (this->mChannelSpecifier.empty() == false)
    {
      auto* ptrChannel = soundManager.GetPtrChannel(this->mChannelSpecifier);
      ptrInternalChannel = ptrChannel->MDY_PRIVATE(GetPtrChannel)();
    }

    auto& refSystem = soundManager.MDY_PRIVATE(GetSystem)();
    if (MDY_CHECK_ISNULL(ptrInternalChannel))
    { // If failed to find channel, just play it with master channel.
      const auto flag = refSystem.playSound(this->mPtrInternalSound, nullptr, false, &this->mPtrInternalChannel);
      MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to set sound instance.");
    }
    else
    {
      const auto flag = refSystem.playSound(
        this->mPtrInternalSound, ptrInternalChannel, false, &this->mPtrInternalChannel);
      MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to set sound instance.");
    }

    // Set volume and pitch. & Set callback and user data.
    this->mPtrInternalChannel->setVolume(this->mVolumeMultiplier);
    this->mPtrInternalChannel->setPitch(this->mPitchMultiplier);
    this->mPtrInternalChannel->setUserData(this);
    this->mPtrInternalChannel->setCallback(__CallbackSoundChannel);

    this->__SetStatus(ESoundState::Play);

    // This function must be after setting status to `Play` `Pause`.
    this->UpdateInternalAttenuationProperty();
    this->UpdateInternal3DPositionVelocity();
    this->UpdateInternalMute();
    this->UpdateInternalLoop();
  }
}

void FSoundInstance::UpdateInternal3DPositionVelocity()
{
  // When `NotValid` and `Stop`, `NotValid` is mPtrInternalSound & mPtrChannel is not valid.
  // and `Stop` is `mPtrInternalSound` is valid but `mPtrChannel` is not valid.
  if (const auto flag = this->GetStatus();
      flag == ESoundState::NotValid 
  ||  flag == ESoundState::Stop)
  {
    return;
  }

  // If we use 3D sound, set position & velocity (not supported yet). 
  if (this->m2DSound == false)
  {
    auto& refTransform    = *this->mPtrActor->GetTransform();
    const auto& position  = refTransform.GetFinalWorldPosition();

    const FMOD_VECTOR newPosition = { position.X, position.Y, position.Z };
    const FMOD_VECTOR velocity = { 0, 0, 0 };
    this->mPtrInternalChannel->set3DAttributes(&newPosition, &velocity);
  }
}

void FSoundInstance::PauseSound()
{
  // `mPtrInternalSound` and `mPtrInternalChannel` is valid on now.
  if (this->mSoundStatus == ESoundState::Play)
  {
    const auto flag = this->mPtrInternalChannel->setPaused(true);
    MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to pause sound instance.");

    this->__SetStatus(ESoundState::Paused);
  }
}

void FSoundInstance::StopSound()
{
  if (this->mSoundStatus == ESoundState::Play
  ||  this->mSoundStatus == ESoundState::Paused)
  {
    const auto flag = this->mPtrInternalChannel->stop();
    MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to play sound instance.");
    this->mPtrInternalChannel = nullptr;

    this->__SetStatus(ESoundState::Stop);
  }
}

const std::string& FSoundInstance::GetSoundSpecifier() const noexcept
{ 
  return this->mSoundSpecifier; 
}

const std::string& FSoundInstance::GetChannelSpecifier() const noexcept
{ 
  return this->mChannelSpecifier; 
}

bool FSoundInstance::IsMuted() const noexcept
{
  return this->mMuted;
}

void FSoundInstance::SetMute(bool iMuted) noexcept
{
  if (this->mMuted != iMuted)
  {
    this->mMuted = iMuted;
    // And, we need to update setting when `Play` `Paused` `Stopped`.
    this->UpdateInternalMute();
  }
}

bool FSoundInstance::IsLooped() const noexcept
{
  return this->mLooped;
}

bool FSoundInstance::Is2DSound() const noexcept
{
  return this->m2DSound;
}

void FSoundInstance::UpdateInternalMute()
{
  if (const auto flag = this->GetStatus(); 
      flag == ESoundState::Play
  ||  flag == ESoundState::Paused)
  {
    this->mPtrInternalChannel->setMute(this->mMuted);
  }
}

void FSoundInstance::SetLoop(bool iLooped)
{
  if (this->mLooped != iLooped)
  {
    this->mLooped = iLooped;
    // And we need to update setting when `Play` `Paused`.
    this->UpdateInternalLoop();
  }
}

void FSoundInstance::UpdateInternalLoop()
{
  if (const auto flag = this->GetStatus(); 
      flag == ESoundState::Play
  ||  flag == ESoundState::Paused)
  {
    this->mPtrInternalChannel->setLoopCount(this->mLooped == true ? -1 : 0);
  }
}

void FSoundInstance::SetVolume(TF32 iVolume)
{
  this->mVolumeMultiplier = iVolume;

  if (const auto flag = this->GetStatus(); flag == ESoundState::Play || flag == ESoundState::Paused)
  {
    this->mPtrInternalChannel->setVolume(iVolume);
  }
}

TF32 FSoundInstance::GetVolume() const noexcept
{
  return this->mVolumeMultiplier;
}

void FSoundInstance::SetPitch(TF32 iPitch)
{
  this->mPitchMultiplier = iPitch;

  if (const auto flag = this->GetStatus(); flag == ESoundState::Play || flag == ESoundState::Paused)
  {
    this->mPtrInternalChannel->setVolume(iPitch);
  }
}

TF32 FSoundInstance::GetPitch() const noexcept
{
  return this->mPitchMultiplier;
}

void FSoundInstance::SetAttenuation(bool iActivated)
{
  if (this->mAttenuation.mActivated != iActivated)
  {
    this->mAttenuation.mActivated = iActivated;
    this->UpdateInternalAttenuationProperty();
  }
}

void FSoundInstance::SetAttenuationMinDistance(TF32 iDistance)
{
  this->mAttenuation.mNearDistance = iDistance;
  this->UpdateInternalAttenuationProperty();
}

void FSoundInstance::SetAttenuationMaxDistance(TF32 iDistance)
{
  this->mAttenuation.mFarDistance = iDistance;
  this->UpdateInternalAttenuationProperty();
}

bool FSoundInstance::IsUsingAttenuation() const noexcept
{
  return this->mAttenuation.mActivated;
}

TF32 FSoundInstance::GetAttenuationMinDistance() const noexcept
{
  return this->mAttenuation.mNearDistance;
}

TF32 FSoundInstance::GetAttenuationMaxDistance() const noexcept
{
  return this->mAttenuation.mFarDistance;
}

void FSoundInstance::UpdateInternalAttenuationProperty()
{
  // When `NotValid` and `Stop`, `NotValid` is mPtrInternalSound & mPtrChannel is not valid.
  // and `Stop` is `mPtrInternalSound` is valid but `mPtrChannel` is not valid.
  if (const auto flag = this->GetStatus();
      flag == ESoundState::NotValid 
  ||  flag == ESoundState::Stop)
  {
    return;
  }

  // Set initial 3D distance to sound.
  const auto flag = this->mPtrInternalSound->set3DMinMaxDistance(
      this->mAttenuation.mNearDistance, 
      this->mAttenuation.mFarDistance);
  MDY_ASSERT_MSG(flag == FMOD_OK, "Failed to create sound instance.");

  // If this instance will use attenuation or not, set 3D level to disable / enable feature.
  if (this->Is2DSound() == false)
  {
    if (this->mAttenuation.mActivated == false) { this->mPtrInternalChannel->set3DLevel(0.0f); }
    else                                        { this->mPtrInternalChannel->set3DLevel(1.0f); }
  }
}

} /// ::dy namespace