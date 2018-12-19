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
#include <Dy/Builtin/Model/UIFontQuad.h>

namespace dy
{

void FDyBtMdUIFontQuad::ConstructBuffer(_MOUT_ PDyModelConstructionVertexDescriptor& buffer) noexcept
{
  PDySubmeshInformationDescriptor submeshDesc;

  //DDyVector2{}, DDyVector2{1, 1}, DDyVector2{}, DDyVector2{1, 0},
  //DDyVector2{}, DDyVector2{0, 0}, DDyVector2{}, DDyVector2{0, 1}
  // Make position, normal, texcoord vertex information
  submeshDesc.mVertices.reserve(4);
  {
    DDyVertexInformation vertex;
    vertex.mPosition  = DDyVector3{};

    vertex.mTexCoords = DDyVector2{ 1, 1 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mTexCoords = DDyVector2{ 1, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mTexCoords = DDyVector2{ 0, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mTexCoords = DDyVector2{ 0, 1 };
    submeshDesc.mVertices.emplace_back(vertex);
  }

  // Make model information
  buffer.mModelName = sName;
  buffer.mSubmeshConstructionInformations.emplace_back(submeshDesc);
}

} /// ::dy namespace