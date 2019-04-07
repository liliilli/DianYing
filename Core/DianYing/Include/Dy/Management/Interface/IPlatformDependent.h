#ifndef GUARD_DY_MANAGEMENT_INTERFACE_IDYWINDOWDEPENDENT_H
#define GUARD_DY_MANAGEMENT_INTERFACE_IDYWINDOWDEPENDENT_H
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

namespace dy
{

/// @interface IPlatformDependent
/// @brief Window depdenent base type.
MDY_INTERFACE MDY_NO_VTABLE IPlatformDependent
{
public:
  virtual ~IPlatformDependent() = default;

  /// @brief Initialize dependent platform window manager.
  virtual void InitializeDep() {};
  /// @brief Release dependent platform window manager.
  virtual void ReleaseDep() {};

  /// @brief  Create console window if OS supports. Even though OS does not support console window,
  /// This function will return EDySuccess::DY_SUCCESS because of conformity with remove function.
  /// @return If succeeded, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE. \n
  MDY_NODISCARD virtual EDySuccess CreateConsoleWindow() = 0;
  /// @brief  Check if console window is created or not.
  /// @return If created anyway, return true or false.
  MDY_NODISCARD virtual bool IsCreatedConsoleWindow() const noexcept = 0;
  /// @brief  Remove console window when console window is initiailzed before.
  /// @return If succeeded, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  MDY_NODISCARD virtual EDySuccess RemoveConsoleWindow() = 0;

  /// @brief Get cpu usage overall percentage. (0 ~ 100%)
  MDY_NODISCARD virtual TF32 GetCpuUsage() = 0;
  /// @brief Get ram usage as byte.
  MDY_NODISCARD virtual TU64 GetRamUsage() = 0;
  /// @brief Check font exist on system path.
  MDY_NODISCARD virtual bool IsFontExistOnSystem(_MIN_ const std::string& iFontKey) const = 0;
  /// @brief Get system font path with iFontKey. If not found, just return null value.
  MDY_NODISCARD virtual std::optional<std::string> GetFontPathOnSystem(_MIN_ const std::string& iFontKey) const = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERFACE_IDYWINDOWDEPENDENT_H