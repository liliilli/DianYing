#ifndef GUARD_DY_CORE_COMPONENT_RESOURCE_SOUNDRESOURCE_H
#define GUARD_DY_CORE_COMPONENT_RESOURCE_SOUNDRESOURCE_H
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

#include <Dy/Core/Resource/Information_Deprecated/SoundInformation_Deprecated.h>
#include <fmod.hpp>

namespace dy
{

///
/// @enum EDySoundStatus
/// @brief Sound status enum type
///
enum class EDySoundStatus
{
  Playing,
  Paused,
  Stopped,
  NoneError
};

///
/// @class CDySoundResource_Deprecated
/// @brief Sound resource type
///
/// @TODO THIS CLASS TYPE IS TEMPORAL.
///
class CDySoundResource_Deprecated final
{
public:
  CDySoundResource_Deprecated() = default;
  CDySoundResource_Deprecated(const CDySoundResource_Deprecated&)             = delete;
  CDySoundResource_Deprecated(CDySoundResource_Deprecated&&)                  = default;
  CDySoundResource_Deprecated& operator=(const CDySoundResource_Deprecated&)  = delete;
  CDySoundResource_Deprecated& operator=(CDySoundResource_Deprecated&&)       = default;
  ~CDySoundResource_Deprecated();

private:
  ///
  /// @brief Initialize resource with information instance.
  ///
  [[nodiscard]]
  EDySuccess pfInitializeResource(const DDySoundInformation_Deprecated& modelInformation);

  FMOD::Sound*    mSoundResourcePtr = nullptr;
  FMOD::Channel*  mSoundChannel     = nullptr;
  EDySoundStatus  mSoundStatus      = EDySoundStatus::NoneError;

  //!
  //! Level pointers binding
  //!

  FORCEINLINE void __pfSetSoundInformationLink(NotNull<DDySoundInformation_Deprecated*> ptr) const noexcept
  {
    this->__mLinkedSoundInformationPtr = ptr;
  }
  FORCEINLINE void __pfResetSoundInformationLink() const noexcept
  {
    this->__mLinkedSoundInformationPtr = nullptr;
  }

  MDY_TRANSIENT DDySoundInformation_Deprecated*  __mLinkedSoundInformationPtr = nullptr;

  friend class DDySoundInformation_Deprecated;
  friend class MDyIOResource_Deprecated;

  // @TODO TEMPORAL
  friend class MDySound;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_SOUNDRESOURCE_H