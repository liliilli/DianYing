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
#include <Dy/Management/Type/Sound/FDySoundGroup.h>
#include <Dy/Management/Type/Sound/FDySoundChannel.h>

namespace dy
{

EDySuccess FDySoundGroup::Initialize(_MIN_ FMOD::System& iPtrSystem, _MIN_ const std::string& iGroupName,  _MIN_ const DDySettingSound::DDetail& iDetail)
{
  if (this->mIsInitilaized == true)
  {
    DyPushLogWarning("Initialized sound group can not be initialized twice.");
    return DY_FAILURE;
  }

  // FMOD only supports one-level channel system (group system), 
  // So we have add a prefix `G_` into `iGroupName`.
  this->mInternalSpecifier = fmt::format("G_{}", iGroupName);
  const auto flag = iPtrSystem.createChannelGroup(this->mInternalSpecifier.c_str(), &this->mInternalGroup);
  if (flag != FMOD_OK)
  {
    // We must revert setting.
    this->mInternalSpecifier.clear();
    this->mIsInitilaized = false;
    return DY_FAILURE;
  }
  else
  {
    // Set flag. 
    this->mIsInitilaized = true;
    // Set initial setting.
    this->SetVolume(iDetail.mVolume);
    this->SetMute(iDetail.mMuted);

    // Add group to master channel.
    FMOD::ChannelGroup* ptrMasterChannel = nullptr;
    iPtrSystem.getMasterChannelGroup(&ptrMasterChannel);
    ptrMasterChannel->addGroup(this->mInternalGroup);

    return DY_SUCCESS;
  }
}

EDySuccess FDySoundGroup::Release()
{
  if (this->mIsInitilaized == false)
  {
    DyPushLogWarning("Initialized sound group can not be released twice.");
    return DY_FAILURE;
  }

  // Remove group from master channel 
  // but added group does not have to unbind registration from master channel.
  const auto flag = this->mInternalGroup->release();
  MDY_ASSERT_FORCE(flag == FMOD_OK, "FMOD release must be succceded, Unexpected error occurred.");
  this->mInternalGroup = nullptr;
  this->mIsInitilaized = false;
  return DY_SUCCESS;
}

void FDySoundGroup::SetVolume(_MIN_ const DDyClamp<TF32, 0, 1>& iVolume)
{
  MDY_ASSERT_FORCE(this->mIsInitilaized == true, "Given sound group must be initialized when use this function.");

  const auto flag = this->mInternalGroup->setVolume(iVolume);
  MDY_ASSERT(flag == FMOD_OK, "Unexpected error occurred.");
}

void FDySoundGroup::SetMute(_MIN_ bool iMuted)
{
  MDY_ASSERT_FORCE(this->mIsInitilaized == true, "Given sound group must be initialized when use this function.");

  const auto flag = this->mInternalGroup->setMute(iMuted);
  MDY_ASSERT(flag == FMOD_OK, "Unexpected error occurred.");
}

void FDySoundGroup::RegisterChannel(_MIN_ FDySoundChannel& iRefChannel)
{
  MDY_ASSERT_FORCE(this->mIsInitilaized == true, "Given sound group must be initialized when use this function.");

  const auto flag = this->mInternalGroup->addGroup(iRefChannel.MDY_PRIVATE(GetPtrChannel)());
  MDY_ASSERT(flag == FMOD_OK, "Unexpected error occurred.");
}

} /// ::dy namespace