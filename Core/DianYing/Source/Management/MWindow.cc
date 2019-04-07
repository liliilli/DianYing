#include <precompiled.h>
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

/// Header file
#include <Dy/Management/MWindow.h>

#include <future>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Dy/Helper/Type/DVector3.h>

#include <Dy/Management/MSetting.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLWindowContextDescriptor.h>
#include <Dy/Core/GEngine.h>

#include <Dy/Include/GlInclude.h>

//!
//! Independent anonymous namespace
//!

namespace
{

void GLAPIENTRY DyGlMessageCallback(
  GLenum source, GLenum type, GLuint id, 
  GLenum severity, GLsizei length, const GLchar* message, 
  const void* userParam)
{
  DyPushLogDebugCritical(
    "DianYing OpenGL callback : %s type = 0x%x, severity = 0x%x, message = %s",
    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
    type,
    severity,
    message
  );
}

void DyGLFWErrorFunction(int errorcode, const char* message)
{
  DyPushLogDebugCritical("GLFW Error occurred. Error code : {}, Message : {}", errorcode, message);
}

/// @brief Callback method for size check and resizing.
void DyGlCallbackFrameBufferSize(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

/// @brief Callback method for closing arbitary glfw window window handle.
void DyGlCallbackWindowClose(GLFWwindow* window)
{
  glfwSetKeyCallback(window, nullptr);
  glfwSetCursorPosCallback(window, nullptr);
  dy::gEngine->TryEndGame();
}

} /// unnamed namespace

//!
//! Implementations
//!

namespace dy
{

struct MWindow::Impl final : public IPlatformDependent
{
  DY_DECLARE_PIMPL_CTOR(MWindow);
  virtual ~Impl();

  /// @brief Check window is should closed this time.
  bool IsWindowShouldClose() const noexcept;

  /// @brief Terminate window. if terminated already, just return EDySuccess::DY_FAILURE. \n
  /// This function must be called in main thread.
  EDySuccess MDY_PRIVATE(TerminateWindow)() noexcept;

  /// @brief Get window context.
  GLFWwindow* GetGLMainWindow() const noexcept;
  /// @brief Get glfw worker window context list
  const std::array<GLFWwindow*, 2>& GetGLWorkerWindowList() const noexcept;

  /// @brief  Create console window if OS supports. Even though OS does not support console window,
  /// This function will return EDySuccess::DY_SUCCESS because of conformity with remove function.
  /// @return If succeeded, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE. \n
  EDySuccess CreateConsoleWindow() override final;
  /// @brief  Check if console window is created or not.
  /// @return If created anyway, return true or false
  bool IsCreatedConsoleWindow() const noexcept override final;
  /// @brief  Remove console window when console window is initiailzed before.
  /// @return If succeeded, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE
  EDySuccess RemoveConsoleWindow() override final;

  /// @brief Get cpu usage overall percentage. (0 ~ 100%)
  TF32 GetCpuUsage() override final;
  /// @brief Get ram usage as byte.
  TU64 GetRamUsage() override final;

  /// @brief Check font exist on system path.
  bool IsFontExistOnSystem(const std::string& iFontKey) const override final;
  /// @brief Get system font path with iFontKey. If not found, just return null value.
  std::optional<std::string> GetFontPathOnSystem(const std::string& iFontKey) const override final;

  void InitializeDep() override final {}; 
  void ReleaseDep() override final {};
  
  GLFWwindow*                 mGlfwWindow     = nullptr;
  std::array<GLFWwindow*, 2>  mGlfwWorkerWnds = {};
  IPlatformDependent*         mDependentWindowContext = nullptr;
};

} /// ::dy namespace
#include <Dy/Management/Inline/MWindowImpl.inl>

//!
//! Proxy
//!

namespace dy
{

bool MWindow::IsWindowShouldClose() const noexcept
{
  return DY_PIMPL->IsWindowShouldClose();
}

EDySuccess MWindow::MDY_PRIVATE(TerminateWindow)() noexcept
{
  return DY_PIMPL->MDY_PRIVATE(TerminateWindow)();
}

GLFWwindow* MWindow::GetGLMainWindow() const noexcept
{
  return DY_PIMPL->GetGLMainWindow();
}

const std::array<GLFWwindow*, 2>& MWindow::GetGLWorkerWindowList() const noexcept
{
  return DY_PIMPL->GetGLWorkerWindowList();
}

EDySuccess MWindow::CreateConsoleWindow()
{
  return DY_PIMPL->CreateConsoleWindow();
}

bool MWindow::IsCreatedConsoleWindow() const noexcept
{
  return DY_PIMPL->IsCreatedConsoleWindow();
}

EDySuccess MWindow::RemoveConsoleWindow() 
{ 
  return DY_PIMPL->RemoveConsoleWindow(); 
}

TF32 MWindow::GetCpuUsage() { return DY_PIMPL->GetCpuUsage(); }
TU64 MWindow::GetRamUsage() { return DY_PIMPL->GetRamUsage(); }

bool MWindow::IsFontExistOnSystem(const std::string& iFontKey) const
{
  return DY_PIMPL->IsFontExistOnSystem(iFontKey);
}

std::optional<std::string> MWindow::GetFontPathOnSystem(const std::string& iFontKey) const
{
  return DY_PIMPL->GetFontPathOnSystem(iFontKey);
}

EDySuccess MWindow::pfInitialize()
{
  DY_INITIALIZE_PIMPL();
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MWindow::pfRelease()
{
  DY_RESET_PIMPL();
  return EDySuccess::DY_SUCCESS;
}

DY_DEFINE_PIMPL(MWindow);
DY_DEFINE_DEFAULT_DESTRUCTOR(MWindow);

} /// ::dy namespace