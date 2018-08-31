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

#include <Dy/Core/Component/Internal/TextureType.h>
#include "MaterialResource.h"

//!
//! Forward declaration
//!

namespace dy
{
class CDyTextureInformation;
class CDyMaterialResource;
}

//!
//! Implementation
//!

namespace dy
{

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
  /// @brief
  ///
  FORCEINLINE uint32_t GetTextureId() const noexcept
  {
    return this->mTextureResourceId;
  };

  ///
  /// @brief
  ///
  FORCEINLINE EDyTextureStyleType GetTextureType() const noexcept
  {
    return this->mTextureType;
  }

  ///
  /// @brief
  ///
  FORCEINLINE int32_t GetTextureWidth() const noexcept
  {
    return this->mTextureWidth;
  }

  ///
  /// @brief
  ///
  FORCEINLINE int32_t GetTextureHeight() const noexcept
  {
    return this->mTextureHeight;
  }

private:
  ///
  /// @brief
  ///
  [[nodiscard]]
  EDySuccess pfInitializeResource(const CDyTextureInformation& textureInformation);

  /// Valid texture id must not be 0.
  // @todo JUST ONLY OPENGL
  EDyTextureStyleType mTextureType  = EDyTextureStyleType::None;
	uint32_t  mTextureResourceId      = 0;
  int32_t   mTextureWidth           = MDY_NOT_INITIALIZED_M1;
  int32_t   mTextureHeight          = MDY_NOT_INITIALIZED_M1;

  //!
  //! Level pointers binding
  //!

  template <typename TType>
  using TBindPtrMap = std::unordered_map<TType*, TType*>;
  ///
  /// @brief
  ///
  void __pfSetPrevLevel(CDyTextureInformation* ptr) const noexcept { __mPrevLevelPtr = ptr; }
  void __pfSetMaterialBind(CDyMaterialResource* ptr) const noexcept
  {
    auto [it, result] = __mBindMaterialPtrs.try_emplace(ptr, ptr);
    if (!result) {
      assert(false);
    }
  }
  void __pfSetMaterialReset(CDyMaterialResource* ptr) const noexcept
  {
    __mBindMaterialPtrs.erase(ptr);
  }
  mutable CDyTextureInformation*            __mPrevLevelPtr   = nullptr;
  mutable TBindPtrMap<CDyMaterialResource>  __mBindMaterialPtrs;

  friend class CDyTextureInformation;
  friend class CDyMaterialResource;
  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_TEXTURE_RESOURCE_H