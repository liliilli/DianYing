#ifndef GUARD_DY_RESOURCE_MATERIAL_RESOURCE_H
#define GUARD_DY_RESOURCE_MATERIAL_RESOURCE_H
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

#include <Dy/Core/Component/Internal/MaterialType.h>
#include <Dy/Core/Component/MeshRenderer.h>

//!
//! Forward declaration
//!

namespace dy
{
class DDyMaterialInformation;
class CDyShaderResource;
class CDyTextureResource;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyMaterialResource
/// @brief Material resource class which manage texture, properties instance (only moveable)
///
class CDyMaterialResource
{
public:
  CDyMaterialResource()  = default;
  CDyMaterialResource(const CDyMaterialResource&)             = delete;
  CDyMaterialResource(CDyMaterialResource&&)                  = default;
  CDyMaterialResource& operator=(const CDyMaterialResource&)  = delete;
  CDyMaterialResource& operator=(CDyMaterialResource&&)       = default;
  ~CDyMaterialResource();

  ///
  /// @brief Get shader resource ptr which may be nullptr or not be.
  ///
  CDyShaderResource* GetShaderResource() noexcept;

  ///
  /// @brief Return binded texture resource pointers list.
  ///
  const std::vector<DDyMaterialTextureTuple>& GetBindedTextureResources() const noexcept
  {
    return this->mTextureResources;
  }

  ///
  /// @brief Get blend mode type.
  ///
  EDyMaterialBlendMode GetBlendModeType() const noexcept
  {
    return this->mBlendMode;
  }

  ///
  /// @brief Get material name of this resource.
  ///
  FORCEINLINE const std::string& GetMaterialName() const noexcept
  {
    return this->mMaterialName;
  }

private:
  ///
  /// @brief
  ///
  [[nodiscard]]
  EDySuccess pInitializeMaterial(const PDyMaterialResourceDescriptor& materialInformation);

  std::string                           mMaterialName         = {};
  DDyMaterialShaderTuple                mShaderResource       = {};
  std::vector<DDyMaterialTextureTuple>  mTextureResources     = {};
  EDyMaterialBlendMode                  mBlendMode            = EDyMaterialBlendMode::Opaque;

  //!
  //! Level pointers binding
  //!

  template <typename TType>
  using TBindPtrMap = std::unordered_map<TType*, TType*>;

  void __pfLinkMaterialInformation(DDyMaterialInformation* ptr) const noexcept
  {
    this->__mLinkedMaterialInformationPtr = ptr;
  }

  void __pfResetShaderResourcePtr() noexcept
  {
    mShaderResource.mShaderName     = "DY_RELEASED";
    mShaderResource.mShaderPointer  = nullptr;
  }

  void __pfResetTextureResourcePtr(CDyTextureResource* ptr) noexcept;

  mutable DDyMaterialInformation*           __mLinkedMaterialInformationPtr     = nullptr;

  friend class CDyTextureResource;
  friend class CDyShaderResource;
  friend class DDyMaterialInformation;
  friend class MDyHeapResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_RESOURCE_MATERIAL_RESOURCE_H