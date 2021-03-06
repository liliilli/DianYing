#ifndef GUARD_DY_MANAGEMENT_TYPE_SOUND_FDyInstantSound3D_H
#define GUARD_DY_MANAGEMENT_TYPE_SOUND_FDyInstantSound3D_H
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
#include <Dy/Core/Resource/Type/TInformationBinder.h>
#include <Dy/Management/Type/Sound/ASoundInstance.h>
#include <Dy/Helper/Type/DVector3.h>

namespace dy
{

/// @class FInstantSound3D
/// @brief Instant 3d sound instance.
class FInstantSound3D final : public ASoundInstance
{
public:
  FInstantSound3D(
    const std::string& iSoundSpecifier, 
    const std::string& iSoundChannel,
    const DVec3& iPosition,
    TF32 iVolumeMultiplier, TF32 iPitchMultiplier, TF32 iDelay,
    TF32 iMinDistance, TF32 iMaxDistance,
    bool iIsLooped);
  virtual ~FInstantSound3D(); 

  /// @brief Try initialize sound resource, 
  /// Succeeded if only sound resource is bound and status is `NotValid` yet.
  EDySuccess TryInitialize();

  /// @brief Try stop playing instance.
  EDySuccess TryStop();

private:
  TDyInformationBinderSound mBinderClipResource;
  /// @brief For lazy initialization of sound resource.
  std::string mSoundChannelSpecifier = "";
  DVec3 mWorldPosition;
  TF32     mMaxVolumeDistance;
  TF32     mMinVolumeDistance;
  TF32     mInitVolume;
  TF32     mInitPitch;
  TF32     mInitDelay;
  bool     mIsLoop;

  FMOD::Sound*    mPtrInternalSound   = nullptr;
  FMOD::Channel*  mPtrInternalChannel = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_SOUND_FDyInstantSound3D_H