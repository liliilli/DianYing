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

#include <Dy/Helper/Type/Matrix4.h>

namespace dy
{

/// @struct DDySkeletonBone
/// @brief Skeleton bone type.
struct DDySkeletonBone final
{
  /// @brief Bone specifier name.
  std::string   mSpecifier = "";
  /// @brief parent bone index of this bone within `mExportedSkeleton` list.
  int           mParentSkeletonBoneIndex = -1;
  /// @brief Local trasform of this bone which is relative to parent.
  DDyMatrix4x4  mLocalTransform = DDyMatrix4x4::IdentityMatrix();
  /// @brief Global and calculated transform.
  DDyMatrix4x4  mGlobalTransform = DDyMatrix4x4::IdentityMatrix();
  /// @brief Offset matrix of bone.
  DDyMatrix4x4  mOffsetMatrix = DDyMatrix4x4::IdentityMatrix();
};

/// @brief One model skeleton complete type.
using TDyModelSkeleton = std::vector<DDySkeletonBone>;

/// @brief Serialization function. `mPtrAiNode` will be neglected.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDySkeletonBone& p);
/// @brief Deserialization function.
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDySkeletonBone& p);

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDySkeletonBone_H
