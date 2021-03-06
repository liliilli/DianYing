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
#include <Dy/Builtin/FrameBuffer/BasicShadow/FDyBtFBBasicShadow.h>
#include <Dy/Builtin/Attachment/BasicShadow/BasicShadow.h>

namespace dy
{

void FDyBtFBBasicShadow::ConstructBuffer(_MOUT_ PDyGlFrameBufferInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName         = sName;
  property.mFrameBufferSize       = DIVec2{1280, 720};
  property.mIsNotUsingPixelShader = true;
  property.mIsUsingDepthBuffer    = true;
  property.mDepthAttachmentSpecifier = FDyBtAttachmentBasicShadow::sName;
}

} /// ::dy namespace