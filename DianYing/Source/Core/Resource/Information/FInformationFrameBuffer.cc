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
#include <Dy/Core/Resource/Information/FInformationFrameBuffer.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

FInformationFrameBuffer::FInformationFrameBuffer(_MIN_ const PDyGlFrameBufferInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName},
    mAttachmentBlendings{metaInfo.mBlendingEquationList},
    mFrameBufferSize{metaInfo.mFrameBufferSize},
    mIsUsingDepthBuffer{metaInfo.mIsUsingDepthBuffer},
    mIsNotUsingPixelShader{metaInfo.mIsNotUsingPixelShader},
    mIsPingpong{metaInfo.mIsPingpong}
{
  for (const auto& attachmentInfo : metaInfo.mColorAttachmentList)
  {
    using TType = decltype(this->mAttachmentInfoList)::value_type::second_type::element_type;
    this->mAttachmentInfoList.emplace_back(
        attachmentInfo, 
        std::make_unique<TType>(attachmentInfo.mAttachmentName)
    );
  }

  if (this->mIsUsingDepthBuffer == true)
  { // If use depth buffer, also bind information.
    this->mDepthAttachment = std::make_pair(
        PDyGlAttachmentBinderInformation{ metaInfo.mDepthAttachmentSpecifier, EDyGlAttachmentType::Depth },
        std::make_unique<TDyInformationBinderAttachment>(metaInfo.mDepthAttachmentSpecifier)
    );
  }
}

const TBlendingEquationList& FInformationFrameBuffer::GetAttachmentBlendings() const noexcept
{
  return this->mAttachmentBlendings;
}

bool FInformationFrameBuffer::IsPingPong() const noexcept
{
  return this->mIsPingpong;
}

} /// ::dy namespace