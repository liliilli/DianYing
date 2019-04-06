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
#include <Dy/Builtin/Abstract/AGLFrameBufferResource.h>

namespace dy
{

void AGLFrameBufferResource::SetDefaultSettings()
{
  // Check blending list count is matched to attachments count.
  const auto blendingsSize    = this->MIOMeta.mBlendingEquationList.size();
  const auto attachmentsSize  = this->MIOMeta.mColorAttachmentList.size();

  // If not matched, resize blending list to attachment list.
  if (blendingsSize != attachmentsSize)
  {
    this->MIOMeta.mBlendingEquationList.resize(attachmentsSize);
  }

  // Make hash value for verifying common framebuffer object in list
  // binding for performance.
  // @TODO DO IT
}

std::any AGLFrameBufferResource::GetMetaInfo()
{
  return this->MIOMeta;
}

} /// ::dy namespace