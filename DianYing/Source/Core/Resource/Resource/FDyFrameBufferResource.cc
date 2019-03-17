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
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Information/FDyFrameBufferInformation.h>
#include <Dy/Management/IO/MIOResource.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>

namespace dy
{

const std::string& FDyFrameBufferResource::GetSpecifierName()
{
  return this->mSpecifierName;
}

const DVectorInt2& FDyFrameBufferResource::GetFrameBufferSize()
{
  return this->mFrameBufferSize;
}

void FDyFrameBufferResource::UnbindFrameBuffer() const noexcept
{
  XGLWrapper::UnbindFrameBufferObject();
}

void FDyFrameBufferResource::PushGlobalStates()
{
  DGlGlobalStates states;
  states.mAttachmentBlendings = this->mAttachmentBlendingList;

  XGLWrapper::PushInternalGlobalState(states);
}

void FDyFrameBufferResource::PopGlobalStates()
{
  XGLWrapper::PopInternalGlobalState();
}

} /// ::dy namespace