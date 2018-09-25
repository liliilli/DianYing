#ifndef GUARD_DY_CORE_COMPONENT_INFORMATION_SOUNDINFORMATION_H
#define GUARD_DY_CORE_COMPONENT_INFORMATION_SOUNDINFORMATION_H
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

#include <Dy/Core/Component/Internal/EtcType.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDySoundResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class DDySoundInformation
/// @brief information class of each sound. (not resource)
///
class DDySoundInformation final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(DDySoundInformation);
  DDySoundInformation(const PDySoundConstructionDescriptor& soundDesc);
  ~DDySoundInformation();

  ///
  /// @brief return sound name to create.
  ///
  [[nodiscard]] FORCEINLINE const std::string& GetSoundName() const noexcept
  {
    return mDesc.mSoundName;
  }

  ///
  /// @brief return sound path.
  ///
  [[nodiscard]] FORCEINLINE const std::string& GetFilePath() const noexcept
  {
    return mDesc.mSoundPath;
  }

  /// Check if information is initiailized as arbitary resource.
  [[nodiscard]] FORCEINLINE bool IsBeingBindedToResource() const noexcept
  {
    return this->__mLinkedSoundResourcePtr != nullptr;
  }

private:
  PDySoundConstructionDescriptor mDesc;

  //!
  //! Resource pointers binding
  //!

  FORCEINLINE void __pfSetSoundResourceLink(NotNull<CDySoundResource*> ptr) const noexcept
  {
    this->__mLinkedSoundResourcePtr = ptr;
  }
  FORCEINLINE void __pfResetSoundResourceLink() const noexcept
  {
    this->__mLinkedSoundResourcePtr = nullptr;
  }

  MDY_TRANSIENT CDySoundResource* __mLinkedSoundResourcePtr = nullptr;

  friend class CDySoundResource;
  friend class MDyHeapResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_SOUNDINFORMATION_H