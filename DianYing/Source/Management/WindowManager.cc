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
#include <Dy/Management/WindowManager.h>

#include <future>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Dy/Helper/Type/Vector3.h>

#include <Dy/Management/SettingManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/WorldManager.h>
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"
#include "Dy/Core/Rendering/Wrapper/PDyGLWindowContextDescriptor.h"

//!
//! Independent anonymous namespace
//!

namespace
{

void GLAPIENTRY DyGlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  MDY_LOG_CRITICAL_D(
      "DianYing OpenGL callback : %s type = 0x%x, severity = 0x%x, message = %s",
      (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
      type,
      severity,
      message
  );
}

void DyGLFWErrorFunction(int errorcode, const char* message)
{
  MDY_LOG_CRITICAL_D("GLFW Error occurred. Error code : {}, Message : {}", errorcode, message);
}

} /// unnamed namespace

//!
//! Platform depdendent anonymous namespace
//!

#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{

///
/// @brief Callback method for size check and resizing.
///
void DyGlCallbackFrameBufferSize(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

///
/// @brief Callback method for closing arbitary glfw window window handle.
///
void DyGlCallbackWindowClose(GLFWwindow* window)
{
  glfwSetKeyCallback(window, nullptr);
  glfwSetCursorPosCallback(window, nullptr);
  glfwDestroyWindow(window);
}

} /// unnamed namespace
#elif defined(MDY_PLATFORM_FLAG_LINUX)
#elif defined(MDY_PLATFORM_FLAG_MACOS)
#endif

//!
//! Implementations
//!

namespace dy
{

bool MDyWindow::IsWindowShouldClose() const noexcept
{
  return glfwWindowShouldClose(this->mGlfwWindow);
}

EDySuccess MDyWindow::MDY_PRIVATE_SPECIFIER(TerminateWindow)() noexcept
{
  bool terminated = false;
  if (terminated == true) { return DY_FAILURE; }

  terminated = true;
  glfwSetWindowShouldClose(this->GetGLMainWindow(), GLFW_TRUE);
  return DY_SUCCESS;
}

const std::array<GLFWwindow*, 2>& MDyWindow::GetGLWorkerWindowList() const noexcept
{
#if defined (NDEBUG) == false 
  for (const auto& ptrWindow : this->mGlfwWorkerWnds)
  { // Validation check.
    MaybeNotUsed(ptrWindow);
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(ptrWindow), "GLFWwindow must be valid.");
  }
#endif
  return this->mGlfwWorkerWnds;
}

void MDyWindow::TempSwapBuffers()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mGlfwWindow), "OpenGL Context must not be null when running.");

  if (this->IsWindowShouldClose() == true) { return; }
  glfwSwapBuffers(this->mGlfwWindow);
  glfwPollEvents();
}

