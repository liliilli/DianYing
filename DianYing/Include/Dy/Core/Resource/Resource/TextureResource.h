#ifndef GUARD_DY_CORE_COMPONENT_RESOURCE_TEXTURE_RESOURCE_H
#define GUARD_DY_CORE_COMPONENT_RESOURCE_TEXTURE_RESOURCE_H
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
#include "MaterialResource.h"

//!
//! Forward declaration
//!

namespace dy
{
class DDyTextureInformation;
class CDyMaterialResource;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyTextureResource
/// @brief Texture resource type which manages texture heap unsafe resources safely.
///
class CDyTextureResource final
{
public:
  CDyTextureResource() = default;
  CDyTextureResource(const CDyTextureResource&) = delete;
  CDyTextureResource(CDyTextureResource&&)      = default;
  CDyTextureResource& operator=(const CDyTextureResource&)  = delete;
  CDyTextureResource& operator=(CDyTextureResource&&)       = default;
  ~CDyTextureResource();

  ///
  /// @brief Get binded texture resource id.
  ///
  [[nodiscard]] FORCEINLINE uint32_t GetTextureId() const noexcept
  {
    return this->mTextureResourceId;
  };

  ///
  /// @brief Get texture type value.
  ///
  [[nodiscard]] FORCEINLINE EDyTextureStyleType GetTextureType() const noexcept
  {
    return this->mTextureType;
  }

  ///
  /// @brief Get texture width size value.
  ///
  [[nodiscard]] FORCEINLINE int32_t GetTextureWidth() const noexcept
  {
    return this->mTextureWidth;
  }

  ///
  /// @brief Get texture height size value.
  ///
  [[nodiscard]] FORCEINLINE int32_t GetTextureHeight() const noexcept
  {
    return this->mTextureHeight;
  }

private:
  ///
  /// @brief
  /// @param  textureInformation Valid texture meta information instance.
  /// @return Success flag.
  ///
  MDY_NODISCARD EDySuccess pfInitializeTextureResource(const DDyTextureInformation& textureInformation);

  ///
  /// @brief
  /// @param  descriptor Valid texture with chunk meta information instance.
  /// @return Success flag.
  ///
  MDY_NODISCARD EDySuccess
  pfInitializeTextureResourceWithChunk(_MIN_ const PDyTextureConstructionBufferChunkDescriptor& descriptor);

  /// Valid texture id must not be 0.
  // @todo JUST ONLY OPENGL
  std::string         mTextureName          = "";
  EDyTextureStyleType mTextureType          = EDyTextureStyleType::None;
	uint32_t            mTextureResourceId    = 0;
  int32_t             mTextureWidth         = MDY_INITIALIZE_DEFINT;
  int32_t             mTextureHeight        = MDY_INITIALIZE_DEFINT;

  //!
  //! Level pointers binding
  //!

  template <typename TType>
  using TBindPtrMap = std::unordered_map<TType*, TType*>;

  ///
  FORCEINLINE void __pfSetTextureInformationLink(NotNull<DDyTextureInformation*> ptr) const noexcept
  {
    this->__mLinkedTextureInformationPtr = ptr;
  }
  ///
  FORCEINLINE void __pfResetTextureInformationLink() const noexcept
  {
    this->__mLinkedTextureInformationPtr = nullptr;
  }
  ///
  void __pfSetMaterialResourceLink(NotNull<CDyMaterialResource*> ptr) const noexcept;
  ///
  FORCEINLINE void __pfResetMaterialResourceLink(NotNull<CDyMaterialResource*> ptr) const noexcept
  {
    this->__mBindMaterialPtrCounters.erase(ptr);
  }

  MDY_TRANSIENT DDyTextureInformation*           __mLinkedTextureInformationPtr   = nullptr;
  MDY_TRANSIENT TBindPtrMap<CDyMaterialResource> __mBindMaterialPtrCounters;

  friend class DDyTextureInformation;
  friend class CDyMaterialResource;
  friend class MDyIOResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_TEXTURE_RESOURCE_H