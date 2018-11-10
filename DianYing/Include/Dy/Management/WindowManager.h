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

#include <Dy/Helper/Macroes.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Helper/Pointer.h>

namespace dy
{

#if defined(MDY_PLATFORM_FLAG_WINDOWS)
///
/// @struct DDyWindowInformationWindows
/// @brief Windows information for windows platform.
///
struct DDyWindowInformationWindows
{
protected:
  dy::Owner<FILE*> mFp                  = nullptr;
  HWND        mWindowHandle             = nullptr;
  HDC         mWindowDeviceContext      = nullptr;
  HGLRC       mWindowGlResourceContext  = nullptr;
  GLFWwindow* mGlfwWindow               = nullptr;
};

///
/// @struct DDyDependentFunctionWindows
/// @brief Windows specific functions
///
struct [[maybe_unused]] DDyDependentFunctionWindows
{

};

#endif

class MDyWindow final : public ISingleton<MDyWindow>, MDY_INHERITENCE_WINDOW_INFORMATION_SUPER
{
  MDY_SINGLETON_DERIVED(MDyWindow);
  MDY_SINGLETON_PROPERTIES(MDyWindow);
public:
  ///
  /// @brief Run application.
  ///
  void Run();

  ///
  /// @brief  Create console window if OS supports. Even though OS does not support console window,
  /// This function will return DY_SUCCESS because of conformity with remove function.
  /// @return If succeeded, return DY_SUCCESS or DY_FAILURE. \n
  ///
  MDY_NODISCARD EDySuccess CreateConsoleWindow();

  ///
  /// @brief  Check if console window is created or not.
  /// @return If created anyway, return true or false.
  ///
  MDY_NODISCARD bool IsCreatedConsoleWindow() const noexcept;

  ///
  /// @brief  Remove console window when console window is initiailzed before.
  /// @return If succeeded, return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess RemoveConsoleWindow();

  ///
  /// @brief Get glfw window context.
  /// @todo TEMPORAL FUNCTION.
  ///
  GLFWwindow* GetGlfwWindowContext() const noexcept
  {
    MDY_ASSERT(this->mGlfwWindow, "GlfwWindow is not initiailized.");
    return this->mGlfwWindow;
  }

private:
  ///
  void pUpdate(float dt);

  ///
  void pRender();

  ///
  std::atomic<bool> mIsConsoleWindowInitialized = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_MANAGER_H