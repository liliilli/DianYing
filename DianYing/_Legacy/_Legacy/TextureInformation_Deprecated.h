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

#include <Dy/Core/Resource/Internal/TextureType.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyTextureResource_Deprecated;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @class DDyTextureInformation_Deprecated
/// @brief Texture information class that not serve heap instance
/// but information to create heap instance.
///
class DDyTextureInformation_Deprecated final
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(DDyTextureInformation_Deprecated);
  DDyTextureInformation_Deprecated(const PDyTextureInstanceMetaInfo& textureConstructionDescriptor);
  ~DDyTextureInformation_Deprecated();

  /// @brief return immutable descriptor information reference.
  FORCEINLINE MDY_NODISCARD const PDyTextureInstanceMetaInfo& GetInformation() const noexcept
  {
    return this->mTextureInformation;
  }

  /// @brief Check if object is being binded to CDyTextureResource_Deprecated instance.
  FORCEINLINE MDY_NODISCARD bool IsBeingBindedToResource() const noexcept
  {
    return this->__mLinkedTextureResourcePtr != nullptr;
  }

  /// @brief Get buffer of image.
  FORCEINLINE MDY_NODISCARD const std::vector<TU08>& GetBuffer() const noexcept
  {
    return this->mTextureImageBuffer;
  }

  /// @brief Get buffer format. (different from mTextureInformation.mTextureColorType)
  FORCEINLINE MDY_NODISCARD EDyImageColorFormatStyle GetFormat() const noexcept
  {
    return this->mImageActualPixelFormat;
  }

private:
  PDyTextureInstanceMetaInfo mTextureInformation{};
  std::vector<TU08>          mTextureImageBuffer{};
  EDyImageColorFormatStyle   mImageActualPixelFormat = EDyImageColorFormatStyle::NoneError;

  //!
  //! Resource pointers binding
  //!

  void __pfLinkTextureResource(CDyTextureResource_Deprecated* ptr) const noexcept;
  MDY_TRANSIENT CDyTextureResource_Deprecated* __mLinkedTextureResourcePtr = nullptr;

  friend class CDyTextureResource_Deprecated;
  friend class MDyIOResource_Deprecated;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_TEXTURE_INFORMATION_H