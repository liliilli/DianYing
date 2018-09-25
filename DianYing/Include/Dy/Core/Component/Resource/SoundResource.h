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

#include <Dy/Core/Component/Information/SoundInformation.h>
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
/// @class CDySoundResource
/// @brief Sound resource type
///
/// @TODO THIS CLASS TYPE IS TEMPORAL.
///
class CDySoundResource final
{
public:
  CDySoundResource() = default;
  CDySoundResource(const CDySoundResource&)             = delete;
  CDySoundResource(CDySoundResource&&)                  = default;
  CDySoundResource& operator=(const CDySoundResource&)  = delete;
  CDySoundResource& operator=(CDySoundResource&&)       = default;
  ~CDySoundResource();

private:
  ///
  /// @brief Initialize resource with information instance.
  ///
  [[nodiscard]]
  EDySuccess pfInitializeResource(const DDySoundInformation& modelInformation);

  FMOD::Sound*    mSoundResourcePtr = nullptr;
  FMOD::Channel*  mSoundChannel     = nullptr;
  EDySoundStatus  mSoundStatus      = EDySoundStatus::NoneError;

  //!
  //! Level pointers binding
  //!

  FORCEINLINE void __pfSetSoundInformationLink(NotNull<DDySoundInformation*> ptr) const noexcept
  {
    this->__mLinkedSoundInformationPtr = ptr;
  }
  FORCEINLINE void __pfResetSoundInformationLink() const noexcept
  {
    this->__mLinkedSoundInformationPtr = nullptr;
  }

  MDY_TRANSIENT DDySoundInformation*  __mLinkedSoundInformationPtr = nullptr;

  friend class DDySoundInformation;
  friend class MDyHeapResource;

  // @TODO TEMPORAL
  friend class MDySound;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_SOUNDRESOURCE_H