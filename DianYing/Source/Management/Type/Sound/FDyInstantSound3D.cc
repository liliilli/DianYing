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
#include <Dy/Management/Type/Sound/FDyInstantSound3D.h>

#include <Dy/Core/Resource/Information/FDySoundInformation.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Management/Internal/Sound/CallbackChannel.h>
#include <Dy/Management/Type/Sound/FDySoundChannel.h>

namespace dy
{

FDyInstantSound3D::FDyInstantSound3D(
    _MIN_ const std::string& iSoundSpecifier, 
    _MIN_ const std::string& iSoundChannel,
    _MIN_ const DDyVector3& iPosition,
    _MIN_ TF32 iVolumeMultiplier, _MIN_ TF32 iPitchMultiplier, _MIN_ TF32 iDelay,
    _MIN_ TF32 iMinDistance, _MIN_ TF32 iMaxDistance,
    _MIN_ bool iIsLooped) :
    mBinderClipResource{iSoundSpecifier},
    mSoundChannelSpecifier{iSoundChannel},
    mWorldPosition{iPosition},
    mMaxVolumeDistance{iMinDistance},
    mMinVolumeDistance{iMaxDistance},
    mInitVolume{iVolumeMultiplier},
    mInitPitch{iPitchMultiplier},
    mInitDelay{iDelay},
    mIsLoop{iIsLooped}
{
  // First check.
  this->TryInitialize();
}

FDyInstantSound3D::~FDyInstantSound3D()
{
  // If internal sound & channel resource is initialized, release them.
  if (this->mSoundStatus != EDySoundStatus::NotValid)
  {
    this->mPtrInternalSound->release();
    this->mPtrInternalChannel = nullptr;
  }
}

EDySuccess FDyInstantSound3D::TryInitialize()
{
  // Failure check.
  if (this->mBinderClipResource.IsResourceExist() == false
  ||  this->GetStatus() != EDySoundStatus::NotValid)
  {
    return DY_FAILURE;
  }

  // Initiate (Initialize)
  auto& soundManager = MDySound::GetInstance();
  auto& refSystem = soundManager.MDY_PRIVATE(GetSystem)();

  // Create sound
  {
    const auto& soundPath = this->mBinderClipResource->GetPath();
    // If this sound instance should be looped, set customized flag.
    decltype(FMOD_DEFAULT) soundFlag = FMOD_LOOP_OFF | FMOD_3D | FMOD_3D_WORLDRELATIVE | FMOD_3D_LINEARROLLOFF;
    if (this->mIsLoop == true)
    {
      soundFlag &= ~FMOD_LOOP_OFF;
      soundFlag |=  FMOD_LOOP_NORMAL;
    }
    // Create 3D Sound.
    {
      const auto flag = refSystem.createSound(soundPath.string().c_str(), soundFlag, nullptr, &this->mPtrInternalSound);
      MDY_ASSERT(flag == FMOD_OK, "Failed to create sound instance.");
    }
    // Set 3D distance.
    {
      const auto flag = this->mPtrInternalSound->set3DMinMaxDistance(this->mMaxVolumeDistance, this->mMinVolumeDistance);
      MDY_ASSERT(flag == FMOD_OK, "Failed to create sound instance.");
    }
  }

  // Set sound instance properties.
  {
    // play sound but let it be paused becasue we have to set up some properties.
    // and Add instant channel to group.
    auto* ptrChannel = soundManager.GetPtrChannel(this->mSoundChannelSpecifier);
    if (MDY_CHECK_ISNULL(ptrChannel))
    {
      // If failed to find channel, just play it with master channel.
      const auto flag = refSystem.playSound(this->mPtrInternalSound, 
          nullptr, true, &this->mPtrInternalChannel);
      MDY_ASSERT(flag == FMOD_OK, "Failed to play sound instance.");
    }
    else
    {
      const auto flag = refSystem.playSound(
          this->mPtrInternalSound, 
          ptrChannel->MDY_PRIVATE(GetPtrChannel)(), 
          true, &this->mPtrInternalChannel);
      MDY_ASSERT(flag == FMOD_OK, "Failed to play sound instance.");
    }

    // Set volume and pitch.
    const FMOD_VECTOR position = { this->mWorldPosition.X, this->mWorldPosition.Y, this->mWorldPosition.Z };
    const FMOD_VECTOR velocity = { 0, 0, 0 };
    this->mPtrInternalChannel->set3DAttributes(&position, &velocity);
    this->mPtrInternalChannel->setVolume(this->mInitVolume);
    this->mPtrInternalChannel->setPitch(this->mInitPitch);
    // @TODO IMPLEMENT DELAY ALSO
    //this->mPtrInternalChannel->setDelay()

    // Set callback and user data.
    this->mPtrInternalChannel->setUserData(this);
    this->mPtrInternalChannel->setCallback(__CallbackSoundChannel);

    // Actiavate sound
    this->mPtrInternalChannel->setPaused(false);
    this->mSoundStatus = EDySoundStatus::Play;
  }

  return DY_SUCCESS;
}

EDySuccess FDyInstantSound3D::TryStop()
{
  if (this->mSoundStatus == EDySoundStatus::NotValid)
  {
    DyPushLogWarning("Failed to try stop instant sound 2d. Sound status is either NotValid or Stop.");
    this->mSoundStatus = EDySoundStatus::Stop;
    return DY_FAILURE;
  }

  const auto flag = this->mPtrInternalChannel->stop();
  MDY_ASSERT(flag == FMOD_OK, "Failed to play sound instance.");
  return DY_SUCCESS;
}

} /// ::dy namespace