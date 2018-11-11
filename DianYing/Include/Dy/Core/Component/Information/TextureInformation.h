#ifndef GUARD_DY_CORE_COMPONENT_INFORMATION_TEXTURE_INFORMATION_H
#define GUARD_DY_CORE_COMPONENT_INFORMATION_TEXTURE_INFORMATION_H
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

#include <Dy/Core/Component/Internal/TextureType.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyTextureResource;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @class DDyTextureInformation
/// @brief Texture information class that not serve heap instance
/// but information to create heap instance.
///
class DDyTextureInformation final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(DDyTextureInformation);
  DDyTextureInformation(const PDyTextureConstructionDescriptor& textureConstructionDescriptor);
  ~DDyTextureInformation();

  ///
  /// @brief return immutable descriptor information reference.
  ///
  [[nodiscard]]
  FORCEINLINE const PDyTextureConstructionDescriptor& GetInformation() const noexcept
  {
    return this->mTextureInformation;
  }

  /// Check if object is being binded to CDyTextureResource instance.
  [[nodiscard]]
  FORCEINLINE bool IsBeingBindedToResource() const noexcept
  {
    return this->__mLinkedTextureResourcePtr != nullptr;
  }

private:
  PDyTextureConstructionDescriptor mTextureInformation;

  //!
  //! Resource pointers binding
  //!

  void __pfLinkTextureResource(CDyTextureResource* ptr) const noexcept;
  MDY_TRANSIENT CDyTextureResource* __mLinkedTextureResourcePtr = nullptr;

  friend class CDyTextureResource;
  friend class MDyIOResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_TEXTURE_INFORMATION_H