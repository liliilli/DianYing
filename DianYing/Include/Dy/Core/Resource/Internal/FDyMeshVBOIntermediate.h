#ifndef GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMESHVBOINTERMEDIATE_H
#define GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMESHVBOINTERMEDIATE_H
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

#include <Dy/Core/Resource/Information/FDyModelInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class DDySubmeshInformation_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyMeshVBOIntermediate
/// @brief Model vertex buffer object intermediate instance type. just used for creating FDyModelResource.
///
class FDyMeshVBOIntermediate final
{
public:
  FDyMeshVBOIntermediate(_MIN_ const PDySubmeshInformationDescriptor& information);
  ~FDyMeshVBOIntermediate();

  /// @brief Reset all properties not to use this anymore.
  void ResetAllProperties() noexcept;

  /// @brief
  MDY_NODISCARD const DDyGlBufferIdInformation& GetBufferIdInfo() const noexcept
  {
    return this->mBufferIdInformation;
  }

  /// @brief
  MDY_NODISCARD const DDySubmeshFlagInformation& GetMeshFlagInfo() const noexcept
  {
    return this->mMeshFlagInformation;
  }

private:
  DDyGlBufferIdInformation  mBufferIdInformation = {};
  DDySubmeshFlagInformation mMeshFlagInformation = {};
};


} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMESHVBOINTERMEDIATE_H