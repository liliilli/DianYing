#ifndef GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyMeshData_H
#define GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyMeshData_H
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

#include <Dy/Meta/Information/MetaInfoModelMesh.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

/// @struct DMeshData
/// @brief Mesh data for storing actual vertex information. 
struct DMeshData final
{
  PDyDefaultMeshInformation mDefaultMeshBuffer;

  std::vector<TF32> mCustomMeshBuffer   = {};
  std::vector<TU32> mIndiceBuffer       = {};
  DDyGLVaoBindInformation mVAOBindingInfo;
  EDyMeshUsage  mMeshUsage = EDyMeshUsage::StaticDraw;
};

/// @brief Deserialization function
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DMeshData& p);

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_MODELMESH_DDyMeshData_H