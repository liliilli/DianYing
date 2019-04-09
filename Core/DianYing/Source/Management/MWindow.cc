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
  TEMP_CAST(dy::gEngine)->TryEndGame();
}

} /// unnamed namespace

//!
//! Implementations
//!

namespace dy
{

struct MWindow::Impl final
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

  GLFWwindow*                 mGlfwWindow     = nullptr;
  std::array<GLFWwindow*, 2>  mGlfwWorkerWnds = {};
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