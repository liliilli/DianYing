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

#include <array>
#include "Data_Vector3.h"
#include "Data_Vector4.h"
#include "Data_Matrix4.h"
#include "Data_VectorInt4.h"

/// @struct DDyVertexInformation
/// @brief Vertex information (Input assembly unit data)
struct DDyVertexInformation final
{
  DDyVector3          mPosition   = {};
  DDyVector3          mNormal     = {};
  DDyVector2          mTexCoords0 = {};
  DDyVector2          mTexCoords1 = {};
  // Tangenet and Bitangent if exist.
  DDyVector3          mTangent    = {};
  DDyVector3          mBitangent  = {};
  // Below elements are used when skeletal animation is applied.
  DDyVectorInt4       mBoneId     = {-1, -1, -1, -1};
  DDyVector4          mWeights    = { 0,  0,  0,  0};
};

void to_json(nlohmann::json& j, const DDyVertexInformation& p);
void from_json(const nlohmann::json& j, DDyVertexInformation& p);

/// @struct DDyGeometryBoneInformation
/// @brief bone matrix information structure.
struct DDyGeometryBoneInformation final
{
  DDyMatrix4x4        mBoneOffsetMatrix     = {};
  DDyMatrix4x4        mFinalTransformation  = DDyMatrix4x4::IdentityMatrix();
};

void to_json(nlohmann::json& j, const DDyGeometryBoneInformation& p);
void from_json(const nlohmann::json& j, DDyGeometryBoneInformation& p);

struct DMesh final
{
  std::vector<DDyVertexInformation> mVertexList;
  std::vector<unsigned int>         mIndexList;
};

void to_json(nlohmann::json& j, const DMesh& p);
void from_json(const nlohmann::json& j, DMesh& p);

