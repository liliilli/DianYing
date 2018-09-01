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
/// @class DVertexInformation
///
struct DVertexInformation final
{
  DVector3 mPosition;
  DVector3 mNormal;
  DVector2 mTexCoords;
};

///
/// @brief
///
struct DBufferIdInformation final
{
  uint32_t mVao = 0;
  uint32_t mVbo = 0;
  uint32_t mEbo = 0;
};

///
/// @brief
///
struct PMeshInformationDescriptor final
{
  std::vector<DVertexInformation> mVertices;
  std::vector<int32_t>            mIndices;
  std::vector<std::string>        mMaterialNames;
};

///
/// @brief
///
struct DMeshFlagInformation final
{
  bool mIsNotHaveIndices :1;
  bool mIsNotHaveTextures:1;

  DMeshFlagInformation() : mIsNotHaveIndices{false}, mIsNotHaveTextures{false} {};
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_GEOMETRY_TYPES_H