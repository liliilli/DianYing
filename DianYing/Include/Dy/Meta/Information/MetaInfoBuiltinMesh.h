#ifndef GUARD_DY_META_INFORMATION_METAINFOBUILTINMESH_H
#define GUARD_DY_META_INFORMATION_METAINFOBUILTINMESH_H
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

#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Core/Resource/Internal/GeometryType.h>

namespace dy
{

///
/// @struct PDyDefaultMeshInformation
/// @brief Defautl buffer vertex list information (new)
///
struct PDyDefaultMeshInformation final
{
  std::vector<DDyVertexInformation> mVertexList;
};

///
/// @enum EDyMeshUsage
/// @brief
///
enum class EDyMeshUsage : unsigned char
{
  StaticDraw,
  DynamicDraw,
};

///
/// @struct PDyBtMeshInstanceMetaInfo
/// @brief Builtin Mesh meta informtaion instance.
///
struct PDyBtMeshInstanceMetaInfo final
{
  std::string mSpecifierName = MDY_INITIALIZE_EMPTYSTR;

  PDyDefaultMeshInformation mDefaultMeshBuffer;
  std::vector<TF32> mCustomMeshBuffer   = {};
  std::vector<TU32> mIndiceBuffer       = {};

  bool          mIsUsingDefaultBinding  = true;
  EDyMeshUsage  mMeshUsage              = EDyMeshUsage::StaticDraw;
};

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOBUILTINMESH_H