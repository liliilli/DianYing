#ifndef GUARD_DY_CORE_COMPONENT_INFORMATION_MODEL_INFORMATION_H
#define GUARD_DY_CORE_COMPONENT_INFORMATION_MODEL_INFORMATION_H
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

#include <Dy/Core/Component/Internal/ModelType.h>
#include <Dy/Core/Component/Information/SubmeshInformation.h>
#include <Dy/Core/Component/Internal/MaterialType.h>

//!
//! Forward declaration
//!

struct  aiMaterial;
struct  aiMesh;
struct  aiNode;
struct  aiScene;

namespace dy
{
enum class EDyTextureMapType : unsigned char;
class CDyModelResource;
class DDyTextureInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class DDyModelInformation
/// @brief Information class for model information.
///
class DDyModelInformation final
{
public:
  DDyModelInformation(const PDyModelConstructionDescriptor& modelConstructionDescriptor);

  DDyModelInformation(const DDyModelInformation&)            = delete;
  DDyModelInformation& operator=(const DDyModelInformation&) = delete;
  DDyModelInformation(DDyModelInformation&&)            = default;
  DDyModelInformation& operator=(DDyModelInformation&&) = default;
  ~DDyModelInformation();

  ///
  /// @brief Return material name list which model information have.
  ///
  FORCEINLINE const std::vector<std::string>& GetBindedMaterialNameLists() const noexcept
  {
    return this->mBindedMaterialName;
  }

  ///
  /// @brief Return mesh information list which have material name, vertex, indice etc.
  ///
  FORCEINLINE const auto& GetMeshInformation() const noexcept
  {
    return this->mSubmeshInformations;
  }

  ///
  /// @brief Check if object is being binded to CDyModelResource instance.
  ///
  FORCEINLINE bool IsBeingBindedToResource() const noexcept
  {
    return this->mLinkedModelResourcePtr != nullptr;
  }

private:
  ///
  /// @brief Process aiMesh, make mesh information description which stores vertex, indices,
  /// innate material information, etc.
  ///
  void __pProcessAssimpMesh(aiMesh* mesh, const aiScene* scene);

  /// Read vertex data, make data, and insert to PDySubmeshInformationDescriptor.
  void __pReadVertexData(const aiMesh* mesh, PDySubmeshInformationDescriptor& desc);

  /// Read bone data.
  void __pReadBoneData(const aiMesh* mesh, PDySubmeshInformationDescriptor& desc);

  /// Read index(element) data, make data, and insert to PDySubmeshInformationDescriptor.
  void __pReadIndiceData(const aiMesh* mesh, PDySubmeshInformationDescriptor& desc);

  /// Read material data and make descriptor.
  PDyMaterialConstructionDescriptor __pReadMaterialData(const aiMaterial* material);

  /// Read material texture data and insert texture information to manager.
  std::optional<std::vector<std::string>> __pLoadMaterialTextures(const aiMaterial* material, EDyTextureMapType type);

  /// Output information log only in debug mode.
  void __pOutputDebugInformationLog();

  ///
  FORCEINLINE const aiScene* pGetModelGeometryResource() const noexcept
  {
    return this->mInternalModelGeometryResource;
  }

  std::string                           mModelName            = "";
  std::string                           mModelRootPath        = "";
  std::vector<DDySubmeshInformation>    mSubmeshInformations  = {};
  std::vector<std::string>              mBindedMaterialName   = {};
  std::vector<std::string>              mTextureLocalPaths    = {};

  // Added 2018-09-14
  std::unordered_map<std::string, TU32>   mBoneStringBoneIdMap            = {};
  std::vector<DDyGeometryBoneInformation> mOverallModelBoneInformations   = {};
  int32_t                                 mModelBoneTotalCount            = 0;
  const aiScene*                          mInternalModelGeometryResource  = nullptr;
  DDyMatrix4x4                            mGlobalInverseTransform         = {};

  //!
  //! Resource pointers binding
  //!

  void __pfSetModelResourceLink(CDyModelResource* ptr) const noexcept { mLinkedModelResourcePtr = ptr; }
  mutable CDyModelResource* mLinkedModelResourcePtr = nullptr;

  friend class CDyModelResource;
  friend class MDyHeapResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_MODEL_INFORMATION_H