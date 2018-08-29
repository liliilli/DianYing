#ifndef GUARD_DY_HELPER_GEOMETRY_MESH_H
#define GUARD_DY_HELPER_GEOMETRY_MESH_H
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

#include <Dy/Helper/enum_flags.h>
#include <Dy/Helper/Geometry/Types.h>

namespace dy
{

///
/// @class DDyMeshInformation
/// @brief
///
class DDyMeshInformation
{
public:
  DDyMeshInformation(const PMeshInformationDescriptor& meshInformation);
  DDyMeshInformation(PMeshInformationDescriptor meshInformation);

  DDyMeshInformation(const DDyMeshInformation&) = delete;
  DDyMeshInformation& operator=(const DDyMeshInformation&) = delete;
  DDyMeshInformation(DDyMeshInformation&&) noexcept = default;
  DDyMeshInformation& operator=(DDyMeshInformation&&) noexcept = default;

  ~DDyMeshInformation();

private:
  ///
  /// @brief
  ///
  [[nodiscard]] EDySuccess pSetupMesh() noexcept;

  PMeshInformationDescriptor mMeshResourceInformation;
  DBufferIdInformation       mBufferIdInformation;
  DMeshFlagInformation       mMeshFlagInformation;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_GEOMETRY_MESH_H