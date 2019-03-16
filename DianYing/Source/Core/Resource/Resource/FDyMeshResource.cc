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
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Resource/Internal/FMeshVBOIntermediate.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLVaoBindDescriptor.h>
#include <Dy/Management/Helper/SProfilingHelper.h>

namespace dy
{

FDyMeshResource::FDyMeshResource(FMeshVBOIntermediate& ioIntermediateInstance) 
  : mSpecifierName{ ioIntermediateInstance.GetSpecifierName() }
{
  this->mBufferIdInformation = ioIntermediateInstance.GetBufferIdInfo();
  this->mMeshFlagInformation = ioIntermediateInstance.GetMeshFlagInfo();
  ioIntermediateInstance.ResetAllProperties();
  
  if (ioIntermediateInstance.IsSupportingInstancing() == true)
  {
    PGLBufferDescriptor desc;
    desc.mBufferType      = EDirectBufferType::VertexBuffer;
    desc.mBufferUsage     = EDyMeshUsage::DynamicDraw;
    desc.mBufferByteSize  = 0;
    { MDY_GRAPHIC_SET_CRITICALSECITON();
      const auto optInstancingBuffer = XGLWrapper::CreateBuffer(desc);
      MDY_ASSERT(optInstancingBuffer.has_value() == true);
      this->mInstancingBufferId = optInstancingBuffer;
    }
  }

  // OPENGL create vao vbo ebo phrase.
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    this->mBufferIdInformation.mVao = XGLWrapper::CreateVertexArrayObject();

    PDyGLVaoBindDescriptor descriptor;
    descriptor.mVaoId         = this->mBufferIdInformation.mVao;
    descriptor.mBoundVboId    = this->mBufferIdInformation.mVbo;
    descriptor.mBoundEboId    = this->mBufferIdInformation.mEbo;
    descriptor.mAttributeInfo = ioIntermediateInstance.GetVaoBindingInfo();
    if (this->mInstancingBufferId.has_value() == true)
    {
      descriptor.mInstancingVboId = *this->mInstancingBufferId;
    }
    XGLWrapper::BindVertexArrayObject(descriptor);
  }

  SProfilingHelper::IncreaseOnBindVertexCount(this->mMeshFlagInformation.mVertexCount);
}

FDyMeshResource::~FDyMeshResource()
{
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    XGLWrapper::DeleteVertexArrayObject(this->mBufferIdInformation.mVao);
    if (this->mBufferIdInformation.mEbo > 0) { XGLWrapper::DeleteBuffer(this->mBufferIdInformation.mEbo); }
    if (this->mBufferIdInformation.mVbo > 0) { XGLWrapper::DeleteBuffer(this->mBufferIdInformation.mVbo); }
  }
  SProfilingHelper::DecreaseOnBindVertexCount(this->mMeshFlagInformation.mVertexCount);
}

const std::string& FDyMeshResource::GetSpecifierName() const noexcept
{
  MDY_ASSERT_MSG(this->mSpecifierName.empty() == false, "Mesh specifier name must be valid.");
  return this->mSpecifierName;
}

bool FDyMeshResource::IsEnabledIndices() const noexcept
{
  return this->mMeshFlagInformation.mIsNotHaveIndices == false;
}

TU32 FDyMeshResource::GetVertexArrayId() const noexcept
{
  return this->mBufferIdInformation.mVao;
}

TU32 FDyMeshResource::GetVertexBufferId() const noexcept
{
  return this->mBufferIdInformation.mVbo;
}

TU32 FDyMeshResource::GetVertexCounts() const noexcept
{
  return this->mMeshFlagInformation.mVertexCount;
}

TU32 FDyMeshResource::GetIndicesCounts() const noexcept
{
  return this->mMeshFlagInformation.mIndiceCount;
}

EDySuccess FDyMeshResource::BindVertexArray() const noexcept
{
  if (this->mBufferIdInformation.mVao == 0) { return DY_FAILURE; }

  XGLWrapper::BindVertexArrayObject(this->mBufferIdInformation.mVao);
  return DY_SUCCESS;
}

bool FDyMeshResource::IsSupportingInstancing() const noexcept
{
  return this->mInstancingBufferId.has_value() == true;
}

} /// ::dy namespace