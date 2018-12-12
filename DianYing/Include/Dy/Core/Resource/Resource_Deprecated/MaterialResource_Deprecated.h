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

#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Core/Resource/MeshRenderer.h>

//!
//! Forward declaration
//!

namespace dy
{
class DDyMaterialInformation_Deprecated;
class CDyShaderResource_Deprecated;
class CDyTextureResource_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyMaterialResource_Deprecated
/// @brief Material resource class which manage texture, properties instance (only moveable)
///
class CDyMaterialResource_Deprecated
{
public:
  CDyMaterialResource_Deprecated()  = default;
  CDyMaterialResource_Deprecated(const CDyMaterialResource_Deprecated&)             = delete;
  CDyMaterialResource_Deprecated(CDyMaterialResource_Deprecated&&)                  = default;
  CDyMaterialResource_Deprecated& operator=(const CDyMaterialResource_Deprecated&)  = delete;
  CDyMaterialResource_Deprecated& operator=(CDyMaterialResource_Deprecated&&)       = default;
  ~CDyMaterialResource_Deprecated();

  ///
  /// @brief Get shader resource ptr which may be nullptr or not be.
  ///
  NotNull<CDyShaderResource_Deprecated*> GetShaderResource() noexcept;

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
  EDySuccess pfInitializeMaterialResource(const PDyMaterialResourceDescriptor& materialInformation);

  std::string                             mMaterialName         = {};
  DDyMaterialShaderTuple                  mShaderResource       = {};
  std::vector<DDyMaterialTextureTuple>    mTextureResources     = {};
  EDyMaterialBlendMode                    mBlendMode            = EDyMaterialBlendMode::Opaque;

  //!
  //! Level pointers binding
  //!

  FORCEINLINE void __pfSetMaterialInformationLink(NotNull<DDyMaterialInformation_Deprecated*> ptr) const noexcept
  {
    this->__mLinkedMaterialInformationPtr = ptr;
  }

  FORCEINLINE void __pfResetMaterialInformationLink() const noexcept
  {
    this->__mLinkedMaterialInformationPtr = nullptr;
  }

  void __pfResetTextureResourcePtr(NotNull<CDyTextureResource_Deprecated*> ptr) noexcept;

  FORCEINLINE void __pfResetShaderResource() noexcept
  {
    this->mShaderResource.mShaderName         = "NOT VALID";
    this->mShaderResource.mValidShaderPointer = nullptr;
  }

  MDY_TRANSIENT DDyMaterialInformation_Deprecated*   __mLinkedMaterialInformationPtr = nullptr;

  friend class CDyTextureResource_Deprecated;
  friend class CDyShaderResource_Deprecated;
  friend class DDyMaterialInformation_Deprecated;
  friend class MDyIOResource_Deprecated;
};

} /// ::dy namespace

#endif /// GUARD_DY_RESOURCE_MATERIAL_RESOURCE_H