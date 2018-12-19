#ifndef GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYINPUTBUTTON_H
#define GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYINPUTBUTTON_H
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
#include <Dy/Management/Type/Input/EDyInputButton.h>
#include <Dy/Management/Type/Input/EDyInputButtonStatus.h>

namespace dy
{

/// @class DDyInputButton
/// @brief Button status binder. Axis and Action must be retrieve values from this.
class DDyInputButton final
{
public:
  /// @brief Update status with from library.
  void Update(_MIN_ int iLowLevelStatus) noexcept;

  /// @brief Get status value on present frame.
  MDY_NODISCARD EDyInputButtonStatus Get() const noexcept;

private:
  EDyInputButtonStatus mStatus = EDyInputButtonStatus::Released;
};

} /// ::dy namesapce

#endif /// GUARD_DY_MANAGEMENT_TYPE_INPUT_DDYINPUTBUTTON_H