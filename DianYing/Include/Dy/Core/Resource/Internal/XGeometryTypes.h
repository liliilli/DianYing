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

#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Helper/Type/DVector4.h>
#include <Dy/Helper/Type/DMatrix4x4.h>

namespace dy
{

/// @struct DDefaultVertexInfo
/// @brief Vertex information (Input assembly unit data)
struct DDefaultVertexInfo final
{
  DVec3    mPosition  = {};
  DVec3    mNormal    = {};
  DVec2    mTexCoord0 = {};
  DVec2    mTexCoord1 = {};
  DVec3    mTangent   = {};
  DVec3    mBitangent = {};
  // Below elements are used when skeletal animation is applied.
  DIVec4   mBoneId    = {-1, -1, -1, -1};
  DVec4    mWeights   = {0};
};

/// @brief Deserialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDefaultVertexInfo& p);

/// @struct DGlBufferIdInformation
/// @brief OpenGL Buffer id information structure.
struct DGlBufferIdInformation final
{
  TU32 mVao = 0;
  TU32 mVbo = 0;
  TU32 mEbo = 0;
};

/// @struct DDySubmeshFlagInformation
/// @brief Submesh information which is not releated to mesh properties directly.
struct DDySubmeshFlagInformation final
{
  TI32 mIndiceCount = 0;
  TI32 mVertexCount = 0;
  bool mIsNotHaveIndices :1;
  bool mIsNotHaveTextures:1;
  bool mIsEnabledSkeletalAnimation:1;

  DDySubmeshFlagInformation() 
    : mIsNotHaveIndices{false}, 
      mIsNotHaveTextures{false}, 
      mIsEnabledSkeletalAnimation{false} 
  {};
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_GEOMETRY_TYPES_H