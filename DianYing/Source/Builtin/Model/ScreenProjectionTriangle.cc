#include <precompiled.h>
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

/// Header file
#include <Dy/Builtin/Model/ScreenProjectionTriangle.h>

namespace dy::builtin
{

void FDyBuiltinModelScreenProjectionTriangle::ConstructBuffer(PDyModelConstructionVertexDescriptor& buffer) noexcept
{
  PDySubmeshInformationDescriptor submeshDesc;

  // Make position, normal, texcoord vertex information
  submeshDesc.mVertices.reserve(3);
  {
    DDyVertexInformation vertex;

    vertex.mPosition  = DDyVector3{ -1, -1, 0 };
    vertex.mNormal    = DDyVector3{ 1 };
    vertex.mTexCoords = DDyVector2{ 0, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition  = DDyVector3{ 3, -1, 0 };
    vertex.mNormal    = DDyVector3{ 1 };
    vertex.mTexCoords = DDyVector2{ 2, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition  = DDyVector3{ -1, 3, 0 };
    vertex.mNormal    = DDyVector3{ 1 };
    vertex.mTexCoords = DDyVector2{ 0, 2 };
    submeshDesc.mVertices.emplace_back(vertex);
  }

  // Make index information
  submeshDesc.mIndices.reserve(3);
  std::array<TU32, 3> indices = { 0, 1, 2 };
  for (const auto index : indices) { submeshDesc.mIndices.emplace_back(index); }

  // Make model information
  buffer.mModelName = FDyBuiltinModelScreenProjectionTriangle::sName;
  buffer.mSubmeshConstructionInformations.emplace_back(submeshDesc);
}

} /// ::dy::builtin namespace