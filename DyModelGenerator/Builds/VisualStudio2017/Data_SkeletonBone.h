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

#include <string>
#include <assimp/scene.h>
#include "Data_Matrix4.h"
#include "HelperPointer.h"

struct DSkeletonBone final
{
  /// @brief Bone specifier name.
  std::string   mSpecifier = "";
  /// @brief parent bone index of this bone within `mExportedSkeleton` list.
  int           mParentSkeletonNodeIndex = -1;
  /// @brief
  int           mBoneOffsetId = -1;
  /// @brief Local trasform of this bone which is relative to parent.
  DDyMatrix4x4  mLocalTransform = DDyMatrix4x4::IdentityMatrix();
  /// @brief This variable is only used in `mPtrAiNode`.
  const aiNode* mPtrAiNode = nullptr;
};

struct DDyBoneOffset
{
  // @brief
  std::string   mBoneName = "";
  // @brief
  int           mIndexSkeletonNode = -1;
  /// @brief Offset matrix of bone.
  DDyMatrix4x4  mBoneOffsetMatrix = DDyMatrix4x4::IdentityMatrix();
};

struct DDySkeleton final
{
  // @brief
  DDyMatrix4x4                mSkeletonRootInverseTransform;
  // @brief
  std::vector<DSkeletonBone>  mExportedSkeleton;
  // @brief
  std::vector<DDyBoneOffset>  mBoneOffsetList;
};

/// @brief Serialization function. `mPtrAiNode` will be neglected.
void to_json(nlohmann::json& j, const DSkeletonBone& p);
/// @brief Deserialization function.
void from_json(const nlohmann::json& j, DSkeletonBone& p);

/// @brief Serialization function. 
void to_json(nlohmann::json& j, const DDyBoneOffset& p);
/// @brief Deserialization function.
void from_json(const nlohmann::json& j, DDyBoneOffset& p);

/// @brief Serialization function. 
void to_json(nlohmann::json& j, const DDySkeleton& p);
/// @brief Deserialization function.
void from_json(const nlohmann::json& j, DDySkeleton& p);
