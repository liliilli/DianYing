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
#include <DInputButton.h>

namespace dy::base
{

void DInputButtonItem::Update(EInputState newState) noexcept
{
  switch (this->mState)
  {
  case EInputState::Released: 
  {
    if (newState == EInputState::Pressed) { this->mState = newState; }
  } break;
  case EInputState::Pressed: 
  {
    if (newState == EInputState::Pressed) { this->mState = EInputState::Repeated; }
    else if (newState == EInputState::Released) { this->mState = newState; }
  } break;
  case EInputState::Repeated: 
  {
    if (newState == EInputState::Released) { this->mState = newState; }
  } break;
  }
}

EInputState DInputButtonItem::Get() const noexcept
{
  return this->mState;
}

} /// ::dy::base namespace
