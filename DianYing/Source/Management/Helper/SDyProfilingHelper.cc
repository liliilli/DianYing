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
#include <Dy/Management/Helper/SDyProfilingHelper.h>
#include <Dy/Management/Internal/MDyProfiling.h>

namespace dy
{

void SDyProfilingHelper::IncreaseOnBindTextureCount(_MIN_ TU32 iInput) noexcept
{
  MDyProfiling::GetInstance().MDY_PRIVATE_SPECIFIER(AddTextureCount)(iInput);
}

void SDyProfilingHelper::DecreaseOnBindTextureCount(_MIN_ TU32 iInput) noexcept
{
  MDyProfiling::GetInstance().MDY_PRIVATE_SPECIFIER(AddTextureCount)(-static_cast<TI32>(iInput));
}

void SDyProfilingHelper::IncreaseOnBindVertexCount(_MIN_ TU32 iInput) noexcept
{
  MDyProfiling::GetInstance().MDY_PRIVATE_SPECIFIER(AddOnBindVertexCount)(iInput);
}

void SDyProfilingHelper::DecreaseOnBindVertexCount(_MIN_ TU32 iInput) noexcept
{
  MDyProfiling::GetInstance().MDY_PRIVATE_SPECIFIER(AddOnBindVertexCount)(-static_cast<TI32>(iInput));
}

} /// ::dy namespace