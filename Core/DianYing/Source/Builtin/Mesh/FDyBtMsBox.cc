#include <precompiled.h>
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

/// Header file
#include <Dy/Builtin/Mesh/FDyBtMsBox.h>

namespace dy
{

void FDyBtMsBox::ConstructBuffer(_MOUT_ PDyMeshInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName = sName;

  auto& submeshDesc = property.mDefaultMeshBuffer;
  submeshDesc.mVertexList.reserve(24);
  {
    DDefaultVertexInfo vertex;

    // Front and back
    vertex.mPosition = DVec3{ 1,  1,  1 };
    vertex.mNormal = DVec3::UnitZ();
    vertex.mTexCoord0 = DVec2{ 1, 1 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ 1, -1,  1 };
    vertex.mNormal = DVec3::UnitZ();
    vertex.mTexCoord0 = DVec2{ 1, 0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1, -1,  1 };
    vertex.mNormal = DVec3::UnitZ();
    vertex.mTexCoord0 = DVec2{ 0, 0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1,  1,  1 };
    vertex.mNormal = DVec3::UnitZ();
    vertex.mTexCoord0 = DVec2{ 0, 1 };
    submeshDesc.mVertexList.emplace_back(vertex);

    vertex.mPosition = DVec3{ -1,  1, -1 };
    vertex.mNormal = DVec3{ 0,  0, -1 };
    vertex.mTexCoord0 = DVec2{ 1,  1 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1, -1, -1 };
    vertex.mNormal = DVec3{ 0,  0, -1 };
    vertex.mTexCoord0 = DVec2{ 1,  0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ 1, -1, -1 };
    vertex.mNormal = DVec3{ 0,  0, -1 };
    vertex.mTexCoord0 = DVec2{ 0,  0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ 1,  1, -1 };
    vertex.mNormal = DVec3{ 0,  0, -1 };
    vertex.mTexCoord0 = DVec2{ 0,  1 };
    submeshDesc.mVertexList.emplace_back(vertex);

    // Up and Down
    vertex.mPosition = DVec3{ 1,  1, -1 };
    vertex.mNormal = DVec3::UnitY();
    vertex.mTexCoord0 = DVec2{ 1, 1 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ 1,  1,  1 };
    vertex.mNormal = DVec3::UnitY();
    vertex.mTexCoord0 = DVec2{ 1, 0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1,  1,  1 };
    vertex.mNormal = DVec3::UnitY();
    vertex.mTexCoord0 = DVec2{ 0, 0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1,  1, -1 };
    vertex.mNormal = DVec3::UnitY();
    vertex.mTexCoord0 = DVec2{ 0, 1 };
    submeshDesc.mVertexList.emplace_back(vertex);

    vertex.mPosition = DVec3{ 1, -1,  1 };
    vertex.mNormal = DVec3{ 0, -1,  0 };
    vertex.mTexCoord0 = DVec2{ 1,  1 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ 1, -1, -1 };
    vertex.mNormal = DVec3{ 0, -1,  0 };
    vertex.mTexCoord0 = DVec2{ 1,  0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1, -1, -1 };
    vertex.mNormal = DVec3{ 0, -1,  0 };
    vertex.mTexCoord0 = DVec2{ 0,  0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1, -1,  1 };
    vertex.mNormal = DVec3{ 0, -1,  0 };
    vertex.mTexCoord0 = DVec2{ 0,  1 };
    submeshDesc.mVertexList.emplace_back(vertex);

    // Left and right
    vertex.mPosition = DVec3{ 1,  1, -1 };
    vertex.mNormal = DVec3::UnitX();
    vertex.mTexCoord0 = DVec2{ 1, 1 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ 1, -1, -1 };
    vertex.mNormal = DVec3::UnitX();
    vertex.mTexCoord0 = DVec2{ 1, 0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ 1, -1,  1 };
    vertex.mNormal = DVec3::UnitX();
    vertex.mTexCoord0 = DVec2{ 0, 0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ 1,  1,  1 };
    vertex.mNormal = DVec3::UnitX();
    vertex.mTexCoord0 = DVec2{ 0, 1 };
    submeshDesc.mVertexList.emplace_back(vertex);

    vertex.mPosition = DVec3{ -1,  1,  1 };
    vertex.mNormal = DVec3{ -1,  0,  0 };
    vertex.mTexCoord0 = DVec2{ 1,  1 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1,  1, -1 };
    vertex.mNormal = DVec3{ -1,  0,  0 };
    vertex.mTexCoord0 = DVec2{ 1,  0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1, -1, -1 };
    vertex.mNormal = DVec3{ -1,  0,  0 };
    vertex.mTexCoord0 = DVec2{ 0,  0 };
    submeshDesc.mVertexList.emplace_back(vertex);
    vertex.mPosition = DVec3{ -1, -1,  1 };
    vertex.mNormal = DVec3{ -1,  0,  0 };
    vertex.mTexCoord0 = DVec2{ 0,  1 };
    submeshDesc.mVertexList.emplace_back(vertex);
  }

  // Make index information
  property.mIndiceBuffer.reserve(36);
  std::array<TU32, 36> indices = {
    0,  3, 1, 1, 3, 2,
    4,  7, 5, 5, 7, 6,
    8, 11, 9, 9,11,10,
    12,15,13,13,15,14,
    16,19,17,17,19,18,
    20,23,21,21,23,22
  };
  for (const auto index : indices) { property.mIndiceBuffer.emplace_back(index); }
}

} /// ::dy namespace