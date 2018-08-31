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

class CDyMaterialResource
{
public:
  CDyMaterialResource()  = default;
  CDyMaterialResource(const CDyMaterialResource&)             = delete;
  CDyMaterialResource(CDyMaterialResource&&)                  = default;
  CDyMaterialResource& operator=(const CDyMaterialResource&)  = delete;
  CDyMaterialResource& operator=(CDyMaterialResource&&)       = default;
  ~CDyMaterialResource();

  // @todo TEMPORAL FUNCTION
  void TemporalRender();

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
  ///
  /// @brief
  ///
  void __pfSetPrevLevel(DDyMaterialInformation* ptr) const noexcept { __mPrevLevelPtr = ptr; }
  void __pfResetShaderPtr() noexcept
  {
    mShaderResource.mShaderName     = "DY_RELEASED";
    mShaderResource.mShaderPointer  = nullptr;
  }
  void __pfResetTexturePtr(CDyTextureResource* ptr) noexcept;
  void __pfSetRendererBind(void* ptr) const noexcept
  {

  }
  void __pfSetRendererReset(void* ptr) const noexcept
  {

  }
  mutable DDyMaterialInformation*           __mPrevLevelPtr     = nullptr;
  mutable TBindPtrMap<void>                 __mBindRendererPtrs;

  friend class CDyTextureResource;
  friend class CDyShaderResource;
  friend class DDyMaterialInformation;
  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_RESOURCE_MATERIAL_RESOURCE_H