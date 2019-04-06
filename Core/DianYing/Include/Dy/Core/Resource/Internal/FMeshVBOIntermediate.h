#ifndef GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMESHVBOINTERMEDIATE_H
#define GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMESHVBOINTERMEDIATE_H
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

#include <Dy/Core/Resource/Information/FInformationModel.h>
#include <Dy/Meta/Type/Mesh/DDyGLVaoBindInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class DDySubmeshInformation_Deprecated;
class FInformationMesh;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FMeshVBOIntermediate
/// @brief Model vertex buffer object intermediate instance type. just used for creating FResourceModel.
class FMeshVBOIntermediate final
{
public:
  FMeshVBOIntermediate(const FInformationMesh& information, bool iIsInstancing);
  ~FMeshVBOIntermediate();

  /// @brief Reset all properties not to use this anymore.
  void ResetAllProperties() noexcept;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get vertex buffer id information.
  MDY_NODISCARD const DGlBufferIdInformation& GetBufferIdInfo() const noexcept;

  /// @brief Get mesh flag property instance.
  MDY_NODISCARD const DDySubmeshFlagInformation& GetMeshFlagInfo() const noexcept;

  /// @brief
  MDY_NODISCARD const DDyGLVaoBindInformation& GetVaoBindingInfo() const noexcept;

  /// @brief Check this will support instancing.
  MDY_NODISCARD bool IsSupportingInstancing() const noexcept;
    
private:
  void MDY_PRIVATE(CreateVertexArrayBuffer)(const FInformationMesh& iInformation);
  void MDY_PRIVATE(CreateElementArrayBuffer)(const FInformationMesh& iInformation);

  std::string             mSpecifierName;
  DGlBufferIdInformation  mBufferIdInformation  = {};
  DDySubmeshFlagInformation mMeshFlagInformation  = {};
  DDyGLVaoBindInformation   mVaoBindAttributeInfo = {};

  bool mIsSupportingInstancing = false;
};


} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMESHVBOINTERMEDIATE_H