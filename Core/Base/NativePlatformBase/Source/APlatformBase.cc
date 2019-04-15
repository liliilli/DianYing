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

#include <APlatformBase.h>

namespace dy
{

APlatformBase::APlatformBase(EXPR_E(EPlatform) platform)
  : mPlatform{platform}
{ }

APlatformBase::~APlatformBase() = default;

ADebugBase& APlatformBase::GetDebugManager() noexcept
{
  return *this->mDebug;
}

AProfilingBase& APlatformBase::GetProfilingManager() noexcept
{
  return *this->mProfiling;
}

base::ALowInput& APlatformBase::GetInputManager() noexcept
{
  return *this->mLowInput;
}

EPlatform::_ APlatformBase::GetPlatformType() const noexcept
{
  return this->mPlatform;
}

bool APlatformBase::IsConsoleWindowCreated() noexcept
{
  return this->mIsConsoleWindowCreated;
}

} /// ::dy namespace