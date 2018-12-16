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
#include <Dy/Core/Resource/Internal/FDyMeshVBOIntermediate.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLBufferDescriptor.h>

namespace dy
{

FDyMeshVBOIntermediate::FDyMeshVBOIntermediate(const PDySubmeshInformationDescriptor& information)
{
  const auto& info = information;
  { // Set flag and count for mesh geometry information & index count.
    const auto indiceSize = static_cast<int32_t>(information.mIndices.size());
    if (indiceSize == 0)  { this->mMeshFlagInformation.mIsNotHaveIndices = true; }
    else                  { this->mMeshFlagInformation.mIndiceCount = indiceSize; }
  }

  { // Set vertex count.
    constexpr auto s = sizeof(decltype(info.mVertices)::value_type::mPosition);
    const auto vertexSize = static_cast<int32_t>(info.mVertices.size() * s);
    this->mMeshFlagInformation.mVertexSize = vertexSize;
  }

  // OPENGL create VBO and EBO phrase.
  // VBO will not be created in this time because we will create VAO at main thread.
  {
    PDyGLBufferDescriptor descriptor;
    descriptor.mBufferType = EDyDirectBufferType::VertexBuffer;
    descriptor.mPtrBuffer  = &info.mVertices[0];
    descriptor.mBufferByteSize = sizeof(DDyVertexInformation) * info.mVertices.size();

    const auto optVboId = FDyGLWrapper::CreateBuffer(descriptor);
    MDY_ASSERT(optVboId.has_value() == true, "VBO creation must be succeeded.");
    this->mBufferIdInformation.mVbo = optVboId.value();
  }

  if (this->mMeshFlagInformation.mIsNotHaveIndices == false)
  {
    PDyGLBufferDescriptor descriptor;
    descriptor.mBufferType = EDyDirectBufferType::ElementBuffer;
    descriptor.mPtrBuffer  = &info.mIndices[0];
    descriptor.mBufferByteSize = sizeof(decltype(info.mIndices)::value_type) * info.mIndices.size();

    const auto optEboId = FDyGLWrapper::CreateBuffer(descriptor);
    MDY_ASSERT(optEboId.has_value() == true, "VBO creation must be succeeded.");
    this->mBufferIdInformation.mEbo = optEboId.value();
  }
}

FDyMeshVBOIntermediate::~FDyMeshVBOIntermediate()
{
  // Release resource when properties are not transferred yet.
  if (this->mBufferIdInformation.mVbo > 0)
  {
    FDyGLWrapper::DeleteBuffer(this->mBufferIdInformation.mVbo);
  }

  if (this->mBufferIdInformation.mEbo > 0)
  {
    FDyGLWrapper::DeleteBuffer(this->mBufferIdInformation.mEbo);
  }
}

void FDyMeshVBOIntermediate::ResetAllProperties() noexcept
{
  this->mBufferIdInformation = {};
  this->mMeshFlagInformation = {};
}

} /// ::dy namespace
