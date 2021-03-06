#ifndef GUARD_DY_META_INFORMATION_METAINFOBUILTINMESH_H
#define GUARD_DY_META_INFORMATION_METAINFOBUILTINMESH_H
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

#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Core/Resource/Internal/XGeometryTypes.h>
#include <Dy/Meta/Type/Mesh/DGlVaoBindInformation.h>
#include <Dy/Meta/Information/CommonResourceMetaInfo.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @struct PDyDefaultMeshInformation
/// @brief Defautl buffer vertex list information (new)
///
struct PDyDefaultMeshInformation final
{
  std::vector<DDefaultVertexInfo> mVertexList;
};

/// @enum EDyMeshUsage
/// @brief Specifies usage of given mesh.
enum class EDyMeshUsage : unsigned char
{
  StaticDraw,   // Static draw, can not modified set value.
  DynamicDraw,  // Dynamic draw, The data store contents will be modified repeatedly and used many times.
  StreamDraw    // The data store contents will be modified once and used at most a few times.
};

///
/// @struct PDyMeshInstanceMetaInfo
/// @brief Builtin Mesh meta informtaion instance.
///
struct PDyMeshInstanceMetaInfo final : public PDyCommonResourceMetaInfo
{
  std::string mSpecifierName = MDY_INITIALIZE_EMPTYSTR;

  //! Builtin
  PDyDefaultMeshInformation mDefaultMeshBuffer;
  std::vector<TF32> mCustomMeshBuffer   = {};
  std::vector<TU32> mIndiceBuffer       = {};

  EDyMeshUsage      mMeshUsage          = EDyMeshUsage::StaticDraw;
  DGlVaoBindInformation mVAOBindingInfo;

  //! External (Follows `PDyDefaultMeshInformation` binding and must StaticDraw.)
  std::string       mExternalPath       = MDY_INITIALIZE_EMPTYSTR;
  bool              mIsCompressed       = false;                    
};

/// @brief Serialization function
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyMeshInstanceMetaInfo& p);
/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyMeshInstanceMetaInfo& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_METAINFOBUILTINMESH_H