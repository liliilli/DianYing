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
#include <Dy/Management/IO/MDyIOResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>

namespace dy
{

const std::string& FDyFrameBufferResource::GetSpecifierName()
{
  return this->mSpecifierName;
}

const DDyVectorInt2& FDyFrameBufferResource::GetFrameBufferSize()
{
  return this->mFrameBufferSize;
}

void FDyFrameBufferResource::UnbindFrameBuffer() const noexcept
{
  FDyGLWrapper::UnbindFrameBufferObject();
}

void FDyFrameBufferResource::PushGlobalStates()
{
  DDyGlGlobalStates states;
  states.mAttachmentBlendings = this->mAttachmentBlendingList;

  FDyGLWrapper::PushInternalGlobalState(states);
}

void FDyFrameBufferResource::PopGlobalStates()
{
  FDyGLWrapper::PopInternalGlobalState();
}

} /// ::dy namespace