#if defined(MDY_PLATFORM_FLAG_WINDOWS)
EDySuccess MDyWindow::pfInitialize()
{
  /// @brief This function is not implemented yet.
  static auto InitializeVulkan = [this]()
  {
    #ifdef false
    dy::DyVkInitialize(windowHandle, hInstance);

    ShowWindow(windowHandle, SW_SHOW);
    SetForegroundWindow(windowHandle);
    SetFocus(windowHandle);

    dy::DyVkRenderLoop();
    dy::DyVkCleanupResources();
    #endif
    return DY_FAILURE;
  };

  /// @brief This function is not implemented yet.
  static auto InitializeDirectX11 = [this]()
  {
    #ifdef false
    MDY_CALL_ASSERT_SUCCESS(DyWin32InitializeWindow(hInstance));
    MDY_CALL_ASSERT_SUCCESS(DyD11InitializeDirect3D());

    ShowWindow(windowHandle, SW_SHOW);
    SetForegroundWindow(windowHandle);
    SetFocus(windowHandle);

    DyD11CreateEffectFx();
    DyD11BindVertexLayout();
    DyD11CreateVertexBuffer();
    DyD11RenderLoop();
    #endif
    return DY_FAILURE;
  };

  /// @brief Initialize OpenGL Context.
  /// This function must be returned `DY_SUCCESS`.
  static auto InitializeOpenGL = [this]()
  {
    glfwInit();

    { // Create descriptor.
      PDyGLWindowContextDescriptor descriptor{};
      descriptor.mWindowName          = "DianYing";
      descriptor.mIsWindowResizable   = false;
      descriptor.mIsWindowVisible     = true;
      descriptor.mIsWindowShouldFocus = true;
      descriptor.mIsUsingDefaultDoubleBuffer = true;

      const auto& settingManager = MDySetting::GetInstance();
      descriptor.mWindowSize = DDyVectorInt2{
          settingManager.GetWindowSizeWidth(),
          settingManager.GetWindowSizeHeight()
      };
      this->mGlfwWindow = FDyGLWrapper::CreateGLWindow(descriptor);

      // Create window instance for giving context to I/O worker thread.
      descriptor.mIsWindowShouldFocus = false;
      descriptor.mIsWindowVisible     = false;
      descriptor.mSharingContext      = this->mGlfwWindow;
      for (auto& ptrWindow : this->mGlfwWorkerWnds) { ptrWindow = FDyGLWrapper::CreateGLWindow(descriptor); }
    }

    // Check validity
    if (this->mGlfwWindow == nullptr) { glfwTerminate(); return DY_FAILURE; }
    FDyGLWrapper::CreateGLContext(this->mGlfwWindow);

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glfwSetInputMode(this->mGlfwWindow, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetFramebufferSizeCallback(this->mGlfwWindow, &DyGlCallbackFrameBufferSize);
    glfwSetWindowCloseCallback(this->mGlfwWindow, &DyGlCallbackWindowClose);

    // If in debug build environment, enable debug output logging.
    #if defined(_DEBUG) || !defined(_NDEBUG)
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(DyGlMessageCallback, nullptr);
      GLuint unusedIds = 0;
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
      glfwSetErrorCallback(&DyGLFWErrorFunction);
    #endif

    // Setting rendering.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    { // Make initialized screen to black for giving liability to users XD
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.f, 0.f, 0.f, 1.0f);
      glfwSwapBuffers(this->mGlfwWindow);
      glfwPollEvents();
    }
    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  MDY_LOG_INFO_D("{} | MDyWindow::pfInitialize().", "FunctionCall");
  MDY_INHERITENCE_WINDOW_INFORMATION_SUPER::InitializeDep();

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  default: MDY_UNEXPECTED_BRANCH(); break;
  case EDyRenderingApi::DirectX12:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::Vulkan:     MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::DirectX11:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::OpenGL:
    MDY_LOG_INFO_D("Initialize OpenGL Context.");
    MDY_CALL_ASSERT_SUCCESS(InitializeOpenGL());

    { // IMGUI Setting
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui_ImplGlfw_InitForOpenGL(this->mGlfwWindow, true);
      ImGui_ImplOpenGL3_Init("#version 430");
      ImGui::StyleColorsDark();
    }
    break;
  }

  return DY_SUCCESS;
}

EDySuccess MDyWindow::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyWindow::pfRelease().", "FunctionCall");

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  default: MDY_UNEXPECTED_BRANCH(); return DY_FAILURE;
  case EDyRenderingApi::DirectX11:  MDY_LOG_INFO_D("Release DirectX11 Context.");  MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::DirectX12:  MDY_LOG_INFO_D("Release DirectX12 Context.");  MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::Vulkan:     MDY_LOG_INFO_D("Release Vulkan Context.");     MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::OpenGL:
    MDY_LOG_INFO_D("Release OpenGL Context.");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    MDY_LOG_INFO_D("Released ImGui Context.");
    break;
  }

  glfwTerminate();
  MDY_INHERITENCE_WINDOW_INFORMATION_SUPER::ReleaseDep();
  return DY_SUCCESS;
}

#elif defined(MDY_PLATFORM_FLAG_LINUX)
#elif defined(MDY_PLATFORM_FLAG_MACOS)
#endif

} /// ::dy namespace