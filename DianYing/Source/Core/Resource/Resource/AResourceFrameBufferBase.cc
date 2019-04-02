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
#include <Dy/Core/Resource/Resource/AResourceFrameBufferBase.h>
#include <Dy/Core/Resource/Information/FInformationFrameBuffer.h>
#include <Dy/Management/IO/MIOResource.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>

namespace dy
{

const std::string& AResourceFrameBufferBase::GetSpecifierName()
{
  return this->mSpecifierName;
}

const DIVec2& AResourceFrameBufferBase::GetFrameBufferSize()
{
  return this->mFrameBufferSize;
}

void AResourceFrameBufferBase::UnbindFrameBuffer() const noexcept
{
  XGLWrapper::UnbindFrameBufferObject();
}

void AResourceFrameBufferBase::PushGlobalStates()
{
  DGlGlobalStates states;
  states.mAttachmentBlendings = this->mAttachmentBlendingList;

  XGLWrapper::PushInternalGlobalState(states);
}

void AResourceFrameBufferBase::PopGlobalStates()
{
  XGLWrapper::PopInternalGlobalState();
}

} /// ::dy namespace