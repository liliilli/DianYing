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
#include <Dy/Core/Resource/Information/FDyFrameBufferInformation.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>
#include <Dy/Management/IO/MDyIOData.h>

namespace dy
{

FDyFrameBufferInformation::FDyFrameBufferInformation(_MIN_ const PDyGlFrameBufferInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName},
    mFrameBufferSize{metaInfo.mFrameBufferSize},
    mIsUsingDefaultDepthBuffer{metaInfo.mIsUsingDefaultDepthBuffer},
    mIsNotUsingPixelShader{metaInfo.mIsNotUsingPixelShader}
{
  // Validiation test.


  // @TODO BIND INFORMATION AS TBinder.
  // Get informations.
  const auto& ioData = MDyIOData::GetInstance();
  for (const auto& attachmentInfo : metaInfo.mAttachmentList)
  {
    const auto* ptr = ioData.GetPtrInformation<EDyResourceType::GLAttachment>(attachmentInfo.mAttachmentName);
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptr), "Ptr must not be null.");
    this->mAttachmentInfoList.emplace_back(attachmentInfo, ptr);
  }
}

} /// ::dy namespace