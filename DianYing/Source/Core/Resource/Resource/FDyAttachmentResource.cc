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
  descriptor.mBorderColor   = iInformation.GetBorderColor();
  descriptor.mBufferSize    = iInformation.GetBufferSize();
  descriptor.mParameterList = iInformation.GetParameterList();
  descriptor.mBufferFormat  = iInformation.GetBufferType();
  if (descriptor.mParameterList.empty() == false) { descriptor.mIsUsingCustomizedParameter = true; }

  const auto optAttachmentId = FDyGLWrapper::CreateAttachment(descriptor);
  MDY_ASSERT(optAttachmentId.has_value() == true, "Attachment creation must be succeeded.");
  this->mAttachmentId = optAttachmentId.value();
}

FDyAttachmentResource::~FDyAttachmentResource()
{
  MDY_CALL_ASSERT_SUCCESS(FDyGLWrapper::DeleteAttachment(this->mAttachmentId, this->IsRenderBuffer()));
}

} /// ::dy namespace