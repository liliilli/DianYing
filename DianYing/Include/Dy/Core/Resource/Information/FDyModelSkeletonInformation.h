#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDyModelSkeletonInformation_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDyModelSkeletonInformation_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Core/Resource/Type/ModelMesh/DDySkeletonBone.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyModelSkelInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyModelSkeletonInformation
/// @brief Model skeleton information instance.
///
class FDyModelSkeletonInformation final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyModelSkeletonInformation);

  /// @brief Construct model information.
  FDyModelSkeletonInformation(_MIN_ const PDyModelSkelInstanceMetaInfo& metaInfo);
  ~FDyModelSkeletonInformation() = default;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get the number of skeleton bone.
  MDY_NODISCARD TU32 GetBoneCount() const noexcept;

  /// @brief Get const reference of skeleton bone. \n
  /// If iIndex is out of bound of range, just assert it, so you have to check oob range with `GetBoneCount()`. 
  MDY_NODISCARD const DDySkeletonBone& GetRefBone(_MIN_ TU32 iIndex) const noexcept;

  /// @brief
  MDY_NODISCARD std::vector<TU32> GetChildrenBoneIdList(_MIN_ TI32 iIndex) const noexcept;

  /// @brief Get inverse transform of root-node.
  MDY_NODISCARD const DDyMatrix4x4& GetRootInverseTransform() const noexcept;

private:
  std::string       mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  DDyMatrix4x4      mRootInvTransform = {};
  TDyModelSkeleton  mSkeletonBoneList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDyModelSkeletonInformation_H