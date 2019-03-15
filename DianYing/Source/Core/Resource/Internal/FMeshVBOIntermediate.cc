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
#include <Dy/Core/Resource/Internal/FMeshVBOIntermediate.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PGLBufferDescriptor.h>
#include <Dy/Core/Resource/Information/FDyMeshInformation.h>
#include <Dy/Meta/Information/MetaInfoModelMesh.h>

namespace dy
{

FMeshVBOIntermediate::FMeshVBOIntermediate(const FDyMeshInformation& information) :
    mSpecifierName{information.GetSpecifierName()},
    mVaoBindAttributeInfo{information.GetMeshInformationList().mVAOBindingInfo}
{
  this->MDY_PRIVATE(CreateVertexArrayBuffer)(information);
  this->MDY_PRIVATE(CreateElementArrayBuffer)(information);
}

void FMeshVBOIntermediate::MDY_PRIVATE(CreateVertexArrayBuffer)(const FDyMeshInformation& iInformation)
{
  const DMeshData& info = iInformation.GetMeshInformationList();
  { // Set vertex count.
    if (info.mVAOBindingInfo.mIsUsingDefaultDyAttributeModel == true)
    {
      this->mMeshFlagInformation.mVertexCount = info.mDefaultMeshBuffer.mVertexList.size();
    }
    else
    {
      MDY_ASSERT_MSG(info.mVAOBindingInfo.mStrideByteSize > 0, "Stride byte size must be valid.");
      this->mMeshFlagInformation.mVertexCount = sizeof(TF32) * info.mCustomMeshBuffer.size() / info.mVAOBindingInfo.mStrideByteSize;
    }
  }

  // OPENGL create VBO and EBO phrase.
  // VBO will not be created in this time because we will create VAO at main thread.
  {
    PGLBufferDescriptor descriptor;
    descriptor.mBufferType  = EDirectBufferType::VertexBuffer;
    descriptor.mBufferUsage = info.mMeshUsage;
    if (info.mVAOBindingInfo.mIsUsingDefaultDyAttributeModel == true)
    { // If using default binding (JUST USING DDYVertexInformation structure...) Let it do the thing.
      descriptor.mPtrBuffer       = &info.mDefaultMeshBuffer.mVertexList[0];
      descriptor.mBufferByteSize  = sizeof(DDefaultVertexInfo) * info.mDefaultMeshBuffer.mVertexList.size();
    }
    else 
    { // If using customized binding, let it do the thing.
      descriptor.mPtrBuffer       = info.mCustomMeshBuffer.data();
      descriptor.mBufferByteSize  = sizeof(TF32) * info.mCustomMeshBuffer.size();
    }

    std::optional<TU32> optVboId;
    { MDY_GRAPHIC_SET_CRITICALSECITON();
      optVboId = XGLWrapper::CreateBuffer(descriptor);
    }
    MDY_ASSERT_MSG(optVboId.has_value() == true, "VBO creation must be succeeded.");
    this->mBufferIdInformation.mVbo = optVboId.value();
  }
}

void FMeshVBOIntermediate::MDY_PRIVATE(CreateElementArrayBuffer)(const FDyMeshInformation& iInformation)
{
  const DMeshData& info = iInformation.GetMeshInformationList();
  { // Set flag and count for mesh geometry information & index count.
    const auto indiceSize = static_cast<TU32>(info.mIndiceBuffer.size());
    if (indiceSize == 0)  { this->mMeshFlagInformation.mIsNotHaveIndices = true; }
    else                  { this->mMeshFlagInformation.mIndiceCount = indiceSize; }
  }

  if (this->mMeshFlagInformation.mIsNotHaveIndices == false)
  {
    PGLBufferDescriptor descriptor;
    descriptor.mBufferType = EDirectBufferType::ElementBuffer;
    descriptor.mPtrBuffer  = &info.mIndiceBuffer[0];
    descriptor.mBufferByteSize = sizeof(decltype(info.mIndiceBuffer)::value_type) * info.mIndiceBuffer.size();
    
    std::optional<TU32> optEboId;
    { MDY_GRAPHIC_SET_CRITICALSECITON();
      optEboId = XGLWrapper::CreateBuffer(descriptor);
    }
    MDY_ASSERT_MSG(optEboId.has_value() == true, "EBO creation must be succeeded.");
    this->mBufferIdInformation.mEbo = optEboId.value();
  };
}

const DDySubmeshFlagInformation & FMeshVBOIntermediate::GetMeshFlagInfo() const noexcept
{
  return this->mMeshFlagInformation;
}

const DDyGLVaoBindInformation & FMeshVBOIntermediate::GetVaoBindingInfo() const noexcept
{
  return this->mVaoBindAttributeInfo;
}

void FMeshVBOIntermediate::ResetAllProperties() noexcept
{
  this->mBufferIdInformation = {};
  this->mMeshFlagInformation = {};
}

const std::string & FMeshVBOIntermediate::GetSpecifierName() const noexcept
{
  MDY_ASSERT_MSG(this->mSpecifierName.empty() == false, "Mesh specifier name must be valid.");
  return this->mSpecifierName;
}

const DGlBufferIdInformation & FMeshVBOIntermediate::GetBufferIdInfo() const noexcept
{
  return this->mBufferIdInformation;
}

FMeshVBOIntermediate::~FMeshVBOIntermediate()
{
  // Release resource when properties are not transferred yet.
  if (this->mBufferIdInformation.mVbo > 0)
  {
    MDY_GRAPHIC_SET_CRITICALSECITON();
    XGLWrapper::DeleteBuffer(this->mBufferIdInformation.mVbo);
  }

  if (this->mBufferIdInformation.mEbo > 0)
  {
    MDY_GRAPHIC_SET_CRITICALSECITON();
    XGLWrapper::DeleteBuffer(this->mBufferIdInformation.mEbo);
  }
}

} /// ::dy namespace
