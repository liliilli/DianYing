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
#include <Dy/Core/Resource/Information/FInformationAttachment.h>
#include <Dy/Meta/Information/MetaInfoAttachment.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy
{

FInformationAttachment::FInformationAttachment(const PDyGlAttachmentInstanceMetaInfo& metaInfo) :
    mSpecifierName{metaInfo.mSpecifierName},
    mParameterList{metaInfo.mParameterList},
    mAttachmentSize{metaInfo.mAttachmentSize},
    mBorderColor{metaInfo.mBorderColor},
    mBufferType{metaInfo.mBufferFormat},
    mAttachmentType{metaInfo.mAttachmentType},
    mMipmapLevels{metaInfo.mMipmapLevels},
    mDepthNumber{metaInfo.mDepthNumber},
    mIsPingpong{metaInfo.mIsPingpong}
{
  MDY_ASSERT_MSG(
    this->mSpecifierName.empty() == false, 
    "Attachment specifier name must be specified.");
}

bool FInformationAttachment::IsPingPong() const noexcept
{
  return this->mIsPingpong;
}

} /// ::dy namespace