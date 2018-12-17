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

namespace dy
{

FDyFrameBufferResource::FDyFrameBufferResource(const FDyFrameBufferInformation& iInformation) :
    mSpecifierName{iInformation.GetSpecifierName()}
{
  PDyGLFrameBufferDescriptor descriptor;
  descriptor.mFrameBufferSize           = iInformation.GetFrameBufferSize();
  descriptor.mIsUsingDefaultDepthBuffer = iInformation.IsUsingDefaultDepthBuffer();
  descriptor.mIsNotUsingPixelShader     = !iInformation.IsUsingPixelShader();

  const auto& colorAttachmentList = iInformation.GetAttachmentInformationBinderList();
  /// @TODO TEMPORAL CODE. MUST USE TBINDER(THAT CAN SUPPORT RAII!!) INSTEAD OF THIS.
  auto& resourceManager = MDyIOResource::GetInstance();
  for (const auto& [binderInfo, ptrInfo] : colorAttachmentList)
  {
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptrInfo), "colorAttachment pointer must not be null. (TEMPORAL)");
    const auto& [specifier, attachmentType] = binderInfo;
    const auto* rescPtr = resourceManager.GetPtrInformation<EDyResourceType::GLAttachment>(specifier);
    descriptor.mAttachmentBindingList.emplace_back(rescPtr->GetAttachmentId(), attachmentType, rescPtr->IsRenderBuffer());
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