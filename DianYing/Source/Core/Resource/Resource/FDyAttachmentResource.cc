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
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Information/FDyAttachmentInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLAttachmentDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>

namespace dy
{

FDyAttachmentResource::FDyAttachmentResource(_MIN_ const FDyAttachmentInformation& iInformation) :
    mSpecifierName{iInformation.GetSpecifierName()},
    mInformationBinder{mSpecifierName}
{
  PDyGLAttachmentDescriptor descriptor;
  descriptor.mBorderColor           = iInformation.GetBorderColor();
  descriptor.mBufferSize            = iInformation.GetBufferSize();
  descriptor.mParameterList         = iInformation.GetParameterList();
  descriptor.mBufferFormat          = iInformation.GetBufferType();
  descriptor.mAttachmentType        = iInformation.GetAttachmentType();
  descriptor.mSpecifiedMipmapLevel  = iInformation.GetMipmapLevel();
  descriptor.mDepthNumber           = iInformation.GetDepthNumber();
  if (descriptor.mParameterList.empty() == false) 
  { 
    descriptor.mIsUsingCustomizedParameter = true; 
  }

  { MDY_GRAPHIC_SET_CRITICALSECITON();
    const auto optAttachmentId = FDyGLWrapper::CreateAttachment(descriptor);
    MDY_ASSERT(optAttachmentId.has_value() == true, "Attachment creation must be succeeded.");
    this->mAttachmentId = optAttachmentId.value();
  }
}

FDyAttachmentResource::~FDyAttachmentResource()
{
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    MDY_CALL_ASSERT_SUCCESS(FDyGLWrapper::DeleteAttachment(this->mAttachmentId, this->IsRenderBuffer()));
  }
}

const EDyGlBufferDataInternalFormat& FDyAttachmentResource::GetBufferType() const noexcept
{
  MDY_ASSERT(this->mInformationBinder.IsResourceExist() == true, "Unexpected error occurred. Information must be valid.");
  return this->mInformationBinder->GetBufferType();
}

const EDyTextureStyleType& FDyAttachmentResource::GetAttachmentType() const noexcept
{
  MDY_ASSERT(this->mInformationBinder.IsResourceExist() == true, "Unexpected error occurred. Information must be valid.");
  return this->mInformationBinder->GetAttachmentType();
}

TU32 FDyAttachmentResource::GetMipmapLevel() const noexcept
{
  MDY_ASSERT(this->mInformationBinder.IsResourceExist() == true, "Unexpected error occurred. Information must be valid.");
  return this->mInformationBinder->GetMipmapLevel();
}

TU32 FDyAttachmentResource::GetDepthNumber() const noexcept
{
  MDY_ASSERT(this->mInformationBinder.IsResourceExist() == true, "Unexpected error occurred. Information must be valid.");
  return this->mInformationBinder->GetDepthNumber();
}

} /// ::dy namespace