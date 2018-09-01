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

class DDyMeshInformation final
{
public:
  DDyMeshInformation(const PMeshInformationDescriptor& meshInformation) :
      mMeshResourceInformation(meshInformation)
  {};

  DDyMeshInformation(const DDyMeshInformation&)             = delete;
  DDyMeshInformation& operator=(const DDyMeshInformation&)  = delete;
  DDyMeshInformation(DDyMeshInformation&&)                  = default;
  DDyMeshInformation& operator=(DDyMeshInformation&&)       = default;

  ///
  /// @brief return immutable descriptor information reference.
  ///
  const PMeshInformationDescriptor& GetInformation() const noexcept
  {
    return this->mMeshResourceInformation;
  }

private:
  PMeshInformationDescriptor mMeshResourceInformation;
  DBufferIdInformation       mBufferIdInformation;
  DMeshFlagInformation       mMeshFlagInformation;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INFORMATION_MESH_INFORMATION_H