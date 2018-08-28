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
#include <Dy/Helper/Geometry/Mesh.h>
#include <utility>

namespace dy
{

DDyMeshInformation::DDyMeshInformation(const PMeshInformationDescriptor& meshInformation) :
    mMeshResourceInformation{ meshInformation }
{
  mMeshFlagInformation.mIsNotHaveIndices = mMeshResourceInformation.mIndices.empty();
  mMeshFlagInformation.mIsNotHaveTextures = mMeshResourceInformation.mTextures.empty();
  MDY_CALL_ASSERT_SUCCESS(pSetupMesh());
}

DDyMeshInformation::DDyMeshInformation(PMeshInformationDescriptor meshInformation) :
    mMeshResourceInformation{ std::move(meshInformation) }
{
  mMeshFlagInformation.mIsNotHaveIndices = mMeshResourceInformation.mIndices.empty();
  mMeshFlagInformation.mIsNotHaveTextures = mMeshResourceInformation.mTextures.empty();
  MDY_CALL_ASSERT_SUCCESS(pSetupMesh());
}

DDyMeshInformation::~DDyMeshInformation()
{
  glDeleteBuffers(1, &mBufferIdInformation.mVbo);
  if (!mMeshFlagInformation.mIsNotHaveIndices)
  {
    glDeleteBuffers(1, &mBufferIdInformation.mEbo);
  }
  glDeleteVertexArrays(1, &mBufferIdInformation.mVao);
}

EDySuccess DDyMeshInformation::pSetupMesh() noexcept
{
  glGenVertexArrays(1, &mBufferIdInformation.mVao);
  glGenBuffers(1, &mBufferIdInformation.mVbo);

  glBindVertexArray(mBufferIdInformation.mVao);
  glBindBuffer(GL_ARRAY_BUFFER, mBufferIdInformation.mVbo);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(DVertexInformation) * mMeshResourceInformation.mVertices.size(),
               &mMeshResourceInformation.mVertices[0],
               GL_STATIC_DRAW);

  if (!mMeshFlagInformation.mIsNotHaveIndices)
  {
    glGenBuffers(1, &mBufferIdInformation.mEbo);
    constexpr auto indiceElementSize = sizeof(decltype(mMeshResourceInformation.mIndices)::value_type);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferIdInformation.mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indiceElementSize * mMeshResourceInformation.mIndices.size(),
                 &mMeshResourceInformation.mIndices[0],
                 GL_STATIC_DRAW);

  }

  {
    // mMeshResourceInformation.mVertices.mPosition (DVector3)
    constexpr auto stride = sizeof(DVertexInformation);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);

    // mMeshResourceInformation.mVertices.mNormal   (DVector3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof DVector3));
    glEnableVertexAttribArray(1);

    if (!mMeshFlagInformation.mIsNotHaveIndices)
    {
      // mMeshResourceInformation.mVertices.mTexCoord (DVector2) (UV1)
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof DVector3 * 2));
      glEnableVertexAttribArray(2);
    }
    glBindVertexArray(0);
  }

  return DY_SUCCESS;
}

} /// ::dy namespace