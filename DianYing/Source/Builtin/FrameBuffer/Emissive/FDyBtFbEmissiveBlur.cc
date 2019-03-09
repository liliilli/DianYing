#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Builtin/FrameBuffer/Emissive/FDyBtFbEmissiveBlur.h>

namespace dy::builtin
{

void FDyBtFbEmissiveBlur::ConstructBuffer(PDyGlFrameBufferInstanceMetaInfo& oProperty) noexcept
{
  oProperty.mSpecifierName   = sName;
  oProperty.mFrameBufferSize = DDyVectorInt2{1280, 720};
  oProperty.mIsUsingDepthBuffer = false;
  oProperty.mIsPingpong = true;

  // dyBtEmissive => dyBtEmissiveTempBlur
  oProperty.mColorAttachmentList.emplace_back("dyBtEmissiveTempBlur", EDyGlAttachmentType::Color0);
}

} /// ::dy::builtin namespace