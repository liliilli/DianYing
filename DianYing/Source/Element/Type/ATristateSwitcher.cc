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
#include <Dy/Element/Type/ATristateSwitcher.h>

namespace dy
{

void ATristateSwitcher::Activate()
{
  const auto previous = this->mActivateFlag.GetOutput();
  this->mActivateFlag.UpdateInput(true);
  const auto present = this->mActivateFlag.GetOutput();

  // Compare previous and present final value, and present is supposed to be a true in this function.
  // so if changed, Try Activate Instance.
  if (previous != present) { this->TryActivateInstance(); }
}

void ATristateSwitcher::Deactivate()
{
  const auto previous = this->mActivateFlag.GetOutput();
  this->mActivateFlag.UpdateInput(false);
  const auto present = this->mActivateFlag.GetOutput();

  // Compare previous and present final value, and present is supposed to be a true in this function.
  // so if changed, Try Activate Instance.
  if (previous != present) { this->TryDeactivateInstance(); }
}

bool ATristateSwitcher::IsActivated() const noexcept
{
  return this->mActivateFlag.GetOutput();
}

void ATristateSwitcher::SetupFlagAsParent(_MIN_ bool iIsActivated)
{
  const auto previous = this->mActivateFlag.GetOutput();
  this->mActivateFlag.UpdateParent(iIsActivated);
  const auto present = this->mActivateFlag.GetOutput();

  if (previous != present)
  {
    if (iIsActivated == true) { this->TryActivateInstance(); }
    else                      { this->TryDeactivateInstance(); }
  }
}

} /// ::dy namespace