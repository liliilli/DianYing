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
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Information/FDyFrameBufferInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLFrameBufferDescriptor.h>
#include <Dy/Management/IO/MDyIOResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FDyFrameBufferResource::FDyFrameBufferResource(const FDyFrameBufferInformation& iInformation) :
    mSpecifierName{iInformation.GetSpecifierName()},
    mBinderFrameBuffer{iInformation.GetSpecifierName()}
{
  PDyGLFrameBufferDescriptor descriptor;
  descriptor.mFrameBufferSize           = iInformation.GetFrameBufferSize();
  descriptor.mIsNotUsingPixelShader     = !iInformation.IsUsingPixelShader();

  { // Bind Attachment list.
    const auto& colorAttachmentList = iInformation.GetAttachmentInformationBinderList();
    for (const auto& [binderInfo, ptrInfo] : colorAttachmentList)
    {
      MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptrInfo), "colorAttachment pointer must not be null. (TEMPORAL)");
      const auto& [specifier, attachmentType] = binderInfo;

      DySafeUniquePtrEmplaceBack(this->mBinderAttachmentList, specifier);
      MDY_ASSERT(this->mBinderAttachmentList.back()->IsResourceExist() == true, "Resource must be valid.");

      const auto& rescPtr = this->mBinderAttachmentList.back()->Get();
      descriptor.mAttachmentBindingList.emplace_back(rescPtr->GetAttachmentId(), attachmentType, rescPtr->IsRenderBuffer());
    }
  }
  
  if (iInformation.IsUsingDepthBuffer() == true)
  { // If using depth buffer, bind depth buffer also.
    const auto& [info, binder] = iInformation.GetDepthBufferBinder();
    this->mBinderDepthBuffer.TryRequireResource(info.mAttachmentName);
    MDY_ASSERT(this->mBinderDepthBuffer.IsResourceExist() == true, "Resource must be valid.");

    const auto& ptrDepth = this->mBinderDepthBuffer;
    descriptor.mIsUsingDepthBuffer = true;
    descriptor.mDepthBufferBinding = std::make_tuple(ptrDepth->GetAttachmentId(), EDyGlAttachmentType::Depth, ptrDepth->IsRenderBuffer());
  }

  const auto optFrameBufferId = FDyGLWrapper::CreateFrameBuffer(descriptor);
  MDY_ASSERT(optFrameBufferId.has_value() == true, "Frame buffer creation must be succeeded.");
  this->mFrameBufferId = optFrameBufferId.value();
}

FDyFrameBufferResource::~FDyFrameBufferResource()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  MDY_CALL_ASSERT_SUCCESS(FDyGLWrapper::DeleteFrameBuffer(this->mFrameBufferId));
}

} /// ::dy namespace