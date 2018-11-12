#ifndef GUARD_DY_MANAGEMENT_INTERFACE_IDYWINDOWDEPENDENT_H
#define GUARD_DY_MANAGEMENT_INTERFACE_IDYWINDOWDEPENDENT_H
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
/// @interface IDyWindowDependent
/// @brief Window depdenent base type.
///
MDY_INTERFACE MDY_NO_VTABLE IDyWindowDependent
{
public:
  virtual ~IDyWindowDependent() = default;

  ///
  /// @brief Initialize dependent platform window manager.
  ///
  virtual void InitializeDep() = 0;

  ///
  /// @brief Release dependent platform window manager.
  ///
  virtual void ReleaseDep() = 0;

  ///
  /// @brief  Create console window if OS supports. Even though OS does not support console window,
  /// This function will return DY_SUCCESS because of conformity with remove function.
  /// @return If succeeded, return DY_SUCCESS or DY_FAILURE. \n
  ///
  virtual MDY_NODISCARD EDySuccess CreateConsoleWindow() = 0;

  ///
  /// @brief  Check if console window is created or not.
  /// @return If created anyway, return true or false.
  ///
  virtual MDY_NODISCARD bool       IsCreatedConsoleWindow() const noexcept = 0;

  ///
  /// @brief  Remove console window when console window is initiailzed before.
  /// @return If succeeded, return DY_SUCCESS or DY_FAILURE.
  ///
  virtual MDY_NODISCARD EDySuccess RemoveConsoleWindow() = 0;

  ///
  /// @brief Get cpu usage overall percentage. (0 ~ 100%)
  ///
  virtual MDY_NODISCARD TF32       GetCpuUsage() = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERFACE_IDYWINDOWDEPENDENT_H