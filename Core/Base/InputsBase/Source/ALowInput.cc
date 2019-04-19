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
#include <ALowInput.h>

namespace dy::base
{

EInputState ALowInput::GetKeyboard(ELowKeyboard id)
{
  if (id == ELowKeyboard::Dy___Error)
  {
    return EInputState::Released;
  }

  return this->sLowKeyboards[id].Get();
}

EInputState ALowInput::GetMouseButton(ELowMouseButton id)
{
  if (id == ELowMouseButton::DyMouse__Error
  ||  id == ELowMouseButton::DyMouse__Sum)
  {
    return EInputState::Released;
  }

  return this->sLowMouseButtons[id].Get();
}

ELowMousePosState ALowInput::GetMousePosState() const noexcept
{
  return this->mLowMousePos.GetFeatureState();
}

void ALowInput::SetMousePosFeatureState(ELowMousePosState newState) noexcept
{
  this->mLowMousePos.SetFeatureState(newState);
}

std::optional<std::pair<int, int>> ALowInput::GetMousePos() const noexcept
{
  // If feature is not enabled yet, just return null value.
  using EEnum = decltype(this->mLowMousePos.GetFeatureState());
  if (this->mLowMousePos.GetFeatureState() == EEnum::Off)
  {
    return std::nullopt;
  }

  // Or,  get a position (screen or virtual) value from container.
  return this->mLowMousePos.GetPresentPosition();
}

std::optional<std::pair<int, int>> ALowInput::GetMousePreviousPos() const noexcept
{
  // If feature is not enabled yet, just return null value.
  using EEnum = decltype(this->mLowMousePos.GetFeatureState());
  if (this->mLowMousePos.GetFeatureState() == EEnum::Off)
  {
    return std::nullopt;
  }

  return this->mLowMousePos.GetPreviousPosition();
}

std::optional<std::pair<int, int>> ALowInput::GetMousePosMovement() const noexcept
{
  // If feature is not enabled yet, just return null value.
  using EEnum = decltype(this->mLowMousePos.GetFeatureState());
  if (this->mLowMousePos.GetFeatureState() == EEnum::Off)
  {
    return std::nullopt;
  }

  return this->mLowMousePos.GetMovementPosition();
}

} /// ::dy::base namespace