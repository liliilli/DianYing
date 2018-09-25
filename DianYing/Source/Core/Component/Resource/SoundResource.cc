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
#include <Dy/Core/Component/Resource/SoundResource.h>

#include <filesystem>
#include <fmod.hpp>

#include <Phitos/Dbg/assert.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

CDySoundResource::~CDySoundResource()
{
  if (this->mSoundResourcePtr)
  {
    this->mSoundResourcePtr->release();
    this->mSoundResourcePtr = nullptr;
  }
}

EDySuccess CDySoundResource::pfInitializeResource(const DDySoundInformation& soundInfo)
{
  static auto CheckIntegrity = [](const DDySoundInformation& soundInfo) -> EDySuccess
  {
    if (soundInfo.GetSoundName().empty()) return DY_FAILURE;
    if (const auto err = std::filesystem::exists(soundInfo.GetFilePath()); !err) return DY_FAILURE;

    return DY_SUCCESS;
  };

  if (CheckIntegrity(soundInfo) == DY_FAILURE)
  {
    MDY_LOG_CRITICAL("Failed to instantiate sound resource. Unexpected error occurred.");
    return DY_FAILURE;
  }

  // Create sound resource
  const auto result = MDySound::GetInstance().pfCreateSoundResource(soundInfo.GetFilePath(), &this->mSoundResourcePtr);
  if (result == DY_FAILURE)
  {
    MDY_LOG_ERROR("{} | Failed to create sound resource. | Sound Name : {} | Sound Path : {}", "CDySoundResource::pfInitializeResource",
                  soundInfo.GetSoundName(), soundInfo.GetFilePath());
    return DY_FAILURE;
  }

#ifdef false
  // Set ESoundType of created sound resource
  ESoundType sound_type = ESoundType::None;
  switch (sound_info->GetSoundType()) {
  case ESoundType::BackgroundMusic:
    sound_type = ESoundType::BackgroundMusic;
    soundResource->setMode(FMOD_LOOP_NORMAL);
    soundResource->setLoopCount(-1);
    break;
  case ESoundType::Effect:
    sound_type = ESoundType::Effect;
    soundResource->setMode(FMOD_LOOP_OFF);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
#endif

  return DY_SUCCESS;
}

} /// ::dy namespace