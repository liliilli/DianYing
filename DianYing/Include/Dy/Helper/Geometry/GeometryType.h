#ifndef GUARD_DY_HELPER_GEOMETRY_TYPES_H
#define GUARD_DY_HELPER_GEOMETRY_TYPES_H
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

#include <vector>
#include <Dy/Helper/Type/Vector3.h>

namespace dy
{

///
/// @class DDyVertexInformation
/// @brief Vertex information (Input assembly unit data)
///
struct DDyVertexInformation final
{
  DDyVector3 mPosition;
  DDyVector3 mNormal;
  DDyVector2 mTexCoords;
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
struct PDySubmeshInformationDescriptor final
{
  std::vector<DDyVertexInformation> mVertices;
  std::vector<int32_t>              mIndices;
  std::string                       mMaterialName = "";
};

///
/// @struct DDySubmeshFlagInformation
/// @brief Submesh information which is not releated to mesh properties directly.
///
struct DDySubmeshFlagInformation final
{
  int32_t                         mIndiceCount      = 0;
  int32_t                         mVertexSize       = 0;
  bool                            mIsNotHaveIndices :1;
  bool                            mIsNotHaveTextures:1;

  DDySubmeshFlagInformation() : mIsNotHaveIndices{false}, mIsNotHaveTextures{false} {};
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_GEOMETRY_TYPES_H