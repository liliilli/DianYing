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
#include <Dy/Management/Helper/SProfilingHelper.h>
#include <Dy/Management/Internal/MProfiling.h>

namespace dy
{

void SProfilingHelper::IncreaseOnBindTextureCount(TU32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddTextureCount)(iInput);
}

void SProfilingHelper::DecreaseOnBindTextureCount(TU32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddTextureCount)(-static_cast<TI32>(iInput));
}

void SProfilingHelper::IncreaseOnBindVertexCount(TU32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddOnBindVertexCount)(iInput);
}

void SProfilingHelper::DecreaseOnBindVertexCount(TU32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddOnBindVertexCount)(-static_cast<TI32>(iInput));
}

void SProfilingHelper::IncreaseOnBindShaderCount(TU32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddOnBindShaderCount)(iInput);
}

void SProfilingHelper::DecreaseOnBindShaderCount(TU32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddOnBindShaderCount)(-static_cast<TI32>(iInput));
}

void SProfilingHelper::IncreaseOnBindActorCount(TU32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddOnBindActorCount)(iInput);
}

void SProfilingHelper::DecreaseOnBindActorCount(TU32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddOnBindActorCount)(-static_cast<TI32>(iInput));
}

void SProfilingHelper::AddScreenRenderedActorCount(TI32 iInput) noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(AddScreenRenderedActorCount)(iInput);
}

void SProfilingHelper::ResetFrameDependentCounts() noexcept
{
  MProfiling::GetInstance().MDY_PRIVATE(ResetFrameDependentCounts)();
}

} /// ::dy namespace