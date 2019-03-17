#ifndef GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDySkeletonBone_H
#define GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDySkeletonBone_H
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

#include <Dy/Helper/Type/DMatrix4x4.h>

namespace dy
{

/// @struct DSkeletonBone
/// @brief Skeleton bone type.
struct DSkeletonBone final
{
  /// @brief Bone specifier name.
  std::string mSpecifier = "";
  /// @brief parent bone index of this bone within `mExportedSkeleton` list.
  int         mParentSkeletonBoneIndex = -1;
  /// @brief
  int         mBoneOffsetId = -1;
  /// @brief Local trasform of this bone which is relative to parent.
  DMatrix4x4  mLocalTransform = DMatrix4x4::IdentityMatrix();
};

struct DBoneOffset
{
  // @brief
  std::string   mBoneName = "";
  // @brief
  int           mIndexSkeletonNode = -1;
  /// @brief Offset matrix of bone.
  DMatrix4x4  mBoneOffsetMatrix = DMatrix4x4::IdentityMatrix();
};

struct DSkeleton final
{
  // @brief
  DMatrix4x4                mSkeletonRootInverseTransform;
  // @brief
  std::vector<DSkeletonBone>mExportedSkeleton;
  // @brief
  std::vector<DBoneOffset>  mBoneOffsetList;
};

/// @brief Serialization function. `mPtrAiNode` will be neglected.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DSkeletonBone& p);
/// @brief Deserialization function.
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DSkeletonBone& p);

/// @brief Deserialization function.
void from_json(const nlohmann::json& j, DBoneOffset& p);

/// @brief Deserialization function.
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DSkeleton& p);

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDySkeletonBone_H
