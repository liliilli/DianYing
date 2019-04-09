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

struct GLFWwindow;

namespace dy
{

class MWindow final : public ISingleton<MWindow>
{
  DY_PIMPL_SINGELTON_DERIVED(MWindow);
  MDY_SINGLETON_PROPERTIES(MWindow);
public:
  /// @brief Check window is should closed this time.
  MDY_NODISCARD bool IsWindowShouldClose() const noexcept;

  /// @brief Terminate window. if terminated already, just return EDySuccess::DY_FAILURE. \n
  /// This function must be called in main thread.
  MDY_NODISCARD EDySuccess MDY_PRIVATE(TerminateWindow)() noexcept;

  /// @brief Get window context.
  MDY_NODISCARD GLFWwindow* GetGLMainWindow() const noexcept;
  /// @brief Get glfw worker window context list
  MDY_NODISCARD const std::array<GLFWwindow*, 2>& GetGLWorkerWindowList() const noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_MANAGER_H