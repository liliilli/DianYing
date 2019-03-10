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
#include <Dy/Builtin/Abstract/ADyGLFramebufferResource.h>

namespace dy
{

void ADyGLFrameBufferResource::SetDefaultSettings()
{
  // Check blending list count is matched to attachments count.
  const auto blendingsSize    = this->mMetaInfo.mBlendingEquationList.size();
  const auto attachmentsSize  = this->mMetaInfo.mColorAttachmentList.size();

  // If not matched, resize blending list to attachment list.
  if (blendingsSize != attachmentsSize)
  {
    this->mMetaInfo.mBlendingEquationList.resize(attachmentsSize);
  }

  // Make hash value for verifying common framebuffer object in list
  // binding for performance.
  // @TODO DO IT
}

std::any ADyGLFrameBufferResource::GetMetaInfo()
{
  return this->mMetaInfo;
}

} /// ::dy namespace