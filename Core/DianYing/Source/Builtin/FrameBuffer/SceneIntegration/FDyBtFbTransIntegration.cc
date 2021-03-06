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
#include <Dy/Builtin/FrameBuffer/SceneIntegration/FDyBtFbTransIntegration.h>
#include <Dy/Builtin/Attachment/SceneIntegration/FDyBtAttOutput.h>

namespace dy
{

void FDyBtFbTransIntegration::ConstructBuffer(_MOUT_ PDyGlFrameBufferInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName       = sName;
  property.mFrameBufferSize     = DIVec2{1280, 720};
  property.mIsUsingDepthBuffer  = false;
  property.mColorAttachmentList.emplace_back((FDyBtAttOutput::sName), EDyGlAttachmentType::Color0);
}

} /// ::dy namespace