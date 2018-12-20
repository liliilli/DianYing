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
#include <Dy/Core/Resource/Information/FDyMeshInformation.h>
#include <Dy/Meta/Information/MetaInfoBuiltinMesh.h>

namespace dy
{

FDyMeshVBOIntermediate::FDyMeshVBOIntermediate(_MIN_ const FDyMeshInformation& information) :
    mSpecifierName{information.GetSpecifierName()}
{
  this->MDY_PRIVATE_SPECIFIER(CreateVertexArrayBuffer)(information);
  this->MDY_PRIVATE_SPECIFIER(CreateElementArrayBuffer)(information);
}

void FDyMeshVBOIntermediate::MDY_PRIVATE_SPECIFIER(CreateVertexArrayBuffer)(_MIN_ const FDyMeshInformation& iInformation)
{
  const PDyBtMeshInstanceMetaInfo& info = iInformation.GetMeshInformationList();
  { // Set vertex count.
    constexpr auto s = sizeof(decltype(info.mDefaultMeshBuffer.mVertexList)::value_type::mPosition);
    const auto vertexSize = static_cast<TU32>(info.mDefaultMeshBuffer.mVertexList.size() * s);
    this->mMeshFlagInformation.mVertexSize = vertexSize;
  }

  // OPENGL create VBO and EBO phrase.
  // VBO will not be created in this time because we will create VAO at main thread.
  {
    PDyGLBufferDescriptor descriptor;
    descriptor.mBufferType  = EDyDirectBufferType::VertexBuffer;
    descriptor.mBufferUsage = info.mMeshUsage;
    descriptor.mIsUsingDefaultBufferStruction = info.mIsUsingDefaultBinding;

    if (descriptor.mIsUsingDefaultBufferStruction == true)
    { // If using default binding (JUST USING DDYVertexInformation structure...) Let it do the thing.
      descriptor.mPtrBuffer       = &info.mDefaultMeshBuffer.mVertexList[0];
      descriptor.mBufferByteSize  = sizeof(DDyVertexInformation) * info.mDefaultMeshBuffer.mVertexList.size();
    }
    else 
    { // If using customized binding, let it do the thing.
      descriptor.mPtrBuffer       = info.mCustomMeshBuffer.data();
      descriptor.mBufferByteSize  = sizeof(TF32) * info.mCustomMeshBuffer.size();
    }

    const auto optVboId = FDyGLWrapper::CreateBuffer(descriptor);
    MDY_ASSERT(optVboId.has_value() == true, "VBO creation must be succeeded.");
    this->mBufferIdInformation.mVbo = optVboId.value();
  }
}

void FDyMeshVBOIntermediate::MDY_PRIVATE_SPECIFIER(CreateElementArrayBuffer)(_MIN_ const FDyMeshInformation& iInformation)
{
  const PDyBtMeshInstanceMetaInfo& info = iInformation.GetMeshInformationList();
  { // Set flag and count for mesh geometry information & index count.
    const auto indiceSize = static_cast<TU32>(info.mIndiceBuffer.size());
    if (indiceSize == 0)  { this->mMeshFlagInformation.mIsNotHaveIndices = true; }
    else                  { this->mMeshFlagInformation.mIndiceCount = indiceSize; }
  }

  if (this->mMeshFlagInformation.mIsNotHaveIndices == false)
  {
    PDyGLBufferDescriptor descriptor;
    descriptor.mBufferType = EDyDirectBufferType::ElementBuffer;
    descriptor.mPtrBuffer  = &info.mIndiceBuffer[0];
    descriptor.mBufferByteSize = sizeof(decltype(info.mIndiceBuffer)::value_type) * info.mIndiceBuffer.size();

    const auto optEboId = FDyGLWrapper::CreateBuffer(descriptor);
    MDY_ASSERT(optEboId.has_value() == true, "EBO creation must be succeeded.");
    this->mBufferIdInformation.mEbo = optEboId.value();
  };
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

const std::string & FDyMeshVBOIntermediate::GetSpecifierName() const noexcept
{
  MDY_ASSERT(this->mSpecifierName.empty() == false, "Mesh specifier name must be valid.");
  return this->mSpecifierName;
}

} /// ::dy namespace
