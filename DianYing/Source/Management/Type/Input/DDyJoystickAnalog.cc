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
#include <Dy/Management/Type/Input/DDyJoystickAnalog.h>

namespace dy
{

void DDyJoystickAnalog::SetBasisValue(_MIN_ TF32 iNewBasis) noexcept
{
  this->mBasisValue = iNewBasis;
}

TF32 DDyJoystickAnalog::GetBasisValue() const noexcept
{
  return this->mBasisValue;
}

void DDyJoystickAnalog::Update(_MIN_ TF32 iValue) noexcept
{
  this->mOriginalValue  = iValue;
  this->mBasisValue     = this->mOriginalValue - this->mBasisValue;
}

TF32 DDyJoystickAnalog::GetDeltaValue() const noexcept
{
  return this->mDeltaValue;
}

TF32 DDyJoystickAnalog::GetOriginalValue() const noexcept
{
  return this->mOriginalValue;
}


} /// ::dy namespace