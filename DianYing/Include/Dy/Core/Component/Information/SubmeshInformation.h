#ifndef GUARD_DY_CORE_COMPONENT_INFORMATION_MESH_INFORMATION_H
#define GUARD_DY_CORE_COMPONENT_INFORMATION_MESH_INFORMATION_H

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

#include <Dy/Helper/Geometry/GeometryType.h>

namespace dy
{

///
/// @class DDySubmeshInformation
/// @brief Store and manage submesh information.
///
class DDySubmeshInformation final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(DDySubmeshInformation);
  DDySubmeshInformation(const PDySubmeshInformationDescriptor& meshInformation) : mMeshResourceInformation(meshInformation) {};
  ~DDySubmeshInformation()                                        = default;

  ///
  /// @brief return immutable descriptor information reference.
  ///
  [[nodiscard]]
  FORCEINLINE const PDySubmeshInformationDescriptor& GetInformation() const noexcept
  {
    return this->mMeshResourceInformation;
  }

private:
  PDySubmeshInformationDescriptor mMeshResourceInformation;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_MESH_INFORMATION_H