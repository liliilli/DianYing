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
#include <Dy/Management/Internal/MDyProfiling.h>

namespace dy
{

EDySuccess MDyProfiling::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyProfiling::pfRelease()
{
  return DY_SUCCESS;
}

TI32 MDyProfiling::GetOnBindTextureCount() const noexcept
{
  return this->mOnBindTextureCount.Get();
}

TI32 MDyProfiling::GetOnBindVertexCount() const noexcept
{
  return this->mOnBindVertexCount.Get();
}

TI32 MDyProfiling::GetOnBindShaderCount() const noexcept
{
  return this->mOnBindShaderCount.Get();
}

void MDyProfiling::MDY_PRIVATE_SPECIFIER(AddTextureCount)(_MIN_ TI32 iInput) noexcept
{
  this->mOnBindTextureCount.Add(iInput);
}

void MDyProfiling::MDY_PRIVATE_SPECIFIER(AddOnBindVertexCount)(_MIN_ TI32 iInput) noexcept
{
  this->mOnBindVertexCount.Add(iInput);
}

void MDyProfiling::MDY_PRIVATE_SPECIFIER(AddOnBindShaderCount)(TI32 iInput) noexcept
{
  this->mOnBindShaderCount.Add(iInput);
}

} /// ::dy namespace