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
#include <Dy/Management/Type/Sound/FDyInstantSound2D.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Core/Resource/Information/FDySoundInformation.h>
#include "Dy/Management/Internal/Sound/CallbackChannel.h"

namespace dy
{

FDyInstantSound2D::FDyInstantSound2D(
    _MIN_ const std::string& iSoundSpecifier, 
    _MIN_ const std::string& iSoundChannel,
    _MIN_ TF32 iVolumeMultiplier, _MIN_ TF32 iPitchMultiplier, _MIN_ TF32 iDelay) :
    mBinderClipResource{iSoundSpecifier},
    mSoundChannelSpecifier{iSoundChannel},
    mInitVolume{iVolumeMultiplier},
    mInitPitch{iPitchMultiplier},
    mInitDelay{iDelay}
{
  // First check.
  this->TryInitialize();
}

FDyInstantSound2D::~FDyInstantSound2D()
{
  // If internal sound & channel resource is initialized, release them.
  if (this->mSoundStatus != EDySoundStatus::NotValid)
  {
    this->mPtrInternalSound->release();
    this->mPtrInternalChannel = nullptr;
  }
}

EDySuccess FDyInstantSound2D::TryInitialize()
{
  // Failure check.
  if (this->mBinderClipResource.IsResourceExist() == false
  ||  this->GetStatus() != EDySoundStatus::NotValid)
  {
    return DY_FAILURE;
  }

  // Initiate (Initialize)
  auto& soundManager = MDySound::GetInstance();
  auto& refSystem = soundManager.MDY_PRIVATE_SPECIFIER(GetSystem)();

  // Create sound
  {
    const auto& soundPath = this->mBinderClipResource->GetPath();
    const auto flag = refSystem.createSound(soundPath.string().c_str(), FMOD_DEFAULT, nullptr, &this->mPtrInternalSound);
    MDY_ASSERT(flag == FMOD_OK, "Failed to create sound instance.");
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
          ptrChannel->MDY_PRIVATE_SPECIFIER(GetPtrChannel)(), 
          true, &this->mPtrInternalChannel);
      MDY_ASSERT(flag == FMOD_OK, "Failed to play sound instance.");
    }

    // Set volume and pitch.
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

} /// ::dy namespace