#ifndef GUARD_DY_MANAGEMENT_MANAGER_H
#define GUARD_DY_MANAGEMENT_MANAGER_H
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
#include <Dy/Management/Interface/ISingletonCrtp.h>

#include <Dy/Management/Platform/DDyWindowInformationWindows.h>

namespace dy
{

class MDyWindow final : public IDySingleton<MDyWindow>, public MDY_INHERITENCE_WINDOW_INFORMATION_SUPER
{
  MDY_SINGLETON_DERIVED(MDyWindow);
  MDY_SINGLETON_PROPERTIES(MDyWindow);
public:
  /// @brief Check window is should closed this time.
  MDY_NODISCARD bool IsWindowShouldClose() const noexcept;

  ///
  /// @brief Get glfw window context.
  /// @todo TEMPORAL FUNCTION.
  ///
  GLFWwindow* GetGlfwWindowContext() const noexcept
  {
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mGlfwWindow), "GlfwWindow is not initiailized.");
    return this->mGlfwWindow;
  }

  /// @brief TEMPORAL FUNCTION FOR SWAPPING BUFFER.
  void TempSwapBuffers();

private:
  GLFWwindow* mGlfwWindow = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_MANAGER_H