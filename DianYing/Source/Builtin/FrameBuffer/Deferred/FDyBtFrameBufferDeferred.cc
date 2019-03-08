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
#include <Dy/Builtin/FrameBuffer/Deferred/FDyBtFrameBufferDeferred.h>

namespace dy
{

void FDyBtFrameBufferDeferred::ConstructBuffer(PDyGlFrameBufferInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName   = "dyBtBasicRender";
  property.mFrameBufferSize = DDyVectorInt2{1280, 720};

  property.mColorAttachmentList.emplace_back("dyBtUnlit",    EDyGlAttachmentType::Color0);
  property.mColorAttachmentList.emplace_back("dyBtNormal",   EDyGlAttachmentType::Color1);
  property.mColorAttachmentList.emplace_back("dyBtSpecular", EDyGlAttachmentType::Color2);
  property.mColorAttachmentList.emplace_back("dyBtEmissive",  EDyGlAttachmentType::Color3);
  property.mColorAttachmentList.emplace_back("dyBtModelPosition", EDyGlAttachmentType::Color4);
  property.mColorAttachmentList.emplace_back("dyBtAtActorId", EDyGlAttachmentType::Color5);

  property.mIsUsingDepthBuffer = true;
  property.mDepthAttachmentSpecifier = "dyBtDefZValue";
}

} /// ::dy namespace