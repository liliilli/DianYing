#ifndef GUARD_DY_MANAGEMENT_TYPE_SOUND_FDyInstantSound2D_H
#define GUARD_DY_MANAGEMENT_TYPE_SOUND_FDyInstantSound2D_H
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

#include <fmod.hpp>
#include <Dy/Management/Type/Sound/EDySoundStatus.h>
#include <Dy/Core/Resource/Type/TDyInformationBinder.h>

namespace dy
{

/// @class FDyInstantSound2D
/// @brief Instant 2d sound instance.
class FDyInstantSound2D final
{
public:
  FDyInstantSound2D(
      _MIN_ const std::string& iSoundSpecifier, _MIN_ const std::string& iSoundChannel,
      _MIN_ TF32 iVolumeMultiplier, _MIN_ TF32 iPitchMultiplier, _MIN_ TF32 iDelay);
  ~FDyInstantSound2D(); 

  /// @brief Get status value of sound instance.
  MDY_NODISCARD const EDySoundStatus& GetStatus() const noexcept; 

  /// @brief Try initialize sound resource, 
  /// Succeeded if only sound resource is bound and status is `NotValid` yet.
  EDySuccess TryInitialize();

  /// @brief
  void MDY_PRIVATE_SPECIFIER(SetStatus)(_MIN_ EDySoundStatus iStatus);

private:
  TDyIInformationBinderSound mBinderClipResource;
  /// @brief For lazy initialization of sound resource.
  std::string mSoundChannelSpecifier = "";
  TF32        mInitVolume;
  TF32        mInitPitch;
  TF32        mInitDelay;

  FMOD::Sound*    mPtrInternalSound   = nullptr;
  FMOD::Channel*  mPtrInternalChannel = nullptr;
  
  EDySoundStatus  mSoundStatus = EDySoundStatus::NotValid;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_FDyInstantSound2D_H