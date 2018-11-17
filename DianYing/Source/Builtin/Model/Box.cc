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
#include <Dy/Builtin/Model/Box.h>
#include <Dy/Core/Component/Internal/ModelType.h>
#include <Dy/Core/Component/Resource/SubmeshResource.h>
#include <Dy/Management/IO/IODataManager.h>

namespace dy::builtin
{

FDyBuiltinModelBox::FDyBuiltinModelBox()
{
  PDySubmeshInformationDescriptor submeshDesc;

  // Make position, normal, texcoord vertex information
  submeshDesc.mVertices.reserve(24);
  {
    DDyVertexInformation vertex;

    // Front and back
    vertex.mPosition = DDyVector3{ 1,  1,  1 };
    vertex.mNormal = DDyVector3::FrontZ();
    vertex.mTexCoords = DDyVector2{ 1, 1 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ 1, -1,  1 };
    vertex.mNormal = DDyVector3::FrontZ();
    vertex.mTexCoords = DDyVector2{ 1, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1, -1,  1 };
    vertex.mNormal = DDyVector3::FrontZ();
    vertex.mTexCoords = DDyVector2{ 0, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1,  1,  1 };
    vertex.mNormal = DDyVector3::FrontZ();
    vertex.mTexCoords = DDyVector2{ 0, 1 };
    submeshDesc.mVertices.emplace_back(vertex);

    vertex.mPosition = DDyVector3{ -1,  1, -1 };
    vertex.mNormal = DDyVector3{ 0,  0, -1 };
    vertex.mTexCoords = DDyVector2{ 1,  1 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1, -1, -1 };
    vertex.mNormal = DDyVector3{ 0,  0, -1 };
    vertex.mTexCoords = DDyVector2{ 1,  0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ 1, -1, -1 };
    vertex.mNormal = DDyVector3{ 0,  0, -1 };
    vertex.mTexCoords = DDyVector2{ 0,  0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ 1,  1, -1 };
    vertex.mNormal = DDyVector3{ 0,  0, -1 };
    vertex.mTexCoords = DDyVector2{ 0,  1 };
    submeshDesc.mVertices.emplace_back(vertex);

    // Up and Down
    vertex.mPosition = DDyVector3{ 1,  1, -1 };
    vertex.mNormal = DDyVector3::UpY();
    vertex.mTexCoords = DDyVector2{ 1, 1 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ 1,  1,  1 };
    vertex.mNormal = DDyVector3::UpY();
    vertex.mTexCoords = DDyVector2{ 1, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1,  1,  1 };
    vertex.mNormal = DDyVector3::UpY();
    vertex.mTexCoords = DDyVector2{ 0, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1,  1, -1 };
    vertex.mNormal = DDyVector3::UpY();
    vertex.mTexCoords = DDyVector2{ 0, 1 };
    submeshDesc.mVertices.emplace_back(vertex);

    vertex.mPosition = DDyVector3{ 1, -1,  1 };
    vertex.mNormal = DDyVector3{ 0, -1,  0 };
    vertex.mTexCoords = DDyVector2{ 1,  1 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ 1, -1, -1 };
    vertex.mNormal = DDyVector3{ 0, -1,  0 };
    vertex.mTexCoords = DDyVector2{ 1,  0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1, -1, -1 };
    vertex.mNormal = DDyVector3{ 0, -1,  0 };
    vertex.mTexCoords = DDyVector2{ 0,  0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1, -1,  1 };
    vertex.mNormal = DDyVector3{ 0, -1,  0 };
    vertex.mTexCoords = DDyVector2{ 0,  1 };
    submeshDesc.mVertices.emplace_back(vertex);

    // Left and right
    vertex.mPosition = DDyVector3{ 1,  1, -1 };
    vertex.mNormal = DDyVector3::RightX();
    vertex.mTexCoords = DDyVector2{ 1, 1 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ 1, -1, -1 };
    vertex.mNormal = DDyVector3::RightX();
    vertex.mTexCoords = DDyVector2{ 1, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ 1, -1,  1 };
    vertex.mNormal = DDyVector3::RightX();
    vertex.mTexCoords = DDyVector2{ 0, 0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ 1,  1,  1 };
    vertex.mNormal = DDyVector3::RightX();
    vertex.mTexCoords = DDyVector2{ 0, 1 };
    submeshDesc.mVertices.emplace_back(vertex);

    vertex.mPosition = DDyVector3{ -1,  1,  1 };
    vertex.mNormal = DDyVector3{ -1,  0,  0 };
    vertex.mTexCoords = DDyVector2{ 1,  1 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1,  1, -1 };
    vertex.mNormal = DDyVector3{ -1,  0,  0 };
    vertex.mTexCoords = DDyVector2{ 1,  0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1, -1, -1 };
    vertex.mNormal = DDyVector3{ -1,  0,  0 };
    vertex.mTexCoords = DDyVector2{ 0,  0 };
    submeshDesc.mVertices.emplace_back(vertex);
    vertex.mPosition = DDyVector3{ -1, -1,  1 };
    vertex.mNormal = DDyVector3{ -1,  0,  0 };
    vertex.mTexCoords = DDyVector2{ 0,  1 };
    submeshDesc.mVertices.emplace_back(vertex);
  }

  // Make index information
  submeshDesc.mIndices.reserve(36);
  std::array<TU32, 36> indices = {
    0,  3, 1, 1, 3, 2,
    4,  7, 5, 5, 7, 6,
    8, 11, 9, 9,11,10,
    12,15,13,13,15,14,
    16,19,17,17,19,18,
    20,23,21,21,23,22
  };
  for (const auto index : indices) { submeshDesc.mIndices.emplace_back(index); }

  // Make model information
  PDyModelConstructionVertexDescriptor modelDesc;
  {
    modelDesc.mModelName = FDyBuiltinModelBox::sName;
    modelDesc.mSubmeshConstructionInformations.emplace_back(submeshDesc);
  }

  auto& manInfo = MDyIOData::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
}

} /// ::dy::builtin namespace