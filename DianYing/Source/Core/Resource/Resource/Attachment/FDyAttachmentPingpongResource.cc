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

#include <Dy/Core/Resource/Resource/Attachment/FDyAttachmentPingpongResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLAttachmentDescriptor.h>

namespace dy
{
FDyAttachmentPingpongResource::FDyAttachmentPingpongResource(const FDyAttachmentInformation& iInformation)
{
  this->mSpecifierName = iInformation.GetSpecifierName();
  this->mInformationBinder.TryRequireResource(this->mSpecifierName);
  this->mIsPingpong = true;

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

  // Target,
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    const auto optAttachmentId = FDyGLWrapper::CreateAttachment(descriptor);
    MDY_ASSERT_MSG(optAttachmentId.has_value() == true, "Attachment creation must be succeeded.");
    this->mAttachmentIds.first = optAttachmentId.value();
  }

  // Attachment binding for texture.
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    const auto optAttachmentId = FDyGLWrapper::CreateAttachment(descriptor);
    MDY_ASSERT_MSG(optAttachmentId.has_value() == true, "Attachment creation must be succeeded.");
    this->mAttachmentIds.second = optAttachmentId.value();
  }
}

FDyAttachmentPingpongResource::~FDyAttachmentPingpongResource()
{
  MDY_GRAPHIC_SET_CRITICALSECITON();
  MDY_CALL_ASSERT_SUCCESS(
    FDyGLWrapper::DeleteAttachment(this->mAttachmentIds.first, this->IsRenderBuffer())
  );
  MDY_CALL_ASSERT_SUCCESS(
    FDyGLWrapper::DeleteAttachment(this->mAttachmentIds.second, this->IsRenderBuffer())
  );
}

TU32 FDyAttachmentPingpongResource::GetSourceAttachmentId() const noexcept
{
  if (this->mIsLeftTarget == true)
  {
    return this->mAttachmentIds.second;
  }
  else
  {
    return this->mAttachmentIds.first;
  }
}

TU32 FDyAttachmentPingpongResource::GetTargetAttachmentId() const noexcept
{
  if (this->mIsLeftTarget == true)
  {
    return this->mAttachmentIds.first;
  }
  else
  {
    return this->mAttachmentIds.second;
  }
}

void FDyAttachmentPingpongResource::Swap()
{
  this->mIsLeftTarget = !this->mIsLeftTarget;
}

} /// ::dy namespace
