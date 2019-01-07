#ifndef GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYJOYSTICKANALOG_H
#define GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYJOYSTICKANALOG_H
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

#include <Dy/Helper/System/Macroes.h>
#include <Dy/Helper/GlobalType.h>

namespace dy
{

/// @class DDyJoystickAnalog
/// @brief Joystick's stick analog value binding.
class DDyJoystickAnalog final
{
public:
  /// @brief Update original value of this stick instance.
  void Update(_MIN_ TF32 iValue) noexcept;

  /// @brief Get original value which is different from delta value, \n
  /// not calibrated value.
  MDY_NODISCARD TF32 GetValue() const noexcept;

private:
  TF32 mValue = MDY_INITIALIZE_DEFINT;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYJOYSTICKANALOG_H