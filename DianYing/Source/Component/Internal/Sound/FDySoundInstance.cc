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
#include <Dy/Component/Internal/Sound/FDySoundInstance.h>
#include <Dy/Meta/Information/PrefabMetaInformation.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Core/Resource/Information/FDySoundInformation.h>
#include <Dy/Management/Internal/Sound/CallbackChannel.h>

namespace dy
{

FDySoundInstance::FDySoundInstance(
    _MIN_ const PDySoundSourceComponentMetaInfo& iInputInfo,
    _MIN_ FDyActor& iRefActor) :
    mSoundSpecifier{iInputInfo.mDetails.mSoundSpecifier},
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
  if (this->TryInitialize() == DY_SUCCESS)
  {

  }

  // 

}

FDySoundInstance::~FDySoundInstance()
{
  // If internal sound & channel resource is initialized, release them.
  if (this->mSoundStatus != EDySoundStatus::NotValid)
  {
    if (this->mPtrInternalSound != nullptr)
    {
      this->mPtrInternalSound->release();
      this->mPtrInternalChannel = nullptr;
      this->mPtrInternalSound   = nullptr;
    }
  }
}

void FDySoundInstance::SetSound(_MIN_ const std::string& iSpecifier)
{
  // Check validity.
  const auto& infoManager = MDyMetaInfo::GetInstance();
  if (infoManager.IsSoundMetaInfoExist(iSpecifier) == false)
  {
    MDY_LOG_ERROR("Failed to set sound. Sound is not exist, `{}`.", iSpecifier);
    return;
  }

  // If sound is paused or playing...
  if (const auto status = this->GetStatus(); status == EDySoundStatus::Paused || status == EDySoundStatus::Play)
  {
    this->StopSound();
  }

  // Release internal resource.
  {
    const auto flag = this->mPtrInternalSound->release();
    MDY_ASSERT(flag == FMOD_OK, "Failed to play sound instance.");
    this->mPtrInternalChannel = nullptr;
    this->mPtrInternalSound   = nullptr;
    this->__SetStatus(EDySoundStatus::NotValid);
  }

  // Set and try require resource.
  this->mSoundSpecifier = iSpecifier;
  this->mBinderClipResource.TryRequireResource(this->mSoundSpecifier);
}

void FDySoundInstance::Update(_MIN_ TF32 dt)
{
  if (this->GetStatus() == EDySoundStatus::NotValid) { this->TryInitialize(); }
}

EDySuccess FDySoundInstance::TryInitialize()
{
  // Failure check.
  if (this->mSoundSpecifier.empty() == true) { return DY_FAILURE; }

  if (this->GetStatus() != EDySoundStatus::NotValid 
  ||  this->mBinderClipResource.IsResourceExist() == false) { return DY_FAILURE; }

  // Initiate (Initialize)
  auto& soundManager = MDySound::GetInstance();
  auto& refSystem = soundManager.MDY_PRIVATE_SPECIFIER(GetSystem)();

  // Create sound
  {
    const auto& soundPath = this->mBinderClipResource->GetPath();
    // If this sound instance should be looped, set customized flag.
    decltype(FMOD_DEFAULT) soundFlag = FMOD_DEFAULT;

    if (this->mLooped == true) { soundFlag |= FMOD_LOOP_NORMAL; } 
    else { soundFlag |= FMOD_LOOP_OFF; }

    if (this->m2DSound == true) { soundFlag |= FMOD_2D; } 
    else { soundFlag |= (FMOD_3D | FMOD_3D_WORLDRELATIVE | FMOD_3D_LINEARROLLOFF); }

    const auto flag = refSystem.createSound(soundPath.string().c_str(), soundFlag, nullptr, &this->mPtrInternalSound);
    MDY_ASSERT(flag == FMOD_OK, "Failed to create sound instance.");
  }

  // Set sound instance properties.
  // play sound but let it be paused becasue we have to set up some properties. and Add instant channel to group.
  FMOD::ChannelGroup* ptrInternalChannel = nullptr;
  if (this->mChannelSpecifier.empty() == false)
  {
    auto* ptrChannel = soundManager.GetPtrChannel(this->mChannelSpecifier);
    ptrInternalChannel = ptrChannel->MDY_PRIVATE_SPECIFIER(GetPtrChannel)();
  }

  if (MDY_CHECK_ISNULL(ptrInternalChannel))
  { // If failed to find channel, just play it with master channel.
    const auto flag = refSystem.playSound(this->mPtrInternalSound, nullptr, true, &this->mPtrInternalChannel);
    MDY_ASSERT(flag == FMOD_OK, "Failed to set sound instance.");
  }
  else
  {
    const auto flag = refSystem.playSound(this->mPtrInternalSound, ptrInternalChannel, true, &this->mPtrInternalChannel);
    MDY_ASSERT(flag == FMOD_OK, "Failed to set sound instance.");
  }

  // Set initial 3D distance to sound.
  {
    const auto flag = this->mPtrInternalSound->set3DMinMaxDistance(
        this->mAttenuation.mNearDistance, 
        this->mAttenuation.mFarDistance);
    MDY_ASSERT(flag == FMOD_OK, "Failed to create sound instance.");
  }

  // If we use 3D sound, 
  if (this->m2DSound == false)
  {
    // If this instance will use attenuation or not, set 3D level to disable / enable feature.
    if (this->mAttenuation.mActivated == false) { this->mPtrInternalChannel->set3DLevel(0.0f); }
    else                                        { this->mPtrInternalChannel->set3DLevel(1.0f); }

    // Set volume and pitch.
    //const FMOD_VECTOR position = { this->mWorldPosition.X, this->mWorldPosition.Y, this->mWorldPosition.Z };
    // @TODO TEMPORARY CODE.
    const FMOD_VECTOR position = { 0, 0, 0 };
    const FMOD_VECTOR velocity = { 0, 0, 0 };
    this->mPtrInternalChannel->set3DAttributes(&position, &velocity);
  }

  // Set volume and pitch. & Set callback and user data.
  this->mPtrInternalChannel->setVolume(this->mVolumeMultiplier);
  this->mPtrInternalChannel->setPitch(this->mPitchMultiplier);
  this->mPtrInternalChannel->setUserData(this);
  this->mPtrInternalChannel->setCallback(__CallbackSoundChannel);

  this->__SetStatus(EDySoundStatus::Stop);

  return DY_SUCCESS;
}

void FDySoundInstance::StopSound()
{
  if (this->mSoundStatus == EDySoundStatus::NotValid)
  {
    MDY_LOG_WARNING("Failed to try stop sound instance. Sound status is either NotValid or Stop.");
    this->mSoundStatus = EDySoundStatus::Stop;
    return;
  }

  const auto flag = this->mPtrInternalChannel->stop();
  MDY_ASSERT(flag == FMOD_OK, "Failed to play sound instance.");
}

} /// ::dy namespace