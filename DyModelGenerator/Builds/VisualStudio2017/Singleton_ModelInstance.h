#pragma once
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <assimp/Importer.hpp>
#include "Interface_Singleton.h"
#include "HelperPointer.h"
#include "Data_AssimpModelNode.h"
#include "Data_MeshData.h"
#include "Enum_ExportFlags.h"

struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiAnimation;
struct aiTexture;

struct Singleton_ModelInstance : public Interface_Singleton<Singleton_ModelInstance>
{
public:
  MDY_SINGLETON_PROPERTIES(Singleton_ModelInstance);
  MDY_SINGLETON_DERIVED(Singleton_ModelInstance);

  /// @brief Read model with assimp using full file path.
  MDY_NODISCARD EDySuccess ReadModelWithPath(const std::string& iPath);
  /// @brief Export mesh with specifier name and mesh index.
  MDY_NODISCARD EDySuccess ExportModelMesh(const std::string& iSpecifier, unsigned iMeshIndex, bool isCompressed = false);
  /// @brief Release model instance.
  void ReleaseModel();

  /// @brief Get pointer of model importer instance. If not valid, it just return nullptr;
  MDY_NODISCARD const Assimp::Importer* GetPtrModelImporter() const noexcept;
  /// @brief Get pointer of model scene, if not valid it just return nullptr.
  MDY_NODISCARD const aiScene* GetPtrModelScene() const noexcept;
  /// @brief Get root node of model main scene.
  MDY_NODISCARD const aiNode* GetPtrRootNodeOfModelScene() const noexcept;

  /// @brief Get model-file full path. If not valid, it just return empty string.
  MDY_NODISCARD const std::string& GetModelFileFullPath() const noexcept;
  /// @brief Get the number of model meshes.
  MDY_NODISCARD unsigned GetNumModelMeshes() const noexcept;
  /// @brief Get exported mesh specifier name, (iSpecifier + `innate_mesh_name`).
  MDY_NODISCARD std::string GetExportedMeshSpecifierName(const std::string& iSpecifier, unsigned iMeshIndex);

  /// @brief Check model has a bone. If model has not been loaded yet, it just return false.
  MDY_NODISCARD bool IsModelHasBones() const noexcept;

  /// @brief Set export flag.
  void SetExportFlag(EExportFlags iFlags, bool isActivated);

private:
  /// @brief Create and return `DyMesh` with iMeshIndex. 
  /// This function does not check oob of given mesh vector.
  MDY_NODISCARD DMesh CreateDyMesh(unsigned iMeshIndex);

  std::string mModelFileFullPath = "";

  std::unique_ptr<Assimp::Importer>         mAssimpModerImporter  = nullptr;
  std::vector<NotNull<const aiMesh*>>       mPtrAssimpModelMeshList;
  std::vector<NotNull<const aiMaterial*>>   mPtrAssimpModelMaterialList;
  std::vector<NotNull<const aiAnimation*>>  mPtrAssimpModelAnimList;
  std::vector<NotNull<const aiTexture*>>    mPtrAssimpModelTextureList;
  std::unique_ptr<Data_AssimpModelNode>     mAssimpModeNode = nullptr;

  EExportFlags mExportFlags = EExportFlags::Flag_None;
};
