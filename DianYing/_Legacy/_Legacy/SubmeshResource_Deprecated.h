#ifndef GUARD_DY_CORE_COMPONENT_RESOURCE_MESH_RESOURCE_H
#define GUARD_DY_CORE_COMPONENT_RESOURCE_MESH_RESOURCE_H
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

#include <Dy/Core/Resource/Information_Deprecated/SubmeshInformation_Deprecated.h>

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDySubmeshResource_Deprecated
/// @brief Submesh resource type which manages submesh resource.
///
class CDySubmeshResource_Deprecated
{
public:
  CDySubmeshResource_Deprecated() = default;
  CDySubmeshResource_Deprecated(const CDySubmeshResource_Deprecated&)             = delete;
  CDySubmeshResource_Deprecated(CDySubmeshResource_Deprecated&&)                  = default;
  CDySubmeshResource_Deprecated& operator=(const CDySubmeshResource_Deprecated&)  = delete;
  CDySubmeshResource_Deprecated& operator=(CDySubmeshResource_Deprecated&&)       = default;
  ~CDySubmeshResource_Deprecated();

  ///
  /// @brief Check whether indice (element) vertexes are binded, so be able to element drawing.
  ///
  [[nodiscard]]
  bool IsEnabledIndices() const noexcept;

  ///
  /// @brief Get VAO id
  ///
  [[nodiscard]]
  int32_t GetVertexArrayId() const noexcept;

  ///
  /// @brief Get Vertex's (one patch) all counts. If not exist, just return 0.
  ///
  [[nodiscard]]
  int32_t GetVertexCounts() const noexcept;

  ///
  /// @brief Get Indices count. If not exist, just return 0.
  ///
  [[nodiscard]]
  int32_t GetIndicesCounts() const noexcept;

private:
  ///
  /// @brief Initialize mesh resource with valid submesh information instance.
  ///
  [[nodiscard]]
  EDySuccess pfInitializeSubmeshResource(const DDySubmeshInformation_Deprecated& meshInformation) noexcept;

  DDyGlBufferIdInformation        mBufferIdInformation     = {};
  DDySubmeshFlagInformation       mMeshFlagInformation     = {};

  friend class CDyModelResource_Deprecated;
  friend class FDyModelVBOIntermediate;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_MESH_RESOURCE_H