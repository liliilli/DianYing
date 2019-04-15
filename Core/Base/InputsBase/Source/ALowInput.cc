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

inline ALowInput::~ALowInput() = default;

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

} /// ::dy::base namespace