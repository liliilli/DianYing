#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationMesh_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationMesh_H
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

#include <Dy/Core/Resource/Internal/XGeometryTypes.h>
#include <Dy/Meta/Information/MetaInfoModel.h>
#include <Dy/Core/Resource/Type/TInformationBinder.h>

namespace dy
{

/// @class FInformationModel
/// @brief Model information instance.
class FInformationModel final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FInformationModel);

  /// @brief Construct model information.
  FInformationModel(_MIN_ const PDyModelInstanceMetaInfo& metaInfo);
  ~FInformationModel() = default;

  /// @brief Get model specifier name.
  FORCEINLINE MDY_NODISCARD const std::string& GetSpecifierName() const noexcept
  {
    return this->mSpecifierName;
  }

  /// @brief Get binded Mesh information list.
  MDY_NODISCARD const auto& GetMeshInformationList() const noexcept
  {
    return this->mMeshInformations;
  }

  /// @brief Get binded Material information list.
  MDY_NODISCARD const auto& GetMaterialInformationList() const noexcept
  {
    return this->mMaterialInformations;
  }

  /// @brief Get initial transform of model.
  MDY_NODISCARD const auto& GetInitialTransform() const noexcept
  {
    return this->mModelTransform;
  }

  /// @brief Get instancing flags list.
  MDY_NODISCARD const std::vector<bool>& GetInstancingFlags() const noexcept;

private:
  std::string mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  std::vector<std::unique_ptr<TDyInformationBinderMesh>>     mMeshInformations;
  std::vector<std::unique_ptr<TDyInformationBinderMaterial>> mMaterialInformations;
  std::vector<bool> mPairInstancingflag;

  /// Valid skeleton reference handle.
  TDyInformationBinderSkeleton         mBinderSkeleton;
  PDyModelInstanceMetaInfo::DTransform  mModelTransform;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationMesh_H
