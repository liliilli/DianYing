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
#include <Dy/Core/Resource/Resource/FrameBuffer/FResourceFrameBufferPingPong.h>
#include <Dy/Core/Resource/Information/FInformationFrameBuffer.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLFrameBufferDescriptor.h>
#include <Dy/Management/IO/MIOResource.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FResourceFrameBufferPingPong::FResourceFrameBufferPingPong(
  const FInformationFrameBuffer& iInformation) 
{
  this->mSpecifierName = iInformation.GetSpecifierName();
  this->mFrameBufferSize = iInformation.GetFrameBufferSize();
  this->mAttachmentBlendingList = iInformation.GetAttachmentBlendings();
  this->mBinderFrameBuffer.TryRequireResource(iInformation.GetSpecifierName());

  // Bind color attachment binding list.
  const auto& colorAttachmentList = iInformation.GetAttachmentInformationBinderList();
  for (const auto& [binderInfo, ptrInfo] : colorAttachmentList)
  {
    MDY_ASSERT_MSG(
      ptrInfo != nullptr,
      "colorAttachment pointer must not be null. (TEMPORAL)");

    const auto& [specifier, _] = binderInfo;
    SafeUniquePtrEmplaceBack(this->mBinderAttachmentList, specifier);

    MDY_ASSERT_MSG(
      this->mBinderAttachmentList.back()->IsResourceExist() == true, 
      "Resource must be valid.");
  }

  // Bind depth attachment.
  if (iInformation.IsUsingDepthBuffer() == true)
  {
    const auto& [info, binder] = iInformation.GetDepthBufferBinder();
    this->mBinderDepthBuffer.TryRequireResource(info.mAttachmentName);
    MDY_ASSERT_MSG(this->mBinderDepthBuffer.IsResourceExist() == true, "Resource must be valid.");
  }

  // Make descriptors.
  std::array<PDyGLFrameBufferDescriptor, 2> descriptors;
  for (size_t i = 0; i < 2; ++i)
  {
    auto& descriptor = descriptors[i];
    descriptor.mFrameBufferSize    = iInformation.GetFrameBufferSize();
    descriptor.mIsUsingPixelShader = iInformation.IsUsingPixelShader();
    descriptor.mIsUsingDepthBuffer = iInformation.IsUsingDepthBuffer();

    std::for_each(
      MDY_BIND_BEGIN_END(this->mBinderAttachmentList), 
      [](auto& iColorBinder) { iColorBinder->Get()->Swap(); }
    );
    
    // Bind Color attachment list.
    for (const auto& [binderInfo, ptrInfo] : colorAttachmentList)
    {
      // Make attachment binding list information into creation descriptor.
      const auto& [_, attachmentType] = binderInfo;
      const auto& rescPtr = this->mBinderAttachmentList.back()->Get();
      descriptor.mAttachmentBindingList.emplace_back(
        rescPtr->GetTargetAttachmentId(), 
        rescPtr->GetAttachmentType(), 
        attachmentType, 
        rescPtr->IsRenderBuffer());
    }
  
    if (descriptor.mIsUsingDepthBuffer == true)
    { // If using depth buffer, bind depth buffer also.
      this->mBinderDepthBuffer->Swap();

      const auto& ptrDepth = this->mBinderDepthBuffer;
      descriptor.mIsUsingDepthBuffer = true;
      descriptor.mDepthBufferBinding = std::make_tuple(
        ptrDepth->GetTargetAttachmentId(), 
        ptrDepth->GetAttachmentType(), 
        EDyGlAttachmentType::Depth, 
        ptrDepth->IsRenderBuffer());
    }
  }

  std::for_each(
    MDY_BIND_BEGIN_END(this->mBinderAttachmentList), 
    [](auto& iColorBinder) { iColorBinder->Get()->Swap(); }
  );
  if (iInformation.IsUsingDepthBuffer() == true) { this->mBinderDepthBuffer->Swap(); }

  // Create frame buffers.
  for (size_t i = 0; i < 2; ++i)
  {
    std::optional<TU32> optFrameBufferId;
    { MDY_GRAPHIC_SET_CRITICALSECITON();
      optFrameBufferId = XGLWrapper::CreateFrameBuffer(descriptors[i]);
    }
    MDY_ASSERT_MSG(
      optFrameBufferId.has_value() == true, 
      "Frame buffer creation must be succeeded.");

    this->mFrameBufferIds[i] = optFrameBufferId.value();
  }
}

FResourceFrameBufferPingPong::~FResourceFrameBufferPingPong()
{
  for (size_t i = 0; i < 2; ++i)
  {
    MDY_GRAPHIC_SET_CRITICALSECITON();
    MDY_CALL_ASSERT_SUCCESS(XGLWrapper::DeleteFrameBuffer(this->mFrameBufferIds[i]));
  }
}

TU32 FResourceFrameBufferPingPong::GetTargetFrameBufferId() const noexcept
{
  if (this->mIsLeftTarget == true)  { return this->mFrameBufferIds[0]; }
  else                              { return this->mFrameBufferIds[1]; }
}

EDySuccess FResourceFrameBufferPingPong::BindFrameBuffer() const noexcept
{
  if (this->GetTargetFrameBufferId() == 0) { return EDySuccess::DY_FAILURE; }

  XGLWrapper::BindFrameBufferObject(this->GetTargetFrameBufferId());
  return EDySuccess::DY_SUCCESS;
}

void FResourceFrameBufferPingPong::Swap()
{
  this->mIsLeftTarget = !this->mIsLeftTarget;

  // Also swap attachments for synchronization.
  std::for_each(
    MDY_BIND_BEGIN_END(this->mBinderAttachmentList), 
    [](auto& iColorBinder) { iColorBinder->Get()->Swap(); }
  );
  if (this->mBinderDepthBuffer.IsResourceExist() == true) 
  { 
    this->mBinderDepthBuffer->Swap(); 
  }
}

} /// ::dy namespace