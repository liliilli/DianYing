#ifndef GUARD_DY_HELPER_GEOMETRY_TYPES_H
#define GUARD_DY_HELPER_GEOMETRY_TYPES_H
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

#include <array>
#include <vector>

#include <Dy/Helper/Type/DVector4.h>
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Helper/Type/DVectorInt4.h>

namespace dy
{

///
/// @struct DDyVertexBoneData
/// @brief Below elements are used when skeletal animation is applied.
///
struct DDyVertexBoneData final
{
};

///
/// @struct DDyVertexInformation
/// @brief Vertex information (Input assembly unit data)
///
struct DDyVertexInformation final
{
  DVector3          mPosition   = {};
  DVector3          mNormal     = {};
  DVector2          mTexCoord0  = {};
  DVector2          mTexCoord1  = {};
  DVector3          mTangent    = {};
  DVector3          mBitangent  = {};
  // Below elements are used when skeletal animation is applied.
  DVectorInt4       mBoneId     = {-1, -1, -1, -1};
  DVector4          mWeights    = { 0,  0,  0,  0};
};

/// @brief Deserialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDyVertexInformation& p);

///
/// @struct DDyGeometryBoneInformation
/// @brief bone matrix information structure.
///
struct DDyGeometryBoneInformation final
{
  DMatrix4x4        mBoneOffsetMatrix     = {};
  DMatrix4x4        mFinalTransformation  = DMatrix4x4::IdentityMatrix();
};

///
/// @struct DDyGlBufferIdInformation
/// @brief OpenGL Buffer id information structure.
///
struct DDyGlBufferIdInformation final
{
  uint32_t mVao = 0;
  uint32_t mVbo = 0;
  uint32_t mEbo = 0;
};

///
/// @struct PDySubmeshInformationDescriptor
/// @brief Submesh information which is releated to mesh properties directly.
///
struct PDySubmeshInformationDescriptor_Deprecated final
{
  std::vector<DDyVertexInformation> mVertices         = {};
  std::vector<int32_t>              mIndices          = {};
  std::string                       mMaterialName     = "";
  TU32                              mBaseVertices     = 0;
  TU32                              mBaseIndices      = 0;
  DMatrix4x4                      mBaseModelMatrix  = DMatrix4x4::IdentityMatrix();

  bool                              mIsEnabledSkeletalAnimation = false;
  bool                              mIsUsingDefaultBinding      = true;
};

///
/// @struct DDySubmeshFlagInformation
/// @brief Submesh information which is not releated to mesh properties directly.
///
struct DDySubmeshFlagInformation final
{
  int32_t                         mIndiceCount      = 0;
  int32_t                         mVertexCount      = 0;
  bool                            mIsNotHaveIndices :1;
  bool                            mIsNotHaveTextures:1;
  bool                            mIsEnabledSkeletalAnimation:1;

  DDySubmeshFlagInformation() : mIsNotHaveIndices{false}, mIsNotHaveTextures{false}, mIsEnabledSkeletalAnimation{false} {};
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_GEOMETRY_TYPES_H