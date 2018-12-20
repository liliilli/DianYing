#ifndef GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H
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
class FDyMeshVBOIntermediate;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyMeshResource
/// @brief
///
class FDyMeshResource final
{
public:
  /// @brief
  FDyMeshResource(_MINOUT_ FDyMeshVBOIntermediate& intermediateInstance);
  ~FDyMeshResource();

  /// @brief Get mesh specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept;

  /// @brief Check whether indice (element) vertexes are binded, so be able to element drawing.
  MDY_NODISCARD bool IsEnabledIndices() const noexcept;

  /// @brief Get VAO id
  MDY_NODISCARD TU32 GetVertexArrayId() const noexcept;

  /// @brief Get Vertex's (one patch) all counts. If not exist, just return 0.
  MDY_NODISCARD TU32 GetVertexCounts() const noexcept;

  /// @brief Get Indices count. If not exist, just return 0.
  MDY_NODISCARD TU32 GetIndicesCounts() const noexcept;

private:
  std::string               mSpecifierName        = MDY_INITIALIZE_EMPTYSTR;
  DDyGlBufferIdInformation  mBufferIdInformation  = {};
  DDySubmeshFlagInformation mMeshFlagInformation  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_RESOURCE_FDYMESHRESOURCE_H