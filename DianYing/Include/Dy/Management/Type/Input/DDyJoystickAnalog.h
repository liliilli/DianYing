#ifndef GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYJOYSTICKANALOG_H
#define GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYJOYSTICKANALOG_H
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

#include <Dy/Helper/System/Macroes.h>
#include <Dy/Helper/GlobalType.h>

namespace dy
{

/// @class DDyJoystickAnalog
/// @brief Joystick's stick analog value binding.
class DDyJoystickAnalog final
{
public:
  /// @brief Set basis value of this stick instance.
  /// Basis value is used to calculate delta value from mOriginalValue when `Update` function is called.
  void SetBasisValue(_MIN_ TF32 iNewBasis) noexcept;

  /// @brief Get basis value of this stick instance.
  MDY_NODISCARD TF32 GetBasisValue() const noexcept;

  /// @brief Update original value of this stick instance.
  void Update(_MIN_ TF32 iValue) noexcept;

  /// @brief Get delta value which is calculated and calibrated value.
  MDY_NODISCARD TF32 GetDeltaValue() const noexcept;

  /// @brief Get original value which is different from delta value, \n
  /// not calibrated value.
  MDY_NODISCARD TF32 GetOriginalValue() const noexcept;

private:
  TF32 mBasisValue    = MDY_INITIALIZE_DEFINT;
  TF32 mOriginalValue = MDY_INITIALIZE_DEFINT;
  TF32 mDeltaValue    = MDY_INITIALIZE_DEFINT;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYJOYSTICKANALOG_H