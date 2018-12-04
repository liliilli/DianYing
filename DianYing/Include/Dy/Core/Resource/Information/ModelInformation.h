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

#include <Dy/Core/Resource/Internal/ModelType.h>
#include <Dy/Core/Resource/Information/SubmeshInformation.h>
#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>

//!
//! Forward declaration
//!

struct  aiMaterial;
struct  aiMesh;
struct  aiNode;
struct  aiScene;

namespace Assimp
{
class Importer;
} /// ::Assimp namespace

namespace dy
{
struct PDyModelInstanceMetaInfo;
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
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(DDyModelInformation);
  DDyModelInformation(const PDyModelInstanceMetaInfo& modelConstructionDescriptor);
  ~DDyModelInformation();

  ///
  /// @brief Return material name list which model information have.
  ///
  [[nodiscard]]
  FORCEINLINE const std::vector<std::string>& GetBindedMaterialNameLists() const noexcept
  {
    return this->mOverallBindedMaterialName;
  }

  ///
  /// @brief Return mesh information list which have material name, vertex, indice etc.
  ///
  [[nodiscard]]
  FORCEINLINE const auto& GetMeshInformation() const noexcept
  {
    return this->mSubmeshInformations;
  }

  ///
  /// @brief Check if object is being binded to CDyModelResource instance.
  ///
  [[nodiscard]]
  FORCEINLINE bool IsBeingBindedToResource() const noexcept
  {
    return this->__mLinkedModelResourcePtr != nullptr;
  }

private:
  ///
  /// @brief Create Animation information, not traversing aiMeshes.
  ///
  void pCreateAnimationInformation(const aiScene& ai_scene);

  ///
  /// @brief
  ///
  void pProcessNode(const aiScene& aiScene, const aiNode& aiNode);

  ///
  /// @brief Process aiMesh, make mesh information description which stores vertex, indices,
  /// innate material information, etc.
  ///
  void __pProcessMeshInformation(const aiScene& aiScene, const aiNode& aiNode, const aiMesh& aiMesh);
  /// Read vertex data, make data, and insert to PDySubmeshInformationDescriptor.
  void __pReadVertexData(const aiMesh& mesh, PDySubmeshInformationDescriptor& desc);
  /// Read bone data.
  void __pReadBoneData(const aiMesh& mesh, PDySubmeshInformationDescriptor& desc);
  /// Read index(element) data, make data, and insert to PDySubmeshInformationDescriptor.
  void __pReadIndiceData(const aiMesh& mesh, PDySubmeshInformationDescriptor& desc);
  /// Read material data and make descriptor.
  PDyMaterialInstanceMetaInfo       __pReadMaterialData(const aiMaterial& material);
  /// Read material texture data and insert texture information to manager.
  std::optional<std::vector<std::string>> __pLoadMaterialTextures(const aiMaterial& material, EDyTextureMapType type);
  /// Read node information from aiScene, and create DMoe's node information.
  void pCreateNodeInformation(const aiNode& aiNode, DMoeBoneNodeInformation& nodeInfo);
  /// Output information log only in debug mode.
  void __pOutputDebugInformationLog();

  std::string                             mModelName                      = "";
  std::string                             mModelRootPath                  = "";

  std::vector<DDySubmeshInformation>      mSubmeshInformations            = {};
  std::vector<std::string>                mOverallBindedMaterialName      = {};
  std::vector<std::string>                mOverallTextureLocalPaths       = {};

  std::unordered_map<std::string, TU32>   mBoneIdMap                      = {};
  std::vector<DDyGeometryBoneInformation> mOverallModelBoneInformations   = {};
  DDyMatrix4x4                            mGlobalTransform                = DDyMatrix4x4::IdentityMatrix();

  std::vector<DMoeAnimationInformation>   mAnimationInformations          = {};
  DMoeBoneNodeInformation                 mRootBoneNode;

  // Added 2018-09-19
  std::atomic<bool>                       mModelInformationLoaded         = false;

  //!
  //! Resource pointers binding
  //!

  FORCEINLINE void __pfSetModelResourceLink(NotNull<CDyModelResource*> ptr) const noexcept
  {
    __mLinkedModelResourcePtr = ptr;
  }
  FORCEINLINE void __pfResetModelResourceLink() const noexcept
  {
    this->__mLinkedModelResourcePtr = nullptr;
  }

  MDY_TRANSIENT CDyModelResource* __mLinkedModelResourcePtr = nullptr;

  friend class CDyModelResource;
  friend class MDyIOResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_MODEL_INFORMATION_H