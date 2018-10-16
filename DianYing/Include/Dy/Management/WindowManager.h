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
  /// @brief Get glfw window context.
  /// @todo TEMPORAL FUNCTION.
  ///
  GLFWwindow* GetGlfwWindowContext() const noexcept
  {
    PHITOS_ASSERT(this->mGlfwWindow, "GlfwWindow is not initiailized.");
    return this->mGlfwWindow;
  }

private:
  ///
  void pUpdate(float dt);

  ///
  void pRender();

};

} /// ::dy namespace

void DyTempInitializeTestResources();

#endif /// GUARD_DY_MANAGEMENT_MANAGER_H