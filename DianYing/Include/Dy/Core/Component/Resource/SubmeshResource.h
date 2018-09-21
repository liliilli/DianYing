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

#include <Dy/Core/Component/Information/SubmeshInformation.h>

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDySubmeshResource
/// @brief Submesh resource type which manages submesh resource.
///
class CDySubmeshResource
{
public:
  CDySubmeshResource() = default;
  CDySubmeshResource(const CDySubmeshResource&)             = delete;
  CDySubmeshResource(CDySubmeshResource&&)                  = default;
  CDySubmeshResource& operator=(const CDySubmeshResource&)  = delete;
  CDySubmeshResource& operator=(CDySubmeshResource&&)       = default;
  ~CDySubmeshResource();

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
  EDySuccess pfInitializeSubmeshResource(const DDySubmeshInformation& meshInformation) noexcept;

  DDyGlBufferIdInformation        mBufferIdInformation     = {};
  DDySubmeshFlagInformation       mMeshFlagInformation     = {};

  friend class CDyModelResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_MESH_RESOURCE_H