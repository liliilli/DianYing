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
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Information/FDyFrameBufferInformation.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLFrameBufferDescriptor.h>
#include <Dy/Management/IO/MDyIOResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FDyFrameBufferResource::FDyFrameBufferResource(const FDyFrameBufferInformation& iInformation) 
  : mSpecifierName{iInformation.GetSpecifierName()},
    mFrameBufferSize{iInformation.GetFrameBufferSize()},
    mAttachmentBlendingList{iInformation.GetAttachmentBlendings()},
    mBinderFrameBuffer{iInformation.GetSpecifierName()}
{
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
      DySafeUniquePtrEmplaceBack(this->mBinderAttachmentList, specifier);

      MDY_ASSERT_MSG(
        this->mBinderAttachmentList.back()->IsResourceExist() == true, 
        "Resource must be valid.");

      // Make attachment binding list information into creation descriptor.
      const auto& rescPtr = this->mBinderAttachmentList.back()->Get();
      descriptor.mAttachmentBindingList.emplace_back(
          rescPtr->GetAttachmentId(), 
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
        ptrDepth->GetAttachmentId(), 
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

FDyFrameBufferResource::~FDyFrameBufferResource()
{
  MDY_GRAPHIC_SET_CRITICALSECITON();
  MDY_CALL_ASSERT_SUCCESS(FDyGLWrapper::DeleteFrameBuffer(this->mFrameBufferId));
}

const std::string& FDyFrameBufferResource::GetSpecifierName()
{
  return this->mSpecifierName;
}

TU32 FDyFrameBufferResource::GetFrameBufferId() const noexcept
{
  return this->mFrameBufferId;
}

const DDyVectorInt2& FDyFrameBufferResource::GetFrameBufferSize()
{
  return this->mFrameBufferSize;
}

EDySuccess FDyFrameBufferResource::BindFrameBuffer() const noexcept
{
  if (this->GetFrameBufferId() == 0) { return DY_FAILURE; }

  FDyGLWrapper::BindFrameBufferObject(this->GetFrameBufferId());
  //FDyGLWrapper::PushBlendingModes(this->mAttachmentBlendingList);
  return DY_SUCCESS;
}

void FDyFrameBufferResource::UnbindFrameBuffer() const noexcept
{
  //FDyGLWrapper::PopBlendingModes(this->mAttachmentBlendingList);
  FDyGLWrapper::UnbindFrameBufferObject();
}

} /// ::dy namespace