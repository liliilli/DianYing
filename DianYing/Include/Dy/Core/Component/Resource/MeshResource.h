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

#include <Dy/Core/Component/Information/MeshInformation.h>

//!
//! Forward declration
//!

namespace dy
{

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyMeshResource
/// @brief
///
class CDyMeshResource
{
public:
  CDyMeshResource() = default;
  CDyMeshResource(const CDyMeshResource&)             = delete;
  CDyMeshResource(CDyMeshResource&&)                  = default;
  CDyMeshResource& operator=(const CDyMeshResource&)  = delete;
  CDyMeshResource& operator=(CDyMeshResource&&)       = default;
  ~CDyMeshResource();

  ///
  /// @brief
  ///
  bool IsEnabledIndices() const noexcept;

  ///
  /// @brief
  ///
  int32_t GetVertexArrayId() const noexcept;

  ///
  /// @brief
  ///
  int32_t GetVertexCounts() const noexcept;

  ///
  /// @brief
  ///
  int32_t GetIndicesCounts() const noexcept;

private:
  ///
  /// @brief
  ///
  [[nodiscard]]
  EDySuccess pfInitializeMesh(const DDyMeshInformation& meshInformation) noexcept;

  DBufferIdInformation       mBufferIdInformation     = {};
  DMeshFlagInformation       mMeshFlagInformation     = {};

  friend class CDyModelResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_MESH_RESOURCE_H