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

#include <Dy/Core/Resource/Internal/EtcType.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDySoundResource_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class DDySoundInformation_Deprecated_Deprecated
/// @brief information class of each sound. (not resource)
///
class DDySoundInformation_Deprecated final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(DDySoundInformation_Deprecated);
  DDySoundInformation_Deprecated(const PDySoundConstructionDescriptor& soundDesc);
  ~DDySoundInformation_Deprecated();

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

  FORCEINLINE void __pfSetSoundResourceLink(NotNull<CDySoundResource_Deprecated*> ptr) const noexcept
  {
    this->__mLinkedSoundResourcePtr = ptr;
  }
  FORCEINLINE void __pfResetSoundResourceLink() const noexcept
  {
    this->__mLinkedSoundResourcePtr = nullptr;
  }

  MDY_TRANSIENT CDySoundResource_Deprecated* __mLinkedSoundResourcePtr = nullptr;

  friend class CDySoundResource_Deprecated;
  friend class MDyIOResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_SOUNDINFORMATION_H