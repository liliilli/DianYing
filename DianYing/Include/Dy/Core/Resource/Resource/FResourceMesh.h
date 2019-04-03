#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H
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
class FMeshVBOIntermediate;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FResourceMesh
/// @brief Resource type of actual mesh. Stores internal GL ids (VBO, EBO, instancing, and VAO)
class FResourceMesh final
{
public:
  /// @brief
  FResourceMesh(FMeshVBOIntermediate& ioIntermediateInstance);
  ~FResourceMesh();

  /// @brief Get mesh specifier name.
  const std::string& GetSpecifierName() const noexcept;

  /// @brief Check whether indice (element) vertexes are binded, so be able to element drawing.
  bool IsEnabledIndices() const noexcept;

  /// @brief Get VAO id
  TU32 GetVertexArrayId() const noexcept;

  /// @brief Get Vertex Buffer (VBO) id
  TU32 GetVertexBufferId() const noexcept;  

  /// @brief Get Vertex's (one patch) all counts. If not exist, just return 0.
  TU32 GetVertexCounts() const noexcept;

  /// @brief Get Indices count. If not exist, just return 0.
  TU32 GetIndicesCounts() const noexcept;

  /// @brief Bind Vertex Array object. if vao id is 0, return DY_FAILURE with doing nothing. 
  EDySuccess BindVertexArray() const noexcept;

  /// @brief Check this mesh is supporting instancing.
  bool IsSupportingInstancing() const noexcept;

  /// @brief Get Instancing buffer when exist.
  MDY_NODISCARD std::optional<TU32> GetInstancingBufferId() const noexcept;

private:
  std::string mSpecifierName;
  DGlBufferIdInformation    mBufferIdInformation  = {};
  DDySubmeshFlagInformation mMeshFlagInformation  = {};
  DDyGLVaoBindInformation   mBindInformation = {};

  /// @brief Instancing buffer id for only used when instancing,
  std::optional<TU32> mInstancingBufferId;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H