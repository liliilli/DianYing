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
#include <Dy/Builtin/FrameBuffer/UIIntegration/FDyBtFbUiBasic.h>
#include <Dy/Builtin/Attachment/UIIntegration/FDyBtAttUiBasic.h>

namespace dy
{

void FDyBtFbUiBasic::ConstructBuffer(_MOUT_ PDyGlFrameBufferInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName       = sName;
  property.mFrameBufferSize     = DDyVectorInt2{1280, 720};
  property.mIsUsingDepthBuffer  = true;
  property.mColorAttachmentList.emplace_back(MSVSTR(FDyBtAttUiBasic::sName), EDyGlAttachmentType::Color0);
}

} /// ::dy namespace