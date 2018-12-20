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
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Resource/Internal/FDyMeshVBOIntermediate.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLVaoBindDescriptor.h>

namespace dy
{

FDyMeshResource::FDyMeshResource(_MINOUT_ FDyMeshVBOIntermediate& intermediateInstance) :
    mSpecifierName{ intermediateInstance.GetSpecifierName() }
{
  this->mBufferIdInformation = intermediateInstance.GetBufferIdInfo();
  this->mMeshFlagInformation = intermediateInstance.GetMeshFlagInfo();
  intermediateInstance.ResetAllProperties();

  // OPENGL create vao vbo ebo phrase.
  this->mBufferIdInformation.mVao = FDyGLWrapper::CreateVertexArrayObject();

  PDyGLVaoBindDescriptor descriptor;
  descriptor.mVaoId       = this->mBufferIdInformation.mVao;
  descriptor.mBoundVboId  = this->mBufferIdInformation.mVbo;
  descriptor.mBoundEboId  = this->mBufferIdInformation.mEbo;
  descriptor.mIsUsingDefaultDyAttributeModel = true;
  FDyGLWrapper::BindVertexArrayObject(descriptor);
}

FDyMeshResource::~FDyMeshResource()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

const std::string & FDyMeshResource::GetSpecifierName() const noexcept
{
  MDY_ASSERT(this->mSpecifierName.empty() == false, "Mesh specifier name must be valid.");
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

TU32 FDyMeshResource::GetVertexCounts() const noexcept
{
  return this->mMeshFlagInformation.mVertexSize;
}

TU32 FDyMeshResource::GetIndicesCounts() const noexcept
{
  return this->mMeshFlagInformation.mIndiceCount;
}

} /// ::dy namespace