#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_HFInformationMesh_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_HFInformationMesh_H
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
#include <Dy/Core/Resource/Type/ModelMesh/DMeshData.h>

namespace dy
{

/// @class FInformationMesh
/// @brief Mesh information instance.
class FInformationMesh final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FInformationMesh);

  /// @brief Construct model information.
  FInformationMesh(_MIN_ const PDyMeshInstanceMetaInfo& metaInfo);
  ~FInformationMesh() = default;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get submesh information list.
  MDY_NODISCARD const DMeshData& GetMeshInformationList() const noexcept;

private:
  std::string mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  DMeshData mProperty;
};


} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_HFInformationMesh_H