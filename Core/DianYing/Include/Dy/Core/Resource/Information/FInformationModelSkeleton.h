#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationModelSkeleton_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationModelSkeleton_H
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

#include <Dy/Core/Resource/Type/ModelMesh/DSkeletonBones.h>

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

/// @class FInformationModelSkeleton
/// @brief Model skeleton information instance.
class FInformationModelSkeleton final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FInformationModelSkeleton);

  /// @brief Construct model information.
  FInformationModelSkeleton(_MIN_ const PDyModelSkelInstanceMetaInfo& metaInfo);
  ~FInformationModelSkeleton() = default;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get the number of skeleton bone.
  MDY_NODISCARD TU32 GetNodeCount() const noexcept;
  /// @brief Get the number of offset + transform input bone count.
  MDY_NODISCARD TU32 GetInputBoneCount() const noexcept;

  /// @brief Get const reference of skeleton bone. \n
  /// If iIndex is out of bound of range, just assert it, so you have to check oob range with `GetNodeCount()`. 
  MDY_NODISCARD const DSkeletonBone& GetRefSkeletonNode(_MIN_ TU32 iIndex) const noexcept;

  /// @brief
  MDY_NODISCARD std::vector<TU32> GetChildrenNodeIdList(_MIN_ TI32 iIndex) const noexcept;

  /// @brief Get inverse transform of root-node.
  MDY_NODISCARD const DMat4& GetRootInverseTransform() const noexcept;
  /// @brief
  MDY_NODISCARD const DMat4& GetOffsetMatrixOfBone(_MIN_ TU32 iIndex) const noexcept;
  /// @brief
  MDY_NODISCARD const DSkeletonBone& GetRefSkeletonNodeFromBoneOffsetId(_MIN_ TU32 iIndex) const noexcept;
  /// @brief
  MDY_NODISCARD const TI32 GetSkeletonNodeIdFromBoneOffsetId(_MIN_ TU32 iIndex) const noexcept;

  /// @brief
  MDY_NODISCARD const std::vector<DBoneOffset>& GetOffsetBoneList() const noexcept;

private:
  std::string       mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  DSkeleton       mSkeletonInfo = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationModelSkeleton_H