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
  /// @brief
  ///
  const std::vector<std::string>& GetBindedMaterialNameLists() const noexcept
  {
    return this->mBindedMaterialName;
  }

private:
  ///
  /// @brief
  ///
  void __pProcessNode(aiNode* node, const aiScene* scene);

  ///
  /// @brief
  ///
  void __pProcessMesh(aiMesh* mesh, const aiScene* scene);

  ///
  /// @brief
  ///
  std::optional<std::vector<std::string>> __pLoadMaterialTextures(aiMaterial* material, EDyTextureMapType type);

  std::string                       mModelRootPath      = "";
  std::vector<DDyMeshInformation>   mMeshInformations;
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