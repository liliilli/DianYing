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
#include <Dy/Component/CDySoundSource.h>
#include <Dy/Management/SoundManager.h>

namespace dy
{

EDySuccess CDySoundSource::Initialize(_MIN_ const PDySoundSourceComponentMetaInfo& descriptor)
{
  if (descriptor.mInitiallyActivated == true) { this->Activate(); }
  // We have to create internal instance anyway. This instance is created & destroyed when only Initialize & Release,
  // not TryActivateInstance & TryDeactivateInstance function.
  auto& soundManager = MDySound::GetInstance();
  // We need reference of actor also, because sound instance with 3D option update position from `Transform`.
  this->mPtrInternalSoundInstance = soundManager.__CreateSoundInstance(descriptor, *this->GetBindedActor());

  return DY_SUCCESS;
}

void CDySoundSource::Release()
{
  // We have to stop sound anyway and set status Vanished to delete from GC.
  this->mPtrInternalSoundInstance->StopSound();
  this->mPtrInternalSoundInstance->__SetStatus(EDySoundStatus::Component_Vanished);

  if (this->IsComponentActivated() == true) { this->Deactivate(); }
}

void CDySoundSource::PlaySound()
{
  this->mPtrInternalSoundInstance->PlaySound();
}

void CDySoundSource::PauseSound()
{
  this->mPtrInternalSoundInstance->PauseSound();
}

void CDySoundSource::StopSound()
{
  this->mPtrInternalSoundInstance->StopSound();
}

bool CDySoundSource::IsPlayable() const noexcept
{
  const auto flag = this->mPtrInternalSoundInstance->GetStatus();

  // Internal instance is automatically changed to `Stop` status when Resource is bound.
  return 
        flag != EDySoundStatus::NotValid
    &&  flag != EDySoundStatus::Component_Vanished;
}

bool CDySoundSource::IsPaused() const noexcept
{
  return this->mPtrInternalSoundInstance->GetStatus() == EDySoundStatus::Paused;
}

bool CDySoundSource::IsStopped() const noexcept
{
  return this->mPtrInternalSoundInstance->GetStatus() == EDySoundStatus::Stop;
}

bool CDySoundSource::IsPlaying() const noexcept
{
  return this->mPtrInternalSoundInstance->GetStatus() == EDySoundStatus::Play;
}

bool CDySoundSource::IsMuted() const noexcept
{
  return this->mPtrInternalSoundInstance->IsMuted();
}

bool CDySoundSource::IsLooped() const noexcept
{
  return this->mPtrInternalSoundInstance->IsLooped();
}

void CDySoundSource::SetNewSound(_MIN_ const std::string& iSoundSpecifier)
{
  this->mPtrInternalSoundInstance->SetSound(iSoundSpecifier);
}

const std::string& CDySoundSource::GetSound() const noexcept
{
  return this->mPtrInternalSoundInstance->GetSoundSpecifier();
}

void CDySoundSource::SetNewChannel(const std::string& iChannelSpecifier)
{
  this->mPtrInternalSoundInstance->SetChannel(iChannelSpecifier);
}

const std::string& CDySoundSource::GetSpecifier() const noexcept
{
  return this->mPtrInternalSoundInstance->GetChannelSpecifier();
}

void CDySoundSource::SetLoop(bool iLooped)
{
  this->mPtrInternalSoundInstance->SetLoop(iLooped);
}

void CDySoundSource::SetMute(_MIN_ bool iMuted)
{
  this->mPtrInternalSoundInstance->SetMute(iMuted);
}

void CDySoundSource::Set2D(_MIN_ bool iActivated)
{
  if (this->Is2DSound() != iActivated)
  {
    this->mPtrInternalSoundInstance->Set2DSound(iActivated);
  }
}

bool CDySoundSource::Is2DSound() const noexcept
{
  return this->mPtrInternalSoundInstance->Is2DSound();
}

void CDySoundSource::Update(_MIN_ TF32 dt) { }

std::string CDySoundSource::ToString()
{
  MDY_UNEXPECTED_BRANCH();
  return "";
}

void CDySoundSource::TryActivateInstance()
{

}

void CDySoundSource::TryDeactivateInstance()
{

}

} /// ::dy namespace