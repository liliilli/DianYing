#ifndef GUARD_DY_HELPER_TYPE_THREESTATEBOOL_H
#define GUARD_DY_HELPER_TYPE_THREESTATEBOOL_H
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

namespace dy
{

///
/// @class DDy3StateBool
/// @brief 3-state boolean helper type.
///
class DDy3StateBool final
{
public:
  DDy3StateBool();
  DDy3StateBool(const bool parent, const bool input);

  ///
  /// @brief Update input boolean value.
  /// @param newInput input value to insert.
  ///
  void UpdateInput(const bool newInput) noexcept;

  ///
  /// @brief Update parent boolean value.
  /// @param newParent parent value to insert.
  ///
  void UpdateParent(const bool newParent) noexcept;

  ///
  /// @brief Get output value.
  /// @return Output value.
  ///
  MDY_NODISCARD bool GetOutput() const noexcept;

  ///
  /// @brief Check output value is changed. but might be ABA problem when values are changed.
  /// @return Check old-output and output, if changed return true or false.
  ///
  MDY_NODISCARD bool IsOutputValueChanged() const noexcept;

private:
  /// @brief Update output / oldOutput value comparing parent and input.
  void pUpdateOutputs();

  //! Tri-state boolean
  //!
  //!       mParent----.                Parent | Input | Output
  //!               __ |                     X |     X |      X
  //!              |  \o                     X |     O |      X
  //! mInput _____o|   \_____o mOutput       O |     X |      X
  //!              |   /                     O |     O |      O
  //!              |__/
  //!

  /// Parent boolean value.
  bool mParent    : 1;

  /// Input boolean value.
  bool mInput     : 1;

  /// Output boolean value.
  MDY_TRANSIENT bool mOutput    : 1;

  /// Old-output boolean value (previous output value)
  MDY_TRANSIENT bool mOldOutput : 1;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_THREESTATEBOOL_H