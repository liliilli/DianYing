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
#include <Dy/Meta/Type/Mesh/DDyGLVaoBindInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class DDySubmeshInformation_Deprecated;
class FDyMeshInformation;
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
  FDyMeshVBOIntermediate(_MIN_ const FDyMeshInformation& information);
  ~FDyMeshVBOIntermediate();

  /// @brief Reset all properties not to use this anymore.
  void ResetAllProperties() noexcept;

  /// @brief Get model specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Get vertex buffer id information.
  MDY_NODISCARD const DDyGlBufferIdInformation& GetBufferIdInfo() const noexcept;

  /// @brief Get mesh flag property instance.
  MDY_NODISCARD const DDySubmeshFlagInformation& GetMeshFlagInfo() const noexcept;

  /// @brief
  MDY_NODISCARD const DDyGLVaoBindInformation& GetVaoBindingInfo() const noexcept;
    
private:
  void MDY_PRIVATE_SPECIFIER(CreateVertexArrayBuffer)(_MIN_ const FDyMeshInformation& iInformation);
  void MDY_PRIVATE_SPECIFIER(CreateElementArrayBuffer)(_MIN_ const FDyMeshInformation& iInformation);

  std::string               mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  DDyGlBufferIdInformation  mBufferIdInformation  = {};
  DDySubmeshFlagInformation mMeshFlagInformation  = {};
  DDyGLVaoBindInformation   mVaoBindAttributeInfo = {};
};


} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INTERNAL_FDYMESHVBOINTERMEDIATE_H