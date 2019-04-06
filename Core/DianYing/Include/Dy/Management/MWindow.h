#ifndef GUARD_DY_MANAGEMENT_MANAGER_H
#define GUARD_DY_MANAGEMENT_MANAGER_H
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
#include <Dy/Management/Interface/ISingletonCrtp.h>

#include <Dy/Management/Platform/MPlatformInfoWindows.h>

namespace dy
{

class MWindow final : public ISingleton<MWindow>, public IPlatformDependent
{
  DY_PIMPL_SINGELTON_DERIVED(MWindow);
  MDY_SINGLETON_PROPERTIES(MWindow);
public:
  /// @brief Check window is should closed this time.
  MDY_NODISCARD bool IsWindowShouldClose() const noexcept;

  /// @brief Terminate window. if terminated already, just return DY_FAILURE. \n
  /// This function must be called in main thread.
  MDY_NODISCARD EDySuccess MDY_PRIVATE(TerminateWindow)() noexcept;

  /// @brief Get window context.
  MDY_NODISCARD GLFWwindow* GetGLMainWindow() const noexcept;
  /// @brief Get glfw worker window context list
  MDY_NODISCARD const std::array<GLFWwindow*, 2>& GetGLWorkerWindowList() const noexcept;

  /// @brief  Create console window if OS supports. Even though OS does not support console window,
  /// This function will return DY_SUCCESS because of conformity with remove function.
  /// @return If succeeded, return DY_SUCCESS or DY_FAILURE. \n
  EDySuccess CreateConsoleWindow() override final;
  /// @brief  Check if console window is created or not.
  /// @return If created anyway, return true or false
  MDY_NODISCARD bool IsCreatedConsoleWindow() const noexcept override final;
  /// @brief  Remove console window when console window is initiailzed before.
  /// @return If succeeded, return DY_SUCCESS or DY_FAILURE
  EDySuccess RemoveConsoleWindow() override final;

  /// @brief Get cpu usage overall percentage. (0 ~ 100%)
  MDY_NODISCARD TF32 GetCpuUsage() override final;
  /// @brief Get ram usage as byte.
  MDY_NODISCARD TU64 GetRamUsage() override final;

  /// @brief Check font exist on system path.
  MDY_NODISCARD bool IsFontExistOnSystem(const std::string& iFontKey) const override final;
  /// @brief Get system font path with iFontKey. If not found, just return null value.
  MDY_NODISCARD std::optional<std::string> GetFontPathOnSystem(const std::string& iFontKey) const override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_MANAGER_H