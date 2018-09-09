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
#include <Dy/Core/Component/Information/MeshInformation.h>
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
class CDyTextureInformation;
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
    return this->mMeshInformations;
  }

  ///
  /// @brief Check if object is being binded to CDyModelResource instance.
  ///
  FORCEINLINE bool IsBeingBinded() const noexcept
  {
    return this->mNextLevelPtr != nullptr;
  }

private:
  ///
  /// @brief Process assimp node so get information and resource from aiNode by iterating
  /// submesh and children meshes.
  ///
  void __pProcessAssimpNode(aiNode* node, const aiScene* scene);

  ///
  /// @brief Process aiMesh, make mesh information description which stores vertex, indices,
  /// innate material information, etc.
  ///
  void __pProcessAssimpMesh(aiMesh* mesh, const aiScene* scene);

  EDySuccess __pLoadVertexData(const aiMesh* mesh, PMeshInformationDescriptor& desc);

  EDySuccess __pLoadIndiceData(const aiMesh* mesh, PMeshInformationDescriptor& desc);

  std::optional<PDyMaterialConstructionDescriptor> __pReadMaterialData(const aiMaterial* material);

  ///
  /// @brief
  ///
  std::optional<std::vector<std::string>> __pLoadMaterialTextures(const aiMaterial* material, EDyTextureMapType type);

  ///
  /// @brief Output information log only in debug mode.
  ///
  void __pOutputDebugInformationLog();

  std::string                       mModelName          = "";
  std::string                       mModelRootPath      = "";
  std::vector<DDyMeshInformation>   mMeshInformations   = {};
  std::vector<std::string>          mBindedMaterialName = {};
  std::vector<std::string>          mTextureLocalPaths  = {};

  //!
  //! Level pointers binding
  //!

  void __pfSetNextLevel(CDyModelResource* ptr) const noexcept { mNextLevelPtr = ptr; }
  mutable CDyModelResource* mNextLevelPtr = nullptr;

  friend class CDyModelResource;
  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_MODEL_INFORMATION_H