#ifndef GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMODELVBOINTERMEDIATED_H
#define GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMODELVBOINTERMEDIATED_H
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

#include <Dy/Core/Resource/Internal/FDyMeshVBOIntermediate.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyModelInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyModelVBOIntermediate
/// @brief Model vertex buffer object intermediate instance type. just used for creating FDyModelResource.
///
class FDyModelVBOIntermediate final
{
public:
  FDyModelVBOIntermediate(_MIN_ const FDyModelInformation& information);

  /// @brief Get intermediate mesh resource list which should be adjustable.
  MDY_NODISCARD std::vector<FDyMeshVBOIntermediate>& GetIntermediateMeshList() noexcept
  {
    return this->mIntermediateMeshResourceList;
  }

private:
  /// Intermediate mesh resources are not bound by VAO yet.
  std::vector<FDyMeshVBOIntermediate> mIntermediateMeshResourceList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMODELVBOINTERMEDIATED_H