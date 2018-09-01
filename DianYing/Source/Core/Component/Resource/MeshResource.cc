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
#include <Dy/Core/Component/Resource/MeshResource.h>

namespace dy
{

CDyMeshResource::~CDyMeshResource()
{
  // Release resources. (OPENGL only)
  if (this->mBufferIdInformation.mEbo > 0)
  {
    glDeleteBuffers(1, &this->mBufferIdInformation.mEbo);
  }
  if (this->mBufferIdInformation.mVbo > 0)
  {
    glDeleteBuffers(1, &this->mBufferIdInformation.mVbo);
  }
  if (this->mBufferIdInformation.mVao > 0)
  {
    glDeleteVertexArrays(1, &this->mBufferIdInformation.mVao);
  }
}

bool CDyMeshResource::IsEnabledIndices() const noexcept
{
  return !this->mMeshFlagInformation.mIsNotHaveIndices;
}

int32_t CDyMeshResource::GetVertexArrayId() const noexcept
{
  return this->mBufferIdInformation.mVao;
}

EDySuccess CDyMeshResource::pfInitializeMesh(const DDyMeshInformation& meshInformation) noexcept
{
  const auto& info      = meshInformation.GetInformation();

  // Set flag and count for mesh geometry information.
  {
    const auto indiceSize = static_cast<int32_t>(info.mIndices.size());
    if (indiceSize == 0)  { this->mMeshFlagInformation.mIsNotHaveIndices = true; }
    else                  { this->mMeshFlagInformation.mIndiceCount = indiceSize; }
  }

  {
    constexpr auto s = sizeof(decltype(info.mVertices)::value_type::mPosition);
    const auto vertexSize = static_cast<int32_t>(info.mVertices.size() * s);
    this->mMeshFlagInformation.mVertexSize = vertexSize;
  }

  // OPENGL create vao vbo ebo phrase.
  glGenVertexArrays(1, &this->mBufferIdInformation.mVao);
  glGenBuffers(1, &this->mBufferIdInformation.mVbo);

  glBindVertexArray(this->mBufferIdInformation.mVao);
  glBindBuffer(GL_ARRAY_BUFFER, this->mBufferIdInformation.mVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(DVertexInformation) * info.mVertices.size(),
               &info.mVertices[0],
               GL_STATIC_DRAW);

  if (!mMeshFlagInformation.mIsNotHaveIndices)
  {
    glGenBuffers(1, &this->mBufferIdInformation.mEbo);

    constexpr auto indiceElementSize = sizeof(decltype(info.mIndices)::value_type);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferIdInformation.mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceElementSize * info.mIndices.size(),
                 &info.mIndices[0],
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

    // mMeshResourceInformation.mVertices.mTexCoord (DVector2) (UV1)
    if (!mMeshFlagInformation.mIsNotHaveIndices)
    {
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof DVector3 * 2));
      glEnableVertexAttribArray(2);
    }
  }

  glBindVertexArray(0);
  return DY_SUCCESS;
}

} /// ::dy namespace