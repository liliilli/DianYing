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
#include <Dy/Core/Resource/Resource/FrameBuffer/FDyFrameBufferGeneralResource.h>
#include <Dy/Core/Resource/Information/FDyFrameBufferInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLFrameBufferDescriptor.h>
#include <Dy/Management/IO/MDyIOResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FDyFrameBufferGeneralResource::FDyFrameBufferGeneralResource(const FDyFrameBufferInformation& iInformation) 
{
  this->mSpecifierName = iInformation.GetSpecifierName();
  this->mFrameBufferSize = iInformation.GetFrameBufferSize();
  this->mAttachmentBlendingList = iInformation.GetAttachmentBlendings();
  this->mBinderFrameBuffer.TryRequireResource(iInformation.GetSpecifierName());

  PDyGLFrameBufferDescriptor descriptor;
  descriptor.mFrameBufferSize    = iInformation.GetFrameBufferSize();
  descriptor.mIsUsingPixelShader = iInformation.IsUsingPixelShader();
  descriptor.mIsUsingDepthBuffer = iInformation.IsUsingDepthBuffer();

  { // Bind Color attachment list.
    const auto& colorAttachmentList = iInformation.GetAttachmentInformationBinderList();
    for (const auto& [binderInfo, ptrInfo] : colorAttachmentList)
    {
      MDY_ASSERT_MSG(
        ptrInfo != nullptr, 
        "colorAttachment pointer must not be null. (TEMPORAL)");

      const auto& [specifier, attachmentType] = binderInfo;
      SafeUniquePtrEmplaceBack(this->mBinderAttachmentList, specifier);

      MDY_ASSERT_MSG(
        this->mBinderAttachmentList.back()->IsResourceExist() == true, 
        "Resource must be valid.");

      // Make attachment binding list information into creation descriptor.
      const auto& rescPtr = this->mBinderAttachmentList.back()->Get();
      descriptor.mAttachmentBindingList.emplace_back(
        rescPtr->GetTargetAttachmentId(), 
        rescPtr->GetAttachmentType(), 
        attachmentType, 
        rescPtr->IsRenderBuffer());
    }
  }
  
  if (descriptor.mIsUsingDepthBuffer == true)
  { // If using depth buffer, bind depth buffer also.
    const auto& [info, binder] = iInformation.GetDepthBufferBinder();
    this->mBinderDepthBuffer.TryRequireResource(info.mAttachmentName);
    MDY_ASSERT_MSG(this->mBinderDepthBuffer.IsResourceExist() == true, "Resource must be valid.");

    const auto& ptrDepth = this->mBinderDepthBuffer;
    descriptor.mIsUsingDepthBuffer = true;
    descriptor.mDepthBufferBinding = std::make_tuple(
      ptrDepth->GetTargetAttachmentId(), 
      ptrDepth->GetAttachmentType(), 
      EDyGlAttachmentType::Depth, 
      ptrDepth->IsRenderBuffer());
  }

  // Create frame buffer.
  std::optional<TU32> optFrameBufferId;
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    optFrameBufferId = FDyGLWrapper::CreateFrameBuffer(descriptor);
  }
  MDY_ASSERT_MSG(optFrameBufferId.has_value() == true, "Frame buffer creation must be succeeded.");

  this->mFrameBufferId = optFrameBufferId.value();
}

FDyFrameBufferGeneralResource::~FDyFrameBufferGeneralResource()
{
  MDY_GRAPHIC_SET_CRITICALSECITON();
  MDY_CALL_ASSERT_SUCCESS(FDyGLWrapper::DeleteFrameBuffer(this->mFrameBufferId));
}

TU32 FDyFrameBufferGeneralResource::GetTargetFrameBufferId() const noexcept
{
  return this->mFrameBufferId;
}

EDySuccess FDyFrameBufferGeneralResource::BindFrameBuffer() const noexcept
{
  if (this->GetTargetFrameBufferId() == 0) { return DY_FAILURE; }

  FDyGLWrapper::BindFrameBufferObject(this->GetTargetFrameBufferId());
  return DY_SUCCESS;
}

} /// ::dy namespace