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

#include <Dy/Core/Resource/Resource/Attachment/FDyAttachmentGeneralResource.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PGLAttachmentDescriptor.h>

namespace dy
{
FDyAttachmentGeneralResource::FDyAttachmentGeneralResource(const FDyAttachmentInformation& iInformation)
{
  this->mSpecifierName = iInformation.GetSpecifierName();
  this->mInformationBinder.TryRequireResource(this->mSpecifierName);

  PGLAttachmentDescriptor descriptor;
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
    const auto optAttachmentId = XGLWrapper::CreateAttachment(descriptor);
    MDY_ASSERT_MSG(optAttachmentId.has_value() == true, "Attachment creation must be succeeded.");
    this->mAttachmentId = optAttachmentId.value();
  }
}

FDyAttachmentGeneralResource::~FDyAttachmentGeneralResource()
{
  MDY_GRAPHIC_SET_CRITICALSECITON();
  MDY_CALL_ASSERT_SUCCESS(
    XGLWrapper::DeleteAttachment(this->mAttachmentId, this->IsRenderBuffer())
  );
}

TU32 FDyAttachmentGeneralResource::GetSourceAttachmentId() const noexcept
{
  return this->mAttachmentId;
}

TU32 FDyAttachmentGeneralResource::GetTargetAttachmentId() const noexcept
{
  return this->GetSourceAttachmentId();
}

} /// ::dy namespace
