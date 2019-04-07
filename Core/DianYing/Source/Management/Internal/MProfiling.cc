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
#include <Dy/Management/Internal/MProfiling.h>

namespace dy
{

EDySuccess MProfiling::pfInitialize()
{
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MProfiling::pfRelease()
{
  return EDySuccess::DY_SUCCESS;
}

TI32 MProfiling::GetOnBindTextureCount() const noexcept
{
  return this->mOnBindTextureCount.Get();
}

TI32 MProfiling::GetOnBindVertexCount() const noexcept
{
  return this->mOnBindVertexCount.Get();
}

TI32 MProfiling::GetOnBindShaderCount() const noexcept
{
  return this->mOnBindShaderCount.Get();
}

TI32 MProfiling::GetOnBindActorCount() const noexcept
{
  return this->mOnBindActorCount.Get();
}

TI32 MProfiling::GetScreenRenderedActorCount() const noexcept
{
  return this->mScreenRenderedActorCount;
}

void MProfiling::MDY_PRIVATE(AddTextureCount)(_MIN_ TI32 iInput) noexcept
{
  this->mOnBindTextureCount.Add(iInput);
}

void MProfiling::MDY_PRIVATE(AddOnBindVertexCount)(_MIN_ TI32 iInput) noexcept
{
  this->mOnBindVertexCount.Add(iInput);
}

void MProfiling::MDY_PRIVATE(AddOnBindShaderCount)(_MIN_ TI32 iInput) noexcept
{
  this->mOnBindShaderCount.Add(iInput);
}

void MProfiling::MDY_PRIVATE(AddOnBindActorCount)(_MIN_ TI32 iInput) noexcept
{
  this->mOnBindActorCount.Add(iInput);
}

void MProfiling::MDY_PRIVATE(AddScreenRenderedActorCount)(_MIN_ TI32 iInput) noexcept
{
  this->mScreenRenderedActorCount += iInput;
}

void MProfiling::MDY_PRIVATE(ResetScreenRenderedActorCount)() noexcept
{
  this->mScreenRenderedActorCount = 0;
}

void MProfiling::MDY_PRIVATE(ResetFrameDependentCounts)() noexcept
{
  this->MDY_PRIVATE(ResetScreenRenderedActorCount)();
}

} /// ::dy namespace