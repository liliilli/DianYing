#pragma once
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

#include <DInputButton.h>
#include <ELowKeyboard.h>
#include <ELowMouse.h>

namespace dy::base 
{

/// @class ALowInput
/// @brief Low-input management type.
class ALowInput
{
public:
  ALowInput() = default;
  virtual ~ALowInput() = 0;

  /// @brief Update keyboard key with derived platform descriptor instance.
  virtual void UpdateKeyboard(void* descriptor) = 0;

  /// @brief Update mouse button with derived platform descriptor instance.
  virtual void UpdateMouseButton(void* descriptor) = 0;

  /// @brief Get keyboard state. ELowKeyboard::Dy_Key__Error must not be used.
  EInputState GetKeyboard(ELowKeyboard id);

  /// @brief Get mouse state. ELowMouseButton::DyMouse__Error, __Sum must not be used.
  EInputState GetMouseButton(ELowMouseButton id);

protected:
  std::array<DInputButtonItem, Dy_Key_Menu>   sLowKeyboards;
  std::array<DInputButtonItem, DyMouse__Sum>  sLowMouseButtons;
};

} /// ::dy::base namespace