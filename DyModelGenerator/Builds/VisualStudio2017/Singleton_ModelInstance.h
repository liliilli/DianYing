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
  /// @brief Release model instance.
  void ReleaseModel();

  /// @brief Get pointer of model importer instance. If not valid, it just return nullptr;
  const Assimp::Importer* GetPtrModelImporter() const noexcept;
  /// @brief Get pointer of model scene, if not valid it just return nullptr.
  const aiScene* GetPtrModelScene() const noexcept;
  /// @brief Get root node of model main scene.
  const aiNode* GetPtrRootNodeOfModelScene() const noexcept;

private:
  std::unique_ptr<Assimp::Importer>         mAssimpModerImporter  = nullptr;
  std::vector<NotNull<const aiMesh*>>       mPtrAssimpModelMeshList;
  std::vector<NotNull<const aiMaterial*>>   mPtrAssimpModelMaterialList;
  std::vector<NotNull<const aiAnimation*>>  mPtrAssimpModelAnimList;
  std::vector<NotNull<const aiTexture*>>    mPtrAssimpModelTextureList;
  std::unique_ptr<Data_AssimpModelNode>     mAssimpModeNode = nullptr;
};
